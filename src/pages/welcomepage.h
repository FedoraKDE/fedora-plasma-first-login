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

#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include "../page.h"

namespace Plasma {
class Label;
}

class WelcomePage : public Page
{
    Q_OBJECT
    Q_CLASSINFO("Title", I18N("Welcome"))

  public:
    WelcomePage();
    ~WelcomePage();

  private:
    Plasma::Label* mLabel;
};

#endif // WELCOMEPAGE_H
