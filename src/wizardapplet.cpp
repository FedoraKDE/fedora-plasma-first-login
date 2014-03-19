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

#include "wizardapplet.h"
#include "wizardpage.h"
#include "sidewidget.h"
#include "frame.h"

#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QGraphicsGridLayout>
#include <QGraphicsSceneResizeEvent>

#include <Plasma/PushButton>
#include <Plasma/Frame>
#include <Plasma/Label>
#include <Plasma/Theme>
#include <Plasma/SvgWidget>

#include <KStandardDirs>

WizardApplet::WizardApplet(QGraphicsItem* parent)
    : Plasma::Applet(parent, QString(), -1)
    , mBackgroundSvg(0)
{
    setBackgroundHints(StandardBackground);
    setHasConfigurationInterface(false);

    connect(&mWizard, SIGNAL(currentIdChanged(int)),
            this, SLOT(wizardPageChanged(int)));
}

WizardApplet::~WizardApplet()
{
}

void WizardApplet::init()
{
    QGraphicsGridLayout *contentLayout = new QGraphicsGridLayout;
    setLayout(contentLayout);

    // Main content
    {
        SideWidget *sideWidget = new SideWidget(&mWizard, this);
        contentLayout->addItem(sideWidget, 0, 0, 2, 1);

        Frame *frame = new Frame(this);
        frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        contentLayout->addItem(frame, 0, 1, 1, 1);

        mContentLayout = new QGraphicsLinearLayout(Qt::Vertical);
        //contentLayout->addItem(mContentLayout, 0, 1, 1, 1);
        frame->setLayout(mContentLayout);

        mPageTitle = new Plasma::Label(this);
        Plasma::Theme* theme = Plasma::Theme::defaultTheme();
        QFont font = theme->font(Plasma::Theme::DesktopFont);
        font.setBold(true);
        font.setPixelSize(35);
        mPageTitle->nativeWidget()->setFont(font);
        mContentLayout->addItem(mPageTitle);
    }

    // Bottom buttons
    {
        QGraphicsLinearLayout* buttonsLayout = new QGraphicsLinearLayout(Qt::Horizontal);
        buttonsLayout->setMaximumHeight(50);
        buttonsLayout->setMinimumHeight(50);
        contentLayout->addItem(buttonsLayout, 1, 1, 1, 1);


        mPrevButton = new Plasma::PushButton(this);
        mPrevButton ->setText(i18n("&Previous"));
        mPrevButton ->setMaximumHeight(30);
        buttonsLayout->addItem(mPrevButton );
        buttonsLayout->setAlignment(mPrevButton , Qt::AlignVCenter);
        connect(mPrevButton, SIGNAL(clicked()),
                &mWizard, SLOT(back()));

        buttonsLayout->addStretch(1);

        mNextButton = new Plasma::PushButton(this);
        mNextButton->setText(i18n("&Next"));
        mNextButton->setMaximumHeight(30);
        buttonsLayout->addItem(mNextButton);
        buttonsLayout->setAlignment(mNextButton, Qt::AlignVCenter);
        connect(mNextButton, SIGNAL(clicked()),
                this, SLOT(slotNextPage()));
    }

    mWizard.init();
}

void WizardApplet::wizardPageChanged(int id)
{
    mNextButton->setEnabled(mWizard.nextId() != -1);
    mPrevButton->setEnabled(id != mWizard.startId());

    WizardPage* page = qobject_cast<WizardPage*>(mWizard.page(id));

    if (mContentLayout->count() > 1) {
        dynamic_cast<QGraphicsWidget*>(mContentLayout->itemAt(1))->hide();
        mContentLayout->removeAt(1);
    }
    mPageTitle->setText(page->title());
    mContentLayout->addItem(page->rootWidget());
    dynamic_cast<QGraphicsWidget*>(page->rootWidget())->show();
    mContentLayout->setStretchFactor(mContentLayout->itemAt(1), 100);
}

void WizardApplet::slotNextPage()
{
    WizardPage * currentPage = qobject_cast<WizardPage *>(mWizard.currentPage());
    if (currentPage) {
        if (currentPage->isComplete()) {
            currentPage->commitChanges();
        }
        mWizard.next();
    }
}
