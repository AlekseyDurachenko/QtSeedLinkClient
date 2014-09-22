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
#include "cselectedstreammanager.h"

CSelectedStreamManager::CSelectedStreamManager(QObject *parent) : QObject(parent)
{
}

QString CSelectedStreamManager::createKey(const QString &net, const QString &sta) const
{
    return net + "." + sta;
}

void CSelectedStreamManager::add(const QString &net, const QString &sta, const QString &chan,
                                   const QString &loc, const QString &dataFlag)
{
    QString key = createKey(net, sta);
    if (mStationIndex.contains(key))
    {
        int sIndex = mStationIndex[key];
        QList<tChannel> &channels = mStations[sIndex].channels;
        // fild for dublicates
        for (int i = 0; i < channels.count(); ++i)
            if (channels[i].channel == chan && channels[i].location == loc && channels[i].dataFlag == dataFlag)
                // stream already exists
                return;
        // add new channel to the current station
        int cIndex = channels.count();
        emit channelAboutToBeAdded(sIndex, cIndex, cIndex);
        channels.append(tChannel(chan, loc, dataFlag));
        emit channelAdded(sIndex, cIndex, cIndex);
    }
    else
    {
        // add new station && add to station new channel
        int sIndex = mStations.count();
        emit stationAboutToBeAdded(sIndex, sIndex);
        mStationIndex[key] = sIndex;
        mStations.append(tStation(net, sta, tChannel(chan, loc, dataFlag)));
        emit stationAdded(sIndex, sIndex);
    }
}

void CSelectedStreamManager::remove(const QString &net, const QString &sta)
{
    QString key = createKey(net, sta);
    if (mStationIndex.contains(key))
    {
        int sIndex = mStationIndex[key];
        emit stationAboutToBeRemoved(sIndex, sIndex);
        mStations.removeAt(sIndex);
        mStationIndex.remove(key);
        emit stationRemoved(sIndex, sIndex);
    }
}

void CSelectedStreamManager::remove(int sIndex)
{
    QString key = createKey(mStations[sIndex].network, mStations[sIndex].station);
    emit stationAboutToBeRemoved(sIndex, sIndex);
    mStations.removeAt(sIndex);
    mStationIndex.remove(key);
    emit stationRemoved(sIndex, sIndex);
}

void CSelectedStreamManager::remove(const QString &net, const QString &sta, const QString &chan,
                                      const QString &loc, const QString &dataFlag)
{
    QString key = createKey(net, sta);
    if (mStationIndex.contains(key))
    {
        int sIndex = mStationIndex[key];
        QList<tChannel> &channels = mStations[sIndex].channels;
        // find channel for remove
        for (int i = 0; i < channels.count(); ++i)
            if (channels[i].channel == chan && channels[i].location == loc && channels[i].dataFlag == dataFlag)
            {
                if (channels.count() == 1)
                {
                    // for last channel we must remove the station
                    emit stationAboutToBeRemoved(sIndex, sIndex);
                    mStations.removeAt(sIndex);
                    mStationIndex.remove(key);
                    emit stationRemoved(sIndex, sIndex);
                }
                else
                {
                    // otherwise remove only founded channel
                    emit channelAboutToBeRemoved(sIndex, i, i);
                    channels.removeAt(i);
                    emit channelRemoved(sIndex, i, i);
                }

                break;
            }
    }
}

void CSelectedStreamManager::remove(int sIndex, int cIndex)
{
    QString key = createKey(mStations[sIndex].network, mStations[sIndex].station);
    QList<tChannel> &channels = mStations[sIndex].channels;
    if (channels.count() == 1)
    {
        // for last channel we must remove the station
        emit stationAboutToBeRemoved(sIndex, sIndex);
        mStations.removeAt(sIndex);
        mStationIndex.remove(key);
        emit stationRemoved(sIndex, sIndex);
    }
    else
    {
        // otherwise remove only founded channel
        emit channelAboutToBeRemoved(sIndex, cIndex, cIndex);
        channels.removeAt(cIndex);
        emit channelRemoved(sIndex, cIndex, cIndex);
    }
}

void CSelectedStreamManager::clear()
{
    emit aboutToBeCleared();
    mStationIndex.clear();
    mStations.clear();
    emit cleared();
}

QList<QgeSeedLinkStation> CSelectedStreamManager::toStationList() const
{
    QList<QgeSeedLinkStation> lst;
    for (int sIndex = 0; sIndex < mStations.count(); ++sIndex)
    {
        QgeSeedLinkStation station(mStations[sIndex].network, mStations[sIndex].station);
        const QList<tChannel> &channels = mStations[sIndex].channels;
        for (int cIndex = 0; cIndex < channels.count(); ++cIndex)
            station.addChannel(channels[cIndex].channel, channels[cIndex].location, channels[cIndex].dataFlag);
        lst.append(station);
    }
    return lst;
}
