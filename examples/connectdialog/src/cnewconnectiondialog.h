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
#ifndef CNEWCONNECTIONDIALOG_H
#define CNEWCONNECTIONDIALOG_H

#include <QDialog>
#include <QgeSeedLinkStation>

// ----------------------------------------------------------------------
namespace Ui {
class NewConnectionDialog;
}

// ----------------------------------------------------------------------
class CNewConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CNewConnectionDialog(QWidget *parent = 0);
    ~CNewConnectionDialog();

    QString name() const;
    QString host() const;
    QString desc() const;
    quint16 port() const;
    QgeSeedLinkStationList stationList() const;

private slots:
    void slot_updateButtons();
    void slot_host_changed();
    void slot_stateChanged();

    void on_toolButton_PresetNew_clicked();
    void on_toolButton_PresetOpen_clicked();
    void on_toolButton_PresetSave_clicked();
    void on_toolButton_PresetSaveAs_clicked();
    void on_comboBox_Preset_currentIndexChanged(int index);

private:
    void newConnection();
    void saveState();
    void preloadConnection();
    void saveConnection(const QString &saveFileName);
    void openConnection(const QString &openFileName);
    QString fileNameFromName(const QString &name) const;
    const QString &connectionPath() const;

private:
    Ui::NewConnectionDialog *ui;
private:
    QString mConnectionPath;
    QString mStateName;
    bool mIsStateChanged;

};

#endif // CNEWCONNECTIONDIALOG_H
