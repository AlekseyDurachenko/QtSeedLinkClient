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
#include "src/cnewconnectiondialog.h"

#include <QtXml>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QMessageBox>
#include <QFileDialog>
#include <QCryptographicHash>
#include "global.h"
#include "cselectedstreammanager.h"
#include "ui_newconnectiondialog.h"

CNewConnectionDialog::CNewConnectionDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::NewConnectionDialog)
{
    G_SETTINGS_INIT();
    mConnectionPath = QFileInfo(settings.fileName()).absolutePath()+
            QDir::separator() + "connections";

    ui->setupUi(this);
    mIsStateChanged = false;
    mStateName = "";

    connect(ui->lineEdit_Host, SIGNAL(textChanged(QString)), SLOT(slot_host_changed()));
    connect(ui->spinBox_Port, SIGNAL(valueChanged(int)), SLOT(slot_host_changed()));
    connect(ui->lineEdit_Name, SIGNAL(textChanged(QString)), SLOT(slot_updateButtons()));

    // state changed connection
    connect(ui->lineEdit_Host, SIGNAL(textChanged(QString)), SLOT(slot_stateChanged()));
    connect(ui->spinBox_Port, SIGNAL(valueChanged(int)), SLOT(slot_stateChanged()));
    connect(ui->lineEdit_Name, SIGNAL(textChanged(QString)), SLOT(slot_stateChanged()));
    connect(ui->plainTextEdit_Description, SIGNAL(textChanged()), SLOT(slot_stateChanged()));
    connect(ui->streamListWidget, SIGNAL(selectedStreamChanged()), SLOT(slot_stateChanged()));

    preloadConnection();
    slot_updateButtons();
}

/*!
 * \brief CNewConnectionDialog::~CNewConnectionDialog
 */
CNewConnectionDialog::~CNewConnectionDialog()
{
    delete ui;
}

/*!
 * \brief CNewConnectionDialog::fileNameFromName
 * \param name the name of conneciton
 * \return fileName whithout path as md5 hash of the name plus ".xml" extension
 */
QString CNewConnectionDialog::fileNameFromName(const QString &name) const
{
    return QString(QCryptographicHash::hash(name.toUtf8(),
                        QCryptographicHash::Md5).toHex()).toUpper() + ".xml";
}

/*!
 * \brief CNewConnectionDialog::connectionPath
 * \return path to preset of connections
 */
const QString &CNewConnectionDialog::connectionPath() const
{
    return mConnectionPath;
}

/*!
 * \brief CNewConnectionDialog::preloadConnection
 *
 * This method preload the all connection presets was stored in the \see connectionPath()
 */
void CNewConnectionDialog::preloadConnection()
{
    foreach (const QString &fileName, QDir(connectionPath()).entryList(QDir::Files))
    {
        // fileName - is filename whithout path
        QFile openFile(connectionPath() + QDir::separator() + fileName);
        if (openFile.open(QIODevice::ReadOnly))
        {
            QXmlSchema schema;
            if (schema.load(QUrl("qrc:/qseedlinkviewer/xmlscheme/connection.preset.v1.xsd")))
            {
                QXmlSchemaValidator validator(schema);
                if (validator.validate(&openFile))
                {
                    if (!openFile.seek(0))
                        continue;

                    QDomDocument document;
                    if (!document.setContent(&openFile))
                        continue;

                    QString name = document.documentElement().attribute("name");
                    ui->comboBox_Preset->addItem(name, fileName);
                }
            }
        }
    }
}

/*!
 * \brief CNewConnectionDialog::name
 * \return The name of the conneciton
 */
QString CNewConnectionDialog::name() const
{
    return ui->lineEdit_Name->text();
}

/*!
 * \brief CNewConnectionDialog::host
 * \return The address of the server
 */
QString CNewConnectionDialog::host() const
{
    return ui->lineEdit_Host->text();
}

/*!
 * \brief CNewConnectionDialog::desc
 * \return the description of the connection
 */
QString CNewConnectionDialog::desc() const
{
    return ui->plainTextEdit_Description->toPlainText();
}

/*!
 * \brief CNewConnectionDialog::port
 * \return the port of the server
 */
quint16 CNewConnectionDialog::port() const
{
    return ui->spinBox_Port->value();
}

/*!
 * \brief CNewConnectionDialog::stationList
 * \return The staiton list for \see QgeSeedLinkClient configurating
 */
QgeSeedLinkStationList CNewConnectionDialog::stationList() const
{
    return ui->streamListWidget->toStationList();
}

/*!
 * \brief CNewConnectionDialog::slot_updateButtons
 * Updating all buttons of the dialog
 */
void CNewConnectionDialog::slot_updateButtons()
{
    ui->toolButton_PresetSaveAs->setEnabled(!ui->lineEdit_Name->text().isEmpty());
    ui->toolButton_PresetSave->setEnabled(!ui->lineEdit_Name->text().isEmpty() && mIsStateChanged);
}

/*!
 * \brief CNewConnectionDialog::slot_host_changed
 */
