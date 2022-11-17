#pragma once

#include "ui_ServerTcp.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QtWidgets/QMainWindow>

class ServerTcp : public QMainWindow {
  Q_OBJECT

public:
  ServerTcp(QWidget *parent = Q_NULLPTR);
  ~ServerTcp();

private:
  Ui::ServerTcpClass ui;
  QTcpServer *m_pTcpServer;
  QTcpSocket *m_pTcpSocket;
  QMap<int, QString> m_testTypesTable;
  int m_testTypesCount;
  QString m_currentTestItem;

private slots:
  void on_btnListen_clicked();

  void on_btnTestStop_clicked();

  void on_btnSetPort_clicked();

  void server_New_Connect();

  void socket_Read_Data();

  void socket_Disconnected();
};
