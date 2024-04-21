#pragma once

#include <QMainWindow>
#include "ui_QSQLite3MultipleCiphersDemo.h"

class QSQLite3MultipleCiphersDemo : public QMainWindow
{
    Q_OBJECT

public:
    QSQLite3MultipleCiphersDemo(QWidget *parent = nullptr);
    ~QSQLite3MultipleCiphersDemo();

private slots:
    void on_createDatabaseButton_clicked();

private:
    Ui::QSQLite3MultipleCiphersDemoClass ui;

    QString makeOptionsLine() const;
};
