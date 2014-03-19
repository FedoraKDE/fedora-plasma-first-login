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

#include "colorthemepage.h"
#include "hoverbutton.h"

#include <KLocalizedString>
#include <KGlobal>
#include <KStandardDirs>
#include <KPushButton>

#include <Plasma/Label>
#include <Plasma/Animator>
#include <Plasma/Animation>

#include <QLabel>
#include <QResizeEvent>
#include <QButtonGroup>
#include <QGraphicsLinearLayout>

ColorThemePage::ColorThemePage(QWidget* parent)
    : WizardPage(parent)
    , mRootWidget(0)
    , mAnimation(Plasma::Animator::create(Plasma::Animator::PixmapTransitionAnimation, this))
{
    setTitle(i18nc("@title:tab", "Color Theme"));

    connect(mAnimation, SIGNAL(currentLoopChanged(int)),
            this, SLOT(updatePixmap()));
}

ColorThemePage::~ColorThemePage()
{
    delete mRootWidget;
}

void ColorThemePage::initializePage()
{
    if (mRootWidget) {
        return;
    }

    mLight = QPixmap(KGlobal::dirs()->findResource("data", QLatin1String("fedora-plasma-first-login/theme-light.png")));
    mDark =  QPixmap(KGlobal::dirs()->findResource("data", QLatin1String("fedora-plasma-first-login/theme-dark.png")));

    mRootWidget = new QGraphicsWidget;
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    mRootWidget->setLayout(layout);

    QImage img = mLight.toImage();
    mImageView = new Plasma::Label(mRootWidget);
    mImageView->nativeWidget()->setPixmap(mLight.scaled(500, 312 , Qt::KeepAspectRatio, Qt::SmoothTransformation));
    //mImageView->setScaledContents(true);
    layout->addItem(mImageView);

    QButtonGroup *buttonGroup = new QButtonGroup(this);

    QGraphicsLinearLayout* buttonsLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    KIcon icon(KGlobal::dirs()->findResource("data", QLatin1String("fedora-plasma-first-login/theme-light-btn.png")));
    mLightButton = new HoverButton(mRootWidget);
    mLightButton->setIcon(icon);
    mLightButton->setText(i18nc("@title:button", "Light Theme"));
    mLightButton->nativeWidget()->setAutoExclusive(true);
    mLightButton->setCheckable(true);
    mLightButton->setChecked(true);
    connect(mLightButton, SIGNAL(mouseEntered()), this, SLOT(onMouseEnteredButton()));
    connect(mLightButton, SIGNAL(mouseLeft()), this, SLOT(onMouseLeftButton()));
    buttonsLayout->addItem(mLightButton);
    buttonGroup->addButton(mLightButton->nativeWidget());

    icon = KIcon(KGlobal::dirs()->findResource("data", QLatin1String("fedora-plasma-first-login/theme-dark-btn.png")));
    mDarkButton = new HoverButton(mRootWidget);
    mDarkButton->setIcon(icon);
    mDarkButton->setText(i18nc("@title:button", "Dark Theme"));
    mDarkButton->nativeWidget()->setAutoExclusive(true);
    mDarkButton->setCheckable(true);
    connect(mDarkButton, SIGNAL(mouseEntered()), this, SLOT(onMouseEnteredButton()));
    connect(mDarkButton, SIGNAL(mouseLeft()), this, SLOT(onMouseLeftButton()));
    buttonsLayout->addItem(mDarkButton);
    buttonGroup->addButton(mDarkButton->nativeWidget());
    layout->addItem(buttonsLayout);
}

QGraphicsLayoutItem* ColorThemePage::rootWidget() const
{
    return mRootWidget;
}

void ColorThemePage::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    mImageView->nativeWidget()->setPixmap(mLight.scaled(event->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void ColorThemePage::onMouseEnteredButton()
{
    QPixmap currentPixmap, targetPixmap;
    if (sender() == mLightButton && mDarkButton->isChecked()) {
        targetPixmap = mLight;
        currentPixmap = mDark;
    } else if (sender() == mDarkButton && mLightButton->isChecked()) {
        targetPixmap = mDark;
        currentPixmap = mLight;
    } else {
        return;
    }

    mAnimation->setProperty("startPixmap", QVariant::fromValue(currentPixmap));
    mAnimation->setProperty("targetPixmap", QVariant::fromValue(targetPixmap));
    mAnimation->start();
}

void ColorThemePage::updatePixmap()
{
    if (!sender()) {
        return;
    }

    const QPixmap pixmap = sender()->property("currentPixmap").value<QPixmap>();
    mImageView->nativeWidget()->setPixmap(pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


void ColorThemePage::onMouseLeftButton()
{
    QPixmap currentPixmap, targetPixmap;
    if (sender() == mLightButton && mDarkButton->isChecked()) {
        targetPixmap = mDark;
        currentPixmap = mLight;
    } else if (sender() == mDarkButton && mLightButton->isChecked()) {
        targetPixmap = mLight;
        currentPixmap = mDark;
    } else {
        return;
    }

    mAnimation->setProperty("startPixmap", QVariant::fromValue(currentPixmap));
    mAnimation->setProperty("targetPixmap", QVariant::fromValue(targetPixmap));
    mAnimation->start();
}
