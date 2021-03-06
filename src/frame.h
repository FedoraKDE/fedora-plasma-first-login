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

#ifndef FRAME_H
#define FRAME_H

#include <QtGui/QGraphicsWidget>

namespace Plasma {
class FrameSvg;
}

class Frame : public QGraphicsWidget
{
    Q_OBJECT

  public:
    explicit Frame(QGraphicsWidget *parent = 0);
    ~Frame();

  protected:
    void resizeEvent(QGraphicsSceneResizeEvent* event);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

  private Q_SLOTS:
    void syncBorders();

  private:
    void syncFrame();

    Plasma::FrameSvg* mBackground;
};

#endif // FRAME_H