void CNewConnectionDialog::slot_host_changed()
{
    ui->streamListWidget->setHost(ui->lineEdit_Host->text(), ui->spinBox_Port->value());
}

/*!
 * \brief CNewConnectionDialog::slot_stateChanged
 * If any changes is happend this slot shuld be called for button state updating
 */
void CNewConnectionDialog::slot_stateChanged()
{
    if (!mIsStateChanged)
    {
        mIsStateChanged = true;
        slot_updateButtons();
    }
}

/*!
 * \brief CNewConnectionDialog::saveState
 * Save the current state (for connection managment purpose). It will be called
 * after Save, Open and New operation.
 */
void CNewConnectionDialog::saveState()
{
    mStateName = ui->lineEdit_Name->text();
    mIsStateChanged = false;
}

/*!
 * \brief CNewConnectionDialog::saveConnection
 * \param saveFileName fileName for the currenct connection parameters
 */
void CNewConnectionDialog::saveConnection(const QString &saveFileName)
{
    QFile saveFile(saveFileName);
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("Warning"), tr("Can't open %1 for write").arg(saveFileName));
    }
    else
    {
        QDomDocument document;
        QDomElement connectionElement = document.createElement("connection");
        connectionElement.setAttribute("name", ui->lineEdit_Name->text());
        connectionElement.setAttribute("host", ui->lineEdit_Host->text());
        connectionElement.setAttribute("desc", ui->plainTextEdit_Description->toPlainText());
        connectionElement.setAttribute("port", ui->spinBox_Port->value());
        foreach (const QgeSeedLinkStation &station, ui->streamListWidget->toStationList())
        {
            QDomElement stationElement = document.createElement("station");
            stationElement.setAttribute("network", station.net());
            stationElement.setAttribute("station", station.sta());
            for (int i = 0; i < station.channelCount(); ++i)
            {
                QDomElement channelElement = document.createElement("channel");
                channelElement.setAttribute("channel", station.chan(i));
                channelElement.setAttribute("location", station.loc(i));
                channelElement.setAttribute("dataflag", station.dataFlag(i));
                stationElement.appendChild(channelElement);
            }
            connectionElement.appendChild(stationElement);
        }
        document.appendChild(connectionElement);
        document.insertBefore(document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""),
                              document.firstChild());
        // save the document
        QTextStream textStream(&saveFile);
        document.save(textStream, 4);

        // check the save status
        if (textStream.status() == QTextStream::WriteFailed)
            QMessageBox::warning(this, tr("Warning"), tr("Can't write data to %1").arg(saveFileName));
    }
}

/*!
 * \brief CNewConnectionDialog::openConnection
 * \param openFileName exist fileName of the connction parameters
 */
void CNewConnectionDialog::openConnection(const QString &openFileName)
{
    QFile openFile(openFileName);
    if (!openFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("Warning"), tr("Can't open %1 for read").arg(openFileName));
        return;
    }

    // Loading sheme for validation of the xml data
    QXmlSchema schema;
    if (!schema.load(QUrl("qrc:/qseedlinkviewer/xmlscheme/connection.preset.v1.xsd")))
    {
        QMessageBox::critical(this, tr("Critical"),
                tr("Can't load xml scheme: qrc:/qseedlinkviewer/xmlscheme/connection.preset.v1.xsd"));
        return;
    }

    // The validation of the xml data
    QXmlSchemaValidator validator(schema);
    if (!validator.validate(&openFile))
    {
        QMessageBox::warning(this, tr("Warning"), tr("Incorect server response"));
        return;
    }

    // back to start of the file
    if (!openFile.seek(0))
    {
        QMessageBox::warning(this, tr("Warning"),
                tr("Oops! Unexpected error: %1 line %2").arg(__FILE__).arg(__LINE__));
        return;
    }

    // read document
    QDomDocument document;
    if (!document.setContent(&openFile))
    {
        QMessageBox::warning(this, tr("Warning"),
                tr("Oops! Unexpected error: %1 line %2").arg(__FILE__).arg(__LINE__));
        return;
    }

    // clear the old data
    ui->streamListWidget->clear();

    // load the new data from the xml
    QDomElement connElement = document.documentElement();
    ui->lineEdit_Name->setText(connElement.attribute("name", ""));
    ui->lineEdit_Host->setText(connElement.attribute("host", ""));
    ui->spinBox_Port->setValue(connElement.attribute("port", "18000").toInt());
    ui->plainTextEdit_Description->setPlainText(connElement.attribute("desc", ""));
    QDomElement stationElement = connElement.firstChildElement();
    while (!stationElement.isNull())
    {
        QString net = stationElement.attribute("network");
        QString sta = stationElement.attribute("station");
        QDomElement channelElement = stationElement.firstChildElement();
        while (!channelElement.isNull())
        {
            QString chan = channelElement.attribute("channel");
            QString loc = channelElement.attribute("location");
            QString dataFlag = channelElement.attribute("dataflag");
            ui->streamListWidget->add(net, sta, chan, loc, dataFlag);
            channelElement = channelElement.nextSiblingElement();
        }
        stationElement = stationElement.nextSiblingElement();
    }

    saveState();
    slot_updateButtons();
}

