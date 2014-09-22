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
#include "src/cmainwindow.h"
#include "ui_mainwindow.h"
#include "cnewconnectiondialog.h"
#include "cseedlinkplotter.h"
#include <QgeWaveform>


CMainWindow::CMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<QgeWaveformProperty>("QgeWaveformProperty");
    // create switcher of multiwindow view
    QActionGroup *windowViewModeGroup = new QActionGroup(this);
    ui->action_SubWindowViewMode->setCheckable(true);
    ui->action_TabbedViewMode->setCheckable(true);
    ui->action_SubWindowViewMode->setChecked(true);
    windowViewModeGroup->addAction(ui->action_SubWindowViewMode);
    windowViewModeGroup->addAction(ui->action_TabbedViewMode);

//    QgeWaveform *wf = new QgeWaveform(this);
//    connect(wf, SIGNAL(changed(QgeWaveformProperty,QVariantMap,qint64,bool,qint64,qint64)),
//            this, SLOT(changed(QgeWaveformProperty,QVariantMap,qint64,bool,qint64,qint64)),
//            Qt::QueuedConnection);
//    wf->lockForWrite();
//    wf->setSampleCount(100);
//    wf->setSampleType(QgeDataType::Float64);
//    wf->setSampleCount(100);
//    wf->emitChanged(0, wf->sampleCount());
//    wf->unlock();
//    qDebug() << *wf;
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::changed(const QgeWaveformProperty &basicPropery, const QVariantMap &properties,
        qint64 arraySize, bool arrayChaned, qint64 firstSample, qint64 lastSample)
{
    qDebug() << "changed: " << arrayChaned << firstSample << lastSample;
}

void CMainWindow::on_action_ConnectionNew_triggered()
{
    CNewConnectionDialog connDlg(this);
    if (connDlg.exec() == QDialog::Accepted)
    {
        CSeedLinkPlotter *client = new CSeedLinkPlotter(this);
        client->setHost(connDlg.host(), connDlg.port());
        client->addStation(connDlg.stationList());
        client->start();
    }
}
