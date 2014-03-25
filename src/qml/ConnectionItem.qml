/*
 * Copyright (C) 2014 Jan Grulich <jgrulich@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.networkmanagement 0.1 as PlasmaNM


PlasmaComponents.ListItem {
    id: connectionItem;

    property bool predictableWirelessPassword: !Uuid && Type == PlasmaNM.Enums.Wireless &&
                                               (SecurityType == PlasmaNM.Enums.StaticWep || SecurityType == PlasmaNM.Enums.WpaPsk ||
                                                SecurityType == PlasmaNM.Enums.Wpa2Psk);

    property bool visiblePasswordDialog: false;

    enabled: true
    height: if (visiblePasswordDialog)
                connectionItemBase.height + expandableComponentLoader.height + padding.margins.top + padding.margins.bottom;
            else
                connectionItemBase.height + padding.margins.top + padding.margins.bottom;

    Item {
        id: connectionItemBase;

        height: Math.max(connectionSvgIcon.height, connectionNameLabel.height + connectionStatusLabel.height);

        anchors {
            left: parent.left;
            right: parent.right;
            top: parent.top;
        }

        PlasmaCore.SvgItem {
            id: connectionSvgIcon;

            width: sizes.iconSize;
            height: width;
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            svg: svgNetworkIcons;
            elementId: ConnectionIcon;
        }

        PlasmaComponents.Label {
            id: connectionNameLabel;

            height: paintedHeight;
            anchors {
                left: connectionSvgIcon.right;
                leftMargin: padding.margins.left;
                right: buttonRow.left;
                top: parent.top;
                topMargin: 0;
            }
            text: ItemUniqueName;
            elide: Text.ElideRight;
            font.weight: ConnectionState == PlasmaNM.Enums.Activated || Uuid ? Font.DemiBold : Font.Normal;
            font.italic: ConnectionState == PlasmaNM.Enums.Activating ? true : false;
        }

        PlasmaComponents.Label {
            id: connectionStatusLabel;

            height: paintedHeight;
            anchors {
                left: connectionSvgIcon.right;
                leftMargin: padding.margins.left;
                right: buttonRow.left;
                top: connectionNameLabel.bottom;
            }

            font.pointSize: theme.smallestFont.pointSize;
            color: "#99"+(theme.textColor.toString().substr(1))
            text: itemText();

            elide: Text.ElideRight;
        }

        PlasmaComponents.Button {
            id: stateChangeButton;

            anchors {
                verticalCenter: parent.verticalCenter;
                right: parent.right;
            }

            implicitWidth: minimumWidth + padding.margins.left + padding.margins.right;
            text:if (ConnectionState == PlasmaNM.Enums.Deactivated)
                    i18n("Connect");
                else
                    i18n("Disconnect");

            onClicked: {
                if (Uuid || !predictableWirelessPassword || visiblePasswordDialog) {
                    if (ConnectionState == PlasmaNM.Enums.Deactivated) {
                        if (!predictableWirelessPassword && !Uuid) {
                            handler.addAndActivateConnection(DevicePath, SpecificPath);
                        } else if (visiblePasswordDialog) {
                            handler.addAndActivateConnection(DevicePath, SpecificPath, expandableComponentLoader.item.password);
                            visiblePasswordDialog = false;
                        } else {
                            handler.activateConnection(ConnectionPath, DevicePath, SpecificPath);
                        }
                    } else {
                        handler.deactivateConnection(ConnectionPath, DevicePath);
                    }
                } else if (predictableWirelessPassword) {
                    visiblePasswordDialog = true;
                }
            }
        }
    }

    Loader {
        id: expandableComponentLoader;

        anchors {
            left: parent.left;
            right: parent.right;
            top: connectionItemBase.bottom;
            topMargin: padding.margins.top;
        }
    }

    Component {
        id: passwordDialogComponent;

        Item {
            height: childrenRect.height + padding.margins.top;

            property alias password: passwordInput.text;
            property alias passwordFocus: passwordInput

            PlasmaCore.SvgItem {
                id: passwordSeparator;

                height: lineSvg.elementSize("horizontal-line").height;
                width: parent.width;
                anchors {
                    left: parent.left;
                    right: parent.right;
                    top: parent.top;
                }
                elementId: "horizontal-line";

                svg: PlasmaCore.Svg {
                    id: lineSvg;
                    imagePath: "widgets/line";
                }
            }

            PlasmaComponents.TextField {
                id: passwordInput;

                width: 200;
                height: implicitHeight;
                anchors {
                    horizontalCenter: parent.horizontalCenter;
                    top: passwordSeparator.bottom;
                    topMargin: padding.margins.top;
                }
                echoMode: showPasswordCheckbox.checked ? TextInput.Normal : TextInput.Password
                placeholderText: i18n("Password...");
                onAccepted: {
                    stateChangeButton.clicked();
                }
            }

            PlasmaComponents.CheckBox {
                id: showPasswordCheckbox;

                anchors {
                    left: passwordInput.left;
                    right: parent.right;
                    top: passwordInput.bottom;
                }
                checked: false;
                text: i18n("Show password");
            }
        }
    }

    states: [
        State {
            name: "collapsed";
            when: !visiblePasswordDialog;
            StateChangeScript { script: if (expandableComponentLoader.status == Loader.Ready) {expandableComponentLoader.sourceComponent = undefined} }
        },

        State {
            name: "expanded";
            when: visiblePasswordDialog;
            StateChangeScript { script: createContent(); }
            PropertyChanges { target: stateChangeButton; visible: true; }
            PropertyChanges { target: connectionItem; enabled: false; }
        }
    ]

    function createContent() {
        expandableComponentLoader.sourceComponent = passwordDialogComponent;
        expandableComponentLoader.item.passwordFocus.forceActiveFocus();
    }

    function itemText() {
        if (ConnectionState == PlasmaNM.Enums.Activating) {
            if (Type == PlasmaNM.Enums.Vpn)
                return VpnState;
            else
                return DeviceState;
        } else if (ConnectionState == PlasmaNM.Enums.Deactivating) {
            if (Type == PlasmaNM.Enums.Vpn)
                return VpnState;
            else
                return DeviceState;
        } else if (ConnectionState == PlasmaNM.Enums.Deactivated) {
            var result = LastUsed;
            if (SecurityType > PlasmaNM.Enums.None)
                result += ", " + SecurityTypeString;
            return result;
        } else if (ConnectionState == PlasmaNM.Enums.Activated) {
            if (Type == PlasmaNM.Enums.Wired) {
                return i18n("Connected") + ", ⬇ " + Download + ", ⬆ " + Upload;
            } else if (Type == PlasmaNM.Enums.Wireless) {
                return i18n("Connected") + ", ⬇ " + Download + ", ⬆ " + Upload;
            } else if (Type == PlasmaNM.Enums.Gsm || Type == PlasmaNM.Enums.Cdma) {
                return i18n("Connected") + ", ⬇ " + Download + ", ⬆ " + Upload;
            } else {
                return i18n("Connected");
            }
        }
    }
}
