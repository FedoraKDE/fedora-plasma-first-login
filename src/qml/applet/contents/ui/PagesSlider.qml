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

PlasmaComponents.PageStack {
    id: root;

    property Wizard wizard: null;

    property int lastPageId: -1;

    Loader {
        id: pageLoader;

        property int pageId: -1;

        asynchronous: true;

        onLoaded: {
            pageLoaded(pageId, item);
        }
    }

    Component.onCompleted: {
        console.log("PagesSider::onCompleted");
        loadPage(0);
    }

    onWizardChanged: {
        wizard.currentPageIdChanged.connect(wizardPageChanged);
    }

    function wizardPageChanged()
    {
        console.log("PagesSlider::wizardPageChanged: " + wizard.currentPageId);
        loadPage(wizard.currentPageId);
    }

    function loadPage(pageId)
    {
        var pageFile = wizard.pages[pageId].source;
        pageLoader.pageId = pageId;
        pageLoader.source = "pages/" + pageFile;
    }

    function pageLoaded(pageId, page)
    {
        console.log("PagesSlider::pageLoaded: " + pageId);
        page.pageInfo = wizard.pages[pageId];
        root.push(page);
    }
}
