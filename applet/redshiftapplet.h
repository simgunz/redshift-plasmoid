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

#ifndef REDSHIFTAPPLET_H
#define REDSHIFTAPPLET_H

#include <Plasma/PopupApplet>
#include <Plasma/DataEngine>
#include <Plasma/Svg>
#include <KIcon>
#include <Plasma/ToolTipContent>
#include <Plasma/IconWidget>
#include <QGraphicsGridLayout>

#include "ui_redshiftConfig.h"

class RedshiftApplet : public Plasma::Applet
{
    Q_OBJECT
public:
    RedshiftApplet(QObject *parent, const QVariantList &args);
    void init();
    void configChanged();
public Q_SLOTS:
    void toggle();
    void dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data);
protected:    
    void createConfigurationInterface(KConfigDialog *parent);
private:
    
    KIcon m_icon;
    Plasma::Svg m_theme;
    Plasma::ToolTipContent m_tooltip;
    Plasma::IconWidget *m_button;
    QGraphicsGridLayout *m_layout;
    Ui::redshiftDialog configDialog;
    Plasma::DataEngine *m_engine;
};

#endif
