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

#include <QGraphicsLinearLayout>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QTreeView>
#include <QProcess>

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

    mWidget = new QGraphicsWidget;
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(10);
    mWidget->setLayout(layout);

    Plasma::Label* label = new Plasma::Label(mWidget);
    label->setText(i18n("<p>Select your language below. This will switch language of all KDE applications.</p>"
                        "<p>If your language is not listed below, click on 'Install more languages' button below.</p>"));
    layout->addItem(label);

    mLangsWidget = new Plasma::TreeView(mWidget);
    mLangsWidget->nativeWidget()->setHeaderHidden(true);
    mLangsWidget->nativeWidget()->setRootIsDecorated(false);
    layout->addItem(mLangsWidget);

    Plasma::PushButton* button = new Plasma::PushButton(mWidget);
    button->setText(i18nc("@action:button", "Install more &languages..."));
    button->setIcon(KIcon(QLatin1String("run-build-install")));
    connect(button, SIGNAL(clicked()),
            this, SLOT(installMoreLanguages()));
    layout->addItem(button);

    Plasma::PushButton* kbdBtn = new Plasma::PushButton(mWidget);
    kbdBtn->setText(i18nc("@action:button", "Setup &keyboard..."));
    kbdBtn->setIcon(KIcon(QLatin1String("preferences-desktop-keyboard")));
    connect(kbdBtn, SIGNAL(clicked()),
            this, SLOT(setupKeyboard()));
    layout->addItem(kbdBtn);
}

LanguagePage::~LanguagePage()
{
    delete mWidget;
}

void LanguagePage::initializePage()
{
    QStandardItemModel* model = new QStandardItemModel(mWidget);
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
}

void LanguagePage::commitChanges()
{
    const QModelIndex currentIndex = mLangsWidget->nativeWidget()->currentIndex();
    if (!currentIndex.isValid()) {
        return;
    }

    const QString lang = currentIndex.data(Qt::UserRole + 1).toString();
    qDebug() << "setting language to" << lang;
    KGlobal::locale()->setLanguage(lang, 0); // FIXME actually apply the language globally at some point
}

QGraphicsLayoutItem* LanguagePage::rootWidget() const
{
    return mWidget;
}

void LanguagePage::installMoreLanguages()
{
    // TODO call initializePage() after the additional languages have been installed
}

void LanguagePage::setupKeyboard()
{
    QProcess::startDetached(QLatin1String("kcmshell4 kcm_keyboard"));
}
