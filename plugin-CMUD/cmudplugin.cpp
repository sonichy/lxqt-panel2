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

#include "cmudplugin.h"
#include <QTimer>
#include <QFile>
#include <QIcon>
#include <QRegularExpression>

CMUDPlugin::CMUDPlugin(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo)
{
    mWidget = new QWidget;
    label = new QLabel;
    QFont font = label->font();
    font.setFamily("Droid Sans Mono");
    label->setFont(font);
    label->setText("↑   0KB/s\n↓   0KB/s");
    label->setStyleSheet("QToolTip { font-family: Droid Sans Mono; }");
    PB_mp = new QProgressBar;
    PB_cp = new QProgressBar;
    PB_mp->setOrientation(Qt::Vertical);
    PB_cp->setOrientation(Qt::Vertical);
    PB_mp->setTextVisible(false);
    PB_cp->setTextVisible(false);
    PB_mp->setFixedWidth(4);
    PB_cp->setFixedWidth(4);
    QString style = "QProgressBar:vertical { background-color:black; border:1px solid black; } QProgressBar::chunk:vertical { background-color:white; }"; 
    PB_mp->setStyleSheet(style);
    PB_cp->setStyleSheet(style);    
    boxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    boxLayout->addWidget(PB_mp);
    boxLayout->addWidget(label);
    boxLayout->addWidget(PB_cp);
    boxLayout->setContentsMargins(0,0,0,0);
    mWidget->setLayout(boxLayout);    
    mWidget->setMinimumWidth(1);
    mWidget->setMinimumHeight(1);

    QTimer *timer = new QTimer;
    timer->setInterval(1000);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));

    //connect(mWidget, &QToolButton::clicked, this, [this] {        
    //});

    i = dbt0 = ubt0 = dbt = ubt = 0;
}

CMUDPlugin::~CMUDPlugin() = default;

//void CMUDPlugin::settingsChanged()
//{    
//}

void CMUDPlugin::realign()
{
    mWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

QDialog *CMUDPlugin::configureDialog()
{
    //return new CMUDConfiguration(settings());
    QDialog *dialog = new QDialog;
    dialog->setWindowTitle("About");
    QHBoxLayout *hbox = new QHBoxLayout;
    QLabel* label1 = new QLabel;
    QIcon icon = QIcon::fromTheme("utilities-system-monitor");
    label1->setPixmap(icon.pixmap(icon.availableSizes().first()));
    hbox->addWidget(label1);
    QLabel *label2= new QLabel;
    label2->setOpenExternalLinks(true);
    label2->setText("<a href='https://github.com/sonichy/lxqt-panel2/tree/master/plugin-CMUD'>CMUD 2.0</a><br>CPU usage, Memory usage, Up netspeed, Down netspeed.<br>sonichy@163.com");
    hbox->addWidget(label2);
    dialog->setLayout(hbox);
    dialog->adjustSize();
    dialog->setFixedSize(dialog->size());
    return dialog;
}

void CMUDPlugin::refresh()
{
    QFile file("/proc/uptime");
    file.open(QIODevice::ReadOnly);
    QByteArray l = file.readLine();
    file.close();
    QTime t(0,0,0);
    t = t.addSecs(l.left(l.indexOf(".")).toInt());
    QString uptime = "UTM: " + t.toString("hh:mm:ss");

    file.setFileName("/proc/stat");
    file.open(QIODevice::ReadOnly);
    l = file.readLine();
    const char *ch;
    ch = l.constData();
    char cpu[5];
    long int user, nice, sys, idle, iowait, irq, softirq, tt;
    sscanf(ch, "%s%ld%ld%ld%ld%ld%ld%ld", cpu, &user, &nice, &sys, &idle, &iowait, &irq, &softirq);
    tt = user + nice + sys + idle + iowait + irq + softirq;
    file.close();
    QString cu = "";
    cp = ((tt - tt0) - (idle - idle0)) * 100 / (tt - tt0);
    if (i > 0)
       cu = "CPU: " + QString::number(cp) + "%";
    idle0 = idle;
    tt0 = tt;

    file.setFileName("/proc/meminfo");
    file.open(QIODevice::ReadOnly);
    l = file.readLine();
    long mt = l.replace("MemTotal:","").replace("kB","").replace(" ","").toLong();
    l = file.readLine();
    l = file.readLine();
    long ma = l.replace("MemAvailable:","").replace("kB","").replace(" ","").toLong();
    l = file.readLine();
    l = file.readLine();
    file.close();
    long mu = mt - ma;
    mp = mu * 100 / mt;
    QString mem = "MEM: " + QString("%1 / %2 = %3").arg(KB(mu)).arg(KB(mt)).arg(QString::number(mp) +"%");

    file.setFileName("/proc/net/dev");
    file.open(QIODevice::ReadOnly);
    l = file.readLine();
    l = file.readLine();
    dbt = ubt = 0;
    while (!file.atEnd()) {
        l = file.readLine();
        QRegularExpression re("\\s+");
        QString s(l);
        s = s.trimmed();       
        QStringList list = s.split(re);
        db = list.at(1).toLong();
        ub = list.at(9).toLong();
        dbt += db;
        ubt += ub;
    }
    file.close();
    QString dss = "";
    QString uss = "";
    if (i > 0) {
        long ds = dbt - dbt0;
        long us = ubt - ubt0;
        dss = BS(ds) + "/s";
        uss = BS(us) + "/s";        
        dbt0 = dbt;
        ubt0 = ubt;
    }
    QString netspeed = "↑" + uss + "\n↓" + dss;
    QString net = "UPB: " + BS(ubt) + "\nDNB: " + BS(dbt);

    i++;
    if (i > 2) i = 2;

    label->setText(netspeed);
    label->setToolTip(uptime + "\n" + cu + "\n" + mem + "\n" + net);
    PB_mp->setValue(mp);
    PB_cp->setValue(cp);
}

QString CMUDPlugin::KB(long k)
{
    QString s = "";
    if (k > 999999) {
        s = QString::number(k/(1024*1024.0), 'f', 2) + "GB";
    } else {
        if (k > 999) {
            s = QString::number(k/1024.0, 'f', 2) + "MB";
        } else {
            s = QString::number(k) + "KB";
        }
    }
    return s;
}

QString CMUDPlugin::BS(long b)
{
    QString s = "";
    float f;
    if (b > 999999999) {
        f = b / (1024 * 1024 * 1024.0);
        if (f >= 100)
            s = " " + QString::number(f, 'f', 0) + "GB";
        else if (f >= 10)
            s = QString::number(f, 'f', 1) + "GB";
        else
            s = QString::number(f, 'f', 2) + "GB";
    } else {
        if (b > 999999) {
            f = b / (1024 * 1024.0);
            if (f >= 100)
                s = " " + QString::number(f, 'f', 0) + "MB";
            else if (f >= 10)
                s = QString::number(f, 'f', 1) + "MB";
            else
                s = QString::number(f, 'f', 2) + "MB";
        } else {
            if (b > 999) {
                f = b / 1024;
                if (f >= 100)
                    s = " " + QString::number(f, 'f', 0) + "KB";
                else if (f >= 10)
                    s = "  " + QString::number(f, 'f', 0) + "KB";
                else
                    s = "   " + QString::number(f, 'f', 0) + "KB";
            } else
                s = "   0KB";
        }
    }
    return s;
}
