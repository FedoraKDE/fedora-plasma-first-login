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

#ifndef TOOLINVOCATION_H
#define TOOLINVOCATION_H

#include <QObject>
#include <QtQml>

class ToolInvocationAttached: public QObject
{
    Q_OBJECT
  protected:
    explicit ToolInvocationAttached(QObject *parent = 0);

  public:
    Q_INVOKABLE void startDetached(const QString &cmdLine, const QStringList & args = QStringList());

  friend class ToolInvocation;
};

/**
 * A simplified wrapper around KToolInvocation.
 *
 * The wrapper is implemented as an attached property, which makes it possible
 * to use it as if its methods were static:
 *
 * @code
 * ToolInvocation.startServiceByDesktopName("kcm_keyboard");
 * @endcode
 */
class ToolInvocation: public QObject
{
    Q_OBJECT
  public:
    explicit ToolInvocation(QObject *parent = 0);

    static ToolInvocationAttached* qmlAttachedProperties(QObject *parent);
};

QML_DECLARE_TYPEINFO(ToolInvocation, QML_HAS_ATTACHED_PROPERTIES)

#endif // TOOLINVOCATION_H
