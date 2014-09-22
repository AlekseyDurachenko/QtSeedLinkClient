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
#ifndef QGESEEDLINKSTATIONMANAGERITEMMODEL_H
#define QGESEEDLINKSTATIONMANAGERITEMMODEL_H

#include <QAbstractItemModel>
#include "cselectedstreammanager.h"

class CSelectedStreamItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    CSelectedStreamItemModel(CSelectedStreamManager *manager, QObject *parent = 0);
    ~CSelectedStreamItemModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

private slots:
    void slotStationAdded(int stationIndexStart, int stationIndexEnd);
    void slotChannelAdded(int stationIndex, int streamIndexStart, int streamIndexEnd);
    void slotStationRemoved(int stationIndexStart, int stationIndexEnd);
    void slotChannelRemoved(int stationIndex, int streamIndexStart, int streamIndexEnd);
    void slotCleared();

private:
    CSelectedStreamManager *mManager;

};

#endif // QGESEEDLINKSTATIONMANAGERITEMMODEL_H
