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
#include "cstreamlistwidget.h"

#include <QXmlSchemaValidator>
#include <QXmlSchema>
#include <QMessageBox>
#include <QgeSeedLinkInfoRequest>
#include <QgeSeedLinkStreamXmlItemModel>

#include "ui_streamlistwidget.h"
#include "cselectedstreammanager.h"
#include "cselectedstreamitemmodel.h"

/*!
 * \brief CStreamListWidget::CStreamListWidget
 * \param parent
 */
CStreamListWidget::CStreamListWidget(QWidget *parent)
    : QWidget(parent)
{
    init();
}

/*!
 * \brief CStreamListWidget::CStreamListWidget
 * \param hostName
 * \param port
 * \param parent
 */
CStreamListWidget::CStreamListWidget(const QString &hostName, quint32 port, QWidget *parent)
    : QWidget(parent)
{
    init(hostName, port);
}

/*!
 * \brief CStreamListWidget::init
 * \param hostName
 * \param port
 */
void CStreamListWidget::init(const QString &hostName, quint32 port)
{
    mHostName = hostName;
    mPort = port;

    ui = new Ui::StreamListWidget;
    ui->setupUi(this);

    ui->resultStreamView->setModel(mManagerModel = new CSelectedStreamItemModel(mManager = new CSelectedStreamManager(this), this));
    ui->resultStreamView->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->seedlinkStreamView->setModel(mSeedLinkModel = new QgeSeedLinkStreamXmlItemModel(QDomDocument(), this));
    ui->seedlinkStreamView->header()->resizeSections(QHeaderView::ResizeToContents);

    connect(ui->seedlinkStreamView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(slot_updateButtons()));
    connect(ui->resultStreamView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(slot_updateButtons()));

    // changes spyer
    connect(mManager, SIGNAL(stationAdded(int,int)), this, SIGNAL(selectedStreamChanged()));
    connect(mManager, SIGNAL(channelAdded(int,int, int)), this, SIGNAL(selectedStreamChanged()));
    connect(mManager, SIGNAL(stationRemoved(int,int)), this, SIGNAL(selectedStreamChanged()));
    connect(mManager, SIGNAL(channelRemoved(int,int,int)), this, SIGNAL(selectedStreamChanged()));
    connect(mManager, SIGNAL(cleared()), this, SIGNAL(selectedStreamChanged()));

    setDataFlagFilter("D");
    slot_updateButtons();
}

/*!
 * \brief CStreamListWidget::~CStreamListWidget
 */
CStreamListWidget::~CStreamListWidget()
{
    delete ui;
}

/*!
 * \brief CStreamListWidget::toStationList
 * \return
 */
QgeSeedLinkStationList CStreamListWidget::toStationList() const
{
    return mManager->toStationList();
}

/*!
 * \brief CStreamListWidget::setDataFlagFilter
 * \param dataFlagFilter
 * \note currently this function is private. It will be public for future. but currently algorithm
 * not designed (visual algorithm for add\remove streams between panels)
 */
void CStreamListWidget::setDataFlagFilter(const QString &dataFlagFilter)
{
    mDataFlagFilter = dataFlagFilter.toUpper() + dataFlagFilter.toLower();

    // RegExp for manual stream adding
    QString pattern("^[A-Za-z0-9_]{1,2}\\.[A-Za-z0-9_]{1,5}\\.[A-Za-z0-9_]{1,3}\\.[A-Za-z0-9_]{0,2}\\.[%1]$");
    mRegExpCustomStreamMask = QRegExp(pattern.arg(mDataFlagFilter));
}

/*!
 * \brief CStreamListWidget::setHost
 * \param hostName
 * \param port
 */
void CStreamListWidget::setHost(const QString &hostName, quint16 port)
{
    mHostName = hostName;
    mPort = port;
}

/*!
 * \brief CStreamListWidget::reset
 * \note hostName, port and stream lists will be cleared
 */
void CStreamListWidget::clear()
{
    mHostName = "";
    mPort = 0;

    ui->lineEdit_Stream->clear();
    mSeedLinkModel->setDocument(QDomDocument());
    mManager->clear();
}

/*!
 * \brief CStreamListWidget::on_pushButton_Refresh_clicked
 */
void CStreamListWidget::on_pushButton_Refresh_clicked()
{
    // If it is empty or port is 0 than hostname is incorrect anyway
    if (mHostName.isEmpty() || !mPort)
        QMessageBox::information(this, tr("Information"), tr("Please enter the hostname on previous tab"));

    ui->pushButton_Refresh->setEnabled(false);
    ui->pushButton_RefreshCancel->setEnabled(true);
    ui->seedlinkStreamView->setEnabled(false);
    mSeedLinkModel->setDocument(QDomDocument());

    // Send the request to the seedlink server
    QgeSeedLinkInfoRequest* request = new QgeSeedLinkInfoRequest(mHostName, mPort, this);
    connect(request, SIGNAL(response(QgeSeedLinkInfoResponse)), this, SLOT(slot_responseReceived(QgeSeedLinkInfoResponse)));
    connect(request, SIGNAL(response(QgeSeedLinkInfoResponse)), request, SLOT(deleteLater()));
    connect(ui->pushButton_RefreshCancel, SIGNAL(clicked()), request, SLOT(deleteLater()));
    request->request(QgeSeedLinkClient::Streams);

    slot_updateButtons();
}

/*!
 * \brief CStreamListWidget::slot_responseReceived
 * \param response
 */
