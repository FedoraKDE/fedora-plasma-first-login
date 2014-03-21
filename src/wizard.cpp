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
#include "page.h"

#include <KDebug>
#include <QTimer>
#include <QMetaClassInfo>

#include <KLocalizedString>

Wizard* Wizard::sInstance = 0;

Wizard::Wizard(QObject* parent)
    : QObject (parent)
    , mCurrentPageId(-1)
{
    QTimer::singleShot(0, this, SLOT(delayedInit()));
}

Wizard* Wizard::instance()
{
    if (sInstance == 0) {
        sInstance = new Wizard;
    }
    return sInstance;
}

Wizard::~Wizard()
{
}

int Wizard::registerPageImpl(int typeId, const QMetaObject* metaObject)
{
    mMetaPages.append(MetaPage{ .metaType = typeId, .metaObject = metaObject });
    return mMetaPages.count() - 1;
}

void Wizard::delayedInit()
{
    setCurrentPage(0);
}

void Wizard::setCurrentPage(int id)
{
    if (id < 0 || id >= mMetaPages.size()) {
        kWarning() << "Invalid page ID";
        return;
    }

    mCurrentPageId = id;
    if (!mPages.contains(mCurrentPageId)) {
        preparePage(mCurrentPageId);
    }

    if (!mInitializedPages.contains(id)) {
        currentPage()->initializePage();
    }

    if (!isLastPage()) {
        mPendingPages.insert(mCurrentPageId + 1);
        QTimer::singleShot(300, this, SLOT(preparePendingPages()));
    }

    Q_EMIT currentPageChanged(id);
}

void Wizard::preparePendingPages()
{
    while (!mPendingPages.isEmpty()) {
        preparePage(*mPendingPages.constBegin());
    }
}

void Wizard::preparePage(int id)
{
    Page* page = qobject_cast<Page*>(static_cast<QObject*>(QMetaType::construct(mMetaPages[id].metaType)));
    Q_ASSERT_X(page, "Wizard::preparePage", "Failed to instantiate page");
    mPages.insert(id, page);
    mPendingPages.remove(id);
}


int Wizard::currentPageId() const
{
    return mCurrentPageId;
}

Page* Wizard::currentPage() const
{
    return mPages[mCurrentPageId];
}

bool Wizard::isLastPage() const
{
    return mCurrentPageId >= mMetaPages.size() - 1;
}

int Wizard::count() const
{
    return mMetaPages.count();
}

QString Wizard::pageTitle(int id) const
{
    if (mPages.contains(id)) {
        return mPages[id]->title();
    }

    const QMetaObject* metaObject = mMetaPages[id].metaObject;
    const int offset = metaObject->indexOfClassInfo("Title");
    return i18n(metaObject->classInfo(offset).value());
}

void Wizard::next()
{
    if (!isLastPage()) {
        Page* page = currentPage();
        if (page->isComplete()) {
            page->commitChanges();
        }
        setCurrentPage(mCurrentPageId + 1);
    }
}

void Wizard::previous()
{
    if (mCurrentPageId > 0) {
        setCurrentPage(mCurrentPageId - 1);
    }
}

