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

            var countryCode = data["country code"];
            var detectedLangs = GlobalLocale.languagesForCountry(countryCode);
            populateModel(detectedLangs);
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
        text: i18n("<p>Select your language below. This will switch language of all KDE applications.</p><br/>" +
                   "<p><em>Note:</em> If your language is auto-detected, it will be placed first in the list and preselected. " +
                    "If it's available but not installed currently, it will be automatically installed in the background.</p>");
    }

    ListView {
        id: languagesListView;

        clip: true;
        currentIndex: 0;

        anchors {
            top: label.bottom;
            bottom: keyboardSetupButton.top;
            left: parent.left;
            right: parent.right;
            topMargin: 15;
            bottomMargin: 15;
        }

        section {
            property: "language";
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

            property string languageCode: code;

            PlasmaCore.IconItem {
                id: itemIcon;

                source: icon;
                active: true;
                smooth: true;
                width: 24;
                height: 24;

                anchors {
                    left: parent.left;
                    verticalCenter: parent.verticalCenter;
                }
            }

            PlasmaComponents.Label {
                id: itemLabel;

                text: language;

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
                    if (page.pageInfo.changes["language"] == languageCode) {
                        page.pageInfo.configure("language", "");
                    }
                } else {
                    ListView.view.currentIndex = index;
                    page.pageInfo.configure("language", languageCode);
                }
            }
        }
    }

    PlasmaComponents.SectionScroller {
        listView: languagesListView;
    }
/*
    PlasmaComponents.ScrollBar {
        flickableItem: languagesListView;
        interactive :true;
    }
*/
    PlasmaComponents.Button {
        id: keyboardSetupButton;

        anchors {
            bottom: parent.bottom;
            left: parent.left;
            right: parent.right;
        }

        text: i18n("Setup Keyboard...");
        iconSource: "preferences-desktop-keyboard";

        onClicked: {
            // NOTE: This does nothing atm, because kcm_keyboard is not ported to Plasma.Next
            ToolInvocation.startServiceByDesktopName("kcm_keyboard");
        }
    }

    function populateModel(detectedLangs)
    {
        console.log("LanguagePage::populateModel: detected languages:" + detectedLangs);
        var allLangs = GlobalLocale.allLanguagesList();
        if (detectedLangs.length > 0) {
            addLanguagesToModel(detectedLangs, "favorites");
            var remainingLangs = allLangs.filter(function(lang) {
                    return (detectedLangs.indexOf(lang) == -1);
                });
            addLanguagesToModel(remainingLangs);
        } else {
            addLanguagesToModel(allLangs);
        }

        busyIndicator.running = false;
        busyIndicator.visible = false;
    }

    function addLanguagesToModel(langs, icon)
    {
        langs.map(function(code) {
            return { "code": code, "name":  GlobalLocale.languageCodeToName(code) };
        })
        .sort(function(a, b) {
            return a.name.localeCompare(b.name);
        })
        .forEach(function(language) {
            if (language.code == "xtest") {
                return;
            }

            model.append(
                { "icon": icon || "",
                  "language": language.name,
                  "code": language.code
                }
            );
        });
    }
}
