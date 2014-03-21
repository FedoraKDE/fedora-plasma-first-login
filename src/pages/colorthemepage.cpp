/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 * Copyright (C) 2014  Lukáš Tinkl <ltinkl@redhat.com>
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
#include <QButtonGroup>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneResizeEvent>
#include <QDebug>
#include <QTimer>

ColorThemePage::ColorThemePage()
    : Page()
    , mImageView(0)
    , mLight(KGlobal::dirs()->findResource("data", QLatin1String("fedora-plasma-first-login/theme-light.png")))
    , mDark(KGlobal::dirs()->findResource("data", QLatin1String("fedora-plasma-first-login/theme-dark.png")))
    , mAnimation(Plasma::Animator::create(Plasma::Animator::PixmapTransitionAnimation, this))
    , mAnimationTimer(new QTimer(this))
{
    mAnimation->setLoopCount(1);
    mAnimation->setProperty("duration", 500);
    mAnimation->setProperty("usesCache", true);
    mAnimationTimer->setInterval(50);
    connect(mAnimationTimer, SIGNAL(timeout()),
            this, SLOT(onAnimationProgressed()));
    connect(mAnimation, SIGNAL(finished()),
            this, SLOT(onAnimationFinished()));

    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(10);
    setLayout(layout);

    Plasma::Label* label = new Plasma::Label(this);
    label->setText(i18n("<p>Select your prefered Plasma and color scheme using the buttons below.</p>"
                        "<p>The preview image will adjust accordingly as you hover over those buttons.</p>"));
    layout->addItem(label);

    mImageView = new Plasma::Label(this);
    mImageView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mImageView->nativeWidget()->setAlignment(Qt::AlignHCenter);
    layout->addItem(mImageView);
    layout->setAlignment(mImageView, Qt::AlignHCenter);

    QButtonGroup *buttonGroup = new QButtonGroup(this);

    QGraphicsLinearLayout* buttonsLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    KIcon icon(KGlobal::dirs()->findResource("data", QLatin1String("fedora-plasma-first-login/theme-light-btn.png")));
    mLightButton = new HoverButton(this);
    mLightButton->setIcon(icon);
    mLightButton->setText(i18nc("@title:button", "Light Theme"));
    mLightButton->nativeWidget()->setAutoExclusive(true);
    mLightButton->setCheckable(true);
    mLightButton->setChecked(true);
    mLightButton->setMaximumHeight(50);
    connect(mLightButton, SIGNAL(mouseEntered()), this, SLOT(onMouseEnteredButton()));
    connect(mLightButton, SIGNAL(mouseLeft()), this, SLOT(onMouseLeftButton()));
    buttonsLayout->addItem(mLightButton);
    buttonGroup->addButton(mLightButton->nativeWidget());

    icon = KIcon(KGlobal::dirs()->findResource("data", QLatin1String("fedora-plasma-first-login/theme-dark-btn.png")));
    mDarkButton = new HoverButton(this);
    mDarkButton->setIcon(icon);
    mDarkButton->setText(i18nc("@title:button", "Dark Theme"));
    mDarkButton->nativeWidget()->setAutoExclusive(true);
    mDarkButton->setCheckable(true);
    mDarkButton->setMaximumHeight(50);
    connect(mDarkButton, SIGNAL(mouseEntered()), this, SLOT(onMouseEnteredButton()));
    connect(mDarkButton, SIGNAL(mouseLeft()), this, SLOT(onMouseLeftButton()));
    buttonsLayout->addItem(mDarkButton);
    buttonGroup->addButton(mDarkButton->nativeWidget());
    layout->addItem(buttonsLayout);

    updatePixmap(mLight, Qt::SmoothTransformation);
}

ColorThemePage::~ColorThemePage()
{
}

void ColorThemePage::commitChanges()
{
    mAnimation->stop();
    // TODO save the changes
}

void ColorThemePage::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    qDebug() << "resize event";
    Page::resizeEvent(event);

    updatePixmap(mLightButton->isChecked() ? mLight : mDark, Qt::SmoothTransformation);
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

    mAnimation->stop();
    mAnimation->setProperty("startPixmap", currentPixmap);
    mAnimation->setProperty("targetPixmap", targetPixmap);
    mAnimation->start();
    mAnimationTimer->start();
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

    mAnimation->stop();
    mAnimation->setProperty("startPixmap", currentPixmap);
    mAnimation->setProperty("targetPixmap", targetPixmap);
    mAnimation->start();
    mAnimationTimer->start();
}

void ColorThemePage::onAnimationFinished()
{
    mAnimationTimer->stop();
    updatePixmap(mAnimation->property("targetPixmap").value<QPixmap>(),
                 Qt::SmoothTransformation);
}

void ColorThemePage::onAnimationProgressed()
{
    updatePixmap(mAnimation->property("currentPixmap").value<QPixmap>(),
                 Qt::FastTransformation);
}


void ColorThemePage::updatePixmap(const QPixmap &pixmap, Qt::TransformationMode mode)
{
    const QPixmap scaled = pixmap.scaled(mImageView->nativeWidget()->size(),
                                         Qt::KeepAspectRatio, mode);
    mImageView->nativeWidget()->setPixmap(scaled);
}
