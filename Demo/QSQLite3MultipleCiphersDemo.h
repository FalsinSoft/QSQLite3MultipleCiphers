#pragma once

#include <QMainWindow>
#include <QSqlDatabase>
#include "ui_QSQLite3MultipleCiphersDemo.h"

class QSQLite3MultipleCiphersDemo : public QMainWindow
{
    Q_OBJECT

public:
    QSQLite3MultipleCiphersDemo(QWidget *parent = nullptr);
    ~QSQLite3MultipleCiphersDemo();

private slots:
    void on_createDatabaseButton_clicked();
    void on_changePasswordButton_clicked();
    void on_removePasswordButton_clicked();

private:
    Ui::QSQLite3MultipleCiphersDemoClass ui;
    QSqlDatabase m_db;

    QString makeOptionsLine(const QStringList &extraOptions) const;
    bool checkDatabase() const;
    bool connectDatabase();
};
