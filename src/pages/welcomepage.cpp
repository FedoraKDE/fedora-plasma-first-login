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

#include "welcomepage.h"

#include <KLocalizedString>
#include <KGlobalSettings>

#include <Plasma/Label>
#include <QtGui/QLabel>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsLinearLayout>

WelcomePage::WelcomePage()
    : Page()
{
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
    setLayout(layout);

    mLabel = new Plasma::Label(this);
    QFont font = KGlobalSettings::generalFont();
    font.setPointSize(16);
    mLabel->setFont(font);
    mLabel->setText(i18n("<h1>Welcome to Fedora Plasma!</h1>"
                       "<p>This wizard will guide you through several steps to personalize your workspace.</p>"
                       "<p>You can skip any step and configure it later from System Settings. Alternatively, you can start this wizard again "
                       "from Application Launcher.</p>"));
    mLabel->nativeWidget()->setWordWrap(true);
    layout->addItem(mLabel);
}

WelcomePage::~WelcomePage()
{
}
