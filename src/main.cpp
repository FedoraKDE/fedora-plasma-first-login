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

#include <KAboutData>
#include <KApplication>
#include <KLocalizedString>
#include <KCmdLineArgs>
#include <KIcon>

#include "view.h"

int main(int argc, char **argv)
{
    KAboutData about("fedora-plasma-first-login",
                     "fedora-plasma-first-login",
                     ki18n("Fedora Plasma First Login Guide"),
                     QByteArray(FPFL_VERSION),
                     ki18n("First Login wizard that will guide user through initial personalization of their profile"),
                     KAboutData::License_GPL_V2,
                     ki18n("Copyright (c) 2014 Fedora Plasma hackers"), // Copyright
                     KLocalizedString(), // Other text
                     "http://fedoraproject.org/wiki/Fedora_Plasma_Product",
                     QByteArray()); // TODO: mail for submitting bugs to RHBZ
    KCmdLineArgs::init(argc, argv, &about);

    KApplication app;
    app.setWindowIcon(KIcon(QLatin1String("start-here-kde-fedora")));
    app.setOrganizationName(QLatin1String("Fedora Plasma"));
    app.setOrganizationDomain(QLatin1String("org.fedoraproject.kde"));

    View view;
    view.resize(800, 600);
    view.show();

    return app.exec();
}
