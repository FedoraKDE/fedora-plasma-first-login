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

#include "languagepage.h"
#include "wizard.h"

#include <KLocale>
#include <KGlobal>
#include <KStandardDirs>
#include <KToolInvocation>
#include <KDebug>

#include <QGraphicsLinearLayout>
#include <QTreeView>
#include <QLabel>
#include <QStandardItemModel>

#include <Plasma/Label>
#include <Plasma/TreeView>
#include <Plasma/PushButton>

#include <PackageKit/packagekit-qt2/Daemon>
#include <PackageKit/packagekit-qt2/Transaction>

LanguagePage::LanguagePage()
    : Page()
{
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(10);
    setLayout(layout);

    Plasma::Label * label = new Plasma::Label(this);
    label->setText(i18n("Select your language below. This will switch language of all KDE applications.<br>"
                        "If your language is not currently available, it will be automatically installed later on.<br><br>"
                        "<em>Note:</em> If your language is auto-detected, it will be placed first in the list and preselected."));
    layout->addItem(label);

    mLangsWidget = new Plasma::TreeView(this);
    mLangsWidget->nativeWidget()->setHeaderHidden(true);
    mLangsWidget->nativeWidget()->setRootIsDecorated(false);
    mLangsWidget->nativeWidget()->setUniformRowHeights(true);
    layout->addItem(mLangsWidget);

    Plasma::PushButton* kbdBtn = new Plasma::PushButton(this);
    kbdBtn->setText(i18nc("@action:button", "Setup &keyboard..."));
    kbdBtn->setIcon(KIcon(QLatin1String("preferences-desktop-keyboard")));
    connect(kbdBtn, SIGNAL(clicked()),
            this, SLOT(setupKeyboard()));
    layout->addItem(kbdBtn);
}

LanguagePage::~LanguagePage()
{
}

void LanguagePage::initializePage()
{
    m_detectedLanguages = Wizard::instance()->detectedLanguages();

    if (!m_detectedLanguages.isEmpty()) {
        m_locationLanguage = m_detectedLanguages.first();
        qDebug() << "(first) detected language" << m_locationLanguage;
    }

    // insert the list of all known languages
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setColumnCount(1);
    Q_FOREACH (const QString& language, KGlobal::locale()->allLanguagesList()) {
        if (language == QLatin1String("x-test") || m_detectedLanguages.contains(language)) // skip test and detected languages
            continue;
        QStandardItem* item = new QStandardItem(KGlobal::locale()->languageCodeToName(language));
        item->setData(language);
        model->appendRow(item);
    }

    mLangsWidget->setModel(model);
    model->sort(0);

    // prepend the list of detected languages, in their order
    QListIterator<QString> i(m_detectedLanguages);
    i.toBack();
    while (i.hasPrevious()) {
        const QString language = i.previous();
        QStandardItem* item = new QStandardItem(KGlobal::locale()->languageCodeToName(language));
        item->setData(language);
        model->insertRow(0, item);
    }

    // pre-select the auto-detected language, if available
    if (!m_locationLanguage.isEmpty()) {
        const QModelIndexList detectedLangIndexes = model->match(model->index(0,0), Qt::UserRole + 1, m_locationLanguage, 1, Qt::MatchExactly);
        if (!detectedLangIndexes.isEmpty()) {
            mLangsWidget->nativeWidget()->selectionModel()->setCurrentIndex(detectedLangIndexes.first(),
                                                                            QItemSelectionModel::SelectCurrent);
        }
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

    if (KGlobal::locale()->installedLanguages().contains(lang)) {
        // TODO install the language using PackageKit-qt!
    }
}

void LanguagePage::setupKeyboard()
{
    KToolInvocation::startServiceByDesktopName(QLatin1String("kcm_keyboard"));
}
