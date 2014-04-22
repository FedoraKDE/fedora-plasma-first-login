/*
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

import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import Qt.labs.folderlistmodel 2.1

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Window  {
    id: chgFaceDlg;
    title: i18n("Change your user picture");
    modality: Qt.ApplicationModal;
    flags: Qt.Dialog;
    width: 400; height: 320;
    color: theme.backgroundColor;

    // intro label
    PlasmaComponents.Label {
        id: label;

        anchors {
            left: parent.left;
            right: parent.right;
            top: parent.top;
            margins: 4;
        }

        wrapMode: Text.WordWrap;
        textFormat: Text.StyledText;
        verticalAlignment: Text.AlignJustify;
        text: i18n("<p>Select your user picture below:</p>");
    }

    // img canvas
    GridView {
        id: gridView;
        width: parent.width;
        flow: GridView.LeftToRight;
        boundsBehavior: Flickable.StopAtBounds;
        focus: true;
        clip: true;

        anchors {
            left: parent.left;
            right: parent.right;
            top: label.bottom;
            bottom: buttonRow.top;
            margins: 4;
        }

        FolderListModel {
            id: folderModel;
            nameFilters: ["*.jpg", "*.jpeg", "*.png"];
            showDirs: false;
            showFiles: true;
            showOnlyReadable: true;
            sortField: FolderListModel.Name;
            folder: "/usr/share/pixmaps/faces/" // TODO export the right directory from C++
        }

        model: folderModel

        delegate: Column  {
            Image {
                id: fileImg;
                source: fileURL;
                width: 80;
                height: 80;
                fillMode: Image.PreserveAspectFit;
            }

            Text {
                id: fileText;
                text: fileBaseName;
                anchors.horizontalCenter: parent.horizontalCenter;
            }
        }

        highlight: Rectangle {
            width: gridView.cellWidth; height: gridView.cellHeight;
            color: theme.highlightColor; radius: 5
            x: gridView.currentItem.x
            y: gridView.currentItem.y
            Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
            Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
        }

        MouseArea {
            anchors.fill: parent;
            acceptedButtons: Qt.LeftButton;
            cursorShape: Qt.PointingHandCursor;
            onClicked: {
                gridView.currentIndex = gridView.indexAt(mouse.x, mouse.y); // FIXME relative coords
            }
        }
    }

    // buttons
    RowLayout {
        id: buttonRow;
        anchors {
            left: parent.left;
            right: parent.right;
            bottom: parent.bottom;
            margins: 4;
        }

        Rectangle {
            Layout.fillWidth: true;
            width: 1;
        }

        PlasmaComponents.Button {
            id: okButton;
            enabled: folderModel.currentIndex != -1;

            text: i18n("OK");
            onClicked: close();
        }

        PlasmaComponents.Button {
            id: cancelButton;

            text: i18n("Cancel");
            onClicked: close();
        }
    }

    function open() {
        chgFaceDlg.visible = true;
    }

    function close() {
        chgFaceDlg.visible = false;
    }
}
