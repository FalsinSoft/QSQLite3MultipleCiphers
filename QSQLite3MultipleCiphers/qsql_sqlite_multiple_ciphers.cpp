#include "qsql_sqlite_multiple_ciphers_p.h"
#include <qvariant.h>
#include <sqlite3mc.h>

Q_DECLARE_OPAQUE_POINTER(sqlite3*)

QT_BEGIN_NAMESPACE

QSQLite3MultipleCiphersDriver::QSQLite3MultipleCiphersDriver(QObject *parent)
    : QSQLiteDriver(parent)
{
}

QSQLite3MultipleCiphersDriver::QSQLite3MultipleCiphersDriver(sqlite3 *connection, QObject *parent)
    : QSQLiteDriver(connection, parent)
{
}

bool QSQLite3MultipleCiphersDriver::open(const QString &db, const QString &user, const QString &password, const QString &host, int port, const QString &connOpts)
{
    if(QSQLiteDriver::open(db, user, password, host, port, connOpts))
    {
        const auto options = QString(connOpts).remove(QLatin1Char(' ')).split(QLatin1Char(';'));
        const auto handle = qvariant_cast<sqlite3*>(QSQLiteDriver::handle());
        QString key, updateKey;

        for(const auto option : options)
        {
            if(parseOption(option, QLatin1String("QSQLITE_MC_KEY"), &key)
            || parseOption(option, QLatin1String("QSQLITE_MC_UPDATE_KEY"), &updateKey))
            {
                continue;
            }
        }

        if(!key.isEmpty())
        {
            auto execCallback = [](void *variable, int x, char **y, char **z)-> int
            {
                if(x > 0) *(static_cast<bool*>(variable)) = false;
                return 0;
            };
            bool emptyDatabase = true;

            if(sqlite3_key(handle, key.toStdString().c_str(), key.length()) != SQLITE_OK
            || sqlite3_exec(handle, "SELECT * FROM sqlite_schema", execCallback, &emptyDatabase, NULL) != SQLITE_OK)
            {
                qWarning() << "Failed to set database key";
                QSQLiteDriver::close();
                return false;
            }
            if(emptyDatabase) sqlite3_exec(handle, "VACUUM", NULL, NULL, NULL); // Required to encrypt database first time
        }
        if(!updateKey.isEmpty())
        {
            if(sqlite3_rekey(handle, updateKey.toStdString().c_str(), updateKey.length()) != SQLITE_OK)
            {
                qWarning() << "Failed to change database key";
                QSQLiteDriver::close();
                return false;
            }
        }

        return true;
    }

    return false;
}

bool QSQLite3MultipleCiphersDriver::parseOption(const QString &line, const QString &name, QString *value) const
{
    const QString buffer(line.trimmed());

    if(buffer.startsWith(name))
    {
        const QString option(buffer.mid(name.length()).trimmed());

        if(option.startsWith(QLatin1Char('=')))
        {
            *value = option.mid(1).trimmed();
            return true;
        }
    }

    return false;
}

bool QSQLite3MultipleCiphersDriver::parseOption(const QString &line, const QString &name, int *value) const
{
    QString buffer;

    if(parseOption(line, name, &buffer))
    {
        bool result;
        *value = buffer.toInt(&result);
        return result;
    }

    return false;
}

QT_END_NAMESPACE

#include "moc_qsql_sqlite_multiple_ciphers_p.cpp"
