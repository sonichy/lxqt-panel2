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

#ifndef DATETIMECONFIGURATION_H
#define DATETIMECONFIGURATION_H

//#include <QAbstractButton>
#include <QDialog>
#include <QLineEdit>
//#include <lxqtpanelglobals.h>
#include "pluginsettings.h"

//class QComboBox;
//namespace Ui {
//    class DatetimeConfiguration;
//};

//namespace GlobalKeyShortcut {
//    class Action;
//}

class DatetimeConfiguration : public QDialog
{
    Q_OBJECT
public:
    explicit DatetimeConfiguration(PluginSettings &settings, QWidget *parent = nullptr);
    explicit DatetimeConfiguration(PluginSettings *settings, QWidget *parent = nullptr) : DatetimeConfiguration(*settings, parent) {}
    ~DatetimeConfiguration() = default;
    PluginSettings &settings() const;

protected slots:
    void loadSettings() const;
    //void dialogButtonsAction(QAbstractButton *btn);

private:
    //Ui::DatetimeConfiguration *ui;
    PluginSettings &mSettings;
    //GlobalKeyShortcut::Action * mShortcut;
    QLineEdit *lineEdit;
};

#endif // LXQTPANELPLUGINCONFIGDIALOG_H
