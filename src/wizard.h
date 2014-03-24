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

#ifndef WIZARD_H
#define WIZARD_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QMetaType>
#include <QSet>

#include "page.h"

class Page;

class Wizard : public QObject
{
    Q_OBJECT

    struct MetaPage {
        int metaType;
        const QMetaObject* metaObject;
    };

  public:
    static Wizard* instance();
    ~Wizard();

    template<typename T>
    int registerPage()
    {
        MetaPage metaPage = { QMetaTypeId2<T>::qt_metatype_id(),
                              &T::staticMetaObject };
        mMetaPages.append(metaPage);
        return mMetaPages.count() - 1;
    }

    int currentPageId() const;
    Page* currentPage() const;

    bool isLastPage() const;

    int count() const;
    QString pageTitle(int id) const;

  public Q_SLOTS:
    void next();
    void previous();
    void setCurrentPage(int id);

  Q_SIGNALS:
    void currentPageChanged(int id);

  private Q_SLOTS:
    void preparePendingPages();
    void preparePage(int id);

  private:
    Wizard(QObject *parent = 0);

  private:
    int mCurrentPageId;


    QVector<MetaPage> mMetaPages;
    QMap<int, Page*> mPages;
    QSet<int> mPendingPages;
    QSet<int> mInitializedPages;

    static Wizard* sInstance;
};

#define WIZARD_REGISTER_PAGE_TYPE(TYPE) \
    Q_DECLARE_METATYPE(TYPE) \
    template<> \
    void* qMetaTypeConstructHelper(const TYPE* t) \
    { \
        Q_UNUSED(t); \
        return new TYPE(); \
    } \
    static const int wizard___pageType##TYPE##TypeId = Wizard::instance()->registerPage<TYPE>();

#endif // WIZARD_H
