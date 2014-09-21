// Copyright 2013, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#ifndef QGESEEDLINKSTATION_H
#define QGESEEDLINKSTATION_H

#include <QtCore>

/*!
 * \brief The QgeSeedLinkStation class provide one seedlink selector
 */
class QgeSeedLinkStation
{
public:
    QgeSeedLinkStation(const QString &net = QString(), const QString &sta = QString(),
                       const QString &seq = QString());
    void addChannel(const QString &chan, const QString &loc, const QString &dataFlag);
    inline int channelCount() const;
    inline const QString &net() const;
    inline const QString &sta() const;
    inline const QString &seq() const;
    inline const QString &chan(int i) const;
    inline const QString &loc(int i) const;
    inline const QString &dataFlag(int i) const;

private:
    QString mNet, mSta, mSeq;
    QStringList mChannels;
    QStringList mLocations;
    QStringList mDataFlags;

};
Q_DECLARE_METATYPE(QgeSeedLinkStation)

/*!
 * \brief QgeSeedLinkStationList The list of the \see QgeSeedLinkStation.
 * The synonyme for QList<QgeSeedLinkStation>
 */
typedef QList<QgeSeedLinkStation> QgeSeedLinkStationList;

/*!
 * \brief QgeSeedLinkSelector::channelCount
 * \return total count of selectors for current station
 */
inline int QgeSeedLinkStation::channelCount() const
{
    return mChannels.count();
}

/*!
 * \brief QgeSeedLinkStation::net
 * \return
 */
const QString &QgeSeedLinkStation::net() const
{
    return mNet;
}

/*!
 * \brief QgeSeedLinkStation::sta
 * \return
 */
const QString &QgeSeedLinkStation::sta() const
{
    return mSta;
}

/*!
 * \brief QgeSeedLinkStation::seq
 * \return
 */
const QString &QgeSeedLinkStation::seq() const
{
    return mSeq;
}

/*!
 * \brief QgeSeedLinkStation::chan
 * \param i
 * \return
 */
inline const QString &QgeSeedLinkStation::chan(int i) const
{
    return mChannels.at(i);
}

/*!
 * \brief QgeSeedLinkStation::loc
 * \param i
 * \return
 */
inline const QString &QgeSeedLinkStation::loc(int i) const
{
    return mLocations.at(i);
}

/*!
 * \brief QgeSeedLinkStation::dataFlag
 * \param i
 * \return
 */
inline const QString &QgeSeedLinkStation::dataFlag(int i) const
{
    return mDataFlags.at(i);
}

#endif // QGESEEDLINKSELECTOR_H
