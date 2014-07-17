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
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras
import ".."

Page
{
    PlasmaExtras.Title {
        id: title;
        text: i18n("Welcome to Fedora Plasma!");
        anchors {
            verticalCenter: parent.verticalCenter;
        }
    }

    PlasmaExtras.Paragraph {
        id: info;
        text: i18n("This wizard will guide you through several steps to personalize your workspace.");
        anchors {
            top: title.bottom;
        }
    }

    PlasmaExtras.Paragraph {
        id: note;
        text: i18n("You can skip any step and configure it later from System Settings. Alternatively, you can start this wizard again " +
                   "from Application Launcher.");
        verticalAlignment: Text.AlignJustify;
        anchors {
            top: info.bottom;
        }
    }
}
