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

// Here we avoid loading the header multiple times
#ifndef REDSHIFTAPPLET_H
#define REDSHIFTAPPLET_H

#include <KIcon>
// We need the Plasma Applet headers
#include <Plasma/Applet>
#include <Plasma/PopupApplet>
#include <Plasma/DataEngine>
#include <Plasma/Svg>
#include <KCModuleProxy>
#include <Plasma/ToolTipContent>
#include <Plasma/IconWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <QElapsedTimer>
#include <QGraphicsGridLayout>

#include "ui_redshiftConfig.h"

class QSizeF;

// Define our plasma Applet
class RedshiftApplet : public Plasma::PopupApplet
{
    Q_OBJECT
public:
    // Basic Create/Destroy
    RedshiftApplet(QObject *parent, const QVariantList &args);
    ~RedshiftApplet();

    // The paintInterface procedure paints the applet to the desktop
   // void paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       // const QRect& contentsRect);
    void init();
    void configChanged();
public Q_SLOTS:
    void toggle();
    void toolTipAboutToShow();
    void toolTipHidden();
    void updateTooltip();
    void dataUpdated(const QString &sourceName, const Plasma::DataEngine::Data &data);
protected:    
    void createConfigurationInterface(KConfigDialog *parent);
private:
    Plasma::DataEngine *m_engine;
    Plasma::ToolTipContent m_tooltip;
    KIcon m_icon;
    Plasma::Svg m_theme;
    Plasma::IconWidget *m_button;
    QGraphicsGridLayout *m_layout;
    Ui::redshiftDialog configDialog;
    //KCModuleProxy *m_redshiftSettingsWidget;
};

#endif
