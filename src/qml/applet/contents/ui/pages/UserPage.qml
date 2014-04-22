/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 * Copyright (C) 2014  Lukáš Tinkl <ltinkl@redhat.com>
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
import QtQuick.Layouts 1.0
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.core 2.0 as PlasmaCore
import org.fedoraproject.kde.FirstLogin 1.0
import ".."

Page
{
    User {
        id: user;
    }

    PlasmaCore.DataSource {
        id: geolocationSource;
        engine: "geolocation";
        connectedSources: [ "location" ];

        onNewData: {
            if (sourceName != "location") {
                return;
            }

            var detectedLocation = data["city"] + ", " + data["country"];
            locationText.text = detectedLocation;
            locationBusyIndicator.running = false;
        }

        Component.onCompleted: {
            if (sources.indexOf("location") == -1) {
                locationText.text = detectedLocation;
                locationBusyIndicator.running = false;
            }
        }
    }

    ChFaceDlg {
        id: chFaceDlg;
    }

    PlasmaComponents.Label {
        id: label;

        anchors {
            left: parent.left;
            right: parent.right;
            top: parent.top;
        }
        wrapMode: Text.WordWrap;
        textFormat: Text.StyledText;
        verticalAlignment: Text.AlignJustify;
        text: i18n("<p>Customize your details below.</p>");
    }

    GridLayout {
        anchors {
            left: parent.left;
            right: parent.right;
            top: label.bottom;
            bottom: changePwButton.top;
            topMargin: 20;
            bottomMargin: 20;
        }

        rowSpacing: 30;
        columns: 2;

        PlasmaComponents.Button {
            iconSource: user.avatarPath;

            minimumWidth: 74;
            minimumHeight: 74;
            width: 74;
            height: 74;

            Layout.columnSpan: 2;
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
            Layout.maximumHeight: 80;

            onClicked: {
                chFaceDlg.exec();
                var str = user.avatarPath;
                user.avatarPath = str;
            }
        }



        PlasmaComponents.Label {
            text: i18n("Full Name:");

            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
        }

        PlasmaComponents.TextField {
            id: nameText;
            text: user.fullName;

            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
            Layout.fillWidth: true;
        }



        PlasmaComponents.Label {
            text: i18n("Organization:");

            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
        }

        PlasmaComponents.TextField {
            id: organizationText;
            text: user.organization;

            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
            Layout.fillWidth: true;
        }



        PlasmaComponents.Label {
            text: i18n("Email:");

            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
        }

        PlasmaComponents.TextField {
            id: emailText;
            text: user.email;

            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
            Layout.fillWidth: true;
        }



        PlasmaComponents.Label {
            text: i18n("Location:");
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
        }

        PlasmaComponents.TextField {
            id: locationText;
            enabled: !locationBusyIndicator.active;

            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter;
            Layout.fillWidth: true;

            PlasmaComponents.BusyIndicator {
                id: locationBusyIndicator;

                anchors.centerIn: parent;
                running: true;
                visible: running;
            }
        }

        Rectangle {
            Layout.fillHeight: true;
            width: 1;
        }
    }

    PlasmaComponents.Button {
        id: changePwButton;
        text: "Change Password...";
        iconSource: "preferences-desktop-user-password";

        anchors {
            left: parent.left;
            right: parent.right;
            bottom: parent.bottom;
        }

        onClicked: {
            ToolInvocation.startServiceByDesktopName("kdepasswd");
        }
    }

    function commitChanged()
    {
        user.fullName = nameText.text;
        user.email = emailText.text;
        user.organization = organizationText.text;
        user.location = locationText.text;
    }
}
