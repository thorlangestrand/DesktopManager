QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

RC_ICONS = C:\Users\admin\Desktop\PC\art\Icons\rinsmile.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Command.cpp \
    CommandForm.cpp \
    ExecuteProcess.cpp \
    GetCommands.cpp \
    HashCommand.cpp \
    RemoveCommandDirectory.cpp \
    ScuffButton.cpp \
    Warn.cpp \
    main.cpp \
    mainwindow.cpp \
    utf8_parse.cpp

HEADERS += \
    Command.h \
    CommandForm.h \
    CommandFormUiStorage.h \
    DirectoryErrorEnum.h \
    ExecuteProcess.h \
    HashCommand.h \
    ScuffButton.h \
    Warn.h \
    mainwindow.h \
    utf8_parse.h

FORMS += \
    mainwindow.ui

LIBS += -lole32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc \
    stylesheets.qrc

DISTFILES += \
    resources/DEATH.png
