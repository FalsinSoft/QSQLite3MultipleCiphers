# QSQLite3MultipleCiphers
QSQLite3MultipleCiphers is a *QSQLiteDriver* derivate plugin wrapping the [SQLite3MultipleCiphers](https://github.com/utelle/SQLite3MultipleCiphers) library able to encrypt the sqlite database. The interface is the same of the original QSQLiteDriver and is possible to set the encryption params through the plugin options.

The database plugin can be requested using the following name:

```
QSqlDatabase::addDatabase("SQLITE_MULTIPLE_CIPHERS");
```

Using the available options you can encrypt a database with a specific password, change the password or delete the password and decrypt the database. To better understand how to use the various ciphers available and work with password, read the library documentation [here](https://utelle.github.io/SQLite3MultipleCiphers/).

The available options are:

```
QSQLITE_MC_KEY=password
QSQLITE_MC_UPDATE_KEY=new_password
QSQLITE_MC_REMOVE_KEY=current_password
QSQLITE_MC_CIPHER=cipher_name
QSQLITE_MC_CIPHER_CONFIG=param:value
```

Please, try the demo app to better understand how to use these options.

# License
QSQLite3MultipleCiphers is released under MIT license but since this plugin contains the original Qt QSQLiteDriver code the license is the same of original Qt framerowk which means LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only.
