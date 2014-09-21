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
#ifndef QGESEEDLINKSTREAMXMLITEMMODEL_H
#define QGESEEDLINKSTREAMXMLITEMMODEL_H

#include <QAbstractItemModel>
#include <QtXml>
#include "qtseedlinkstreamxmlitem.h"

class QgeSeedLinkStreamXmlItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    QgeSeedLinkStreamXmlItemModel(QDomDocument document, QObject *parent = 0);
    ~QgeSeedLinkStreamXmlItemModel();
    
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void setDocument(QDomDocument document);

private:
    QDomDocument mDomDocument;
    QgeSeedLinkStreamXmlItem *mRootItem;

};

#endif // QGESEEDLINKSTREAMXMLMODEL_H
