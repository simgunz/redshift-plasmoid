/***************************************************************************
 *   Copyright (C) 2010 by Simone Gaiarin <simgunz@gmail.com>                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "redshiftapplet.h"

//QT
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <QGraphicsLinearLayout>
//Plasma
#include <plasma/svg.h>
#include <plasma/theme.h>
#include <Plasma/DataEngine>
#include <Plasma/Extender>
#include <Plasma/ExtenderItem>
#include <Plasma/TextBrowser>
#include <Plasma/Label>
#include <Plasma/ToolTipContent>
#include <Plasma/ToolTipManager>

//KDE
#include <KLocale>
#include <KGlobal>
#include <KStandardDirs>
#include <KNotification>
#include <KCModuleInfo>
#include <KConfigDialog>
#include <KCModuleProxy>

#include <QTimer>
#include <QTime>

#include <cstdlib>
#include <time.h>

#include "redshiftsettings.h"

RedshiftApplet::RedshiftApplet(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args),
    m_theme(this),
    m_icon("redshift")
{
    // this will get us the standard applet background, for free!
    setBackgroundHints(StandardBackground);
    setAspectRatioMode(Plasma::ConstrainedSquare );
    setHasConfigurationInterface(true);  
    //setPopupIcon(m_icon);
    //resize(200, 200);
    //resize(graphicsWidget()->minimumSize());

    m_theme.setImagePath("widgets/battery-oxygen");
    m_theme.setContainsMultipleImages(true);
}


RedshiftApplet::~RedshiftApplet()
{
    if (hasFailedToLaunch()) {
        // Do some cleanup here
    } else {
        // Save settings
    }    
}

void RedshiftApplet::init()
{
    // A small demonstration of the setFailedToLaunch function    
    
    //m_engine = dataEngine("redshift");
    if (m_icon.isNull()) {
        setFailedToLaunch(true, i18n("No world to say hello"));
    }


    m_tooltip.setMainText("Redshift");
    m_tooltip.setImage(KIcon("redshift"));
    m_tooltip.setAutohide(true);
    Plasma::ToolTipManager::self()->setContent(this,m_tooltip);
    
    //Plasma::DataEngine::Data data = m_dataengine->query(m_user);
    //m_engine->connectSource("Status",this);
    
    m_button = new Plasma::IconWidget(this);
    m_button->setIcon(KIcon("redshift-status-off"));
    m_layout = new QGraphicsGridLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->addItem(m_button,0,0);
    connect(m_button,SIGNAL(clicked()),this,SLOT(toggle()));
    
    m_engine = dataEngine("redshift");
    m_engine->connectSource("Controller",this);
    
}

void RedshiftApplet::toggle()
{
    Plasma::Service *service = m_engine->serviceForSource("Controller");
    service->startOperationCall(service->operationDescription("toggle"));
}

void RedshiftApplet::dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data)
{
    if(data["Status"].toString() == "Running")
        m_button->setIcon(KIcon("redshift-status-on"));
    else
        m_button->setIcon(KIcon("redshift-status-off"));
}

void RedshiftApplet::createConfigurationInterface(KConfigDialog *parent)
{     
    QWidget *widget = new QWidget(parent);
    configDialog.setupUi(widget);
    parent->addPage(widget, RedshiftSettings::self(), i18n("General"), "redshift");    

    //parent->setButtons( KDialog::Ok | KDialog::Cancel);
    //connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
}

void RedshiftApplet::toolTipAboutToShow()
{
    //KNotification::event(KNotification::Notification, "Titolo","Tempo scaduto", KIcon("redshift_kde").pixmap(QSize(32,32)));
    //KNotification::event(KNotification::Catastrophe, "Titolo","Tempo scaduto", KIcon("redshift_kde").pixmap(QSize(32,32)));    
    
}

void RedshiftApplet::toolTipHidden()
{
   
    //m_timeleft = 0;
}

void RedshiftApplet::updateTooltip()
{
    
}

void RedshiftApplet::configChanged()
{    
    Plasma::Service *service = m_engine->serviceForSource("Controller");
    service->startOperationCall(service->operationDescription("restart"));
}


// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(redshift, RedshiftApplet)

#include "redshiftapplet.moc"
