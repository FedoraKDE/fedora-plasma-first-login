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
#include "wizard.h"

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QTreeView>

#include <KGlobal>
#include <KLocale>
#include <KStandardDirs>
#include <KToolInvocation>
#include <KDebug>

#include <Plasma/Label>
#include <Plasma/TreeView>
#include <Plasma/PushButton>

RegionPage::RegionPage()
    : Page()
{
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(10);
    setLayout(layout);

    Plasma::Label* label = new Plasma::Label(this);
    label->setText(i18n("<p>Select your region below. This will apply the region's settings globally in KDE.</p>"));
    layout->addItem(label);

    m_infoLabel = new Plasma::Label(this);
    layout->addItem(m_infoLabel);

    mRegionsWidget = new Plasma::TreeView(this);
    mRegionsWidget->nativeWidget()->setHeaderHidden(true);
    mRegionsWidget->nativeWidget()->setRootIsDecorated(false);
    layout->addItem(mRegionsWidget);

    Plasma::PushButton* langBtn = new Plasma::PushButton(this);
    langBtn->setText(i18nc("@action:button", "&Date && time settings..."));
    langBtn->setIcon(KIcon(QLatin1String("preferences-system-time")));
    connect(langBtn, SIGNAL(clicked()),
            this, SLOT(slotDateTimeSettings()));
    layout->addItem(langBtn);
}

RegionPage::~RegionPage()
{
}

void RegionPage::initializePage()
{
    if (!Wizard::instance()->detectedCountry().isEmpty()) {
        m_detectedCountry = Wizard::instance()->detectedCountry();
        qDebug() << "detected country" << m_detectedCountry;
    }

    QStandardItemModel* model = new QStandardItemModel(this);
    Q_FOREACH (const QString & countryCode, KGlobal::locale()->allCountriesList()) {
        const QString flag = KGlobal::dirs()->findResource("locale", QString::fromLatin1("l10n/%1/flag.png").arg(countryCode));
        QStandardItem* item = new QStandardItem(QIcon(flag), KGlobal::locale()->countryCodeToName(countryCode));
        item->setData(countryCode);
        model->appendRow(item);
    }
    mRegionsWidget->setModel(model);
    mRegionsWidget->nativeWidget()->sortByColumn(0, Qt::AscendingOrder);

    // pre-select the auto-detected country, if available
    if (!m_detectedCountry.isEmpty()) {
        const QModelIndexList detectedCountryIndexes = model->match(model->index(0,0), Qt::UserRole + 1, m_detectedCountry, 1, Qt::MatchExactly);
        if (!detectedCountryIndexes.isEmpty()) {
            mRegionsWidget->nativeWidget()->selectionModel()->setCurrentIndex(detectedCountryIndexes.first(),
                                                                              QItemSelectionModel::SelectCurrent);
            m_infoLabel->setText(i18n("We have detected your country: %1",
                                      KGlobal::locale()->countryCodeToName(m_detectedCountry)));
        }
    }
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

void RegionPage::slotDateTimeSettings()
{
    KToolInvocation::startServiceByDesktopName(QLatin1String("clock"));
}
