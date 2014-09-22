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
#ifndef QGESEEDLINKSTREAMMANAGER_H
#define QGESEEDLINKSTREAMMANAGER_H

#include <QtCore>
#include <QObject>
#include <QgeSeedLinkStation>

class CSelectedStreamManager : public QObject
{
    Q_OBJECT
public:
    explicit CSelectedStreamManager(QObject *parent = 0);

    inline int stationCount() const;
    inline int channelCount(int sIndex) const;
    inline const QString &network(int sIndex) const;
    inline const QString &station(int sIndex) const;
    inline const QString &channel(int sIndex, int cIndex) const;
    inline const QString &location(int sIndex, int cIndex) const;
    inline const QString &dataFlag(int sIndex, int cIndex) const;

    QList<QgeSeedLinkStation> toStationList() const;

signals:
    void stationAboutToBeAdded(int stationIndexStart, int stationIndexEnd);
    void channelAboutToBeAdded(int stationIndex, int streamIndexStart, int streamIndexEnd);
    void stationAboutToBeRemoved(int stationIndexStart, int stationIndexEnd);
    void channelAboutToBeRemoved(int stationIndex, int streamIndexStart, int streamIndexEnd);
    void aboutToBeCleared();
    void stationAdded(int stationIndexStart, int stationIndexEnd);
    void channelAdded(int stationIndex, int streamIndexStart, int streamIndexEnd);
    void stationRemoved(int stationIndexStart, int stationIndexEnd);
    void channelRemoved(int stationIndex, int streamIndexStart, int streamIndexEnd);
    void cleared();

public slots:
    void add(const QString &net, const QString &sta, const QString &chan,
             const QString &loc, const QString &dataFlag);
    void remove(const QString &net, const QString &sta);
    void remove(const QString &net, const QString &sta, const QString &chan,
                const QString &loc, const QString &dataFlag);
    void remove(int sIndex);
    void remove(int sIndex, int cIndex);
    void clear();

private:
    QString createKey(const QString &net, const QString &sta) const;

private:
    struct tChannel {
        tChannel(const QString &chan, const QString &loc, const QString &dflag)
        { channel = chan; location = loc; dataFlag = dflag; }
        QString channel;
        QString location;
        QString dataFlag;
    };
    struct tStation {
        tStation(const QString &net, const QString &sta)
        { network = net; station = sta; }
        tStation(const QString &net, const QString &sta, const tChannel &chan)
        { network = net; station = sta; channels.append(chan); }
        QString network;
        QString station;
        QList<tChannel> channels;
    };

    QList<tStation> mStations;
    QHash<QString, int> mStationIndex;

};

inline int CSelectedStreamManager::stationCount() const
{ return mStations.count(); }

inline int CSelectedStreamManager::channelCount(int sIndex) const
{ return mStations[sIndex].channels.count(); }

inline const QString &CSelectedStreamManager::network(int sIndex) const
{ return mStations[sIndex].network; }

inline const QString &CSelectedStreamManager::station(int sIndex) const
{ return mStations[sIndex].station; }

inline const QString &CSelectedStreamManager::channel(int sIndex, int cIndex) const
{ return mStations[sIndex].channels[cIndex].channel; }

inline const QString &CSelectedStreamManager::location(int sIndex, int cIndex) const
{ return mStations[sIndex].channels[cIndex].location; }

inline const QString &CSelectedStreamManager::dataFlag(int sIndex, int cIndex) const
{ return mStations[sIndex].channels[cIndex].dataFlag; }

#endif // QGESEEDLINKSTREAMMANAGER_H
