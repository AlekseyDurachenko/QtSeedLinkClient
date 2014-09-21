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
#ifndef QGESEEDLINKINFORESPONSE_H
#define QGESEEDLINKINFORESPONSE_H

#include <QObject>
#include "qtseedlinkclient.h"

class QgeSeedLinkInfoResponse
{
public:
    explicit QgeSeedLinkInfoResponse();
    void setLevel(QgeSeedLinkClient::InfoLevel level, const QByteArray& xml);
    inline QByteArray level(QgeSeedLinkClient::InfoLevel level) const;
    inline const QgeSeedLinkError &lastError() const;
    inline QgeSeedLinkClient::InfoLevels levels() const;

private:
    QHash<QgeSeedLinkClient::InfoLevel, QByteArray> mLevelHash;
    QgeSeedLinkClient::InfoLevels mLevels;
    QgeSeedLinkError mLastError;

};

inline QByteArray QgeSeedLinkInfoResponse::level(QgeSeedLinkClient::InfoLevel level) const
{
    return mLevelHash[level];
}

inline const QgeSeedLinkError &QgeSeedLinkInfoResponse::lastError() const
{
    return mLastError;
}

inline QgeSeedLinkClient::InfoLevels QgeSeedLinkInfoResponse::levels() const
{
    return mLevels;
}


#endif // QGESEEDLINKINFORESPONSE_H
