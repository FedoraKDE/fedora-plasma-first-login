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

#ifndef USER_H
#define USER_H

#include <QObject>

#include <QDBusInterface>
#include <KUser>
#include <KEMailSettings>

class User : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString avatarPath
               READ avatarPath
               WRITE setAvatarPath
               NOTIFY avatarChanged)
    Q_PROPERTY(QString fullName
               READ fullName
               WRITE setFullName)
    Q_PROPERTY(QString organization
               READ organization
               WRITE setOrganization)
    Q_PROPERTY(QString email
               READ email
               WRITE setEmail)
    Q_PROPERTY(QString location
               READ location
               WRITE setLocation)

  public:
    explicit User(QObject *parent = 0);

    QString avatarPath() const;
    void setAvatarPath(const QString &avatarPath);
    Q_INVOKABLE bool removeAvatar();
    Q_INVOKABLE bool copyAvatar(const QString &newAvatar);

    QString fullName() const;
    void setFullName(const QString &fullName);

    QString organization() const;
    void setOrganization(const QString &organization);

    QString email() const;
    void setEmail(const QString &email);

    QString location() const;
    void setLocation(const QString &location);

Q_SIGNALS:
    void avatarChanged();

  private:
    KUser mUser;
    QDBusInterface mAccountManager;
    QDBusInterface *mUserIface;
    KEMailSettings mEmail;
};

#endif // USER_H
