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

#include "wizard.h"
#include "wizardpage.h"
#include "pages/welcomepage.h"
#include "pages/languagepage.h"
#include "pages/regionpage.h"
#include "pages/colorthemepage.h"

#include <KDebug>

#include <Plasma/Containment>
#include <QTimer>

Wizard::Wizard()
    : QWizard()
    , mCurrentPageId(-1)
{
    connect(this, SIGNAL(currentIdChanged(int)),
            this, SLOT(onCurrentIdChanged(int)));
}

Wizard::~Wizard()
{
}

void Wizard::init()
{
    registerPage<WelcomePage>();
    registerPage<LanguagePage>();
    registerPage<RegionPage>();
    registerPage<ColorThemePage>();

    // Workaround for QWizard not initializing itself until show() is called
    // (which we don't call)
    restart();
}


template<typename T>
int Wizard::registerPage()
{
    T *page = new T(this);
    if (page->shouldSkip()) {
        kDebug() << "Skipped page" << page->title();
        page->deleteLater();
        return -1;
    }

    kDebug() << "Added page" << page->title();
    return addPage(page);
}

void Wizard::onCurrentIdChanged(int id)
{
    if (mCurrentPageId > -1 && id > mCurrentPageId) {
        WizardPage *previousPage = qobject_cast<WizardPage*>(page(mCurrentPageId));
        if (!previousPage) {
            return;
        }
    }

    mCurrentPageId = id;
    page(mCurrentPageId)->initializePage();
}
