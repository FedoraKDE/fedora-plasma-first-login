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

#ifndef LANGUAGEPAGE_H
#define LANGUAGEPAGE_H

#include "../page.h"

#include <PackageKit/packagekit-qt2/Daemon>
#include <PackageKit/packagekit-qt2/Transaction>

namespace Plasma {
class TreeView;
}

class LanguagePage : public Page
{
    Q_OBJECT
    Q_CLASSINFO("Title", I18N_NOOP("Language"))

  public:
    LanguagePage();
    ~LanguagePage();

    void initializePage();
    void commitChanges();

  private Q_SLOTS:
    void setupKeyboard();
    void onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);
    void onSearchTransactionFinished(PackageKit::Transaction::Exit status, uint runtime);
    void onInstallTransactionFinished(PackageKit::Transaction::Exit status, uint runtime);
    void onItemProgress(const QString &itemID, PackageKit::Transaction::Status status, uint percentage);
    void onMessage(PackageKit::Transaction::Message type, const QString &message);
    void onError(PackageKit::Transaction::Error error, const QString &details);

  private:
    Plasma::TreeView* mLangsWidget;
    QString m_locationLanguage;
    QStringList m_detectedLanguages;
    PackageKit::Transaction * m_searchTrans;
    PackageKit::Transaction * m_installTrans;
};

#endif // LANGUAGEPAGE_H
