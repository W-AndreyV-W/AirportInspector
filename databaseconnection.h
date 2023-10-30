#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QObject>
#include <QVector>
#include <QString>
//#include <QMultiMap>
#include <QtConcurrent>
//#include <QMap>
//#include <QTableWidget>
//#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QDate>
//#include <QSqlRelationalTableModel>
//#include <QSqlQueryModel>
//#include <QSqlRecord>
#include <QSqlError>

#include <QDebug>


#define POSTGRE_DRIVER "QPSQL"
#define SETCOLUMN 3

enum fieldsForConnect {
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};

class DatabaseConnection : public QObject
{
    Q_OBJECT

public:

    explicit DatabaseConnection(QObject *parent = nullptr);
    ~DatabaseConnection();

    void setConnectionData();
    void addDatabaseConnection(QString driver);
    void connectToDatabase();
    void getErrorDatabase();
    void disconnectDatabase();
    QSqlDatabase& getSqlDatabase();

signals:

    void sig_SendStatusConnection(bool status);
    void sig_ErrorDatabase(QSqlError error);

private:

    QSqlDatabase* database;


    qint8 size_connect = 5;
    QVector<QString> connection_data;
};

#endif // DATABASECONNECTION_H
