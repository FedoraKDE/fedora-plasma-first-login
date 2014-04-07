/*
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

#include "userpage.h"
#include "dialogs/chfacedlg.h"
#include "wizard.h"

#include <QGraphicsWidget>
#include <QGraphicsGridLayout>
#include <QDir>
#include <QDBusReply>

#include <KGlobal>
#include <KLocale>
#include <KStandardDirs>
#include <KToolInvocation>
#include <KDebug>
#include <KLineEdit>

#include <Plasma/Label>
#include <Plasma/PushButton>
#include <Plasma/LineEdit>
#include <Plasma/CheckBox>

#include <unistd.h>
#include <sys/types.h>

UserPage::UserPage()
    : Page(),
      m_user(KUser(getuid())),
      m_accountManager(QLatin1String("org.freedesktop.Accounts"),
                       QLatin1String("/org/freedesktop/Accounts"),
                       QLatin1String("org.freedesktop.Accounts"),
                       QDBusConnection::systemBus(), this),
      m_userIface(0)
{
    QDBusReply<QDBusObjectPath> reply = m_accountManager.asyncCall(QLatin1String("FindUserById"), static_cast<qint64>(m_user.uid()));
    if (reply.isValid()) {
        const QString userPath = reply.value().path();
        qDebug() << "User DBUS path" << userPath;
        m_userIface = new QDBusInterface(QLatin1String("org.freedesktop.Accounts"),
                                         userPath,
                                         QLatin1String("org.freedesktop.Accounts.User"),
                                         QDBusConnection::systemBus(), this);
    } else {
        qWarning() << "Failed to find the user in the DBUS AccountService";
    }

    m_email.setProfile(m_email.defaultProfileName());

    //qDebug() << "Loading user details:";
    //qDebug() << "UID:" << m_user.uid();

    QGraphicsGridLayout* layout = new QGraphicsGridLayout;
    layout->setSpacing(10);

    Plasma::Label* label = new Plasma::Label(this);
    label->setText(i18n("<p>Customize your user details below.</p>"));
    layout->addItem(label, 0, 0, 1, 2);

    m_avatarBtn = new Plasma::PushButton(this);
    m_avatarBtn->setIcon(QIcon(m_user.faceIconPath()));
    m_avatarBtn->setToolTip(i18n("Click this button to change your user picture."));
    m_avatarBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_avatarBtn->setMinimumSize(QSize(74,74));
    m_avatarBtn->setMaximumSize(QSize(74,74));
    connect(m_avatarBtn, SIGNAL(clicked()), SLOT(changeAvatar()));
    layout->setRowFixedHeight(1, 74);
    layout->addItem(m_avatarBtn, 1, 0, 1, 2, Qt::AlignCenter);

    Plasma::Label* label3 = new Plasma::Label(this);
    label3->setText(i18n("Full name:"));
    layout->addItem(label3, 2, 0, Qt::AlignLeft | Qt::AlignTop);

    leFullname = new Plasma::LineEdit(this);
    leFullname->setText(fullUserName());
    layout->addItem(leFullname, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);

    Plasma::Label* label4 = new Plasma::Label(this);
    label4->setText(i18n("Organization:"));
    layout->addItem(label4, 3, 0, Qt::AlignLeft | Qt::AlignTop);

    leOrg = new Plasma::LineEdit(this);
    leOrg->setText(m_email.getSetting(KEMailSettings::Organization));
    layout->addItem(leOrg, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);

    Plasma::Label* label5 = new Plasma::Label(this);
    label5->setText(i18n("Email:"));
    layout->addItem(label5, 4, 0, Qt::AlignLeft | Qt::AlignTop);

    leEmail = new Plasma::LineEdit(this);
    leEmail->setText(m_email.getSetting(KEMailSettings::EmailAddress));
    layout->addItem(leEmail, 4, 1, Qt::AlignLeft | Qt::AlignVCenter);

    m_useLocationCB = new Plasma::CheckBox(this);
    m_useLocationCB->setText(i18n("Use the following location:"));
    layout->addItem(m_useLocationCB, 5, 0, Qt::AlignLeft | Qt::AlignVCenter);

    leLocation = new Plasma::LineEdit(this);
    leLocation->setText(Wizard::instance()->detectedLocation());
    leLocation->setEnabled(false);
    connect(m_useLocationCB, SIGNAL(toggled(bool)),
            leLocation->nativeWidget(), SLOT(setEnabled(bool)));
    m_useLocationCB->setChecked(!leLocation->text().isEmpty());
    layout->addItem(leLocation, 5, 1, Qt::AlignLeft | Qt::AlignVCenter);

    Plasma::PushButton* passBtn = new Plasma::PushButton(this);
    passBtn->setText(i18nc("@action:button", "&Change password..."));
    passBtn->setIcon(KIcon(QLatin1String("preferences-desktop-user-password")));
    passBtn->setMaximumHeight(30);
    connect(passBtn, SIGNAL(clicked()), SLOT(changePassword()));
    layout->addItem(passBtn, 6, 0, 1, 2);

    setLayout(layout);
}

UserPage::~UserPage()
{
    delete m_userIface;
}

void UserPage::initializePage()
{
}

void UserPage::commitChanges()
{
    // TODO use "chfn -f" to also store the full name?
    m_email.setSetting(KEMailSettings::RealName, leFullname->text());
    m_email.setSetting(KEMailSettings::Organization, leOrg->text());
    m_email.setSetting(KEMailSettings::EmailAddress, leEmail->text());

    // save to the AccountService
    if (m_userIface) {
        m_userIface->asyncCall(QLatin1String("SetRealName"), leFullname->text());
        m_userIface->asyncCall(QLatin1String("SetEmail"), leEmail->text());
        if (m_useLocationCB->isChecked()) {
            m_userIface->asyncCall(QLatin1String("SetLocation"), leLocation->text());
        }
        //qDebug() << "user face path" << m_user.faceIconPath();
        m_userIface->asyncCall(QLatin1String("SetIconFile"), m_user.faceIconPath());
    }
}

QString UserPage::fullUserName() const
{
    // 1st try KEMailSettings, then KUser
    QString result = m_email.getSetting(KEMailSettings::RealName);
    if (result.isEmpty()) {
        result = m_user.property(KUser::FullName).toString();
    }

    return result;
}

void UserPage::changeAvatar()
{
    ChFaceDlg * dlg = new ChFaceDlg(0);
    if (dlg->exec() == QDialog::Accepted) {
        m_avatarBtn->setIcon(dlg->faceImage());
    }
    delete dlg;
}

void UserPage::changePassword()
{
    KToolInvocation::startServiceByDesktopName(QLatin1String("kdepasswd"));
}
