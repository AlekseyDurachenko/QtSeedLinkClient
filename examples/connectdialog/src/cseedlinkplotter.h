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
#ifndef CSEEDLINKPLOTTER_H
#define CSEEDLINKPLOTTER_H

#include <QgeSeedLinkClient>
#include <QtCore>
#include <QtGui>
#include "cstreamplotitem.h"

class CSeedLinkPlotter : public QgeSeedLinkClient
{
    Q_OBJECT
public:
    explicit CSeedLinkPlotter(QObject *parent = 0);

signals:
public slots:
protected:
    void seedLinkMsRecord(const QString &seqNumber, const char *msrec, qint64 size);

private:
    QHash<QString, CStreamPlotItem> mPlotItemHash;

};

#endif // CSEEDLINKCLIENT_H
