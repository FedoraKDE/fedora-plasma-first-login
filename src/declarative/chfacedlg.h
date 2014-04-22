/**
 *  Copyright 2003 Braden MacDonald <bradenm_k@shaw.ca>
 *  Copyright 2003 Ravikiran Rajagopal <ravi@ee.eng.ohio-state.edu>
 *  Copyright 2014 Lukáš Tinkl <ltinkl@redhat.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *
 */

/**
 * @file UserInfo-chface: Dialog for choosing a new face for your user.
 * @author Braden MacDonald
 */

#ifndef CHFACEDLG_H
#define CHFACEDLG_H

#include <QObject>
#include <QPixmap>

#include <KDialog>

#include "ui_chfacedlg.h"

class ChFaceDlg : public KDialog
{
    Q_OBJECT
public:
    explicit ChFaceDlg(QWidget *parent=0);

    ~ChFaceDlg();

    /**
     * Will return the currently selected face, or a null pixmap if the user hit the "remove image" button
    */
    QPixmap faceImage() const
    {
        if(ui.m_FacesWidget->currentItem())
            return ui.m_FacesWidget->currentItem()->icon().pixmap(64);
        else
            return QPixmap();
    }

private Q_SLOTS:
    void slotFaceWidgetSelectionChanged( QListWidgetItem *item )
    { enableButtonOk(!item->icon().isNull() ); }

    void slotGetCustomImage();
    void slotRemoveImage();

    void slotOk();

private:
    void addCustomPixmap( const QString &imPath, bool saveCopy );
    QStringList m_faceDirs;
    Ui::faceDlg ui;
};

#endif // CHFACEDLG_H
