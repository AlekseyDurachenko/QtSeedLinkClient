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
#include "qtseedlinkstation.h"

/*!
 * \brief QgeSeedLinkSelector::QgeSeedLinkSelector
 * \param net
 * \param sta
 */
QgeSeedLinkStation::QgeSeedLinkStation(const QString &net, const QString &sta, const QString &seq)
{
    mNet = net;
    mSta = sta;
    mSeq = seq;
}

/*!
 * \brief QgeSeedLinkSelector::addChannel
 * \param chan
 * \param loc
 * \param dataFlag
 * \param seqNumber
 */
void QgeSeedLinkStation::addChannel(const QString &chan, const QString &loc, const QString &dataFlag)
{
    mChannels.push_back(chan);
    mLocations.push_back(loc);
    mDataFlags.push_back(dataFlag);
}
