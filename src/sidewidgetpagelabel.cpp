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

#include "sidewidgetpagelabel.h"
#include "wizard.h"

#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QDebug>
#include <QCursor>

#include <KStandardDirs>
#include <KIconLoader>

#include <Plasma/Label>
#include <Plasma/Theme>


#include <QDebug>

SideWidgetPageLabel::SideWidgetPageLabel(const QString& text, int pageId, QGraphicsItem* parent)
    : QGraphicsWidget(parent)
    , mState(Default)
    , mPageId(pageId)
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    setLayout(layout);

    mImage = new Plasma::Label(this);
    mImage->setMaximumWidth(30);
    mImage->setMinimumWidth(30);
    mImage->setMinimumHeight(30);
    layout->addItem(mImage);

    mText = new Plasma::Label(this);
    QFont textFont = Plasma::Theme::defaultTheme()->font(Plasma::Theme::DefaultFont);
    textFont.setBold(true);
    mText->setFont(textFont);

#if 0 // BUG BUG BUG, has no effect :(
    QPalette textPalette;
    textPalette.setColor(QPalette::Text, Qt::black);
    textPalette.setColor(QPalette::Link, Qt::black);  // FIXME has no effect on links :/
    textPalette.setColor(QPalette::LinkVisited, Qt::black);
    mText->setPalette(textPalette);
#endif

    mText->setCursor(Qt::PointingHandCursor);

    connect(mText, SIGNAL(linkActivated(QString)), SLOT(linkActivated(QString)));
    layout->addItem(mText);

    setText(text);
    setState(SideWidgetPageLabel::Default);
}

SideWidgetPageLabel::~SideWidgetPageLabel()
{
}

int SideWidgetPageLabel::pageId() const
{
    return mPageId;
}

void SideWidgetPageLabel::setText(const QString& text)
{
    mText->setText(QString::fromLatin1("<a href=\"page:%1\">").arg(mPageId) + text + QLatin1String("</a>"));
}

QString SideWidgetPageLabel::text() const
{
    return mText->text();
}

void SideWidgetPageLabel::setState(SideWidgetPageLabel::State state)
{
    kDebug() << state;
    mState = state;
    QString iconName;
    switch (state) {
    case SideWidgetPageLabel::Active:
        iconName = QLatin1String("go-next");
        break;
    case SideWidgetPageLabel::Done:
        iconName = QLatin1String("dialog-ok-apply");
        break;
    case SideWidgetPageLabel::Skipped:
        iconName = QLatin1String("dialog-ok");
        break;
    default:
        break;
    }

    if (iconName.isEmpty()) {
        mImage->setImage(QLatin1String("NotReallyAValidImageFilePath"));
    } else {
        mImage->setImage(KIconLoader::global()->iconPath(iconName, KIconLoader::Toolbar, false));
    }
}

void SideWidgetPageLabel::linkActivated(const QString &link)
{
    if (link.startsWith(QLatin1String("page:"))) {
        const int pageId = link.section(QLatin1Char(':'), 1).toInt();
        Wizard::instance()->setCurrentPage(pageId);
    }
}

SideWidgetPageLabel::State SideWidgetPageLabel::state() const
{
    return mState;
}
