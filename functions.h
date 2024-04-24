#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <sql.h>
#include <sqlext.h>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QStringList>

class Functions
{
public:
    Functions();
    ~Functions();
    bool connOpen();
    void connClose();
    QMap<QString, QString> getTableNames();
    bool addRecord(const QString& tableName, const QMap<QString, QVariant>& values);
    bool removeRecord(const QString& tableName, const QString& condition);
    bool updateRecord(const QString& tableName, const QMap<QString, QVariant>& updates, const QString& condition);

private:
    SQLHENV henv;
    SQLHDBC hdbc;

    QMap<QString, QString> getColumnNames(const QString& tableName);
    // QString getColumnValues(const QString& tableName, const QMap<QString, QVariant>& values);
    // QString getUpdateValues(const QMap<QString, QVariant>& updates);
};


#endif // DATABASE_H

