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
import QtQuick.Dialogs 1.1
import Qt.labs.folderlistmodel 2.1
import org.fedoraproject.kde.FirstLogin 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Window  {
    id: chgFaceDlg;
    title: i18n("Change your user picture");
    modality: Qt.ApplicationModal;
    flags: Qt.Dialog;
    width: 500; height: 400;
    color: theme.backgroundColor;

    property bool isAccepted: false;

    signal accepted();
    signal rejected();

    onVisibilityChanged: {
        if (visibility == 0 /*QWindow.Hidden*/) {
            if (isAccepted)
                accepted();
            else
                rejected();
        }
    }

    User {
        id: user;
    }

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

        Keys.onEscapePressed: {
            event.accepted = true;
            //console.log("ESC KEY PRESSED " + event.key);
            isAccepted = false;
            close();
        }

        FolderListModel {
            id: folderModel;
            nameFilters: ["*.jpg", "*.jpeg", "*.png"];
            showDirs: false;
            showFiles: true;
            showOnlyReadable: true;
            sortField: FolderListModel.Name;
            folder: StandardPaths.avatarsDir();
        }

        model: folderModel

        delegate: Column  {
            Image {
                id: fileImg;
                source: filePath;
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

        PlasmaComponents.Button {
            id: removeButton;

            text: i18n("Remove");
            onClicked: {
                user.removeAvatar();
                isAccepted = true;
                close();
            }
        }

        PlasmaComponents.Button {
            id: customButton;

            text: i18n("Custom Image...");
            onClicked: {
                fileDialog.open();
            }
        }

        Rectangle {
            Layout.fillWidth: true;
            width: 1;
        }

        PlasmaComponents.Button {
            id: okButton;
            enabled: folderModel.currentIndex != -1;

            text: i18n("OK");
            onClicked: {
                saveFaceImage();
                isAccepted = true;
                close();
            }
        }

        PlasmaComponents.Button {
            id: cancelButton;

            text: i18n("Cancel");
            onClicked: {
                isAccepted = false;
                close();
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: i18n("Please choose an image");
        nameFilters: ["*.jpg", "*.jpeg", "*.png"];
        folder: "/usr/share/pixmaps/faces/";
        selectMultiple: false;
        selectExisting: true;
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrl);
            addExtraAvatar(fileDialog.fileUrl);
            isAccepted = true;
            close();
        }
        onRejected: {
            console.log("File dlg canceled");
            isAccepted = false;
            close();
        }
    }

    function saveFaceImage() {
        if (gridView.currentItem) {
            console.log("Current index: " + gridView.currentIndex);
            var newAvatarPath = folderModel.get(gridView.currentIndex, "filePath");
            console.log("Saving user avatar: " + newAvatarPath);
            user.copyAvatar(newAvatarPath);
        }
    }

    function addExtraAvatar(path) {
        console.log("Saving extra avatar: " + path);
        user.copyAvatar(path);
    }
}
