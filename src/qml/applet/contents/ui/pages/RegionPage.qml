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
import QtQuick.Layouts 1.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.fedoraproject.kde.FirstLogin 1.0
import ".."

Page
{
    id: page;

    PlasmaCore.DataSource {
        id: geolocationSource;
        engine: "geolocation";
        connectedSources: [ "location" ];

        onNewData: {
            if (sourceName != "location") {
                return;
            }

            var detectedLocation = data["city"] + ", " + data["country"];
            populateModel(detectedLocation);
        }

        Component.onCompleted: {
            if (sources.indexOf("location") == -1) {
                populateModel();
            }
        }
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
        text: i18n("<p>Select your region below. This will apply the region's settings globally in KDE.</p><br/>" +
                   "<p><em>Note:</em> If your country is auto-detected, it will be preselected.</p>");
    }

    ListView {
        id: regionsListView;

        boundsBehavior: Flickable.StopAtBounds;
        clip: true;
        currentIndex: -1;

        anchors {
            top: label.bottom;
            bottom: keyboardSetupButton.top;
            left: parent.left;
            right: parent.right;
            topMargin: 15;
            bottomMargin: 15;
        }

        section {
            property: "region";
            criteria: ViewSection.FirstCharacter;
        }

        PlasmaComponents.BusyIndicator {
            id: busyIndicator;

            anchors.centerIn: parent;
            running: true;
        }

        model: ListModel {
            id: model;
        }

        delegate: PlasmaComponents.ListItem {
            id: itemDelegate;

            height: 40;

            enabled: true;
            checked: ListView.isCurrentItem;

            property string countryCode: code;

            PlasmaCore.IconItem {
                id: itemIcon;

                source: flag;
                active: true;
                smooth: true;
                width: units.iconSizes.medium;
                height: units.iconSizes.medium;

                anchors {
                    left: parent.left;
                    verticalCenter: parent.verticalCenter;
                }
            }

            PlasmaComponents.Label {
                id: itemLabel;

                text: country;

                anchors {
                    left: itemIcon.right;
                    right: parent.right;
                    leftMargin: 20;
                    verticalCenter: parent.verticalCenter;
                }
            }

            onClicked: {
                if (ListView.isCurrentItem) {
                    ListView.currentIndex = -1;
                    if (page.pageInfo.changes["country"] == countryCode) {
                        page.pageInfo.configure("country", "");
                    }
                } else {
                    ListView.view.currentIndex = index;
                    page.pageInfo.configure("country", countryCode);
                }
            }
        }
    }

    PlasmaComponents.SectionScroller {
        listView: regionsListView;
    }

    PlasmaComponents.Button {
        id: keyboardSetupButton;

        anchors {
            bottom: parent.bottom;
            left: parent.left;
            right: parent.right;
        }

        text: i18n("Date & Time Settings...");
        iconSource: "preferences-desktop-time";

        onClicked: {
            // NOTE: This does nothing atm, because kcm_keyboard is not ported to Plasma.Next
            ToolInvocation.startServiceByDesktopName("clock");
        }
    }

    function populateModel(detectedRegion)
    {
        console.log("RegionPage::populateModel: detected region:" + detectedRegion);
        var index = 0;
        GlobalLocale.allCountriesList().map(function(code) {
            var name = GlobalLocale.countryCodeToName(code);
            return { "code": code,
                     "country":  name || code,
                     "flag": GlobalLocale.flagForCountry(code),
                   };
        })
        .sort(function(a, b) {
            return a.country.localeCompare(b.country);
        })
        .forEach(function(region) {
            model.append(region);
            if (region.country == detectedRegion) {
                regionsListView.currentIndex = index;
            }
            index++;
        });
        busyIndicator.running = false;
        busyIndicator.visible = false;
    }

    function addLanguagesToModel(langs, icon)
    {

    }
}
