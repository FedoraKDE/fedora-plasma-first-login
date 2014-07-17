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

import QtQuick 2.0
import org.kde.kquickcontrolsaddons 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.networkmanagement 0.2 as PlasmaNM

PlasmaComponents.ListItem {
    id: connectionItem;

    property bool predictableWirelessPassword: !Uuid && Type == PlasmaNM.Enums.Wireless &&
                                               (SecurityType == PlasmaNM.Enums.StaticWep || SecurityType == PlasmaNM.Enums.WpaPsk ||
                                                SecurityType == PlasmaNM.Enums.Wpa2Psk);

    property bool visiblePasswordDialog: false;
    property int iconSize: units.iconSizes.medium;

    enabled: true;
    checked: ListView.isCurrentItem;

    height: if (visiblePasswordDialog)
                connectionItemBase.height + expandableComponentLoader.height + padding.margins.top + padding.margins.bottom;
            else
                connectionItemBase.height + padding.margins.top + padding.margins.bottom;

    PlasmaCore.Svg {
        id: svgNetworkIcons;

        multipleImages: true;
        imagePath: "icons/network";
    }

    PlasmaCore.FrameSvgItem {
        id: padding

        anchors.fill: parent

        imagePath: "widgets/viewitem"
        prefix: "hover"
        opacity: 0
    }

    Item {
        id: connectionItemBase;

        anchors {
            left: parent.left;
            right: parent.right;
            top: parent.top;
        }

        height: Math.max(connectionSvgIcon.height, connectionNameLabel.height + connectionStatusLabel.height);

        PlasmaCore.SvgItem {
            id: connectionSvgIcon;

            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }

            height: iconSize;
            width: height;
            elementId: ConnectionIcon;
            svg: svgNetworkIcons;
        }

        PlasmaComponents.Label {
            id: connectionNameLabel;

            anchors {
                left: connectionSvgIcon.right;
                leftMargin: padding.margins.left;
                right: stateChangeButton.left;
                bottom: connectionSvgIcon.verticalCenter
            }

            height: paintedHeight;
            elide: Text.ElideRight;
            font.weight: ConnectionState == PlasmaNM.Enums.Activated ? Font.DemiBold : Font.Normal;
            font.italic: ConnectionState == PlasmaNM.Enums.Activating ? true : false;
            text: ItemUniqueName;

        }

        PlasmaComponents.Label {
            id: connectionStatusLabel;

            anchors {
                left: connectionSvgIcon.right;
                leftMargin: padding.margins.left;
                right: stateChangeButton.left;
                top: connectionNameLabel.bottom;
            }

            height: paintedHeight;
            elide: Text.ElideRight;
            font.pointSize: theme.smallestFont.pointSize;
            opacity: 0.6
            text: itemText();
        }

        PlasmaComponents.BusyIndicator {
            id: connectingIndicator;

            anchors {
                right: stateChangeButton.left;
                rightMargin: padding.margins.right;
                verticalCenter: parent.verticalCenter;
            }

            height: iconSize;
            width: height;
            running: ConnectionState == PlasmaNM.Enums.Activating;
            visible: running;
        }

        PlasmaComponents.Button {
            id: stateChangeButton;

            anchors {
                right: parent.right;
                rightMargin: padding.margins.right;
                verticalCenter: parent.verticalCenter;
            }

            text: if (ConnectionState == PlasmaNM.Enums.Deactivated)
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
            property alias password: passwordInput.text;
            property alias passwordFocus: passwordInput

            height: childrenRect.height + padding.margins.top;

            PlasmaCore.SvgItem {
                id: passwordSeparator;

                anchors {
                    left: parent.left;
                    right: parent.right;
                    top: parent.top;
                }

                height: lineSvg.elementSize("horizontal-line").height;
                width: parent.width;
                elementId: "horizontal-line";

                svg: PlasmaCore.Svg {
                    id: lineSvg;
                    imagePath: "widgets/line";
                }
            }

            PlasmaComponents.TextField {
                id: passwordInput;

                anchors {
                    horizontalCenter: parent.horizontalCenter;
                    top: passwordSeparator.bottom;
                    topMargin: padding.margins.top;
                }

                height: implicitHeight;
                width: 200;
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
