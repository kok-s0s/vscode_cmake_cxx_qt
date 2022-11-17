#include "ServerTcp.h"

ServerTcp::ServerTcp(QWidget *parent) : QMainWindow(parent) {
  ui.setupUi(this);

  QString fireFreqB0A[] = {"3300", "2000", "5000"};
  QString fireFreqB1A[] = {"3300", "2000", "5000"};
  QString fireHvcA[] = {"013", "048", "081", "114", "141", "163",
                        "178", "196", "214", "227", "242", "255"};
  QString focusDepthA[] = {"020", "040", "060", "080",
                           "110", "140", "180", "230"};
  m_testTypesCount = 0;
  QString temp = "";

  for (int i = 0; i < 3; ++i) {
    for (auto hvca : fireHvcA) {
      for (auto deptha : focusDepthA) {
        temp =
            fireFreqB0A[i] + "_" + fireFreqB1A[i] + "_" + hvca + "_" + deptha;
        m_testTypesTable.insert(m_testTypesCount++, temp);
        temp = "";
      }
    }
  }

  QString localHostName = QHostInfo::localHostName();
  QHostInfo info = QHostInfo::fromName(localHostName);
  QStringList list;
  foreach (QHostAddress address, info.addresses())
    if (address.protocol() == QAbstractSocket::IPv4Protocol) {
      list = address.toString().split('.');
      ui.ipAddress->setText(address.toString());
      if (list[2] == "0")
        break;
    }

  ui.lineEditPort->setText("11000");
  ui.btnTestStop->setEnabled(false);
  ui.lineEditPort->setEnabled(false);
  ui.textEdit_Recv->setEnabled(false);
  ui.textEdit_Send->setEnabled(false);

  m_pTcpServer = new QTcpServer();
  connect(m_pTcpServer, &QTcpServer::newConnection, this,
          &ServerTcp::server_New_Connect);
}

ServerTcp::~ServerTcp() {
  m_pTcpServer->close();
  m_pTcpServer->deleteLater();
}

void ServerTcp::on_btnListen_clicked() {
  if (ui.btnListen->text() == tr("Start Listening")) {
    if (!m_pTcpServer->listen(QHostAddress::Any,
                              ui.lineEditPort->text().toInt())) {
      qDebug() << m_pTcpServer->errorString();
      return;
    }

    qDebug() << "Listen successfully!";
    ui.btnListen->setText(tr("Cancel Listening"));
  } else {
    if (m_pTcpSocket->state() == QAbstractSocket::ConnectedState)
      m_pTcpSocket->disconnectFromHost();
    else {
      m_pTcpSocket->abort();
      m_pTcpServer->close();
    }
    ui.btnListen->setText("Start Listening");
    ui.btnTestStop->setEnabled(false);
  }
}

void ServerTcp::on_btnSetPort_clicked() {
  if (ui.btnSetPort->text() == tr("Modify")) {
    ui.lineEditPort->setEnabled(true);
    ui.btnSetPort->setText(tr("Set"));
  } else if (ui.btnSetPort->text() == tr("Set")) {
    qDebug() << ui.lineEditPort->text();
    ui.lineEditPort->setEnabled(false);
    ui.btnSetPort->setText(tr("Modify"));
  }
}

void ServerTcp::on_btnTestStop_clicked() {
  ui.btnTestStop->setText(m_currentTestItem);
  ui.btnTestStop->setEnabled(false);
  QString testPaused = "pause";
  m_pTcpSocket->write(testPaused.toLatin1());
  m_pTcpSocket->flush();
}

void ServerTcp::server_New_Connect() {
  m_pTcpSocket = m_pTcpServer->nextPendingConnection();

  connect(m_pTcpSocket, &QTcpSocket::readyRead, this,
          &ServerTcp::socket_Read_Data);
  connect(m_pTcpSocket, &QTcpSocket::disconnected, this,
          &ServerTcp::socket_Disconnected);
  ui.btnListen->setEnabled(true);
  qDebug() << "A Client connect!";
}

void ServerTcp::socket_Read_Data() {
  QByteArray buffer;

  buffer = m_pTcpSocket->readAll();
  if (!buffer.isEmpty()) {
    ui.textEdit_Recv->setText("");
    QString str = ui.textEdit_Recv->text();
    str += buffer;

    if (str == "connect") {
      qDebug() << "Client completes a connection" << endl;
      QString check = "1";
      m_pTcpSocket->write(check.toLatin1());
      m_pTcpSocket->flush();
    } else {
      m_currentTestItem = str;
      qDebug() << str.toInt() << endl;
      if (str.toInt() < m_testTypesCount) {
        QStringList paramlist =
            m_testTypesTable.value(str.toInt()).split(QRegExp("_"));
        qDebug() << paramlist[0].toInt() << " " << paramlist[1].toInt() << " "
                 << paramlist[2].toInt() << " " << paramlist[3].toDouble()
                 << endl;

        ui.textEdit_Recv->setText(str);
        ui.textEdit_Send->setText(m_testTypesTable.value(str.toInt()));
        ui.btnTestStop->setText(tr("TEST PAUSED"));
        ui.btnTestStop->setEnabled(true);

        QEventLoop eventloop;
        QTimer::singleShot(2000, &eventloop, SLOT(quit()));
        eventloop.exec();

        m_pTcpSocket->write(m_testTypesTable.value(str.toInt()).toLatin1());
        m_pTcpSocket->flush();
      } else {
        QString errorMes = "wrong operand";
        ui.textEdit_Recv->setText(str);
        ui.textEdit_Send->setText(errorMes);

        QEventLoop eventloop;
        QTimer::singleShot(2000, &eventloop, SLOT(quit()));
        eventloop.exec();

        m_pTcpSocket->write(errorMes.toLatin1());
        m_pTcpSocket->flush();
      }
    }
  }
}

void ServerTcp::socket_Disconnected() { qDebug() << "Disconnected!"; }
