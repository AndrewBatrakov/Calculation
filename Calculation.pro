#-------------------------------------------------
#
# Project created by QtCreator 2018-06-07T21:12:00
#
#-------------------------------------------------

QT      +=  core\
            sql\
            xml\
            network\
            widgets\
            printsupport\
            axcontainer\

TARGET = Calculation
TEMPLATE = app

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

RC_FILE   = EmployeeClient.rc

RESOURCES += \
    anyneedfiles.qrc

include(Calculations.pri)
include(Services.pri)
include(SQLTable.pri)
