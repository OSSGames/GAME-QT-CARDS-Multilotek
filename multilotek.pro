# -------------------------------------------------
# Project created by QtCreator 2009-05-26T22:51:49
# -------------------------------------------------
TARGET = multilotek
TEMPLATE = app
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/drawing.cpp \
    src/statistics.cpp \
    src/export_import.cpp \
    src/about.cpp \
    src/globals.cpp
HEADERS += src/mainwindow.h \
    src/drawing.h \
    src/statistics.h \
    src/export_import.h \
    src/about.h \
    src/globals.h
FORMS += ui/mainwindow.ui \
    ui/about.ui
UI_DIR = cache
MOC_DIR = cache
RCC_DIR = cache
OBJECTS_DIR = cache
win32:RC_FILE = icon.rc
unix { 
    INSTALL_PREFIX = /usr
    target.path = $${INSTALL_PREFIX}/bin
    icons.path = $${INSTALL_PREFIX}/share/icons
    icons.files = images/$${TARGET}16.png \
        images/$${TARGET}32.png \
        images/$${TARGET}48.png \
        images/$${TARGET}64.png \
        images/$${TARGET}128.png
    desktop.path = $${INSTALL_PREFIX}/share/applications
    desktop.files = desktop/$${TARGET}.desktop
    INSTALLS = target \
        icons \
        desktop
}
OTHER_FILES += COPYING \
    README
RESOURCES += resource.qrc
TRANSLATIONS = translations/multilotek_pl_PL.ts
CODECFORTR = UTF-8
