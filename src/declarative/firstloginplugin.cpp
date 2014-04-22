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

#include "firstloginplugin.h"

#include <QtQml>

#include "toolinvocation.h"
#include "locales.h"
#include "standardpaths.h"
#include "user.h"
#include "chfacedlg.h"


void FirstLoginPlugin::registerTypes(const char* uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("org.fedoraproject.kde.FirstLogin"));

    qmlRegisterType<ToolInvocation>(uri, 1, 0, "ToolInvocation");
    // Prevent conflict with "Locale" type (????)
    qmlRegisterType<Locale>(uri, 1, 0, "GlobalLocale");
    qmlRegisterType<StandardPaths>(uri, 1, 0, "StandardPaths");
    qmlRegisterType<User>(uri, 1, 0, "User");
    //qmlRegisterType<ChFaceDlg>(uri, 1, 0, "ChFaceDlg");
}
