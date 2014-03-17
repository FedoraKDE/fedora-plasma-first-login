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

#ifndef SIDEWIDGET_H
#define SIDEWIDGET_H

#include <Plasma/Frame>

class SideWidgetPageLabel;
class QGraphicsLinearLayout;
class Wizard;

class SideWidget : public Plasma::Frame
{
    Q_OBJECT

  public:
    SideWidget(Wizard* wizard, QGraphicsWidget* parent = 0);
    ~SideWidget();

  private Q_SLOTS:
    void onPageChanged(int id);
    void onPageAdded(int id);

  private:
    Wizard* mWizard;

    QGraphicsLinearLayout* mTitlesLayout;
    QMap<int, SideWidgetPageLabel*> mTitleLabels;
    int mActivePage;
};

#endif // SIDEWIDGET_H
