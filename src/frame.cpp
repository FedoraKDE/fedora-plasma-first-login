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

#include "frame.h"

#include <Plasma/FrameSvg>
#include <QPainter>

Frame::Frame(QGraphicsWidget* parent)
    : QGraphicsWidget(parent)
{
    mBackground = new Plasma::FrameSvg(this);
    mBackground->setImagePath(QLatin1String("widgets/button"));
    mBackground->setCacheAllRenderedFrames(true);
    mBackground->setElementPrefix(QLatin1String("normal"));

    connect(mBackground, SIGNAL(repaintNeeded()), this, SLOT(syncBorders()));
    syncFrame();
}

Frame::~Frame()
{
}

void Frame::syncBorders()
{
    qreal left, top, right ,bottom;
    mBackground->getMargins(left, top, right, bottom);
    setContentsMargins(left, top, right, bottom);
}

void Frame::syncFrame()
{
    mBackground->resizeFrame(size());
}

void Frame::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    syncFrame();

    QGraphicsWidget::resizeEvent(event);
}

void Frame::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPixmap bufferPixmap(rect().size().toSize());
    bufferPixmap.fill(Qt::transparent);

    QPainter buffPainter(&bufferPixmap);
    mBackground->paintFrame(&buffPainter);
    buffPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    buffPainter.fillRect(bufferPixmap.rect(), QColor(0, 0, 0, 128));
    painter->drawPixmap(0, 0, bufferPixmap);
}
