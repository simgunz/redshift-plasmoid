/*******************************************************************
* redshiftosdwidget.cpp
* adapted from kdemultimedia/kmix/osdwidget.cpp
* Copyright  2009    Aurélien Gâteau <agateau@kde.org>
* Copyright  2009    Dario Andres Rodriguez <andresbajotierra@gmail.com>
* Copyright  2009    Christian Esken <christian.esken@arcor.de>
* Copyright  2010    Felix Geyer <debfx-kde@fobos.de>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
******************************************************************/

#include "redshiftosdwidget.h"

// Qt
#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QFont>

// KDE
#include <KIcon>
#include <KDialog>
#include <KWindowSystem>
#include <Plasma/FrameSvg>
#include <Plasma/Label>
#include <Plasma/Meter>
#include <Plasma/Theme>
#include <Plasma/WindowEffects>

RedshiftOSDWidget::RedshiftOSDWidget(QWidget * parent)
    : QGraphicsView(parent),
      m_background(new Plasma::FrameSvg(this)),
      m_scene(new QGraphicsScene(this)),
      m_container(new QGraphicsWidget),
      m_iconLabel(new Plasma::Label),
      m_temperatureLabel(new Plasma::Label),
      m_hideTimer(new QTimer(this))
{
    //Setup the window properties
    setWindowFlags(Qt::X11BypassWindowManagerHint);
    setFrameStyle(QFrame::NoFrame);
    viewport()->setAutoFillBackground(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAttribute(Qt::WA_TranslucentBackground);

    //Cache the icon pixmaps
    QSize iconSize = QSize(KIconLoader::SizeLarge, KIconLoader::SizeLarge);

    m_redshiftPixmap = KIcon("redshift").pixmap(iconSize);

    //Setup the widgets
    m_background->setImagePath("widgets/tooltip");

    m_iconLabel->nativeWidget()->setPixmap(m_redshiftPixmap);
    m_iconLabel->nativeWidget()->setFixedSize(iconSize);
    m_iconLabel->setMinimumSize(iconSize);
    m_iconLabel->setMaximumSize(iconSize);

    QFont font = m_temperatureLabel->font();
    //TODO:Fix this -10
    font.setPixelSize(iconSize.height()-10);
    m_temperatureLabel->setFont(font);
    m_temperatureLabel->setText(QString::fromUtf8("9999 °K"));
    m_temperatureLabel->setWordWrap(false);

    //Setup the auto-hide timer
    m_hideTimer->setInterval(2000);
    m_hideTimer->setSingleShot(true);
    connect(m_hideTimer, SIGNAL(timeout()), this, SLOT(hide()));

    //Setup the OSD layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(m_container);
    layout->addItem(m_iconLabel);
    layout->addItem(m_temperatureLabel);

    m_scene->addItem(m_container);
    setScene(m_scene);
}

void RedshiftOSDWidget::activateOSD()
{
    m_hideTimer->start();
}

void RedshiftOSDWidget::setCurrentTemperature(int temperature)
{
    qDebug() << temperature;
    m_temperatureLabel->setText(QString::fromUtf8("%1 °K").arg(temperature));
}

void RedshiftOSDWidget::drawBackground(QPainter *painter, const QRectF &/*rectF*/)
{
    painter->save();
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    m_background->paintFrame(painter);
    painter->restore();
}

QSize RedshiftOSDWidget::sizeHint() const
{
    int iconSize = m_iconLabel->nativeWidget()->pixmap()->height();
    int meterHeight = iconSize;
    int labelWidth = m_temperatureLabel->nativeWidget()->fontMetrics().width(m_temperatureLabel->text());
    qreal left, top, right, bottom;
    m_background->getMargins(left, top, right, bottom);
    //TODO: Fix this +10
    return QSize(iconSize + labelWidth + left + right + 10, meterHeight + top + bottom);
}

void RedshiftOSDWidget::resizeEvent(QResizeEvent*)
{
    m_background->resizeFrame(size());
    m_container->setGeometry(0, 0, width(), height());
    qreal left, top, right, bottom;
    m_background->getMargins(left, top, right, bottom);
    m_container->layout()->setContentsMargins(left, top, right, bottom);

    m_scene->setSceneRect(0, 0, width(), height());
    if (!KWindowSystem::compositingActive()) {
        setMask(m_background->mask());
    }
}

void RedshiftOSDWidget::showEvent(QShowEvent *event)
{
    Plasma::WindowEffects::overrideShadow(winId(), true);
}

#include "redshiftosdwidget.moc"
