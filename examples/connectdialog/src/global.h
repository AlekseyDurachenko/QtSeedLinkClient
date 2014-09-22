// Copyright (C) 2013, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#ifndef GLOBAL_H
#define GLOBAL_H

#include "version.h"

/*! The settings initialization
*/
#define G_SETTINGS_INIT()                                               \
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,      \
        QString(gProjectFamily) + QDir::separator() + QString(gProjectName), gProgramName);

/*! The full programm name
*/
#define G_FULL_PROGRAM_NAME()                                           \
    (QString("%1 - %2.%3%4 build %5 (%6.%7.%8)")                        \
        .arg(gProgramName).arg(gVersionMajor).arg(gVersionMinor)        \
        .arg(gVersionStatusShort).arg(gVersionBuild)                    \
        .arg(gVersionYear).arg(gVersionMonth).arg(gVerstionDay))

/*! The short programm name
*/
#define G_SHORT_PROGRAM_NAME()                                          \
    (QString("%1").arg(gProgramName))

/*! About information
*/
#define G_PROGRAM_ABOUT()                                               \
    ("<center>" + G_FULL_PROGRAM_NAME() + "<br>" +                      \
    QString("Author: %1, %2").arg(gAuthors).arg(gYears) +                    \
    "</center>")

#endif
