#include "mainstuff.h"
#include "ui_mainstuff.h"

mainstuff::mainstuff(SQLHDBC hdbc, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainstuff),
    model(new QStandardItemModel(this))
{
    ui->setupUi(this);
    qDebug() << "UI setup complete";


    listView = findChild<QListView*>("listView");
    if(listView) {
        connect(listView, &QListView::clicked, this, &mainstuff::onItemClicked);
        qDebug() << "ListView found and connected";
    } else {
        qDebug() << "ListView not found in the UI!";
    }

    if (!db.connOpen()) {
        QMessageBox::critical(this, "Error", "Failed to connect to the Function");
        return;
    }
    qDebug() << "Database connection opened successfully";

    QMap<QString, QString> tableNamesMap = db.getTableNames();
    qDebug() << "Table Names Map: " << tableNamesMap;

    QStringList tableNames = tableNamesMap.keys();
    qDebug() << "Table Names List: " << tableNames;

    foreach (const QString &tableName, tableNames) {
        model->appendRow(new QStandardItem(tableName));
    }
    qDebug() << "Items added to model";

    listView->setModel(model);

    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex index = model->index(row, 0);
        qDebug() << "Item:" << model->data(index).toString();
    }
}

mainstuff::~mainstuff()
{
    db.connClose();
    delete ui;
}


void mainstuff::onItemClicked(const QModelIndex &index) {
    selectedTableName = model->data(index).toString();
    qDebug() << selectedTableName;
}

void mainstuff::on_LoadButton_clicked() {
    // Check if a table name is selected
    if (selectedTableName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a table from the list first.");
        return;
    }
    qDebug() << selectedTableName;

    // Now load the table using the selected table name
    SQLCHAR query[512];
    snprintf((char*)query, sizeof(query), "SELECT * FROM %s", selectedTableName.toUtf8().constData());
    qDebug() << "Query:" << query;

    SQLHSTMT hstmt;
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, hdbc, &hstmt);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        QMessageBox::critical(this, "Error", "Failed to allocate statement handle");
        return;
    }

    ret = SQLExecDirect(hstmt, query, SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        SQLSMALLINT rec = 0;
        SQLCHAR state[7];
        SQLINTEGER native;
        SQLCHAR msg[1024];
        while (SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, ++rec, state, &native, msg, sizeof(msg), NULL) == SQL_SUCCESS) {
            QMessageBox::critical(this, "Error", QString::fromLocal8Bit(reinterpret_cast<char*>(state)) + ": " + QString::fromLocal8Bit(reinterpret_cast<char*>(msg)));
        }
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        return;
    }

    ui->tableView->setModel(nullptr);

    SQLSMALLINT columnCount;
    ret = SQLNumResultCols(hstmt, &columnCount);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        QMessageBox::critical(this, "Error", "Failed to get number of columns");
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        return;
    }

    // Retrieve the column names
    QStringList columnNames;
    for (int i = 0; i < columnCount; i++) {
        SQLCHAR columnName[256];
        SQLSMALLINT nameLength;
        ret = SQLDescribeCol(hstmt, i + 1, columnName, sizeof(columnName), &nameLength, NULL, NULL, NULL, NULL);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
            QMessageBox::critical(this, "Error", "Failed to get column name");
            SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
            return;
        }
        columnNames << QString::fromLocal8Bit(reinterpret_cast<char*>(columnName)).left(nameLength);
    }

    // Create a new model to hold the data
    QStandardItemModel *tableModel = new QStandardItemModel(this);

    // Set the column headers
    tableModel->setHorizontalHeaderLabels(columnNames);

    // Get the data rows
    while (SQLFetch(hstmt) == SQL_SUCCESS) {
        QList<QStandardItem*> rowItems;
        for (int i = 0; i < columnCount; i++) {
            SQLCHAR data[256];
            SQLLEN dataLength;
            ret = SQLGetData(hstmt, i + 1, SQL_C_CHAR, data, sizeof(data), &dataLength);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
                QMessageBox::critical(this, "Error", "Failed to get row data");
                SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
                return;
            }
            QStandardItem* item = new QStandardItem(QString::fromLocal8Bit(reinterpret_cast<char*>(data)));
            rowItems << item;
        }
        tableModel->appendRow(rowItems);
    }

    // Set the model for the tableView
    ui->tableView->setModel(tableModel);

    // Clean up
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}



void mainstuff::onDSclicked() {
    // Implementation for handling the DS button click
}

void mainstuff::onPSclicked() {
    // Implementation for handling the PS button click
}

void mainstuff::onAddclicked()
{
    // Implementation for handling the add button click
}

void mainstuff::onDeleteclicked() {
    // Implementation for handling the delete button click
}

void mainstuff::onUpdateclicked() {
    // Implementation for handling the update button click
}



