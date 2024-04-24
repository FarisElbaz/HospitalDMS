#ifndef MAINSTUFF_H
#define MAINSTUFF_H

#include <QMainWindow>
#include "functions.h"
#include <QComboBox>
#include <QTableView>
#include <QMessageBox>
#include <QStandardItemModel>
#include "ui_mainstuff.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class mainstuff;
}
QT_END_NAMESPACE

class mainstuff : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainstuff(SQLHDBC hdbc, QWidget *parent = nullptr);
    ~mainstuff();

private slots:
    void onPSclicked();

    void onDSclicked();

    void onAddclicked();

    void onUpdateclicked();

    void onDeleteclicked();

    // void onTableSelected();

    // void on_comboBox_activated(int index);

    void onItemClicked(const QModelIndex &index);

    void on_LoadButton_clicked();

private:
    Ui::mainstuff *ui;
    QListView *listView;
    QStandardItemModel *model;
    QTableView *tableview;
    SQLHSTMT hstmt;
    SQLHDBC hdbc;
    Functions db;
    QString selectedTableName;
};

#endif // MAINSTUFF_H
