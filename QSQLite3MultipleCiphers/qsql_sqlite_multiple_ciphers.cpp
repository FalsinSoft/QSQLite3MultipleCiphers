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

QSQLite3MultipleCiphersDriver::~QSQLite3MultipleCiphersDriver()
{
    QSQLiteDriver::close();
    sqlite3_shutdown();
}

bool QSQLite3MultipleCiphersDriver::open(const QString &db, const QString &user, const QString &password, const QString &host, int port, const QString &connOpts)
{
    if(QSQLiteDriver::open(db, user, password, host, port, connOpts))
    {
        const auto handle = qvariant_cast<sqlite3*>(QSQLiteDriver::handle());
        const auto options = connOpts.split(QLatin1Char(';'));
        QString key, updateKey, removeKey, cipher;
        CipherConfigs cipherConfigs;
        QStringList configs;

        for(const auto option : options)
        {
            QString config;

            if(parseOption(option, QLatin1String("QSQLITE_MC_KEY"), &key)
            || parseOption(option, QLatin1String("QSQLITE_MC_UPDATE_KEY"), &updateKey)
            || parseOption(option, QLatin1String("QSQLITE_MC_REMOVE_KEY"), &removeKey)
            || parseOption(option, QLatin1String("QSQLITE_MC_CIPHER"), &cipher)
            || parseOption(option, QLatin1String("QSQLITE_MC_CIPHER_CONFIG"), &config))
            {
                if(!config.isEmpty()) configs << config;
            }
        }

        if(!cipher.isEmpty())
        {
            if(sqlite3mc_cipher_index(cipher.toStdString().c_str()) == -1)
            {
                qWarning() << "Incorrect cipher name";
                QSQLiteDriver::close();
                return false;
            }
            for(const auto &config : configs)
            {
                const auto params = config.split(QLatin1Char(':'));
                bool validValue;
                QString param;
                int value;

                if(params.size() != 2)
                {
                    qWarning() << "Incorrect cipher config params (" << config << ")";
                    QSQLiteDriver::close();
                    return false;
                }
                param = params.at(0);
                value = params.at(1).toInt(&validValue, params.at(1).toLower().startsWith(QLatin1String("0x")) ? 16 : 10);
                if(validValue == false)
                {
                    qWarning() << "Incorrect cipher value";
                    QSQLiteDriver::close();
                    return false;
                }

                cipherConfigs << CipherConfig{ param, value };
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

            if(!setCipherConfiguration(handle, cipher, cipherConfigs))
            {
                qWarning() << "Unable to set chiper configuration";
                QSQLiteDriver::close();
                return false;
            }
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
            if(!setCipherConfiguration(handle, cipher, cipherConfigs))
            {
                qWarning() << "Unable to set chiper configuration";
                QSQLiteDriver::close();
                return false;
            }
            if(sqlite3_rekey(handle, updateKey.toStdString().c_str(), updateKey.length()) != SQLITE_OK)
            {
                qWarning() << "Failed to change database key";
                QSQLiteDriver::close();
                return false;
            }
        }
        else if(!removeKey.isEmpty())
        {
            if(!setCipherConfiguration(handle, cipher, cipherConfigs))
            {
                qWarning() << "Unable to set chiper configuration";
                QSQLiteDriver::close();
                return false;
            }
            if(sqlite3_key(handle, removeKey.toStdString().c_str(), removeKey.length()) != SQLITE_OK
            || sqlite3_rekey(handle, NULL, 0) != SQLITE_OK)
            {
                qWarning() << "Failed to remove database key";
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

bool QSQLite3MultipleCiphersDriver::setCipherConfiguration(sqlite3 *handle, const QString &cipher, const CipherConfigs &configs) const
{
    if(!cipher.isEmpty())
    {
        if(sqlite3mc_config(handle, "cipher", sqlite3mc_cipher_index(cipher.toStdString().c_str())) == -1)
        {
            return false;
        }
        for(const auto &config : configs)
        {
            if(sqlite3mc_config_cipher(handle,
                                       cipher.toStdString().c_str(),
                                       config.param.toStdString().c_str(),
                                       config.value) == -1)
            {
                return false;
            }
        }
    }

    return true;
}

QT_END_NAMESPACE

#include "moc_qsql_sqlite_multiple_ciphers_p.cpp"
