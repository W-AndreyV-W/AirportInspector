#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QObject>
#include <QString>
#include <QtConcurrent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


#define POSTGRE_DRIVER "QPSQL"

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
