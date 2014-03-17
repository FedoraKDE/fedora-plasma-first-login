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

#include "wizardview.h"
#include "wizardapplet.h"

#include <QResizeEvent>
#include <QTimer>

#include <Plasma/Containment>

#include <KDebug>

WizardView::WizardView(QWidget* parent)
    : Plasma::View(0, parent)
    , mApplet(0)
{
    setTrackContainmentChanges(true);
    setWallpaperEnabled(true);

    setScene(&mCorona);
    setContainment(mCorona.addContainment(QLatin1String("image"), QVariantList()));

    QTimer::singleShot(0, this, SLOT(delayedInit()));
}

WizardView::~WizardView()
{
}

void WizardView::resizeEvent(QResizeEvent* event)
{
    kDebug() << event->oldSize() << event->size();
    containment()->resize(event->size());
    if (mApplet) {
        mApplet->resize(event->size());
    }
}

void WizardView::delayedInit()
{
    mApplet = new WizardApplet(containment());
    containment()->addApplet(mApplet, QPointF(0, 0));
    mApplet->resize(size());
    mApplet->init();
    mApplet->show();

    containment()->setImmutability(Plasma::SystemImmutable);
    mApplet->setImmutability(Plasma::SystemImmutable);
}
