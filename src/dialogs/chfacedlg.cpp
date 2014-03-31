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
 * @file UserInfo's Dialog for changing your face.
 * @author Braden MacDonald
 */

#include "chfacedlg.h"

#include <QImage>
#include <QDir>
#include <QCheckBox>
#include <QFileInfo>
#include <QDebug>

#include <KLocalizedString>
#include <KFileDialog>
#include <KImageIO>
#include <KMessageBox>
#include <KUrl>
#include <KStandardDirs>
#include <KGlobalSettings>

static const int CFG_faceSize = 96;

ChFaceDlg::ChFaceDlg(QWidget *parent)
    : KDialog( parent )
{
    m_faceDirs = KGlobal::dirs()->findDirs("data", QLatin1String("kdm/pics/users/"));
    m_faceDirs.append(QLatin1String("/usr/share/pixmaps/faces/")); // gdm stuff, will be checked if exists ;)
    qDebug() << "face dirs:" << m_faceDirs;

    setCaption( i18nc("@title:window", "Change your Face") );
    setButtons( Ok|Cancel|User1|User2 );
    setDefaultButton( Cancel );

    setButtonText( User1, i18n("Custom Image...") );
    setButtonText( User2, i18n("Remove Image") );

    QWidget *faceDlg = new QWidget;
    ui.setupUi(faceDlg);

    setMainWidget(faceDlg);

    connect( ui.m_FacesWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
             SLOT(slotFaceWidgetSelectionChanged(QListWidgetItem*)) );

    connect( ui.m_FacesWidget, SIGNAL(doubleClicked(QModelIndex)), SLOT(slotOk()) );
    connect( this, SIGNAL(okClicked()), this, SLOT(slotOk()));

    connect( this, SIGNAL(user1Clicked()), this, SLOT(slotGetCustomImage()) );

    connect( this, SIGNAL(user2Clicked()), this, SLOT(slotRemoveImage()) );

    // Filling the icon view
    QDir facesDir;
    Q_FOREACH(const QString & faceDir, m_faceDirs) {
        facesDir.setPath(faceDir);
        if (facesDir.exists()) {
            Q_FOREACH(const QString & pic, facesDir.entryList( QDir::Files | QDir::Readable )) {
                //qDebug() << "found" << pic << "in" << faceDir;
                new QListWidgetItem( QIcon( faceDir + pic ), pic.section(QLatin1Char('.'),0,0), ui.m_FacesWidget );
            }
        }
    }

    enableButtonOk( false ); // since no item is pre-selected, we must only enable the Ok button once a selection is done!

    resize( 420, 400 );
}

ChFaceDlg::~ChFaceDlg()
{
}

void ChFaceDlg::addCustomPixmap( const QString &imPath, bool saveCopy )
{
    QImage pix( imPath );

    qDebug() << "adding custom pixmap" << imPath;

    if (pix.isNull()) {
        KMessageBox::sorry( this, i18n("There was an error loading the image.") );
        return;
    }

    if ( (pix.width() > CFG_faceSize) || (pix.height() > CFG_faceSize) ) {
        pix = pix.scaled( CFG_faceSize, CFG_faceSize, Qt::KeepAspectRatio, Qt::SmoothTransformation ); // Should be no bigger than certain size.
    }

    QFileInfo fi(imPath);

    if ( saveCopy ) {
        // If we should save a copy
        const QString saveLocation = KGlobal::dirs()->saveLocation("data", QLatin1String("kdm/pics/users/"));
        pix.save(saveLocation + fi.completeBaseName() + QLatin1String(".png"), "PNG");
    }

    // pre-select the new image
    QListWidgetItem* newface = new QListWidgetItem( QIcon(QPixmap::fromImage(pix)), fi.baseName(), ui.m_FacesWidget );
    ui.m_FacesWidget->scrollToItem( newface );
    ui.m_FacesWidget->setCurrentItem( newface );
}

void ChFaceDlg::slotGetCustomImage()
{
    QCheckBox* checkWidget = new QCheckBox(i18n("&Save copy in custom faces folder for future use"), this);

    KFileDialog dlg(KGlobalSettings::picturesPath(), KImageIO::pattern(KImageIO::Reading), this, checkWidget);

    dlg.setOperationMode( KFileDialog::Opening );
    dlg.setCaption( i18nc("@title:window", "Choose Image") );
    dlg.setMode(KFile::File | KFile::LocalOnly | KFile::ExistingOnly);

    if (dlg.exec() == QDialog::Accepted)
        addCustomPixmap( dlg.selectedFile(), checkWidget->isChecked() );
}

void ChFaceDlg::slotRemoveImage()
{
    ui.m_FacesWidget->clearSelection();
    QFile::remove(QDir::homePath() + QLatin1String("/.face.icon"));
    accept();
}

void ChFaceDlg::slotOk()
{
    // save the currently selected image to the ~/.face.icon file
    const QPixmap pix = ui.m_FacesWidget->currentItem()->icon().pixmap(CFG_faceSize);
    if (!pix.isNull()) {
        pix.save(QDir::homePath() + QLatin1String("/.face.icon"), "PNG");
        pix.save(QDir::homePath() + QLatin1String("/.face"), "PNG"); // gdm compat :S
    }

    accept();
}

#include "chfacedlg.moc"
