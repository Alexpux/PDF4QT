#    Copyright (C) 2021 Jakub Melka
#
#    This file is part of PDF4QT.
#
#    PDF4QT is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    with the written consent of the copyright owner, any later version.
#
#    PDF4QT is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with PDF4QT.  If not, see <https://www.gnu.org/licenses/>.

TEMPLATE = lib
DEFINES += OUTPUTPREVIEWPLUGIN_LIBRARY

QT += gui widgets

include(../../Pdf4Qt.pri)

LIBS += -L$$OUT_PWD/../..

LIBS += -lPdf4QtLib

INCLUDEPATH += $$PWD/../../Pdf4QtLib/Sources

DESTDIR = $$OUT_PWD/../../pdfplugins

SOURCES += \
    inkcoveragedialog.cpp \
    outputpreviewdialog.cpp \
    outputpreviewplugin.cpp \
    outputpreviewwidget.cpp

HEADERS += \
    inkcoveragedialog.h \
    outputpreviewdialog.h \
    outputpreviewplugin.h \
    outputpreviewwidget.h

CONFIG += force_debug_info

DISTFILES += \
    OutputPreviewPlugin.json

RESOURCES += \
    icons.qrc

FORMS += \
    inkcoveragedialog.ui \
    outputpreviewdialog.ui
