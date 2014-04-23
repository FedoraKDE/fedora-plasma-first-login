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
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.1

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

    ChgFaceDlg {
        id: chFaceDlg;

        onAccepted: {
            console.log("User accepted dlg");
            btnImage.source = "";
            btnImage.source = user.avatarPath;
        }
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

        Button {
            implicitWidth: 96;
            implicitHeight: 96;
            tooltip: i18n("Click to change your user picture");

            Image {
                id: btnImage;
                cache: false;
                anchors.fill: parent;
                anchors.margins: 4;
                fillMode: Image.PreserveAspectFit;
                smooth: true;
                source: user.avatarPath;
            }

            Layout.columnSpan: 2;
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter;
            Layout.maximumHeight: 100;

            onClicked: {
                console.log("Initial avatar path:" + user.avatarPath);
                locationBusyIndicator.running = false; // stop the busy indicator to prevent a crash
                chFaceDlg.show();
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
            enabled: !locationBusyIndicator.running;

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
        text: i18n("Change Password...");
        iconSource: "preferences-desktop-user-password";

        anchors {
            left: parent.left;
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
