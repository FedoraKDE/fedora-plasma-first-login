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

#include "view.h"
#include "applet.h"

#include <QResizeEvent>
#include <QTimer>

#include <Plasma/Containment>

#include <KDebug>

View::View(QWidget* parent)
    : Plasma::View(0, parent)
    , mApplet(0)
{
    setTrackContainmentChanges(true);
    setWallpaperEnabled(false); // wallpaper causes bad side effects for the navigation links :/ and look better imo w/o
    setWindowTitle(i18n("Fedora Plasma first login wizard"));

    setScene(&mCorona);
    setContainment(mCorona.addContainment(QLatin1String("image"), QVariantList()));

    QTimer::singleShot(0, this, SLOT(delayedInit()));
}

View::~View()
{
}

void View::resizeEvent(QResizeEvent* event)
{
    kDebug() << event->oldSize() << event->size();
    containment()->resize(event->size());
    if (mApplet) {
        mApplet->resize(event->size());
    }
}

void View::delayedInit()
{
    mApplet = new Applet(containment());
    containment()->addApplet(mApplet, QPointF(0, 0));
    mApplet->resize(size());
    mApplet->init();
    mApplet->show();
    mApplet->installEventFilter(this);

    installEventFilter(mApplet);

    containment()->setImmutability(Plasma::SystemImmutable);
    mApplet->setImmutability(Plasma::SystemImmutable);
}

bool View::eventFilter(QObject* object, QEvent* event)
{
    // HACK: Capture move and context menu events for the applet to prevent the
    // applet from being draggable and showing context menu
    if (object == mApplet && (event->type() == QEvent::GraphicsSceneMove ||
                              event->type() == QEvent::GraphicsSceneMouseMove ||
                              event->type() == QEvent::GraphicsSceneContextMenu)) {
        return true;
    }

    return QObject::eventFilter(object, event);
}
