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

#include "languagepage.h"

#include <KLocale>
#include <KGlobal>
#include <KStandardDirs>
#include <KToolInvocation>
#include <KDebug>

#include <QGraphicsLinearLayout>
#include <QStandardItemModel>
#include <QTreeView>

#include <Plasma/Label>
#include <Plasma/TreeView>
#include <Plasma/PushButton>
#include <Plasma/DataEngineManager>

#include <PackageKit/packagekit-qt2/Daemon>
#include <PackageKit/packagekit-qt2/Transaction>

LanguagePage::LanguagePage()
    : Page()
{
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(10);
    setLayout(layout);

    Plasma::DataEngine *loc = Plasma::DataEngineManager::self()->loadEngine(QLatin1String("geolocation"));
    loc->connectSource(QLatin1String("location"), this);

    Plasma::Label * label = new Plasma::Label(this);
    label->setText(i18n("<p>Select your language below. This will switch language of all KDE applications.</p>"
                          "If your language is not listed below, click on 'Install more languages' button below."));
    layout->addItem(label);

    m_infoLabel = new Plasma::Label(this);
    layout->addItem(m_infoLabel);

    mLangsWidget = new Plasma::TreeView(this);
    mLangsWidget->nativeWidget()->setHeaderHidden(true);
    mLangsWidget->nativeWidget()->setRootIsDecorated(false);
    layout->addItem(mLangsWidget);

    Plasma::PushButton* button = new Plasma::PushButton(this);
    button->setText(i18nc("@action:button", "Install more &languages..."));
    button->setIcon(KIcon(QLatin1String("run-build-install")));
    connect(button, SIGNAL(clicked()),
            this, SLOT(installMoreLanguages()));
    layout->addItem(button);

    Plasma::PushButton* kbdBtn = new Plasma::PushButton(this);
    kbdBtn->setText(i18nc("@action:button", "Setup &keyboard..."));
    kbdBtn->setIcon(KIcon(QLatin1String("preferences-desktop-keyboard")));
    connect(kbdBtn, SIGNAL(clicked()),
            this, SLOT(setupKeyboard()));
    layout->addItem(kbdBtn);
}

LanguagePage::~LanguagePage()
{
    Plasma::DataEngineManager::self()->unloadEngine(QLatin1String("geolocation"));
}

void LanguagePage::initializePage()
{
    QStandardItemModel* model = new QStandardItemModel(this);
    Q_FOREACH (const QString& language, KGlobal::locale()->installedLanguages()) {
        QLocale loc(language);
        QString langName = loc.nativeLanguageName();
        if (langName.isEmpty())
            langName = KGlobal::locale()->languageCodeToName(language);
        QStandardItem* item = new QStandardItem(langName);
        item->setData(language);
        model->appendRow(item);
    }
    mLangsWidget->setModel(model);
    mLangsWidget->nativeWidget()->sortByColumn(0, Qt::AscendingOrder);

    // pre-select the auto-detected language
    const QModelIndexList detectedLangIndexes = model->match(model->index(0,0), Qt::UserRole + 1, m_locationLanguage, 1, Qt::MatchExactly);
    if (!detectedLangIndexes.isEmpty()) {
        mLangsWidget->nativeWidget()->selectionModel()->setCurrentIndex(detectedLangIndexes.first(),
                                                                        QItemSelectionModel::SelectCurrent);
        m_infoLabel->setText(i18n("We have detected your language: %1",
                                  KGlobal::locale()->languageCodeToName(m_locationLanguage)));
    }
}

void LanguagePage::commitChanges()
{
    const QModelIndex currentIndex = mLangsWidget->nativeWidget()->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }

    const QString lang = currentIndex.data(Qt::UserRole + 1).toString();
    kDebug() << "setting language to" << lang;
    KGlobal::locale()->setLanguage(lang, 0); // FIXME actually apply the language globally at some point
}

void LanguagePage::installMoreLanguages()
{
    // TODO call initializePage() after the additional languages have been installed
}

void LanguagePage::setupKeyboard()
{
    KToolInvocation::startServiceByDesktopName(QLatin1String("kcm_keyboard"));
}

void LanguagePage::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    if (source == QLatin1String("location")) {
        const QString countryCode = data[QLatin1String("country code")].toString().toLower();
        qDebug() << "country code: " << countryCode;
        if (!countryCode.isEmpty()) {
            QString localeConfig = KGlobal::dirs()->locate("locale", QString::fromUtf8("l10n/%1/entry.desktop").arg(countryCode));
            KConfig cfg(localeConfig);
            KConfigGroup cfgGroup(&cfg, "KCM Locale");
            QStringList langs = cfgGroup.readEntry("Languages", QStringList() << QLatin1String("en_US"));
            m_locationLanguage = langs.first();
            qDebug() << "auto-detected language:" << m_locationLanguage;
        }
    }
}
