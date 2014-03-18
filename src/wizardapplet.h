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

#ifndef WIZARDAPPLET_H
#define WIZARDAPPLET_H

#include <Plasma/Applet>

#include "wizard.h"
#include "sidewidgetpagelabel.h"

class QGraphicsLinearLayout;
namespace Plasma {
class PushButton;
class Label;
}

class WizardApplet : public Plasma::Applet
{
    Q_OBJECT

  public:
    WizardApplet(QGraphicsItem* parent = 0);
    ~WizardApplet();

    void init();

  private Q_SLOTS:
    void wizardPageChanged(int id);

  private:
    Wizard mWizard;

    Plasma::FrameSvg* mBackgroundSvg;
    Plasma::PushButton* mPrevButton;
    Plasma::PushButton* mNextButton;
    Plasma::Label* mPageTitle;

    QGraphicsLinearLayout* mContentLayout;
};

#endif // WIZARDAPPLET_H