/*!
 * \brief CNewConnectionDialog::newConnection
 */
void CNewConnectionDialog::newConnection()
{
    ui->comboBox_Preset->setCurrentIndex(-1);
    ui->lineEdit_Name->clear();
    ui->lineEdit_Host->clear();
    ui->spinBox_Port->setValue(18000);
    ui->plainTextEdit_Description->clear();
    ui->streamListWidget->clear();

    saveState();
    slot_updateButtons();
}


/*!
 * \brief CNewConnectionDialog::on_toolButton_PresetNew_clicked
 */
void CNewConnectionDialog::on_toolButton_PresetNew_clicked()
{
    newConnection();
}

/*!
 * \brief CNewConnectionDialog::on_toolButton_PresetSave_clicked
 */
void CNewConnectionDialog::on_toolButton_PresetSave_clicked()
{
    QString newName = ui->lineEdit_Name->text();
    QString newFileName = connectionPath() + QDir::separator() + fileNameFromName(newName);
    QString oldFileName = connectionPath() + QDir::separator() + fileNameFromName(mStateName);

    // path not exists? create it!
    if (!QDir(connectionPath()).exists() && !QDir().mkpath(connectionPath()))
    {
        QMessageBox::warning(this, tr("Warning"), tr("Can't create path %s").arg(connectionPath()));
        return;
    }

    // replace? sure?
    if (QFileInfo(newFileName).exists() && (mStateName != newName))
        if (QMessageBox::question(this, tr("Warning"),
                    tr("Are you sure you want to replace another connection?"),
                    QMessageBox::Yes|QMessageBox::Cancel) == QMessageBox::Cancel)
            return;

    // remove prev connection
    if (QFileInfo(oldFileName).exists())
        if (!QFile::remove(oldFileName))
            QMessageBox::warning(this, tr("Warning"), tr("Can't remove %s").arg(oldFileName));

    // save new connection
    saveConnection(newFileName);

    // Change item name/data in the comboBox. Algorithm:
    // 1. "Connection Saved As Preset" and "Name is changed" and "New Name is not exists":
    //    change connection name and fileName data
    // 2. "Connection Saved As Preset" and "Name is changed" and "New Name is exists":
    //    remove old connection with exists name and add new connection with same name
    // 3. "Connection Not Saved As Preset":
    //    add new connection item
    //
    // - Always select current comboBox item (by name) as the active preset
    ui->comboBox_Preset->blockSignals(true);
    if (mStateName != "")
    {
        int newIndex = ui->comboBox_Preset->findText(newName);
        if (newIndex != -1)
        {
            ui->comboBox_Preset->setItemText(newIndex, newName);
            ui->comboBox_Preset->setItemData(newIndex, QFileInfo(newFileName).fileName());
        }
        else
        {
            ui->comboBox_Preset->addItem(newName, QFileInfo(newFileName).fileName());
        }
        if (mStateName != newName)
        {
            int oldIndex = ui->comboBox_Preset->findText(mStateName);
            ui->comboBox_Preset->removeItem(oldIndex);
        }
    }
    else
    {
        ui->comboBox_Preset->addItem(newName, QFileInfo(newFileName).fileName());
    }
    ui->comboBox_Preset->setCurrentIndex(ui->comboBox_Preset->findText(newName));
    ui->comboBox_Preset->blockSignals(false);

    saveState();
    slot_updateButtons();
}

/*!
 * \brief CNewConnectionDialog::on_toolButton_PresetOpen_clicked
 */
void CNewConnectionDialog::on_toolButton_PresetOpen_clicked()
{
    G_SETTINGS_INIT();

    // Choose the preset
    QString openFileName = QFileDialog::getOpenFileName(this, tr("Open Preset..."),
            settings.value("LastDirectory", "").toString(), tr("Xml (*.xml)"));

    // Choose canceled
    if (openFileName.isEmpty())
        return;

    // Store current directory
    settings.setValue("LastDirectory", QFileInfo(openFileName).absolutePath());

    // Open connection from the file
    openConnection(openFileName);
}

/*!
 * \brief CNewConnectionDialog::on_toolButton_PresetSaveAs_clicked
 */
void CNewConnectionDialog::on_toolButton_PresetSaveAs_clicked()
{
    G_SETTINGS_INIT();

    // Choose save file name
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save Preset As..."),
        settings.value("LastDirectory", "").toString(), tr("Xml (*.xml)"));

    // File not selected
    if (saveFileName.isEmpty())
        return;

    // Store current directory
    settings.setValue("LastDirectory", QFileInfo(saveFileName).absolutePath());

    // Save connection
    saveConnection(saveFileName);
}

/*!
 * \brief CNewConnectionDialog::on_comboBox_Preset_currentIndexChanged
 * \param index
 */
void CNewConnectionDialog::on_comboBox_Preset_currentIndexChanged(int index)
{
    if (index != -1)
        openConnection(connectionPath() + QDir::separator() +
                ui->comboBox_Preset->itemData(index, Qt::UserRole).toString());
}
