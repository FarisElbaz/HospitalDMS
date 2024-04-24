#include "functions.h"

Functions::Functions() : henv(nullptr), hdbc(nullptr) {}

Functions::~Functions() {
    connClose();
}

bool Functions::connOpen() {
    // Allocate the environment handle
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Allocate the connection handle
    SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

    // Connection string
    // Replace the connection string with your actual connection details
    SQLCHAR* connectionString = (SQLCHAR*)"DRIVER={ODBC Driver 18 for SQL Server};SERVER=localhost;DATABASE=hospital;UID=sa;PWD=reallyStrongPwd123;Encrypt=no";

    // Connect to the SQL Server
    SQLRETURN ret = SQLDriverConnect(hdbc, NULL, connectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

void Functions::connClose() {
    if (hdbc!= nullptr) {
        SQLDisconnect(hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    }
    if (henv!= nullptr) {
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
    }
}

QMap<QString, QString> Functions::getTableNames() {
    QMap<QString, QString> tableNames;
    SQLHSTMT hstmt;
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    // SQLTablesA query to retrieve only user-created tables
    SQLCHAR query[] = "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE = 'BASE TABLE'";

    if (SQLExecDirectA(hstmt, query, SQL_NTS) != SQL_SUCCESS) {
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        return tableNames;
    }

    SQLCHAR tableName[256];
    SQLLEN len;

    while (SQLFetch(hstmt) == SQL_SUCCESS) {
        SQLGetData(hstmt, 1, SQL_C_CHAR, tableName, sizeof(tableName), &len);
        QString name = QString::fromUtf8(reinterpret_cast<const char*>(tableName));
        qDebug() << "Table Name: " << name;
        tableNames[name] = "";
        // You can add a description here if needed
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return tableNames;
}

