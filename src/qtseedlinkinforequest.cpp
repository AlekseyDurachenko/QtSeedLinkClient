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
#include "qtseedlinkinforequest.h"

QgeSeedLinkInfoRequest::QgeSeedLinkInfoRequest(QObject *parent) :
    QObject(parent)
{
    mSeedLinkClient = new QgeSeedLinkClient(this);
    connect(mSeedLinkClient, SIGNAL(info(QgeSeedLinkClient::InfoLevel,QByteArray)), this, SLOT(info(QgeSeedLinkClient::InfoLevel,QByteArray)));
    connect(mSeedLinkClient, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

QgeSeedLinkInfoRequest::QgeSeedLinkInfoRequest(const QString &hostName, quint16 port, QObject *parent) :
    QObject(parent)
{
    mSeedLinkClient = new QgeSeedLinkClient(hostName, port, this);
    connect(mSeedLinkClient, SIGNAL(info(QgeSeedLinkClient::InfoLevel,QByteArray)), this, SLOT(info(QgeSeedLinkClient::InfoLevel,QByteArray)));
    connect(mSeedLinkClient, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void QgeSeedLinkInfoRequest::setHost(const QString &hostName, quint16 port)
{
    mSeedLinkClient->setHost(hostName, port);
}


void QgeSeedLinkInfoRequest::request(QgeSeedLinkClient::InfoLevels levels)
{
    mResponse = QgeSeedLinkInfoResponse();
    mSeedLinkClient->setFetchInfoLevels(levels);
    mSeedLinkClient->start();
}

void QgeSeedLinkInfoRequest::info(QgeSeedLinkClient::InfoLevel level, const QByteArray &xml)
{
    mResponse.setLevel(level, xml);
}

void QgeSeedLinkInfoRequest::disconnected()
{
    qDebug() << mResponse.levels();
    emit response(mResponse);
}
