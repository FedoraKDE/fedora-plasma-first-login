/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 * Copyright (C) 2014  Lukáš Tinkl <ltink@redhat.com>
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
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.fedoraproject.kde.FirstLogin 1.0
import ".."

Page
{
    id: page;

    PlasmaExtras.Title {
        id: title;
        text: i18n("Color Theme");
    }

    PlasmaExtras.Paragraph {
        id: info;
        text: i18n("Select your prefered Plasma and color scheme using the buttons below.");
        anchors {
            top: title.bottom;
        }
    }

    PlasmaExtras.Paragraph {
        id: note;
        text: i18n("The preview image will adjust accordingly as you hover over those buttons.");
        anchors {
            top: info.bottom;
        }
    }

    Item {
        anchors {
            left: parent.left;
            right: parent.right;
            top: note.bottom;
            bottom: buttonRow.top;
            bottomMargin: 20;
            topMargin: 20;
        }

        Image {
            id: lightThemeImage;
            source: StandardPaths.appData("theme-light.png");
            anchors.fill: parent;
            fillMode: Image.PreserveAspectFit;
            smooth: true;
        }

        Image {
            id: darkThemeImage;
            source: StandardPaths.appData("theme-dark.png");
            anchors.fill: parent;
            fillMode: Image.PreserveAspectFit;
            smooth: true;
            opacity: 0.0;

            Behavior on opacity {
                PropertyAnimation {
                    duration: 250;
                }
            }
        }
    }

    PlasmaComponents.ButtonRow {
        id: buttonRow;

        exclusive: true;

        anchors {
            left: parent.left;
            right: parent.right;
            bottom: parent.bottom;
        }

        PlasmaComponents.Button {
            id: lightThemeButton;

            minimumWidth: buttonRow.width / 2 - buttonRow.spacing / 2;

            text: i18n("Light Theme");
            iconSource: StandardPaths.appData("theme-light-btn.png");
            checkable: true;

            MouseArea {
                anchors.fill: parent;

                hoverEnabled: true;
                propagateComposedEvents: true;
                onEntered: {
                    darkThemeImage.opacity = 0.0;
                }
                onExited: {
                    darkThemeImage.opacity = lightThemeButton.checked ? 0.0 : 1.0;
                }
            }
        }

        PlasmaComponents.Button {
            id: darkThemeButton;

            minimumWidth: buttonRow.width / 2 - buttonRow.spacing / 2;

            text: i18n("Dark Theme");
            iconSource: StandardPaths.appData("theme-dark-btn.png");
            checkable: true;

            MouseArea {
                anchors.fill: parent;

                hoverEnabled: true;
                propagateComposedEvents: true;
                onEntered: {
                    darkThemeImage.opacity = 1.0;
                }
                onExited: {
                    darkThemeImage.opacity = darkThemeButton.checked ? 1.0 : 0.0;
                }
            }
        }

        onCheckedButtonChanged: {
            page.pageInfo.configure("colorTheme", checkedButton == lightThemeButton ? "light" : "dark");
        }
    }
}
