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
#include <QGraphicsLinearLayout>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QTreeView>
#include <QProcess>

#include <KGlobal>
#include <KLocale>
#include <KStandardDirs>
#include <KToolInvocation>
#include <KDebug>

#include <Plasma/Label>
#include <Plasma/TreeView>
#include <Plasma/PushButton>

RegionPage::RegionPage(QWidget* parent)
    : WizardPage(parent)
    , mWidget(0)
{
    setTitle(i18nc("@title:tab", "Region"));
    //qDebug() << "User KLocale language is" << KGlobal::locale()->language();

    mWidget = new QGraphicsWidget;
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(10);
    mWidget->setLayout(layout);

    Plasma::Label* label = new Plasma::Label(mWidget);
    label->setText(i18n("<p>Select your region below. This will apply the region's settings globally in KDE.</p>"));
    layout->addItem(label);

    mRegionsWidget = new Plasma::TreeView(mWidget);
    mRegionsWidget->nativeWidget()->setHeaderHidden(true);
    mRegionsWidget->nativeWidget()->setRootIsDecorated(false);
    layout->addItem(mRegionsWidget);

    Plasma::PushButton* langBtn = new Plasma::PushButton(mWidget);
    langBtn->setText(i18nc("@action:button", "&Date && time settings..."));
    langBtn->setIcon(KIcon(QLatin1String("preferences-system-time")));
    connect(langBtn, SIGNAL(clicked()),
            this, SLOT(slotDateTimeSettings()));
    layout->addItem(langBtn);
}

RegionPage::~RegionPage()
{
    delete mWidget;
}

void RegionPage::initializePage()
{
    const QString userLang = KGlobal::locale()->language();
    //qDebug() << "User KLocale language is" << userLang;
    QLocale userLocale = QLocale(userLang);
    //qDebug() << "User QLocale language is" << userLocale.nativeLanguageName();

    QList<QLocale> matchingRegions = QLocale::matchingLocales(userLocale.language(), QLocale::AnyScript, QLocale::AnyCountry);

    QStandardItemModel* model = new QStandardItemModel(mWidget);
    Q_FOREACH (const QLocale & loc, matchingRegions) {
        const QString countryCode = loc.name().section(QLatin1Char('_'), 1).toLower(); // cs_CZ
        const QString flag = KGlobal::dirs()->findResource("locale", QString::fromLatin1("l10n/%1/flag.png").arg(countryCode));
        QString countryName = loc.nativeCountryName();
        if (countryName.isEmpty())
            countryName = KGlobal::locale()->countryCodeToName(countryCode);
        QStandardItem* item = new QStandardItem(QIcon(flag), countryName);
        item->setData(countryCode);
        model->appendRow(item);
    }
    mRegionsWidget->setModel(model);
    mRegionsWidget->nativeWidget()->sortByColumn(0, Qt::AscendingOrder);
}

void RegionPage::commitChanges()
{
    const QModelIndex currentIndex = mRegionsWidget->nativeWidget()->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }

    const QString country = currentIndex.data(Qt::UserRole + 1).toString();
    kDebug() << "setting country to" << country;
    KGlobal::locale()->setCountry(country, 0); // FIXME actually apply the country globally at some point
}

QGraphicsLayoutItem* RegionPage::rootWidget() const
{
    return mWidget;
}

void RegionPage::slotDateTimeSettings()
{
    KToolInvocation::startServiceByDesktopName(QLatin1String("clock"));
}
