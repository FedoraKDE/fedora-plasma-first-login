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

#ifndef COLORTHEMEPAGE_H
#define COLORTHEMEPAGE_H

#include "../page.h"

class QGraphicsWidget;
class HoverButton;

namespace Plasma {
class Label;
class Animation;
}

class ColorThemePage : public Page
{
    Q_OBJECT

  public:
    ColorThemePage(QWidget* parent);
    ~ColorThemePage();

    void initializePage();
    QGraphicsLayoutItem* rootWidget() const;

    void commitChanges();

  protected:
    void resizeEvent(QResizeEvent* event);

  private Q_SLOTS:
    void onMouseEnteredButton();
    void onMouseLeftButton();
    void updatePixmap();

  private:
    QGraphicsWidget* mRootWidget;
    Plasma::Label* mImageView;
    QPixmap mLight;
    QPixmap mDark;

    HoverButton* mLightButton;
    HoverButton* mDarkButton;

    Plasma::Animation* mAnimation;
    QTimer* mAnimationTimer;
};

#endif // COLORTHEMEPAGE_H