void CStreamListWidget::slot_responseReceived(const QgeSeedLinkInfoResponse& response)
{
    ui->pushButton_Refresh->setEnabled(true);
    ui->pushButton_RefreshCancel->setEnabled(false);
    ui->seedlinkStreamView->setEnabled(true);

    if (response.lastError().type() == QgeSeedLinkError::NoError)
    {
        // We used request "INFO STREAMS"
        QByteArray xml = response.level(QgeSeedLinkClient::Streams);

        // Loading sheme for validation of the xml data
        QXmlSchema schema;
        if (schema.load(QUrl("qrc:/xmlscheme/seedlink.stream.xsd")))
        {
            // The validation of the xml data
            QXmlSchemaValidator validator(schema);
            if (validator.validate(xml))
            {
                // Load streams to the seedlink strams model view
                QDomDocument document;
                document.setContent(xml);
                mSeedLinkModel->setDocument(document);
            }
            else
            {
                QMessageBox::warning(this, tr("Warning"), tr("Incorect server response"));
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Critical"),
                                  tr("Can't load xml scheme: qrc:/xmlscheme/seedlink.stream.xsd"));
        }

        //QFile file("~/out2.xml");
        //file.open(QIODevice::WriteOnly);
        //file.write(xml);
        //qDebug() << xml;
    }
    else
    {
        QMessageBox::warning(this, tr("SeedLink"), "Error");
    }

    slot_updateButtons();
}

/*!
 * \brief CStreamListWidget::on_toolButton_Add_clicked
 */
void CStreamListWidget::on_toolButton_Add_clicked()
{
    // Expected string format:
    // - NET.STA.CHAN.LOC.mDataFilterFlag
    QStringList itemList = ui->lineEdit_Stream->text().toUpper().split(".");
    mManager->add(itemList[0], itemList[1], itemList[2], itemList[3], itemList[4]);
    ui->lineEdit_Stream->selectAll();

    slot_updateButtons();
}

/*!
 * \brief CStreamListWidget::on_lineEdit_Stream_textChanged
 * \param text
 */
void CStreamListWidget::on_lineEdit_Stream_textChanged(const QString &text)
{
    ui->toolButton_Add->setEnabled(mRegExpCustomStreamMask.exactMatch(text));
}

/*!
 * \brief CStreamListWidget::on_toolButton_StreamAdd_clicked
 */
void CStreamListWidget::on_toolButton_StreamAdd_clicked()
{
    QModelIndexList indexList = ui->seedlinkStreamView->selectionModel()->selectedRows();
    foreach (const QModelIndex &index, indexList)
    {
        const QgeSeedLinkStreamXmlItem* item = QgeSeedLinkStreamXmlItem::fromIndex(index);
        for (int i = 0; i < item->channelCount(); ++i)
            mManager->add(item->network(), item->station(), item->channel(i), item->location(i), item->dataFlag(i));
    }

    slot_updateButtons();
}

/*!
 * \brief CStreamListWidget::on_toolButton_StreamAddAll_clicked
 */
void CStreamListWidget::on_toolButton_StreamAddAll_clicked()
{
    int indexRowCount = mSeedLinkModel->rowCount();
    for (int row = 0; row < indexRowCount; ++row)
    {
        QModelIndex index = mSeedLinkModel->index(row, 0);
        const QgeSeedLinkStreamXmlItem* item = QgeSeedLinkStreamXmlItem::fromIndex(index);
        for (int i = 0; i < item->channelCount(); ++i)
            mManager->add(item->network(), item->station(), item->channel(i), item->location(i), item->dataFlag(i));
    }

    slot_updateButtons();
}

/*!
 * \brief CStreamListWidget::on_toolButton_StreamRemoveAll_clicked
 */
void CStreamListWidget::on_toolButton_StreamRemoveAll_clicked()
{
    mManager->clear();

    slot_updateButtons();
}

/*!
 * \brief CStreamListWidget::on_toolButton_StreamRemove_clicked
 */
void CStreamListWidget::on_toolButton_StreamRemove_clicked()
{
    QModelIndexList indexList = ui->resultStreamView->selectionModel()->selectedRows();
    foreach (const QModelIndex &index, indexList)
        if (index.parent().isValid())
            mManager->remove(index.internalId(), index.row());
        else
            mManager->remove(index.row());

    slot_updateButtons();
}

/*!
 * \brief CStreamListWidget::slot_updateButtons
 */
void CStreamListWidget::slot_updateButtons()
{
    ui->toolButton_StreamRemove->setEnabled(ui->resultStreamView->selectionModel()->selectedRows().count());
    ui->toolButton_StreamRemoveAll->setEnabled(mManagerModel->rowCount());
    ui->toolButton_StreamAdd->setEnabled(ui->seedlinkStreamView->selectionModel()->selectedRows().count());
    ui->toolButton_StreamAddAll->setEnabled(mSeedLinkModel->rowCount());
}

/*!
 * \brief CStreamListWidget::on_pushButton_RefreshCancel_clicked
 */
void CStreamListWidget::on_pushButton_RefreshCancel_clicked()
{
    ui->pushButton_Refresh->setEnabled(true);
    ui->pushButton_RefreshCancel->setEnabled(false);
    ui->seedlinkStreamView->setEnabled(true);
}

/*!
 * \brief CStreamListWidget::add
 * \param net
 * \param sta
 * \param chan
 * \param loc
 * \param dataFlag
 */
void CStreamListWidget::add(const QString &net, const QString &sta, const QString &chan, const QString &loc, const QString &dataFlag)
{
    mManager->add(net, sta, chan, loc, dataFlag);
}
