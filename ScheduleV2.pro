#-------------------------------------------------
#
# Project created by QtCreator 2018-03-31T21:54:35
#
#-------------------------------------------------

QT += core gui xml printsupport sql
QT += widgets
TARGET = "Schedule v2.0"
TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
RC_ICONS = Icon.ico
VERSION = 0.0.0.1
QMAKE_TARGET_COMPANY = "lex1133's Software"
QMAKE_TARGET_PRODUCT = "Schedule v2.0"
QMAKE_TARGET_DESCRIPTION = "��������� ��� ������ � ������������ ��������� ����������"
QMAKE_TARGET_COPYRIGHT = "Copyright"

DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    xmlparser.cpp \
    catalogsform.cpp \
    scheduleform.cpp \
    bookaudienceform.cpp \
    settingsform.cpp \
    previousprojects.cpp

HEADERS += \
        mainwindow.h \
    items.h \
    xmlparser.h \
    catalogsform.h \
    scheduleform.h \
    bookaudienceform.h \
    settingsform.h \
    previousprojects.h

FORMS += \
        mainwindow.ui \
    catalogsform.ui \
    scheduleform.ui \
    bookaudienceform.ui \
    settingsform.ui \
    previousprojects.ui


RESOURCES += \
    icons.qrc




