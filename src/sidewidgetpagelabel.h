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

#ifndef SIDEWIDGETPAGELABEL_H
#define SIDEWIDGETPAGELABEL_H

#include <QtGui/QGraphicsWidget>

namespace Plasma {
class Label;
}

class SideWidgetPageLabel : public QGraphicsWidget
{
    Q_OBJECT

  public:
    enum State {
        Default,
        Active,
        Done,
        Skipped
    };

    SideWidgetPageLabel(const QString& text, int pageId, QGraphicsItem* parent = 0);
    ~SideWidgetPageLabel();

    QString text() const;
    void setText(const QString& text);

    int pageId() const;

    State state() const;
    void setState(State state);

  private Q_SLOTS:
    void linkActivated(const QString & link);

  private:
    State mState;
    int mPageId;

    Plasma::Label* mImage;
    Plasma::Label* mText;
};

#endif // SIDEWIDGETPAGELABEL_H
