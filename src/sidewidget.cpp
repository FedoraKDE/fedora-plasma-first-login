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

#include "sidewidget.h"

#include "wizard.h"
#include "sidewidgetpagelabel.h"

#include <KStandardDirs>

#include <QGraphicsLinearLayout>
#include <QLabel>

#include <Plasma/Label>


const static int SideBarWidth = 200;

SideWidget::SideWidget(QGraphicsWidget* parent)
    : Plasma::Frame(parent)
    , mTitlesLayout(0)
    , mActivePage(-1)
{
    connect(Wizard::instance(), SIGNAL(currentPageChanged(int)),
            this, SLOT(onPageChanged(int)));

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    setMaximumWidth(SideBarWidth);
    setMinimumWidth(SideBarWidth);

    QGraphicsLinearLayout* frameLayout = new QGraphicsLinearLayout(Qt::Vertical);
    setLayout(frameLayout);
    frameLayout->setMinimumWidth(SideBarWidth);
    frameLayout->setMaximumWidth(SideBarWidth);

    Plasma::Label* label = new Plasma::Label(this);
    label->setImage(KGlobal::dirs()->findResource("xdgdata-icon", QLatin1String("hicolor/256x256/apps/fedora-logo-icon.png")));
    label->nativeWidget()->setScaledContents(true);
    label->setAlignment(Qt::AlignCenter);
    label->setMaximumSize(150, 150);
    frameLayout->addItem(label);
    frameLayout->setAlignment(label, Qt::AlignHCenter);

    mTitlesLayout = new QGraphicsLinearLayout(Qt::Vertical);
    frameLayout->addItem(mTitlesLayout);

    frameLayout->addStretch(2);

    for (int i = 0; i < Wizard::instance()->count(); ++i) {
        SideWidgetPageLabel* label = new SideWidgetPageLabel(Wizard::instance()->pageTitle(i), i, this);
        mTitleLabels.insert(i, label);
        mTitlesLayout->addItem(label);
    }
}

SideWidget::~SideWidget()
{
}

void SideWidget::onPageChanged(int id)
{
   if (mActivePage > -1) {
        SideWidgetPageLabel* activeLabel = mTitleLabels.value(mActivePage);
        if (mActivePage < id) {
            activeLabel->setState(SideWidgetPageLabel::Done);
        } else {
            activeLabel->setState(SideWidgetPageLabel::Default);
        }
    }

    SideWidgetPageLabel* label = mTitleLabels.value(id);
    if (label) {
        label->setState(SideWidgetPageLabel::Active);
    }

    mActivePage = id;
}

