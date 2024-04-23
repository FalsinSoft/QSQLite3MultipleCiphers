#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
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
    const QString fileName(ui.fileName->text().trimmed());
    QString password;

    if(!connectDatabase()) return;

    password = QInputDialog::getText(this, tr("Insert password"), QString());
    if(password.isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Insert a valid password"));
        return;
    }

    m_db.setDatabaseName(fileName);
    m_db.setConnectOptions(makeOptionsLine(QStringList() << QString("QSQLITE_MC_KEY=%1").arg(password)));

    QFile::remove(fileName);
    if(m_db.open())
    {
        QSqlQuery query(m_db);

        query.exec("CREATE TABLE test_table (string_field TEXT, number_field INTEGER)");
        query.exec("INSERT INTO test_table (string_field, number_field) VALUES ('abcdefghi', 1234)");

        if(!checkDatabase())
        {
            QMessageBox::warning(this,
                                 tr("Error"),
                                 tr("Unable to query database"));
            m_db.close();
            return;
        }
        m_db.close();
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

void QSQLite3MultipleCiphersDemo::on_changePasswordButton_clicked()
{
    const QString fileName(ui.fileName->text().trimmed());
    QString password, newPassword;

    if(!connectDatabase()) return;

    password = QInputDialog::getText(this, tr("Insert current password"), QString());
    if(password.isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Insert a valid password"));
        return;
    }
    newPassword = QInputDialog::getText(this, tr("Insert new password"), QString());
    if(newPassword.isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Insert a valid password"));
        return;
    }

    m_db.setDatabaseName(fileName);
    m_db.setConnectOptions(makeOptionsLine(QStringList() << QString("QSQLITE_MC_KEY=%1").arg(password) << QString("QSQLITE_MC_UPDATE_KEY=%1").arg(newPassword)));

    if(!m_db.open())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Unable to open database"));
        return;
    }
    if(!checkDatabase())
    {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Unable to query database"));
        m_db.close();
        return;
    }
    m_db.close();

    QMessageBox::information(this,
                             tr("Information"),
                             tr("Password changed successfully"));
}

void QSQLite3MultipleCiphersDemo::on_removePasswordButton_clicked()
{
    const QString fileName(ui.fileName->text().trimmed());
    QString password;

    if(!connectDatabase()) return;

    password = QInputDialog::getText(this, tr("Insert current password"), QString());
    if(password.isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Insert a valid password"));
        return;
    }

    m_db.setDatabaseName(fileName);
    m_db.setConnectOptions(makeOptionsLine(QStringList() << QString("QSQLITE_MC_REMOVE_KEY=%1").arg(password)));

    if(!m_db.open())
    {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Unable to open database"));
        return;
    }
    if(!checkDatabase())
    {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Unable to query database"));
        m_db.close();
        return;
    }
    m_db.close();

    QMessageBox::information(this,
                             tr("Information"),
                             tr("Password removed successfully"));
}

QString QSQLite3MultipleCiphersDemo::makeOptionsLine(const QStringList &extraOptions) const
{
    QStringList options(extraOptions);

    options << QString("QSQLITE_MC_CIPHER=%1").arg(ui.cipher->currentText());
    options << QString("QSQLITE_MC_CIPHER_CONFIG=%1:%2").arg("kdf_iter").arg(ui.kdf_iter->text());
    options << QString("QSQLITE_MC_CIPHER_CONFIG=%1:%2").arg("kdf_algorithm").arg(ui.kdf_algorithm->text());

    return options.join(";");
}

bool QSQLite3MultipleCiphersDemo::connectDatabase()
{
    if(!m_db.isValid())
    {
        if(!QSqlDatabase::isDriverAvailable("SQLITE_MULTIPLE_CIPHERS"))
        {
            QMessageBox::warning(this,
                                 tr("Warning"),
                                 tr("Database driver not loaded"));
            return false;
        }
        m_db = QSqlDatabase::addDatabase("SQLITE_MULTIPLE_CIPHERS");
        if(!m_db.isValid())
        {
            QMessageBox::warning(this,
                                 tr("Warning"),
                                 tr("Unable to add database"));
            return false;
        }

        return true;
    }

    return true;
}

bool QSQLite3MultipleCiphersDemo::checkDatabase() const
{
    QSqlQuery query(m_db);

    query.exec("SELECT string_field, number_field FROM test_table");
    if(query.next())
    {
        if(query.value(0).toString() == "abcdefghi"
        && query.value(1).toInt() == 1234)
        {
            return true;
        }
    }

    return false;
}
