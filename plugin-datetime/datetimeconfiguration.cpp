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

#include "datetimeconfiguration.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

DatetimeConfiguration::DatetimeConfiguration(PluginSettings &settings, QWidget *parent)
    : QDialog(parent)
    //, ui(new Ui::DatetimeConfiguration)
    , mSettings(settings)
{
    //setAttribute(Qt::WA_DeleteOnClose); //close crash
    setObjectName(QStringLiteral("DatetimeConfigurationWindow"));
    //ui->setupUi(this);
    setWindowTitle(tr("Datetime"));
    QVBoxLayout *vbox = new QVBoxLayout;
    QHBoxLayout *hbox = new QHBoxLayout;
    QLabel *label = new QLabel(QStringLiteral("Memo"));
    lineEdit = new QLineEdit;
    hbox->addWidget(label);
    hbox->addWidget(lineEdit);
    vbox->addLayout(hbox);
    hbox = new QHBoxLayout;
    hbox->addStretch();
    QPushButton *pushButton_confirm = new QPushButton(QStringLiteral("Confirm"));
    hbox->addWidget(pushButton_confirm);
    QPushButton *pushButton_cancel = new QPushButton(QStringLiteral("Cancel"));
    hbox->addWidget(pushButton_cancel);
    hbox->addStretch();
    vbox->addLayout(hbox);
    setLayout(vbox);
    setFixedSize(300, 200);    

    loadSettings();

    connect(pushButton_confirm, &QPushButton::clicked, [=](){
        this->settings().setValue(QStringLiteral("Memo"), lineEdit->text());
        close();
    });

    connect(pushButton_cancel, &QPushButton::clicked, [=](){
        close();
    });

}

void DatetimeConfiguration::loadSettings() const
{
    QString text = settings().value(QStringLiteral("Memo"), QStringLiteral("")).toString();
    lineEdit->setText(text);
}

PluginSettings& DatetimeConfiguration::settings() const
{
    return mSettings;
}