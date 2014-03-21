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

#include <type_traits>

class Page;

class Wizard : public QObject
{
    Q_OBJECT

  public:
    static Wizard* instance();
    ~Wizard();

    template<typename T>
    int registerPage()
    {
        static_assert(std::is_base_of<Page,T>(), "Wizard::registerPage must be passed a subclass of Page");
        int typeId = 0;
        if ((typeId = QMetaType::type(T::staticMetaObject.className())) == 0) {
            typeId = qRegisterMetaType<T>(T::staticMetaObject.className());
        }
        return registerPageImpl(typeId, &T::staticMetaObject);
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
    void delayedInit();

  private:
    Wizard(QObject *parent = 0);
    int registerPageImpl(int typeId, const QMetaObject *metaObj);

  private:
    int mCurrentPageId;

    typedef struct {
        int metaType;
        const QMetaObject* metaObject;
    } MetaPage;
    QVector<MetaPage> mMetaPages;
    QMap<int, Page*> mPages;
    QSet<int> mPendingPages;
    QSet<int> mInitializedPages;

    static Wizard* sInstance;
};

template<typename T>
void wizardRegisterPage()
{
    Wizard::instance()->registerPage<T>();
}

#endif // WIZARD_H
