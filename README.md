# LXQt-Panel plugin

## Dependence
lxqt-build-tools, clang  

## Build & Install
./install.sh
/usr/lib/x86_64-linux-gnu/lxqt-panel/*.so  
Arch: /usr/lib/lxqt-panel/*.so  
/usr/share/lxqt/lxqt-panel/*.desktop  

*.desktop  
[Desktop Entry]  
Type=Service  
ServiceTypes=LXQtPanel/Plugin  
Icon=clock  
Name=World Clock  
Comment=Displays a clock with calendar and time zones

## Restart
![alt](lxqt-panel_restart.jpg)  
## Debug: 
`killall lxqt-panel && lxqt-panel &`  

~/.config/lxqt/panel.conf  
[pluginName]

## Rename
### Change file content
renameplugin.sh Example Datetime  
### Change file name
exampleplugin.cpp → datetimeplugin.cpp  
exampleplugin.h → datetimeplugin.h  
...  
example.desktop.in → datetime.desktop.in  
example.desktop.yaml → datetime.desktop.yaml

## datetime.desktop.yaml
Desktop Entry/Name: "Datetime"  
Desktop Entry/Comment: "Datetime Comment"  
Desktop Entry/Name[zh_CN]: "日期时间"  
Desktop Entry/Comment[zh_CN]: "日期时间插件"

## Attention
### QString::QString(const char*)’ is private within this context
QStringLiteral("") needed.  

QString s(QByteArray) → QString s = QString::fromUtf8(QByteArray)  

configurationDialog do not use exec(), it will cause settings.setValue() fail.

### Failed to extract plugin meta data from '/usr/lib/lxqt-panel/libdatetime.so': '/usr/lib/lxqt-panel/libdatetime.so' is not a Qt plugin (metadata not found)"
set(CMAKE_AUTOMOC ON)

## Reference
https://github.com/elviosak/plugin-example
