/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright: 2021 LXQt team
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "datetimeplugin.h"
#include "datetimeconfiguration.h"

#include <QTimer>
#include <QDateTime>
#include <QCalendarWidget>
#include <QLayout>

#define QSL(text) QStringLiteral(text)

DatetimePlugin::DatetimePlugin(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo)
{
    mWidget = new QToolButton;
    mWidget->setAutoRaise(true);
    mWidget->setMinimumWidth(1);
    mWidget->setMinimumHeight(1);

    QTimer *timer = new QTimer;
    timer->setInterval(1000);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));

    QCalendarWidget *calendar = new QCalendarWidget;
    calendar->setWindowFlags(Qt::Popup);
    calendar->setFixedSize(400, 300);
    connect(mWidget, &QToolButton::clicked, this, [=] {
        calendar->setGeometry(calculatePopupWindowPos(calendar->size())); // auto position
        if (calendar->isVisible())
            calendar->hide();
        else
            calendar->show();
    });

    mText = settings()->value(QSL("Memo"), QSL("")).toString();
}

DatetimePlugin::~DatetimePlugin() = default;

void DatetimePlugin::settingsChanged()
{
    mText = settings()->value(QSL("Memo"), QSL("")).toString();
}

void DatetimePlugin::realign()
{
    mWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

QDialog *DatetimePlugin::configureDialog(){
    return new DatetimeConfiguration(settings());
}

void DatetimePlugin::refresh()
{
    mText = settings()->value(QSL("Memo"), QSL("")).toString();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    mWidget->setText(QLocale::system().toString(currentDateTime, QSL("HH:mm\nM/d ddd")));
    mWidget->setToolTip(QLocale::system().toString(currentDateTime, QSL("yyyy-MM-dd dddd HH:mm:ss")) + QSL("\n") + mText.replace(QSL("\\n"), QSL("\n")));
}
