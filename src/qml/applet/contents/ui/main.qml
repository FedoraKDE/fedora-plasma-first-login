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

App
{
    wizard: Wizard {

        pages: [

            PageInfo {
                title: i18n("Welcome");
                source: "WelcomePage.qml";
                status: 1; // FIXME: This should be generic
            },
            PageInfo {
                title: i18n("Language");
                source: "LanguagePage.qml";
            },

            PageInfo {
                title: i18n("Region");
                source: "RegionPage.qml";
            },

            PageInfo {
                title: i18n("Color Theme");
                source: "ColorThemePage.qml";
            },

            PageInfo {
                title: i18n("Network Settings");
                source: "NetworkPage.qml";
            },

            PageInfo {
                title: i18n("User Details");
                source: "UserPage.qml"
            }

        ]

        Component.onCompleted: {
            console.log("Wizard::onCompleted");
        }
    }
}
