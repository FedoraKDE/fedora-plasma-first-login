/*
 * Copyright (C) 2014 Jan Grulich <jgrulich@redhat.com>
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

#include "networkpage.h"

#include <QDeclarativeContext>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include <KGlobal>
#include <KStandardDirs>

NetworkPage::NetworkPage()
    : Page()
    , m_view(new QDeclarativeView())
{
    QString importPath = KGlobal::dirs()->installPath("module");
    importPath.append(QLatin1String("imports"));

    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
    setLayout(layout);

    m_view->engine()->addImportPath(importPath);
    m_view->setSource(QUrl::fromLocalFile(KGlobal::dirs()->findResource("data", QLatin1String("fedora-plasma-first-login/qml/main.qml"))));
    m_view->setStyleSheet(QLatin1String("background: transparent"));
    m_view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    m_view->setAttribute(Qt::WA_TranslucentBackground);
    m_view->show();

    QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget(this);
    proxyWidget->setWidget(m_view);

    layout->addItem(proxyWidget);
}

NetworkPage::~NetworkPage()
{
    delete m_view;
}
