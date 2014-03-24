/**
 *  Copyright 2003 Braden MacDonald <bradenm_k@shaw.ca>
 *  Copyright 2003 Ravikiran Rajagopal <ravi@ee.eng.ohio-state.edu>
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

#include <QLayout>
#include <QLabel>
#include <QImage>
#include <QPushButton>
#include <QDir>
#include <QCheckBox>
#include <QFileInfo>

#include <klocale.h>
#include <kfiledialog.h>
#include <kimagefilepreview.h>
#include <kimageio.h>
#include <kmessagebox.h>
//#include <konq_operations.h>
#include <kurl.h>
#include <KStandardDirs>

//#include "settings.h" // KConfigXT

static const int CFG_faceSize = 64;
static const QString CFG_userFaceDir = QString::fromLatin1("$HOME/.faces/");

ChFaceDlg::ChFaceDlg(QWidget *parent)
    : KDialog( parent )
{
    m_faceDir = KGlobal::dirs()->resourceDirs("data").last() + QLatin1String("kdm/pics/users/");

    setCaption( i18nc("@title:window", "Change your Face") );
    setButtons( Ok|Cancel|User1|User2 );
    setDefaultButton( Ok );

    setButtonText( User1, i18n("Custom Image...") );
    setButtonText( User2, i18n("Remove Image") );

    QWidget *faceDlg = new QWidget;
    ui.setupUi(faceDlg);

    setMainWidget(faceDlg);

    connect( ui.m_FacesWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
             SLOT(slotFaceWidgetSelectionChanged(QListWidgetItem*)) );

    connect( ui.m_FacesWidget, SIGNAL(doubleClicked(QModelIndex)), SLOT(accept()) );
    connect( this, SIGNAL(okClicked()), this, SLOT(accept()));

    connect( this, SIGNAL(user1Clicked()), this, SLOT(slotGetCustomImage()) );

    connect( this, SIGNAL(user2Clicked()), this, SLOT(slotRemoveImage()) );

    // Filling the icon view
    QDir facesDir( m_faceDir );
    if ( facesDir.exists() )
    {
        Q_FOREACH(const QString & pic, facesDir.entryList( QDir::Files | QDir::Readable ))
            new QListWidgetItem( QIcon( m_faceDir + pic ), pic.section(QLatin1Char('.'),0,0), ui.m_FacesWidget );
    }

    facesDir.setPath( CFG_userFaceDir );
    if ( facesDir.exists() )
    {
        Q_FOREACH(const QString & pic, facesDir.entryList( QDir::Files | QDir::Readable))
            new QListWidgetItem( QIcon( m_faceDir + pic ), pic.section(QLatin1Char('.'),0,0), ui.m_FacesWidget );
    }


    enableButtonOk( false ); // since no item is pre-selected, we must only enable the Ok button once a selection is done!
    //connect( this, SIGNAL(okClicked()), SLOT(slotSaveCustomImage()) );

    resize( 420, 400 );
}

ChFaceDlg::~ChFaceDlg()
{
}

void ChFaceDlg::addCustomPixmap( const QString &imPath, bool saveCopy )
{
    QImage pix( imPath );
    // TODO: save pix to TMPDIR/userinfo-tmp,
    // then scale and copy *that* to ~/.faces

    if (pix.isNull()) {
        KMessageBox::sorry( this, i18n("There was an error loading the image.") );
        return;
    }

    if ( (pix.width() > CFG_faceSize) || (pix.height() > CFG_faceSize) ) {
        pix = pix.scaled( CFG_faceSize, CFG_faceSize, Qt::KeepAspectRatio, Qt::SmoothTransformation ); // Should be no bigger than certain size.
    }

    if ( saveCopy ) {
        // If we should save a copy:
        QDir userfaces( CFG_userFaceDir );
        if ( !userfaces.exists( ) )
            userfaces.mkdir( userfaces.absolutePath() );

        pix.save( userfaces.absolutePath() + QLatin1String("/.userinfo-tmp") , "PNG" );
        // TODO QFile::rename
        //KonqOperations::copy( this, KonqOperations::COPY, KUrl::List( KUrl( userfaces.absolutePath() + "/.userinfo-tmp" ) ), KUrl( userfaces.absolutePath() + '/' + QFileInfo(imPath).fileName().section('.',0,0) ) );
#if 0
        if ( !pix.save( userfaces.absolutePath() + '/' + imPath , "PNG" ) )
            KMessageBox::sorry(this, i18n("There was an error saving the image:\n%1", userfaces.absolutePath() ) );
#endif
    }

    // pre-select the new image
    QListWidgetItem* newface = new QListWidgetItem( QIcon(QPixmap::fromImage(pix)), QFileInfo(imPath).baseName(), ui.m_FacesWidget );
    ui.m_FacesWidget->scrollToItem( newface );
    ui.m_FacesWidget->setCurrentItem( newface );
}

void ChFaceDlg::slotGetCustomImage()
{
    QCheckBox* checkWidget = new QCheckBox( i18n("&Save copy in custom faces folder for future use"), 0 );

    KFileDialog dlg( QDir::homePath(), KImageIO::pattern( KImageIO::Reading ), this, checkWidget);

    dlg.setOperationMode( KFileDialog::Opening );
    dlg.setCaption( i18nc("@title:window", "Choose Image") );
    dlg.setMode( KFile::File | KFile::LocalOnly );

    KImageFilePreview *ip = new KImageFilePreview( &dlg );
    dlg.setPreviewWidget( ip );
    if (dlg.exec() == QDialog::Accepted)
        addCustomPixmap( dlg.selectedFile(), checkWidget->isChecked() );
}

void ChFaceDlg::slotRemoveImage()
{
    ui.m_FacesWidget->clearSelection();
    // TODO should wipe the ~/.face file
    accept();
}

#include "chfacedlg.moc"
