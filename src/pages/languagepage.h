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

#ifndef LANGUAGEPAGE_H
#define LANGUAGEPAGE_H

#include "../wizardpage.h"
#include <QGraphicsWidget>

namespace Plasma {
class TreeView;
}

class LanguagePage : public WizardPage
{
    Q_OBJECT

  public:
    LanguagePage(QWidget* parent);
    ~LanguagePage();

    void initializePage();
    void commitChanges();

    QGraphicsLayoutItem* rootWidget() const;

  private Q_SLOTS:
    void installMoreLanguages();

  private:
    QGraphicsWidget* mWidget;
    Plasma::TreeView* mLangsWidget;
    bool mInitialized;
};

#endif // LANGUAGEPAGE_H
