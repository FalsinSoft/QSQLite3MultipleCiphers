// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <qsqldriverplugin.h>
#include <qstringlist.h>
#include "qsql_sqlite_multiple_ciphers_p.h"
#include "qsql_sqlite_vfs_p.h"

QT_BEGIN_NAMESPACE

class QSQLite3MultipleCiphersDriverPlugin : public QSqlDriverPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QSqlDriverFactoryInterface" FILE "QSQLite3MultipleCiphers.json")

public:
    QSQLite3MultipleCiphersDriverPlugin();

    QSqlDriver* create(const QString &) override;
};

QSQLite3MultipleCiphersDriverPlugin::QSQLite3MultipleCiphersDriverPlugin()
    : QSqlDriverPlugin()
{
    register_qt_vfs();
}

QSqlDriver* QSQLite3MultipleCiphersDriverPlugin::create(const QString &name)
{
    if(name == QLatin1String("SQLITE_MULTIPLE_CIPHERS"))
    {
        QSQLite3MultipleCiphersDriver *driver = new QSQLite3MultipleCiphersDriver();
        return driver;
    }

    return nullptr;
}

QT_END_NAMESPACE

#include "smain.moc"
