/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
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
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaCore.FrameSvgItem {
    id: sideBar;

    property Wizard wizard: null;

    imagePath: "widgets/frame";
    prefix: "raised";

    PlasmaCore.IconItem {
        id: icon;
        source: "fedora-logo-icon";
        active: true;
        smooth: true;

        width: units.iconSizes.enormous;
        height: units.iconSizes.enormous;

        anchors {
            top: parent.top;
            horizontalCenter: parent.horizontalCenter;
            margins: 20;
        }
    }

    Column {
        anchors {
            left: parent.left;
            right: parent.right;
            top: icon.bottom;
            topMargin: 50;
        }
        Repeater {
            model: wizard.pages.length;

            Item {
                anchors {
                    left: parent.left;
                    right: parent.right;
                    margins: 20;
                }

                height: 50;

                PlasmaCore.IconItem {
                    id: itemIcon;
                    source: wizard.pages[index].status == 1 ? "go-next" :
                            wizard.pages[index].status == 2 ? "dialog-ok-apply" :
                            wizard.pages[index].statis == 3 ? "dialog-ok" :
                            "";
                    width: units.iconSizes.medium;
                    anchors {
                        left: parent.left;
                        verticalCenter: parent.verticalCenter;
                    }
                }

                Text {
                    text: wizard.pages[index].title;
                    anchors {
                        left: itemIcon.right;
                        right: parent.right;
                        leftMargin: 20;
                        verticalCenter: parent.verticalCenter;
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        console.log("SidePanel::onCompleted");
    }
}
