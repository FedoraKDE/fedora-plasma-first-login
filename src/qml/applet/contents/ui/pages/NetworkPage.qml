/*
 * Copyright (C) 2014 Jan Grulich <jgrulich@redhat.com>
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
import QtQuick.Layouts 1.1
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.networkmanagement 0.2 as PlasmaNM
import org.fedoraproject.kde.FirstLogin 1.0
import ".."

Page
{
    id: page;

    PlasmaNM.Handler {
        id: handler;
    }

    PlasmaNM.NetworkModel {
        id: connectionModel;
    }

    PlasmaNM.AppletProxyModel {
        id: appletProxyModel;

        sourceModel: connectionModel;
    }

    PlasmaComponents.Label {
        id: label;
        anchors {
            top: parent.top;
            left: parent.left;
            right: parent.right;
        }
        wrapMode: Text.WordWrap;
        textFormat: Text.StyledText;
        verticalAlignment: Text.AlignJustify;
        text: i18n("<p>Below you can see the list of available networks to connect to.</p><br/>" +
                   "<p><em>Note:</em> If no wireless connections show up, make sure to enable the Wifi kill switch. </p>");
    }

    ListView {
        id: networkListView;

        anchors {
            top: label.bottom;
            bottom: parent.bottom;
            left: parent.left;
            right: parent.right;
            topMargin: 15;
            bottomMargin: 15;
        }

        boundsBehavior: Flickable.StopAtBounds;
        clip: true;
        currentIndex: -1;
        model: appletProxyModel;

        delegate: ConnectionItem {
            onStateChanged: {
                if (state == "expanded") {
                    networkListView.currentIndex = index;
                }
            }

            onClicked: {
                networkListView.currentIndex = index;
            }
        }
    }

    PlasmaComponents.SectionScroller {
        listView: networkListView;
    }
}
