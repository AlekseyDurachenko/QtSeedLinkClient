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
#ifndef QGESEEDLINKINFOREQUEST_H
#define QGESEEDLINKINFOREQUEST_H

#include <QObject>
#include "qtseedlinkclient.h"
#include "qtseedlinkinforesponse.h"

class QgeSeedLinkInfoRequest : public QObject
{
    Q_OBJECT
public:
    explicit QgeSeedLinkInfoRequest(QObject *parent = 0);
    QgeSeedLinkInfoRequest(const QString &hostName, quint16 port = 18000, QObject *parent = 0);

    void setHost(const QString &hostName, quint16 port = 18000);
    inline const QString &hostName() const;
    inline quint16 port() const;

signals:
    void response(const QgeSeedLinkInfoResponse& response);

public slots:
    void request(QgeSeedLinkClient::InfoLevels levels);

private slots:
    void info(QgeSeedLinkClient::InfoLevel level, const QByteArray& xml);
    void disconnected();

private:
    QgeSeedLinkInfoResponse mResponse;
    QgeSeedLinkClient *mSeedLinkClient;
    QString mHostName;
    quint16 mPort;

};

/*!
 * \brief host
 * \return
 */
inline const QString &QgeSeedLinkInfoRequest::hostName() const
{
    return mHostName;
}

/*!
 * \brief port
 * \return
 */
inline quint16 QgeSeedLinkInfoRequest::port() const
{
    return mPort;
}


#endif // QGESEEDLINKINFOREQUEST_H
