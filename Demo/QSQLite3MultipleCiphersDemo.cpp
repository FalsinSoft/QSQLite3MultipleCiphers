#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlDatabase>
#include "QSQLite3MultipleCiphersDemo.h"
#include <QDir>
QSQLite3MultipleCiphersDemo::QSQLite3MultipleCiphersDemo(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
}

QSQLite3MultipleCiphersDemo::~QSQLite3MultipleCiphersDemo()
{}

void QSQLite3MultipleCiphersDemo::on_createDatabaseButton_clicked()
{
    QSqlDatabase db;

    if(!QSqlDatabase::isDriverAvailable("SQLITE_MULTIPLE_CIPHERS"))
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Database driver not loaded"));
        return;
    }
    db = QSqlDatabase::addDatabase("SQLITE_MULTIPLE_CIPHERS");
    if(!db.isValid())
    {
        QMessageBox::warning(this,
                                tr("Warning"),
                                tr("Unable to add database"));
        return;
    }

    db.setDatabaseName(ui.fileName->text().trimmed());
    db.setConnectOptions(makeOptionsLine());

    if(db.open())
    {
        QSqlQuery query(db);

        query.exec("CREATE TABLE test_table (string_field TEXT, number_field INTEGER)");
        query.exec("INSERT INTO test_table (string_field, number_field) VALUES ('abcdefghi', 1234)");

        db.close();
    }
    else
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Unable to open database"));
        return;
    }

    QMessageBox::information(this,
                             tr("Information"),
                             tr("Database created successfully"));
}

QString QSQLite3MultipleCiphersDemo::makeOptionsLine() const
{
    QStringList options;

    options << QString("QSQLITE_MC_KEY=%1").arg(ui.password->text().trimmed());
    if(ui.setNewPassword->isChecked()) options << QString("QSQLITE_MC_UPDATE_KEY=%1").arg(ui.newPassword->text().trimmed());
    options << QString("QSQLITE_MC_CIPHER=%1").arg(ui.cipher->currentText());

    return options.join(";");
}
