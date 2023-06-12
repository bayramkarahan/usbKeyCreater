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
icon.path = /usr/share/usbKeyCreater

rule.files = 99-local.rules
rule.path = /etc/udev/rules.d/

mount.files = usb-mount.sh
mount.path = /root/

service.files = usb-mount@.service
service.path = /etc/systemd/system/


INSTALLS += target icon desktop_file rule mount service
DISTFILES +=99-local.rules \
            usb-mount.sh \
            usb-mount@.service

RESOURCES += \
    icons.qrc

