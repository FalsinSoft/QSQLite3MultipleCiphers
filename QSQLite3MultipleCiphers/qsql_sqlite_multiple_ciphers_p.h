#ifndef QSQL_SQLITE_MULTIPLE_CIPHERS_H
#define QSQL_SQLITE_MULTIPLE_CIPHERS_H

#include "qsql_sqlite_p.h"

#ifdef QT_PLUGIN
#define Q_EXPORT_SQLDRIVER_SQLITE_MULTIPLE_CIPHERS
#else
#define Q_EXPORT_SQLDRIVER_SQLITE_MULTIPLE_CIPHERS Q_SQL_EXPORT
#endif

QT_BEGIN_NAMESPACE

class Q_EXPORT_SQLDRIVER_SQLITE_MULTIPLE_CIPHERS QSQLite3MultipleCiphersDriver : public QSQLiteDriver
{
    Q_OBJECT
public:
    explicit QSQLite3MultipleCiphersDriver(QObject *parent = nullptr);
    explicit QSQLite3MultipleCiphersDriver(sqlite3 *connection, QObject *parent = nullptr);
    ~QSQLite3MultipleCiphersDriver();
    bool open(const QString &db,
              const QString &user,
              const QString &password,
              const QString &host,
              int port,
              const QString &connOpts) override;

private:
    bool parseOption(const QString &line, const QString &name, QString *value) const;
    bool parseOption(const QString &line, const QString &name, int *value) const;
};

QT_END_NAMESPACE

#endif // QSQL_SQLITE_MULTIPLE_CIPHERS_H
