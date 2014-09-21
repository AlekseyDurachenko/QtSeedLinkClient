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
#ifndef QGESEEDLINKSTREAMXMLITEM_H
#define QGESEEDLINKSTREAMXMLITEM_H

#include <QVector>
#include <QDomNode>
#include <QModelIndex>

class QgeSeedLinkStreamXmlItem
{
public:
    QgeSeedLinkStreamXmlItem(const QDomNode &node, int row, QgeSeedLinkStreamXmlItem *parent = 0);
    ~QgeSeedLinkStreamXmlItem();
    QgeSeedLinkStreamXmlItem *child(int i);
    QgeSeedLinkStreamXmlItem *parent();
    QDomNode node() const;
    int row();

    bool isChannel() const;
    int channelCount() const;
    QString station() const;
    QString network() const;
    QString channel(int i) const;
    QString location(int i) const;
    QString dataFlag(int i) const;

public:
    static QgeSeedLinkStreamXmlItem *fromIndex(const QModelIndex &index);

private:
    QDomNode mDomNode;
    QVector<QgeSeedLinkStreamXmlItem*> mChildItems;
    QgeSeedLinkStreamXmlItem *mParentItem;
    int mRowNumber;
    bool mIsStation;

};

#endif // QGESEEDLINKSTREAMXMLITEM_H
