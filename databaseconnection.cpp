#include "databaseconnection.h"

DatabaseConnection::DatabaseConnection(QObject *parent)
    : QObject{parent}
{
    database = new QSqlDatabase();

    setConnectionData();
    addDatabaseConnection(POSTGRE_DRIVER);
    connectToDatabase();
}

DatabaseConnection::~DatabaseConnection() {

    disconnectDatabase();

    delete database;
}

void DatabaseConnection::setConnectionData() {

    connection_data.resize(size_connect);
    connection_data[hostName] = "981757-ca08998.tmweb.ru";
    connection_data[dbName] = "demo";
    connection_data[login] = "netology_usr_cpp";
    connection_data[pass] = "CppNeto3";
    connection_data[port] = "5432";
}

void DatabaseConnection::addDatabaseConnection(QString driver) {

    *database = QSqlDatabase::addDatabase(driver, connection_data[dbName]);
}

void DatabaseConnection::connectToDatabase() {

    auto fun_connect = QtConcurrent::run([&] () {

        database->setHostName(connection_data[hostName]);
        database->setDatabaseName(connection_data[dbName]);
        database->setUserName(connection_data[login]);
        database->setPassword(connection_data[pass]);
        database->setPort(connection_data[port].toInt());

        bool status = database->open();

        emit sig_SendStatusConnection(status);
    });
}

void DatabaseConnection::getErrorDatabase() {

    emit sig_ErrorDatabase(database->lastError());
}

void DatabaseConnection::disconnectDatabase() {

    if (database->open()) {

        *database = QSqlDatabase::database(connection_data[dbName]);
        database->close();
    }
}

QSqlDatabase& DatabaseConnection::getSqlDatabase() {

    return *database;
}
