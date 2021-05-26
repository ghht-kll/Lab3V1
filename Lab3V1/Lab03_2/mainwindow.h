#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QByteArray>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QException>
#include <QtSql>
#include <QDateTime>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTcpSocket>
#include <QTextEdit>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    bool isEmptyLineEdit();
    void insertNewMessageFromBD(QString name, QString message);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void slotConnect();
    void readyread();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QStandardItemModel* qStandardItemModel;
    QTcpSocket * socket;

};
#endif // MAINWINDOW_H
