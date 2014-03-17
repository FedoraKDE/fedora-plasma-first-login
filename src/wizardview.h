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

#ifndef WIZARDVIEW_H
#define WIZARDVIEW_H

#include <Plasma/View>
#include <Plasma/Corona>

class WizardApplet;
/**
 * Creates the background containment, the content applet and takes care about
 * their layout
 */
class WizardView : public Plasma::View
{
    Q_OBJECT

  public:
    WizardView(QWidget* parent = 0);
    ~WizardView();

  protected:
    void resizeEvent(QResizeEvent* event);

  private Q_SLOTS:
    void delayedInit();

  private:
    Plasma::Corona mCorona;
    WizardApplet* mApplet;
};

#endif // WIZARDVIEW_H
