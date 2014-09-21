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
#include "qtseedlinkstreamxmlitem.h"

QgeSeedLinkStreamXmlItem::QgeSeedLinkStreamXmlItem(const QDomNode &node, int row, QgeSeedLinkStreamXmlItem *parent)
{
    mDomNode = node;
    mRowNumber = row;
    mParentItem = parent;
    mIsStation = (mDomNode.toElement().tagName() == "station");

    // nodes not loaded
    mChildItems.resize(mDomNode.childNodes().count());
    for (int i = 0; i < mDomNode.childNodes().count(); ++i)
        mChildItems[i] = 0;
}

QgeSeedLinkStreamXmlItem::~QgeSeedLinkStreamXmlItem()
{
    qDeleteAll(mChildItems);
}

QgeSeedLinkStreamXmlItem *QgeSeedLinkStreamXmlItem::child(int i)
{
    if (mChildItems[i])
        return mChildItems[i];

    if (i >= 0 && i < mDomNode.childNodes().count())
    {
        QDomNode childNode = mDomNode.childNodes().item(i);
        QgeSeedLinkStreamXmlItem *childItem = new QgeSeedLinkStreamXmlItem(childNode, i, this);
        mChildItems[i] = childItem;
        return childItem;
    }

    return 0;
}

QgeSeedLinkStreamXmlItem *QgeSeedLinkStreamXmlItem::parent()
{
    return mParentItem;
}

QDomNode QgeSeedLinkStreamXmlItem::node() const
{
    return mDomNode;
}

int QgeSeedLinkStreamXmlItem::row()
{
    return mRowNumber;
}

bool QgeSeedLinkStreamXmlItem::isChannel() const
{
    return !mIsStation;
}

int QgeSeedLinkStreamXmlItem::channelCount() const
{
    return mIsStation ? (mDomNode.childNodes().count()) : (1);
}

QString QgeSeedLinkStreamXmlItem::station() const
{
    if (mIsStation)
        return mDomNode.toElement().attribute("name");
    return mDomNode.parentNode().toElement().attribute("name");
}

QString QgeSeedLinkStreamXmlItem::network() const
{
    if (mIsStation)
        return mDomNode.toElement().attribute("network");
    return mDomNode.parentNode().toElement().attribute("network");
}

QString QgeSeedLinkStreamXmlItem::channel(int i) const
{
    if (mIsStation)
        return mDomNode.childNodes().at(i).toElement().attribute("seedname");
    return mDomNode.toElement().attribute("seedname");
}

QString QgeSeedLinkStreamXmlItem::location(int i) const
{
    if (mIsStation)
        return mDomNode.childNodes().at(i).toElement().attribute("location");
    return mDomNode.toElement().attribute("location");
}

QString QgeSeedLinkStreamXmlItem::dataFlag(int i) const
{
    if (mIsStation)
        return mDomNode.childNodes().at(i).toElement().attribute("type");
    return mDomNode.toElement().attribute("type");
}

QgeSeedLinkStreamXmlItem *QgeSeedLinkStreamXmlItem::fromIndex(const QModelIndex &index)
{
    return reinterpret_cast<QgeSeedLinkStreamXmlItem*>(index.internalPointer());
}
