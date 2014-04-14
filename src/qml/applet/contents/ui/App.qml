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
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    id: root;

    property Wizard wizard: null;

    SidePanel {
        id: sidePanel;
        wizard: root.wizard;

        width: 250;  // FIXME: percentage?
        anchors {
            left: parent.left;
            top: parent.top;
            bottom: parent.bottom;
        }
    }

    PagesSlider {
        id: pages;
        wizard: root.wizard;

        anchors {
            left: sidePanel.right;
            right: parent.right;
            top: parent.top;
            bottom: navigationPane.top;
        }
    }

    Item {
        id: navigationPane;

        anchors {
            left: sidePanel.right;
            right: parent.right;
            bottom: parent.bottom;
        }
        height: 50;

        PlasmaComponents.Button {
            id: previousButton;
            text: i18n("Previous");
            enabled: wizard.currentPageId > 0;
            anchors {
                left: parent.left;
                verticalCenter: parent.verticalCenter;
                margins: 20;
            }

            onClicked: wizard.previous();
        }

        PlasmaComponents.Button {
            id: nextButton;
            text: !wizard.isLastPage ? i18n("Next") : i18n("Finish");
            anchors {
                right: parent.right;
                verticalCenter: parent.verticalCenter;
                margins: 20;
            }

            onClicked: wizard.next();
        }
    }
}
