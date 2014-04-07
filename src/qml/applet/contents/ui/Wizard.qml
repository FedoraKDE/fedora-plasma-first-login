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

Item {
    property list<Item> pages;

    property int currentPageId: 0;

    property bool isLastPage: true;

    function next()
    {
        pages[currentPageId].status = 2; // Done
        if (currentPageId < pages.length) {
            currentPageId++;
            pages[currentPageId].status = 1; // Active
            console.log("Wizard::next");
        }
    }

    function previous()
    {
        pages[currentPageId].status = 0; // Default
        if (currentPageId > 0) {
            currentPageId--;
            pages[currentPageId].status = 1; // Active
            console.log("Wizard::previous");
        }
    }

    onPagesChanged: {
        isLastPage = currentPageId >= pages.length - 1;
        console.log("Wizard::onPagesChanged: " + pages.length);
    }

    onCurrentPageIdChanged: {
        isLastPage = currentPageId >= pages.length - 1;
        console.log("Wizard::onCurrentPageIdChanged: " + currentPageId);
    }
}
