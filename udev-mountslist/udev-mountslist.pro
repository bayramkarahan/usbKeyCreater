QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#-ludev -lcrypto -lssl
CONFIG += link_pkgconfig
#DEFINES += LINK_LIBUDEV
PKGCONFIG += libudev

PKGCONFIG += libcrypto
PKGCONFIG += libssl

TARGET = udev-mountslist
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

target.path = /usr/bin

command.files = command.conf
command.path = /usr/share/usbkeycreater/


service.files = udev-mountslist.service
service.path = /etc/systemd/system/

autostartdesktop.files = setmountusbdisklink.desktop
autostartdesktop.path = /etc/xdg/autostart/

disklinkscript.files = setmountusbdisklink.sh
disklinkscript.path = /usr/bin/


INSTALLS += target service autostartdesktop disklinkscript command
SOURCES += \
        main.cpp

DISTFILES +=usb-mount.service\
            setmountusbdisklink.desktop \
            setmountusbdisklink.sh \
            command.conf



