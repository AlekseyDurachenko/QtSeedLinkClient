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
#ifndef QGESEEDLINKCLIENT_H
#define QGESEEDLINKCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QgeMiniSeedRecord>
#include <QgeMiniSeedDecoder>
#include "qtseedlinkerror.h"
#include "qtseedlinkstation.h"

/*!
 * \brief gSeedLinkHeaderSize the header size of seedlink packet
 * \brief gSeedLinkMsRecordSize the size of miniseed record. seedlink protocol supporty only record with size 512 bytes
 * \brief gSeedLinkRecordSize the size of seedlink packet with miniseed record size
 */
const qint64 gSeedLinkHeaderSize = 8;
const qint64 gSeedLinkMsRecordSize = 512;
const qint64 gSeedLinkRecordSize = gSeedLinkHeaderSize + gSeedLinkMsRecordSize;

/*!
 * \brief The QgeSeedLinkClient class provides basic operation for manipulation with seedlink servers.
 */
class QgeSeedLinkClient : public QObject
{
    Q_OBJECT
public:
    enum Mode
    {
        Multistation = 0,
        Unistation = 1
    };

    enum InfoLevel
    {
        NoLevels        = 0x00,
        Id              = 0x01,
        Capabilities    = 0x02,
        Stations        = 0x04,
        Streams         = 0x08,
        Gaps            = 0x10,
        Connections     = 0x20,
        All             = 0x40
    };
    Q_DECLARE_FLAGS(InfoLevels, InfoLevel)

private:
    enum ReadyState
    {
        Nothing = 0,
        Hello,
        Station,
        Select,
        Data,
        End
    };

public:
    explicit QgeSeedLinkClient(QObject *parent = 0);
    QgeSeedLinkClient(const QString &hostName, quint16 port = 18000, QObject *parent = 0);
    ~QgeSeedLinkClient();

    void setFetchInfoLevels(InfoLevels levels);

    void setMode(Mode mode);
    inline Mode mode() const;
    void setHost(const QString &hostName, quint16 port = 18000);
    inline const QString &host() const;
    inline quint16 port() const;

    void addStation(const QgeSeedLinkStation &station);
    void addStation(const QList<QgeSeedLinkStation> &stations);

protected:
    virtual void seedLinkMsRecord(const QString &seqNumber, const char *msrec, qint64 size);
    virtual void seedLinkInfoLevel(InfoLevel level, const QByteArray& xml);

signals:
    void disconnected();
    void connected();
    void started();
    void stoped();
    void error(const QgeSeedLinkError &err);
    void info(QgeSeedLinkClient::InfoLevel level, const QByteArray& xml);

public slots:
    void start();
    void stop();

private slots:
    void slot_readyRead();
    void slot_connected();
    void slot_disconnected();

private:
    void seedLinkPacket(char *slhdr, char *msrec);

    bool sendCommand(const QString &command);

    void sendCommandHello();
    void sendCommandStation();
    void sendCommandSelect();
    void sendCommandData();
    void sendCommandEnd();
    void sendCommandNextInfoLevel();
    void sendCommandBye();

private:
    // Ready state
    ReadyState mReadyState;
    int mCurrentStation;
    int mCurrentChannel;

    InfoLevels mInfoLevels;
    QList<InfoLevel> mInfoLevelList;
    QByteArray mInfoLevelXml;
    int mCurrentInfoLevel;

    Mode mMode;
    char *mBuffer;
    QTcpSocket *mSocket;
    QString mHostName;
    quint16 mPort;
    bool mIsStarted;
    QList<QgeSeedLinkStation> mStations;

    //
    int mHelloRespStringCount;
    QString mHelloSoftware;
    QString mHelloOrganization;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QgeSeedLinkClient::InfoLevels)

/*!
 * \brief host
 * \return
 */
inline const QString &QgeSeedLinkClient::host() const
{
    return mHostName;
}

/*!
 * \brief port
 * \return
 */
inline quint16 QgeSeedLinkClient::port() const
{
    return mPort;
}

/*!
 * \brief mode
 * \return
 */
inline QgeSeedLinkClient::Mode QgeSeedLinkClient::mode() const
{
    return mMode;
}

#endif // QGESEEDLINKCLIENT_H
