QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
TARGET = usbKeyCreater
SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

target.path = /usr/bin

desktop_file.files = usbKeyCreater.desktop
desktop_file.path = /usr/share/applications/

icon.files = icons/app.svg
icon.path = /usr/share/usbKeyCreater/

command.files = command.conf
command.path = /usr/share/usbKeyCreater/


service.files = usb-mount.service
service.path = /etc/systemd/system/

autostartdesktop.files = setmountusbdisklink.desktop
autostartdesktop.path = /etc/xdg/autostart/

disklinkscript.files = setmountusbdisklink.sh
disklinkscript.path = /usr/bin/

udevusb.files = udev-mountslist
udevusb.path = /usr/bin/

INSTALLS += target icon desktop_file service autostartdesktop disklinkscript udevusb command
DISTFILES +=usb-mount.service\
            setmountusbdisklink.desktop \
            setmountusbdisklink.sh \
            udev-mountslist \
            command.conf

RESOURCES += \
    icons.qrc

