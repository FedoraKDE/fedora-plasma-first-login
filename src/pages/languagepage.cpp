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

#include <KLocalizedString>
#include <KLocale>
#include <KGlobal>
#include <KStandardDirs>

#include <QGraphicsLinearLayout>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QTreeView>

#include <Plasma/Label>
#include <Plasma/TreeView>
#include <Plasma/PushButton>

#include <PackageKit/packagekit-qt2/Daemon>
#include <PackageKit/packagekit-qt2/Transaction>

#include <QDebug>

LanguagePage::LanguagePage(QWidget* parent)
    : WizardPage(parent)
    , mWidget(0)
{
    setTitle(i18nc("@title:tab", "Language"));
}

LanguagePage::~LanguagePage()
{
    delete mWidget;
}


bool LanguagePage::shouldSkip() const
{
    return KGlobal::locale()->installedLanguages().size() < 2;
}

void LanguagePage::initializePage()
{
    if (mWidget) {
        return;
    }

    mWidget = new QGraphicsWidget;
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(40);
    mWidget->setLayout(layout);

    Plasma::Label* label = new Plasma::Label(mWidget);
    label->setText(i18n("<p>Select your language below. This will switch language of all KDE applications.</p>"
                        "<p>If your langauge is not listed below, click on 'Install more languages' button below.</p>"));
    layout->addItem(label);

    mLangsWidget = new Plasma::TreeView(mWidget);
    mLangsWidget->nativeWidget()->setHeaderHidden(true);
    mLangsWidget->nativeWidget()->setRootIsDecorated(false);
    layout->addItem(mLangsWidget);

    QStandardItemModel* model = new QStandardItemModel(mWidget);
    Q_FOREACH (const QString& language, KGlobal::locale()->installedLanguages()) {
        const QString flag = KGlobal::dirs()->findResource("locale", QString::fromLatin1("l10n/%1/flag.png").arg(language.right(2).toLower()));
        QStandardItem* item = new QStandardItem(QIcon(flag), KGlobal::locale()->languageCodeToName(language));
        item->setData(language);
        model->appendRow(item);
    }
    mLangsWidget->setModel(model);

    Plasma::PushButton* button = new Plasma::PushButton(mWidget);
    button->setText(i18nc("@action:button", "Install more languages"));
    button->setIcon(KIcon(QLatin1String("run-build-install")));
    connect(button, SIGNAL(clicked()),
            this, SLOT(installMoreLanguages()));
    layout->addItem(button);
}

void LanguagePage::commitChanges()
{
    const QModelIndex currentIndex = mLangsWidget->nativeWidget()->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }

    const QString lang = currentIndex.data(Qt::UserRole + 1).toString();
    KGlobal::locale()->setLanguage(lang, 0);
}

QGraphicsLayoutItem* LanguagePage::rootWidget() const
{
    return mWidget;
}


void LanguagePage::installMoreLanguages()
{
}

