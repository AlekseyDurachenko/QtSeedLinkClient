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
#include "qtseedlinkclient.h"
#include "qtminiseeddecoder.h"

/*!
 * \brief QgeSeedLinkClient::QgeSeedLinkClient
 * \param parent
 */
QgeSeedLinkClient::QgeSeedLinkClient(QObject *parent)
    : QObject(parent)
{
    mSocket = new QTcpSocket(this);
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(slot_readyRead()));
    connect(mSocket, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(mSocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(mSocket, SIGNAL(connected()), this, SLOT(slot_connected()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));

    mBuffer = new char [gSeedLinkRecordSize];
    mHostName = "127.0.0.1";
    mPort = 18000;
    mIsStarted = false;
    mReadyState = Nothing;
    mMode = Multistation;
}

/*!
 * \brief QgeSeedLinkClient::QgeSeedLinkClient
 * \param hostName
 * \param port
 * \param parent
 */
QgeSeedLinkClient::QgeSeedLinkClient(const QString &hostName, quint16 port, QObject *parent)
    : QObject(parent)
{
    mSocket = new QTcpSocket(this);
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(slot_readyRead()));
    connect(mSocket, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(mSocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(mSocket, SIGNAL(connected()), this, SLOT(slot_connected()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));

    mBuffer = new char [gSeedLinkRecordSize];
    mIsStarted = false;
    mHostName = hostName;
    mPort = port;
    mReadyState = Nothing;
    mMode = Multistation;
}

/*!
 * \brief QgeSeedLinkClient::~QgeSeedLinkClient
 */
QgeSeedLinkClient::~QgeSeedLinkClient()
{
    delete [] mBuffer;
}

/*!
 * \brief QgeSeedLinkClient::addFetchInfoLevel
 * \param level
 */
void QgeSeedLinkClient::setFetchInfoLevels(InfoLevels levels)
{
    mInfoLevels = levels;
}

/*!
 * \brief QgeSeedLinkClient::setMode
 * \param mode
 * \note the default mode is QgeSeedLinkClient::Multistation
 */
void QgeSeedLinkClient::setMode(QgeSeedLinkClient::Mode mode)
{
    mMode = mode;
}

/*!
 * \brief QgeSeedLinkClient::setHost
 * \param hostName
 * \param port
 */
void QgeSeedLinkClient::setHost(const QString &hostName, quint16 port)
{
    mHostName = hostName;
    mPort = port;
}


/*!
 * \brief QgeSeedLinkClient::start
 */
void QgeSeedLinkClient::start()
{
    mIsStarted = true;
    mSocket->connectToHost(mHostName, mPort);
    emit started();
}

/*!
 * \brief QgeSeedLinkClient::stop
 */
void QgeSeedLinkClient::stop()
{
    mIsStarted = false;
    mSocket->disconnectFromHost();
    emit stoped();
}

/*!
 * \brief QgeSeedLinkClient::slot_readyRead
 */
void QgeSeedLinkClient::slot_readyRead()
{
    if (mReadyState == Hello)
    {
        while (mSocket->canReadLine())
        {
            mHelloRespStringCount -= 1;

            if (mHelloRespStringCount == 1)
                mHelloSoftware = mSocket->readLine().trimmed();
            else if (mHelloRespStringCount == 0)
                mHelloOrganization = mSocket->readLine().trimmed();

            if (mHelloRespStringCount == 0)
            {
                if (mInfoLevels == NoLevels)
                    sendCommandStation();
                else
                    sendCommandNextInfoLevel();
            }

        }
    }
    else if (mReadyState == Station)
    {
        if (mSocket->canReadLine())
        {
            if (mSocket->readLine().trimmed() == "OK")
            {
                sendCommandSelect();
            }
            else
            {
                // ERROR
            }
        }
    }
    else if (mReadyState == Select)
    {
        if (mSocket->canReadLine())
        {
            if (mSocket->readLine().trimmed() == "OK")
            {
                sendCommandSelect();
            }
            else
            {
                // ERROR
            }
        }
    }
    else if (mReadyState == Data)
    {
        if (mSocket->canReadLine())
        {
            if (mSocket->readLine().trimmed() == "OK")
            {
                if (mStations.count() > mCurrentStation)
                    sendCommandStation();
                else
                    sendCommandEnd();
            }
            else
            {
                // ERROR
            }
        }
    }
    else if (mReadyState == End)
    {
        while (mSocket->bytesAvailable() >= gSeedLinkRecordSize)
        {
            if (mSocket->read(mBuffer, gSeedLinkRecordSize) == gSeedLinkRecordSize)
            {
                seedLinkPacket(mBuffer, &mBuffer[gSeedLinkHeaderSize]);
            }
            else
            {
                // ERROR
            }
        }
    }
}

/*!
 * \brief QgeSeedLinkClient::slot_connected
 */
void QgeSeedLinkClient::slot_connected()
{
    sendCommandHello();
}

/*!
 * \brief QgeSeedLinkClient::slot_disconnected
 */
void QgeSeedLinkClient::slot_disconnected()
{
}

/*!
 * \brief QgeSeedLinkClient::sendCommandHello
 */
void QgeSeedLinkClient::sendCommandHello()
{
    if (sendCommand("HELLO"))
    {
        mReadyState = Hello;
        mHelloRespStringCount = 2;
        mCurrentStation = 0;
    }
    else
    {
        // ERROR
    }
}

/*!
 * \brief QgeSeedLinkClient::sendCommandStation
 */
void QgeSeedLinkClient::sendCommandStation()
{
    if (mStations.count() > mCurrentStation && mMode == Multistation)
    {
        QString command = QString("STATION %1 %2").arg(mStations.at(mCurrentStation).sta(), mStations.at(mCurrentStation).net());

        if (sendCommand(command))
        {
            mReadyState = Station;
            mCurrentChannel = 0;
        }
        else
        {
            // ERROR
        }
    }
    else if (mMode == Unistation)
    {
        sendCommandData();
    }
    else
    {
        // ERROR
    }
}

/*!
 * \brief QgeSeedLinkClient::sendCommandSelect
 */
void QgeSeedLinkClient::sendCommandSelect()
{
    QgeSeedLinkStation station = mStations.at(mCurrentStation);

    if (station.channelCount() > mCurrentChannel)
    {
        QString command = QString("SELECT %1%2.%3").arg(station.loc(mCurrentChannel),
                                                        station.chan(mCurrentChannel),
                                                        station.dataFlag(mCurrentChannel));

        if (sendCommand(command))
        {
            mReadyState = Select;
            mCurrentChannel += 1;
        }
        else
        {
            // ERROR
        }
    }
    else
    {
        sendCommandData();
    }
}

/*!
 * \brief QgeSeedLinkClient::sendCommandData
 */
void QgeSeedLinkClient::sendCommandData()
{
    if (mMode == Multistation)
    {
        if (sendCommand(QString("DATA %1").arg(mStations.at(mCurrentStation).seq())))
        {
            mCurrentStation += 1;
            mReadyState = Data;
        }
        else
        {
            // ERROR
        }
    }
    else if (mMode == Unistation)
    {
        if (sendCommand("DATA"))
        {
            mReadyState = End;
        }
        else
        {
            // ERROR
        }
    }
}

/*!
 * \brief QgeSeedLinkClient::sendCommandEnd
 */
void QgeSeedLinkClient::sendCommandEnd()
{
    if (sendCommand("END"))
    {
        mReadyState = End;
    }
    else
    {
        // ERROR
    }
}

/*!
 * \brief QgeSeedLinkClient::sendCommandNextInfoLevel
 */
void QgeSeedLinkClient::sendCommandNextInfoLevel()
{
    if (mReadyState == Hello)
    {
        mReadyState = End;
        mCurrentInfoLevel = 0;
    }

    if (mInfoLevels & Id)
    {
        sendCommand("INFO ID");
        mInfoLevelList.push_back(Id);
    }
    if (mInfoLevels & Capabilities)
    {
        sendCommand("INFO CAPABILITIES");
        mInfoLevelList.push_back(Capabilities);
    }
    if (mInfoLevels & Stations)
    {
        sendCommand("INFO STATIONS");
        mInfoLevelList.push_back(Stations);
    }
    if (mInfoLevels & Streams)
    {
        sendCommand("INFO STREAMS");
        mInfoLevelList.push_back(Streams);
    }
    if (mInfoLevels & Gaps)
    {
        sendCommand("INFO GAPS");
        mInfoLevelList.push_back(Gaps);
    }
    if (mInfoLevels & Connections)
    {
        sendCommand("INFO CONNECTIONS");
        mInfoLevelList.push_back(Connections);
    }
    if (mInfoLevels & All)
    {
        sendCommand("INFO ALL");
        mInfoLevelList.push_back(All);
    }
}

void QgeSeedLinkClient::sendCommandBye()
{
    if (sendCommand("BYE"))
    {
        mIsStarted = false;
        mSocket->disconnectFromHost();
    }
    else
    {
        // ERROR
    }
}

/*!
 * \brief QgeSeedLinkClient::addStation
 * \param station
 */
void QgeSeedLinkClient::addStation(const QgeSeedLinkStation& station)
{
    mStations.push_back(station);
}

/*!
 * \brief QgeSeedLinkClient::addStation
 * \param stations
 */
void QgeSeedLinkClient::addStation(const QList<QgeSeedLinkStation> &stations)
{
    foreach (const QgeSeedLinkStation &station, stations)
        mStations.push_back(station);
}

/*!
 * \brief QgeSeedLinkClient::seedLinkPacket
 * \param slhdr 8 byte slheader
 * \param msrec 512 byte miniseed record
 */
void QgeSeedLinkClient::seedLinkPacket(char *slhdr, char *msrec)
{
    if (qstrncmp(slhdr, "SLINFO", 6))
    {
        seedLinkMsRecord(QByteArray::fromRawData(&slhdr[2], 6), msrec, gSeedLinkMsRecordSize);
    }
    else
    {
        QgeMiniSeedRecord* rec = QgeMiniSeedDecoder::decode(msrec, gSeedLinkMsRecordSize);
        if (slhdr[7] == '*')
        {
            mInfoLevelXml.push_back(rec->toAscii());
        }
        else
        {
            mInfoLevelXml.push_back(rec->toAscii());
            seedLinkInfoLevel(mInfoLevelList[mCurrentInfoLevel], mInfoLevelXml);
            mInfoLevelXml.clear();

            // end session
            if (mCurrentInfoLevel >= mInfoLevelList.count() - 1)
                sendCommandBye();
            else
                mCurrentInfoLevel += 1;
        }
        delete rec;
    }
}

/*!
 * \brief QgeSeedLinkClient::sendCommand
 * \param command
 * \return
 */
bool QgeSeedLinkClient::sendCommand(const QString &command)
{
    qDebug() << command;
    QByteArray cmd = (command + "\r\n").toAscii();
    return (mSocket->write(cmd) == cmd.count());
}

/*!
 * \brief QgeSeedLinkClient::seedLinkMsRecord
 * \param seqNumber seedlink packet sequence number
 * \param msrec miniseed record
 * \param size size of miniseed record
 */
void QgeSeedLinkClient::seedLinkMsRecord(const QString &seqNumber, const char *msrec, qint64 size)
{
    int pos = 0;
    QgeMiniSeedDecoder decoder;
    qDebug() << seqNumber << decoder.readRecord(msrec, size, &pos)->sampleCount();
}

/*!
 * \brief QgeSeedLinkClient::seedLinkInfoLevel
 * \param level
 * \param xml
 */
void QgeSeedLinkClient::seedLinkInfoLevel(QgeSeedLinkClient::InfoLevel level, const QByteArray &xml)
{
    emit info(level, xml);
}

