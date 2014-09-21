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
#include "qtseedlinkstreamxmlitemmodel.h"

QgeSeedLinkStreamXmlItemModel::QgeSeedLinkStreamXmlItemModel(QDomDocument document, QObject *parent) :
    QAbstractItemModel(parent)
{
    mDomDocument = document;
    mRootItem = new QgeSeedLinkStreamXmlItem(mDomDocument.documentElement(), 0);
}

QgeSeedLinkStreamXmlItemModel::~QgeSeedLinkStreamXmlItemModel()
{
    delete mRootItem;
}

int QgeSeedLinkStreamXmlItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 5;
}

void QgeSeedLinkStreamXmlItemModel::setDocument(QDomDocument document)
{
    mDomDocument = document;
    if (mRootItem)
        delete mRootItem;
    mRootItem = new QgeSeedLinkStreamXmlItem(mDomDocument.documentElement(), 0);
    reset();
}

QVariant QgeSeedLinkStreamXmlItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    QgeSeedLinkStreamXmlItem *item = reinterpret_cast<QgeSeedLinkStreamXmlItem*>(index.internalPointer());

    if (item->node().toElement().tagName() == "station")
    {
        switch (index.column())
        {
        case 0:
            return item->node().toElement().attribute("network");
        case 1:
            return item->node().toElement().attribute("name");
        }
    }
    else
    {
        switch (index.column())
        {
        case 0:
            return item->parent()->node().toElement().attribute("network");
        case 1:
            return item->parent()->node().toElement().attribute("name");
        case 2:
            return item->node().toElement().attribute("seedname");
        case 3:
            return item->node().toElement().attribute("location");
        case 4:
            return item->node().toElement().attribute("type");
        }
    }

    return QVariant();
}

Qt::ItemFlags QgeSeedLinkStreamXmlItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant QgeSeedLinkStreamXmlItemModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex QgeSeedLinkStreamXmlItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QgeSeedLinkStreamXmlItem *parentItem;
    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<QgeSeedLinkStreamXmlItem*>(parent.internalPointer());

    QgeSeedLinkStreamXmlItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex QgeSeedLinkStreamXmlItemModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    QgeSeedLinkStreamXmlItem *childItem = static_cast<QgeSeedLinkStreamXmlItem*>(child.internalPointer());
    QgeSeedLinkStreamXmlItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == mRootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int QgeSeedLinkStreamXmlItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    QgeSeedLinkStreamXmlItem *parentItem;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<QgeSeedLinkStreamXmlItem*>(parent.internalPointer());

    return parentItem->node().childNodes().count();
}
