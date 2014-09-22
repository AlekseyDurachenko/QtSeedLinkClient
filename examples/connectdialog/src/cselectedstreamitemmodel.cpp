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
#include "cselectedstreamitemmodel.h"

CSelectedStreamItemModel::CSelectedStreamItemModel(CSelectedStreamManager *manager, QObject *parent) :
    QAbstractItemModel(parent)
{
    mManager = manager;
    connect(mManager, SIGNAL(stationAboutToBeAdded(int,int)), SLOT(slotStationAdded(int,int)));
    connect(mManager, SIGNAL(stationAboutToBeRemoved(int,int)), SLOT(slotStationRemoved(int,int)));
    connect(mManager, SIGNAL(channelAboutToBeAdded(int,int,int)), SLOT(slotChannelAdded(int,int,int)));
    connect(mManager, SIGNAL(channelAboutToBeRemoved(int,int,int)), SLOT(slotChannelRemoved(int,int,int)));
    connect(mManager, SIGNAL(aboutToBeCleared()), SLOT(slotCleared()));
}

CSelectedStreamItemModel::~CSelectedStreamItemModel()
{
}

int CSelectedStreamItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 5;
}

QVariant CSelectedStreamItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    if (index.internalId() == -1)
    {
        switch (index.column())
        {
        case 0:
            return mManager->network(index.row());
        case 1:
            return mManager->station(index.row());
        }
    }
    else
    {
        switch (index.column())
        {
        case 0:
            return mManager->network(index.parent().row());
        case 1:
            return mManager->station(index.parent().row());
        case 2:
            return mManager->channel(index.internalId(), index.row());
        case 3:
            return mManager->location(index.internalId(), index.row());
        case 4:
            return mManager->dataFlag(index.internalId(), index.row());
        }
    }

    return QVariant();
}

Qt::ItemFlags CSelectedStreamItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant CSelectedStreamItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return tr("Network");
        case 1:
            return tr("Station");
        case 2:
            return tr("Channel");
        case 3:
            return tr("Location");
        case 4:
            return tr("Type");
        }
    }

    return QVariant();
}

QModelIndex CSelectedStreamItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid())
        return createIndex(row, column, -1);

    return createIndex(row, column, parent.row());
}

QModelIndex CSelectedStreamItemModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    if (child.internalId() == -1)
        return QModelIndex();

    return createIndex(child.internalId(), 0, -1);
}

int CSelectedStreamItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        return mManager->stationCount();

    if (parent.internalId() == -1)
        return mManager->channelCount(parent.row());

    return 0;
}

void CSelectedStreamItemModel::slotStationAdded(int stationIndexStart, int stationIndexEnd)
{
    beginInsertRows(QModelIndex(), stationIndexStart, stationIndexEnd);
    endInsertRows();
}

void CSelectedStreamItemModel::slotChannelAdded(int stationIndex, int streamIndexStart, int streamIndexEnd)
{
    beginInsertRows(createIndex(stationIndex, 0, -1), streamIndexStart, streamIndexEnd);
    endInsertRows();
}

void CSelectedStreamItemModel::slotStationRemoved(int stationIndexStart, int stationIndexEnd)
{
    beginRemoveRows(QModelIndex(), stationIndexStart, stationIndexEnd);
    endRemoveRows();
}

void CSelectedStreamItemModel::slotChannelRemoved(int stationIndex, int streamIndexStart, int streamIndexEnd)
{
    beginRemoveRows(createIndex(stationIndex, 0, -1), streamIndexStart, streamIndexEnd);
    endRemoveRows();
}

void CSelectedStreamItemModel::slotCleared()
{
    beginResetModel();
    endResetModel();
}
