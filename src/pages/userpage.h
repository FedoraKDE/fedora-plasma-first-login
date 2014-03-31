/*
 * Copyright (C) 2014 Lukáš Tinkl <ltinkl@redhat.com>
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

#ifndef USERPAGE_H
#define USERPAGE_H

#include "../page.h"

#include <QDBusInterface>

#include <KUser>
#include <KEMailSettings>

namespace Plasma {
class TreeView;
class PushButton;
class LineEdit;
class CheckBox;
}

class QGraphicsWidget;

class UserPage : public Page
{
    Q_OBJECT
    Q_CLASSINFO("Title", I18N_NOOP("User Details"))

  public:
    UserPage();
    ~UserPage();

    void initializePage();
    void commitChanges();

    QString fullUserName() const;

  private Q_SLOTS:
    void changeAvatar();
    void changePassword();

  private:
    Plasma::LineEdit * leFullname;
    Plasma::LineEdit * leOrg;
    Plasma::LineEdit * leEmail;
    Plasma::LineEdit * leLocation;
    Plasma::PushButton* m_avatarBtn;
    Plasma::CheckBox *m_useLocationCB;
    KUser m_user;
    KEMailSettings m_email;
    QDBusInterface m_accountManager;
    QDBusInterface * m_userIface;
};

#endif // USERPAGE_H
