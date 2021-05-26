#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Client-Server 2");
    this->socket = new QTcpSocket(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data_message.db");
    if(!db.open())
        qDebug() << "Нет подключения к БД" << db.lastError();
    else qDebug() << "Есть подключение к БД";
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::isEmptyLineEdit()
{
    if(this->ui->lineEdit->text() == nullptr)
    {
        QMessageBox::information(nullptr,"Ошибка","Строка пуста");
        return false;
    }
    else return true;
}

void MainWindow::insertNewMessageFromBD(QString name, QString message)
{
    QSqlQuery query;
    QString str_insert = "INSERT INTO message_table (name, message, time)"
                         "VALUES ('%1', '%2', '%3');";
    QString row = str_insert.arg(name)
                            .arg(message)
                            .arg(QDateTime::currentDateTime().toString());
    if(!query.exec(row))
        QMessageBox::information(nullptr,"Ошибка","Что-то не то с БД");
}

void MainWindow::on_pushButton_clicked()
{
    QString str = ui->lineEdit->text() + "\n";
    QByteArray arr;
    QString strName = this->ui->lineEditName->text().toLocal8Bit() + ": ";
    QDataStream dst (& arr, QIODevice :: ReadWrite);
    dst << strName << str;
    socket->write(arr);
    insertNewMessageFromBD(strName , ui->lineEdit->text());
}

void MainWindow::on_pushButton_2_clicked()
{
    QSqlQuery query;
    if (query.exec("SELECT * FROM message_table"))
    {
        this->qStandardItemModel = new QStandardItemModel();
        qStandardItemModel->setHorizontalHeaderLabels(QStringList()<<"Name" << "Message" << "Time");

        QSqlRecord rec = query.record();
        QString name,message,time;

        while (query.next())
        {
            name = query.value(rec.indexOf("name")).toString();
            message = query.value(rec.indexOf("message")).toString();
            time = query.value(rec.indexOf("time")).toString();

            QStandardItem* itemCol1(new QStandardItem(name));
            QStandardItem* itemCol2(new QStandardItem(message));
            QStandardItem* itemCol3(new QStandardItem(time));
            qStandardItemModel->appendRow(QList<QStandardItem*>()<<itemCol1<<itemCol2<<itemCol3);
        }
        this->ui->tableView->setModel(qStandardItemModel);
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    this->socket->connectToHost("127.0.0.1", 2323, QTcpSocket::ReadWrite);
    connect(this->socket, SIGNAL(connected()), this, SLOT(slotConnect()));
}

void MainWindow::slotConnect()
{
    this->ui->plainTextEdit->insertPlainText(QString :: fromLocal8Bit ("Connected"));
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(readyread()));
}

void MainWindow::readyread()
{
    QByteArray arr = this->socket->readAll();
    QDataStream* dst = new QDataStream(&arr, QIODevice::ReadOnly);
    QString str1;
    QString str2;
    (*dst) >> str1 >> str2;
    this->ui->plainTextEdit->insertPlainText("\n");
    this->ui->plainTextEdit->insertPlainText(str1);
    this->ui->plainTextEdit->insertPlainText(str2);

}
