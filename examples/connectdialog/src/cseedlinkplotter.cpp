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
#include "cseedlinkplotter.h"
#include <QgeMiniSeedRecord>
#include <QgeMiniSeedDecoder>

#include <QDebug>

CSeedLinkPlotter::CSeedLinkPlotter(QObject *parent) :
    QgeSeedLinkClient(parent)
{
}

void CSeedLinkPlotter::seedLinkMsRecord(const QString &seqNumber, const char *msrec, qint64 size)
{
    QgeMiniSeedRecord *record = QgeMiniSeedDecoder::decode(msrec, size);
    if (record)
    {
        QString key = QString("%1.%2.%3.%4").arg(record->network(), record->station(),
                record->channel(), record->location());

        if (!mPlotItemHash.contains(key))
            mPlotItemHash[key] = CStreamPlotItem();

        mPlotItemHash[key].addRecord(record);
    }
    else
    {
        qWarning() << "Error in crecord: " << seqNumber;
    }
}
