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

#include "regionpage.h"

#include <QGraphicsWidget>
#include <QDebug>
#include <QGraphicsLinearLayout>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QTreeView>

#include <KGlobal>
#include <KLocale>
#include <KStandardDirs>

#include <Plasma/Label>
#include <Plasma/TreeView>
#include <Plasma/PushButton>

RegionPage::RegionPage(QWidget* parent)
    : WizardPage(parent)
    , mWidget(0)
{
    setTitle(i18nc("@title:tab", "Region"));
    qDebug() << "User KLocale language is" << KGlobal::locale()->language();

    mWidget = new QGraphicsWidget;
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(40);
    mWidget->setLayout(layout);

    Plasma::Label* label = new Plasma::Label(mWidget);
    label->setText(i18n("<p>Select your region below. This will apply the region's settings globally in KDE.</p>"));
    layout->addItem(label);

    mRegionsWidget = new Plasma::TreeView(mWidget);
    mRegionsWidget->nativeWidget()->setHeaderHidden(true);
    mRegionsWidget->nativeWidget()->setRootIsDecorated(false);
    layout->addItem(mRegionsWidget);
}

RegionPage::~RegionPage()
{
    delete mWidget;
}

void RegionPage::initializePage()
{
    const QString userLang = KGlobal::locale()->language();
    qDebug() << "User KLocale language is" << userLang;
    QLocale userLocale = QLocale(userLang);
    qDebug() << "User QLocale language is" << userLocale.nativeLanguageName();

    QList<QLocale> matchingRegions = QLocale::matchingLocales(userLocale.language(), QLocale::AnyScript, QLocale::AnyCountry);

    QStandardItemModel* model = new QStandardItemModel(mWidget);
    Q_FOREACH (const QLocale & loc, matchingRegions) {
        const QString countryCode = loc.name().section(QLatin1Char('_'), 1).toLower();
        qDebug() << "adding matching country" << countryCode;
        const QString flag = KGlobal::dirs()->findResource("locale", QString::fromLatin1("l10n/%1/flag.png").arg(countryCode));
        QStandardItem* item = new QStandardItem(QIcon(flag), loc.nativeCountryName());
        item->setData(countryCode);
        model->appendRow(item);
    }
    mRegionsWidget->setModel(model);
}

void RegionPage::commitChanges()
{
    // TODO
}

QGraphicsLayoutItem* RegionPage::rootWidget() const
{
    return mWidget;
}
