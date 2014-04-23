/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 * Copyright (C) 2014  Lukáš Tinkl <ltinkl@redhat.com>
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

#include "user.h"

#include <QDBusConnection>
#include <QDBusReply>
#include <QDebug>
#include <QFile>
#include <QDir>

#include <unistd.h>
#include <sys/types.h>

User::User(QObject* parent)
    : QObject(parent)
    , mUser(KUser(getuid()))
    , mAccountManager(QLatin1String("org.freedesktop.Accounts"),
                      QLatin1String("/org/freedesktop/Accounts"),
                      QLatin1String("org.freedesktop.Accounts"),
                      QDBusConnection::systemBus(), this)
    , mUserIface(0)
{
    QDBusReply<QDBusObjectPath> reply = mAccountManager.asyncCall(QLatin1String("FindUserById"),
                                                                  static_cast<qint64>(mUser.userId().nativeId()));
    if (reply.isValid()) {
        const QString userPath = reply.value().path();
        mUserIface = new QDBusInterface(QLatin1String("org.freedesktop.Accounts"),
                                        userPath,
                                        QLatin1String("org.freedesktop.Accounts.User"),
                                        QDBusConnection::systemBus(), this);
    } else {
        qWarning() << "Failed to find AccountService:" << reply.error().message();
    }

    mEmail.setProfile(mEmail.defaultProfileName());
}

void User::setAvatarPath(const QString& avatarPath)
{
    if (mUserIface) {
        mUserIface->asyncCall(QLatin1String("SetIconFile"), avatarPath);
    }

    Q_EMIT avatarChanged();
}

bool User::removeAvatar()
{
    QFile::remove(QDir::homePath() + QLatin1String("/.face"));
    return QFile::remove(QDir::homePath() + QLatin1String("/.face.icon"));
}

bool User::copyAvatar(const QString &newAvatar)
{
    removeAvatar();
    QFile::copy(newAvatar, QDir::homePath() + QLatin1String("/.face")); // gdm compat :S
    return QFile::copy(newAvatar, QDir::homePath() + QLatin1String("/.face.icon"));
}

QString User::avatarPath() const
{
    return mUser.faceIconPath();
}

void User::setEmail(const QString& email)
{
    mEmail.setSetting(KEMailSettings::EmailAddress, email);
    if (mUserIface) {
        mUserIface->asyncCall(QLatin1String("SetEmail"), email);
    }
}

QString User::email() const
{
    return mEmail.getSetting(KEMailSettings::EmailAddress);
}

void User::setFullName(const QString& fullName)
{
    mEmail.setSetting(KEMailSettings::RealName, fullName);
    if (mUserIface) {
        mUserIface->asyncCall(QLatin1String("SetRealName"), fullName);
    }
}

QString User::fullName() const
{
    // 1st try KEMailSettings, then KUser
    const QString result = mEmail.getSetting(KEMailSettings::RealName);
    if (result.isEmpty()) {
        return mUser.property(KUser::FullName).toString();
    }

    return result;
}

void User::setOrganization(const QString& organization)
{
    mEmail.setSetting(KEMailSettings::Organization, organization);
}

QString User::organization() const
{
    return mEmail.getSetting(KEMailSettings::Organization);
}

void User::setLocation(const QString& location)
{
    if (mUserIface) {
        mUserIface->asyncCall(QLatin1String("SetLocation"), location);
    }
}

QString User::location() const
{
    if (mUserIface) {
        QDBusReply<QString> reply = mUserIface->asyncCall(QLatin1String("Location"));
        if (reply.isValid()) {
            return reply;
        }
    }

    return QString();
}
