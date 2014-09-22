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
#ifndef CSTREAMLISTWIDGET_H
#define CSTREAMLISTWIDGET_H

#include <QWidget>
#include <QgeSeedLinkInfoResponse>
#include <QgeSeedLinkStation>

// ----------------------------------------------------------------------
class QgeSeedLinkStreamXmlItemModel;
class CSelectedStreamItemModel;
class CSelectedStreamManager;
// ----------------------------------------------------------------------
namespace Ui {
    class StreamListWidget;
}

// ----------------------------------------------------------------------
class CStreamListWidget : public QWidget
{
    Q_OBJECT    
public:
    explicit CStreamListWidget(QWidget *parent = 0);
    CStreamListWidget(const QString &hostName, quint32 port, QWidget *parent = 0);
    ~CStreamListWidget();

    QgeSeedLinkStationList toStationList() const;    

signals:
    void selectedStreamChanged();

public slots:
    void add(const QString &net, const QString &sta, const QString &chan,
             const QString &loc, const QString &dataFlag);
    void setHost(const QString &hostName, quint16 port = 18000);
    void clear();

private slots:
    void slot_responseReceived(const QgeSeedLinkInfoResponse& responce);
    void slot_updateButtons();
    void on_lineEdit_Stream_textChanged(const QString &text);
    void on_pushButton_Refresh_clicked();
    void on_pushButton_RefreshCancel_clicked();
    void on_toolButton_Add_clicked();
    void on_toolButton_StreamAdd_clicked();
    void on_toolButton_StreamAddAll_clicked();
    void on_toolButton_StreamRemoveAll_clicked();
    void on_toolButton_StreamRemove_clicked();

private:
    void init(const QString &hostName = QString(), quint32 port = 0);
    void setDataFlagFilter(const QString &dataFlagFilter);

private:
    Ui::StreamListWidget *ui;
private:
    QgeSeedLinkStreamXmlItemModel *mSeedLinkModel;
    CSelectedStreamItemModel *mManagerModel;
    CSelectedStreamManager *mManager;
    QRegExp mRegExpCustomStreamMask;
    QString mDataFlagFilter;
    QString mHostName;
    quint16 mPort;

};

#endif // CSTREAMLISTWIDGET_H
