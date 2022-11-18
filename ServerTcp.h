#pragma once

#include "ui_ServerTcp.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QtWidgets/QMainWindow>

class ServerTcp : public QMainWindow {
  Q_OBJECT

private:
  Ui::ServerTcpClass ui;
  QTcpServer *m_pTcpServer;
  QTcpSocket *m_pTcpSocket;
  QMap<int, QString> m_testTypesTable;
  int m_testTypesCount;
  QString m_currentTestItem;

public:
  ServerTcp(QWidget *parent = Q_NULLPTR);
  ~ServerTcp();
  Q_SIGNAL void sendMessage(const QString &mes);
  void getMessage(const QString &mes);

private:
  Q_SLOT void on_pushButton_clicked();
  Q_SLOT void on_btnListen_clicked();
  Q_SLOT void on_btnTestStop_clicked();
  Q_SLOT void on_btnSetPort_clicked();
  Q_SLOT void server_New_Connect();
  Q_SLOT void socket_Read_Data();
  Q_SLOT void socket_Disconnected();
};
