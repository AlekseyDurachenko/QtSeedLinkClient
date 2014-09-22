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
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <qgeminiseedrecord.h>
#include <qgeminiseeddecoder.h>
#include <qgeseedlinkclient.h>
#include <qgeseedlinkinforequest.h>
#include <QgeDateTime>
#include "cmainwindow.h"
#include "cnewconnectiondialog.h"

int main(int argc, char *argv[])
{
    QTime time;
    time.start();
    QApplication app(argc, argv, true);
    //QCoreApplication app(argc, argv);
    qDebug() << "elapsed: %d ms" << time.elapsed();

    CMainWindow wnd;
    wnd.show();
    //CNewConnectionDialog dlg;
    //dlg.show();
    return app.exec();

}
