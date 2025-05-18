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

#ifndef CMUDPLUGIN_H
#define CMUDPLUGIN_H

#include <ilxqtpanelplugin.h>
#include <pluginsettings.h>

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QBoxLayout>
#include <QString>
#include <QDialog>

class CMUDPlugin : public QObject, public ILXQtPanelPlugin
{
    // Data available from ILXQtPanelPlugin:
    //
    // struct LXQT_PANEL_API ILXQtPanelPluginStartupInfo
    // {
    //     ILXQtPanel *lxqtPanel;
    //     PluginSettings *settings;
    //     const LXQt::PluginInfo *desktopFile;
    // };
    //
    // ILXQtPanelPlugin(const ILXQtPanelPluginStartupInfo &startupInfo):
    //     mSettings(startupInfo.settings),
    //     mPanel(startupInfo.lxqtPanel),
    //     mDesktopFile(startupInfo.desktopFile)
    //

    Q_OBJECT

public:
    CMUDPlugin(const ILXQtPanelPluginStartupInfo &startupInfo);
    ~CMUDPlugin();

    // this is the widget that gets added to panel,
    // should be available immediately after constructor
    virtual QWidget *widget() override { return mWidget; }

    // identifier for lxqt themes (qss)
    virtual QString themeId() const override {
        return QStringLiteral("CMUDPlugin");
    }

    // enum Flag {
    //     NoFlags              = 0,   ///< It does not have any properties set.
    //     PreferRightAlignment = 1,   /**< The plugin prefers right alignment (for cmud the clock plugin);
    //                                     otherwise the plugin prefers left alignment (like main menu).
    //                                     This flag is used only at the first start, later positions of all
    //                                     plugins are saved in a config, and this saved information is used. */
    //     HaveConfigDialog     = 2,   ///< The plugin have a configuration dialog.
    //     SingleInstance       = 4,   ///< The plugin allows only one instance to run.
    //     NeedsHandle          = 8    ///< The plugin needs a handle for the context menu
    // };
    virtual Flags flags() const { return HaveConfigDialog | PreferRightAlignment; }
    // settings
    //void settingsChanged();
    QDialog *configureDialog() override;

    // panel layout stuff
    bool isSeparate() const override { return false; }
    bool isExpandable() const override { return false; }
    // for layout update
    void realign() override;

private:
    QWidget *mWidget;
    QLabel *label;
    QProgressBar *PB_cp, *PB_mp;
    QBoxLayout *boxLayout;   
    QString KB(long b);
    QString BS(long b);
    //void settingsChanged();
    long int db, ub, dbt0, ubt0, dbt, ubt, tt0, idle0;
    int i, cp, mp;

private slots:
    void refresh();

};
// ***************************************************

class CMUDLibrary: public QObject, public ILXQtPanelPluginLibrary
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lxqt.org/Panel/PluginInterface/3.0")
    Q_INTERFACES(ILXQtPanelPluginLibrary)
public:
    ILXQtPanelPlugin *instance(const ILXQtPanelPluginStartupInfo &startupInfo) const
    {
        return new CMUDPlugin(startupInfo);
    }
};

#endif // CMUDPLUGIN_H
