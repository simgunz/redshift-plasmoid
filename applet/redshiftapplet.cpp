/**************************************************************************
*   Copyright (C) 2012 by Simone Gaiarin <simgunz@gmail.com>              *
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
**************************************************************************/

#include "redshiftapplet.h"

#include <QGraphicsLinearLayout>
//Plasma
#include <Plasma/Svg>
#include <Plasma/Theme>
#include <Plasma/DataEngine>
#include <Plasma/ToolTipContent>
#include <Plasma/ToolTipManager>

//KDE
#include <KLocale>
#include <KConfigDialog>


#include "redshiftsettings.h"

RedshiftApplet::RedshiftApplet(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_theme(this),
    m_icon("redshift")
{
    setBackgroundHints(StandardBackground);
    setAspectRatioMode(Plasma::ConstrainedSquare);
    setHasConfigurationInterface(true);  
}

void RedshiftApplet::init()
{
    m_tooltip.setMainText(i18n("Redshift"));
    m_tooltip.setSubText(i18n("Click to toggle it on"));    
    m_tooltip.setImage(KIcon("redshift-status-off"));
    Plasma::ToolTipManager::self()->setContent(this,m_tooltip);
    
    m_button = new Plasma::IconWidget(this);
    m_button->setIcon(KIcon("redshift-status-off"));
    m_layout = new QGraphicsGridLayout(this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->addItem(m_button,0,0);        
    m_engine = dataEngine("redshift");
    m_engine->connectSource("Controller",this);    
    connect(m_button,SIGNAL(clicked()),this,SLOT(toggle()));
}

void RedshiftApplet::dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data)
{
    if(data["Status"].toString() == "Running")
    {
        m_button->setIcon(KIcon("redshift-status-on"));
        m_tooltip.setSubText(i18n("Click to toggle it off"));
        m_tooltip.setImage(KIcon("redshift-status-on"));
    }
    else
    {
        m_button->setIcon(KIcon("redshift-status-off"));
        m_tooltip.setSubText(i18n("Click to toggle it on"));
        m_tooltip.setImage(KIcon("redshift-status-off"));
    }
    Plasma::ToolTipManager::self()->setContent(this,m_tooltip);
}

void RedshiftApplet::createConfigurationInterface(KConfigDialog *parent)
{     
    QWidget *widget = new QWidget(parent);
    configDialog.setupUi(widget);
    parent->addPage(widget, RedshiftSettings::self(), i18n("General"), "redshift");    
}

void RedshiftApplet::toggle()
{
    Plasma::Service *service = m_engine->serviceForSource("Controller");
    service->startOperationCall(service->operationDescription("toggle"));
}

void RedshiftApplet::configChanged()
{    
    Plasma::Service *service = m_engine->serviceForSource("Controller");
    service->startOperationCall(service->operationDescription("restart"));
}

K_EXPORT_PLASMA_APPLET(redshift, RedshiftApplet)

#include "redshiftapplet.moc"
