test_suite_list = []
"""
/*jslint-disable*/
/*
shRollupFetch
{
    "fetchList": [
        {
            "comment": true,
            "url": "https://github.com/coleifer/pysqlite3/blob/0.5.0/LICENSE"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\nif __name__ != \"__main__\":\n",
            "replaceList": [
                {
                    "aa": "^.+\n",
                    "bb": "    $&",
                    "flags": "gm",
                    "substr": ""
                }
            ],
            "url": "https://github.com/coleifer/pysqlite3/blob/0.5.0/pysqlite3/dbapi2.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/coleifer/pysqlite3/blob/0.5.0/test/backup.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/coleifer/pysqlite3/blob/0.5.0/test/dbapi.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/coleifer/pysqlite3/blob/0.5.0/test/factory.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/coleifer/pysqlite3/blob/0.5.0/test/hooks.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/coleifer/pysqlite3/blob/0.5.0/test/regression.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/coleifer/pysqlite3/blob/0.5.0/test/transactions.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/coleifer/pysqlite3/blob/0.5.0/test/ttypes.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/coleifer/pysqlite3/blob/0.5.0/test/userfunctions.py"
        }
    ],
    "replaceList": [
        {
            "aa": "\\/tmp\\/",
            "bb": ".tmp/",
            "flags": "g",
            "substr": ""
        },
        {
            "aa": "\\nif __name__ == .__main__.:\\n    .*?test.*?\\n",
            "bb": "\ntest_suite_list.append(suite)\n",
            "flags": "g",
            "substr": ""
        },
        {
            "aa": "from pysqlite3 import dbapi2 as sqlite",
            "bb": "from . import sqlmath_dbapi2 as sqlite",
            "flags": "g",
            "substr": ""
        }
    ]
}
-        con.execute('insert into foo(x) values ("%s")' % unicode_value)
+# hack-pysqlite3 - enforce SQLITE_DQS
+        con.execute('insert into foo(x) values (\'%s\')' % unicode_value)

-    from pysqlite3._sqlite3 import *
+# hack-pysqlite3 - fix import
+    from ._sqlmath import *
*/


/*
repo https://github.com/coleifer/pysqlite3/tree/0.5.0
committed 2022-12-13T22:13:35Z
*/


/*
file https://github.com/coleifer/pysqlite3/blob/0.5.0/LICENSE
*/
/*
Copyright (c) 2004-2007 Gerhard Häring

This software is provided 'as-is', without any express or implied warranty. In
no event will the authors be held liable for any damages arising from the use
of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software in
       a product, an acknowledgment in the product documentation would be
       appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/


/*
file https://github.com/coleifer/pysqlite3/blob/0.5.0/pysqlite3/dbapi2.py
*/

"""
if __name__ != "__main__":
#-*- coding: ISO-8859-1 -*-
    # pysqlite2/dbapi2.py: the DB-API 2.0 interface
    #
    # Copyright (C) 2004-2005 Gerhard Häring <gh@ghaering.de>
    #
    # This file is part of pysqlite.
    #
    # This software is provided 'as-is', without any express or implied
    # warranty.  In no event will the authors be held liable for any damages
    # arising from the use of this software.
    #
    # Permission is granted to anyone to use this software for any purpose,
    # including commercial applications, and to alter it and redistribute it
    # freely, subject to the following restrictions:
    #
    # 1. The origin of this software must not be misrepresented; you must not
    #    claim that you wrote the original software. If you use this software
    #    in a product, an acknowledgment in the product documentation would be
    #    appreciated but is not required.
    # 2. Altered source versions must be plainly marked as such, and must not be
    #    misrepresented as being the original software.
    # 3. This notice may not be removed or altered from any source distribution.

    import datetime
    import time
    import collections.abc

# hack-pysqlite3 - fix import
    from ._sqlmath import *

    paramstyle = "qmark"

    threadsafety = 1

    apilevel = "2.0"

    Date = datetime.date

    Time = datetime.time

    Timestamp = datetime.datetime

    def DateFromTicks(ticks):
        return Date(*time.localtime(ticks)[:3])

    def TimeFromTicks(ticks):
        return Time(*time.localtime(ticks)[3:6])

    def TimestampFromTicks(ticks):
        return Timestamp(*time.localtime(ticks)[:6])

    version_info = tuple([int(x) for x in version.split(".")])
    sqlite_version_info = tuple([int(x) for x in sqlite_version.split(".")])

    Binary = memoryview
    collections.abc.Sequence.register(Row)

    def register_adapters_and_converters():
        def adapt_date(val):
            return val.isoformat()

        def adapt_datetime(val):
            return val.isoformat(" ")

        def convert_date(val):
            return datetime.date(*map(int, val.split(b"-")))

        def convert_timestamp(val):
            datepart, timepart = val.split(b" ")
            year, month, day = map(int, datepart.split(b"-"))
            timepart_full = timepart.split(b".")
            hours, minutes, seconds = map(int, timepart_full[0].split(b":"))
            if len(timepart_full) == 2:
                microseconds = int('{:0<6.6}'.format(timepart_full[1].decode()))
            else:
                microseconds = 0

            val = datetime.datetime(year, month, day, hours, minutes, seconds, microseconds)
            return val


        register_adapter(datetime.date, adapt_date)
        register_adapter(datetime.datetime, adapt_datetime)
        register_converter("date", convert_date)
        register_converter("timestamp", convert_timestamp)

    register_adapters_and_converters()

    # Clean up namespace

    del(register_adapters_and_converters)
"""


/*
file https://github.com/coleifer/pysqlite3/blob/0.5.0/test/backup.py
*/

"""
from . import sqlmath_dbapi2 as sqlite
import unittest


@unittest.skipIf(sqlite.sqlite_version_info < (3, 6, 11), "Backup API not supported")
class BackupTests(unittest.TestCase):
    def setUp(self):
        cx = self.cx = sqlite.connect(":memory:")
        cx.execute('CREATE TABLE foo (key INTEGER)')
        cx.executemany('INSERT INTO foo (key) VALUES (?)', [(3,), (4,)])
        cx.commit()

    def tearDown(self):
        self.cx.close()

    def verify_backup(self, bckcx):
        result = bckcx.execute("SELECT key FROM foo ORDER BY key").fetchall()
        self.assertEqual(result[0][0], 3)
        self.assertEqual(result[1][0], 4)

    def test_bad_target_none(self):
        with self.assertRaises(TypeError):
            self.cx.backup(None)

    def test_bad_target_filename(self):
        with self.assertRaises(TypeError):
            self.cx.backup('some_file_name.db')

    def test_bad_target_same_connection(self):
        with self.assertRaises(ValueError):
            self.cx.backup(self.cx)

    def test_bad_target_closed_connection(self):
        bck = sqlite.connect(':memory:')
        bck.close()
        with self.assertRaises(sqlite.ProgrammingError):
            self.cx.backup(bck)

    def test_bad_target_in_transaction(self):
        bck = sqlite.connect(':memory:')
        bck.execute('CREATE TABLE bar (key INTEGER)')
        bck.executemany('INSERT INTO bar (key) VALUES (?)', [(3,), (4,)])
        with self.assertRaises(sqlite.OperationalError) as cm:
            self.cx.backup(bck)
        if sqlite.sqlite_version_info < (3, 8, 8):
            self.assertEqual(str(cm.exception), 'target is in transaction')

    def test_keyword_only_args(self):
        with self.assertRaises(TypeError):
            with sqlite.connect(':memory:') as bck:
                self.cx.backup(bck, 1)

    def test_simple(self):
        with sqlite.connect(':memory:') as bck:
            self.cx.backup(bck)
            self.verify_backup(bck)

    def test_progress(self):
        journal = []

        def progress(status, remaining, total):
            journal.append(status)

        with sqlite.connect(':memory:') as bck:
            self.cx.backup(bck, pages=1, progress=progress)
            self.verify_backup(bck)

        self.assertEqual(len(journal), 2)
        self.assertEqual(journal[0], sqlite.SQLITE_OK)
        self.assertEqual(journal[1], sqlite.SQLITE_DONE)

    def test_progress_all_pages_at_once_1(self):
        journal = []

        def progress(status, remaining, total):
            journal.append(remaining)

        with sqlite.connect(':memory:') as bck:
            self.cx.backup(bck, progress=progress)
            self.verify_backup(bck)

        self.assertEqual(len(journal), 1)
        self.assertEqual(journal[0], 0)

    def test_progress_all_pages_at_once_2(self):
        journal = []

        def progress(status, remaining, total):
            journal.append(remaining)

        with sqlite.connect(':memory:') as bck:
            self.cx.backup(bck, pages=-1, progress=progress)
            self.verify_backup(bck)

        self.assertEqual(len(journal), 1)
        self.assertEqual(journal[0], 0)

    def test_non_callable_progress(self):
        with self.assertRaises(TypeError) as cm:
            with sqlite.connect(':memory:') as bck:
                self.cx.backup(bck, pages=1, progress='bar')
        self.assertEqual(str(cm.exception), 'progress argument must be a callable')

    def test_modifying_progress(self):
        journal = []

        def progress(status, remaining, total):
            if not journal:
                self.cx.execute('INSERT INTO foo (key) VALUES (?)', (remaining+1000,))
                self.cx.commit()
            journal.append(remaining)

        with sqlite.connect(':memory:') as bck:
            self.cx.backup(bck, pages=1, progress=progress)
            self.verify_backup(bck)

            result = bck.execute("SELECT key FROM foo"
                                 " WHERE key >= 1000"
                                 " ORDER BY key").fetchall()
            self.assertEqual(result[0][0], 1001)

        self.assertEqual(len(journal), 3)
        self.assertEqual(journal[0], 1)
        self.assertEqual(journal[1], 1)
        self.assertEqual(journal[2], 0)

    def test_failing_progress(self):
        def progress(status, remaining, total):
            raise SystemError('nearly out of space')

        with self.assertRaises(SystemError) as err:
            with sqlite.connect(':memory:') as bck:
                self.cx.backup(bck, progress=progress)
        self.assertEqual(str(err.exception), 'nearly out of space')

    def test_database_source_name(self):
        with sqlite.connect(':memory:') as bck:
            self.cx.backup(bck, name='main')
        with sqlite.connect(':memory:') as bck:
            self.cx.backup(bck, name='temp')
        with self.assertRaises(sqlite.OperationalError) as cm:
            with sqlite.connect(':memory:') as bck:
                self.cx.backup(bck, name='non-existing')
        self.assertIn(
            str(cm.exception),
            ['SQL logic error', 'SQL logic error or missing database']
        )

        self.cx.execute("ATTACH DATABASE ':memory:' AS attached_db")
        self.cx.execute('CREATE TABLE attached_db.foo (key INTEGER)')
        self.cx.executemany('INSERT INTO attached_db.foo (key) VALUES (?)', [(3,), (4,)])
        self.cx.commit()
        with sqlite.connect(':memory:') as bck:
            self.cx.backup(bck, name='attached_db')
            self.verify_backup(bck)


def suite():
    return unittest.makeSuite(BackupTests)

test_suite_list.append(suite)
"""


/*
file https://github.com/coleifer/pysqlite3/blob/0.5.0/test/dbapi.py
*/

"""
#-*- coding: iso-8859-1 -*-
# pysqlite2/test/dbapi.py: tests for DB-API compliance
#
# Copyright (C) 2004-2010 Gerhard H�ring <gh@ghaering.de>
#
# This file is part of pysqlite.
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

import threading
import unittest
from . import sqlmath_dbapi2 as sqlite

#from test.support import TESTFN, unlink
TESTFN = '.tmp/pysqlite3_test'
from os import unlink


class ModuleTests(unittest.TestCase):
    def CheckAPILevel(self):
        self.assertEqual(sqlite.apilevel, "2.0",
                         "apilevel is %s, should be 2.0" % sqlite.apilevel)

    def CheckThreadSafety(self):
        self.assertEqual(sqlite.threadsafety, 1,
                         "threadsafety is %d, should be 1" % sqlite.threadsafety)

    def CheckParamStyle(self):
        self.assertEqual(sqlite.paramstyle, "qmark",
                         "paramstyle is '%s', should be 'qmark'" %
                         sqlite.paramstyle)

    def CheckWarning(self):
        self.assertTrue(issubclass(sqlite.Warning, Exception),
                     "Warning is not a subclass of Exception")

    def CheckError(self):
        self.assertTrue(issubclass(sqlite.Error, Exception),
                        "Error is not a subclass of Exception")

    def CheckInterfaceError(self):
        self.assertTrue(issubclass(sqlite.InterfaceError, sqlite.Error),
                        "InterfaceError is not a subclass of Error")

    def CheckDatabaseError(self):
        self.assertTrue(issubclass(sqlite.DatabaseError, sqlite.Error),
                        "DatabaseError is not a subclass of Error")

    def CheckDataError(self):
        self.assertTrue(issubclass(sqlite.DataError, sqlite.DatabaseError),
                        "DataError is not a subclass of DatabaseError")

    def CheckOperationalError(self):
        self.assertTrue(issubclass(sqlite.OperationalError, sqlite.DatabaseError),
                        "OperationalError is not a subclass of DatabaseError")

    def CheckIntegrityError(self):
        self.assertTrue(issubclass(sqlite.IntegrityError, sqlite.DatabaseError),
                        "IntegrityError is not a subclass of DatabaseError")

    def CheckInternalError(self):
        self.assertTrue(issubclass(sqlite.InternalError, sqlite.DatabaseError),
                        "InternalError is not a subclass of DatabaseError")

    def CheckProgrammingError(self):
        self.assertTrue(issubclass(sqlite.ProgrammingError, sqlite.DatabaseError),
                        "ProgrammingError is not a subclass of DatabaseError")

    def CheckNotSupportedError(self):
        self.assertTrue(issubclass(sqlite.NotSupportedError,
                                   sqlite.DatabaseError),
                        "NotSupportedError is not a subclass of DatabaseError")

    def CheckErrorCodeOnException(self):
        with self.assertRaises(sqlite.Error) as cm:
            db = sqlite.connect('/no/such/file/exists')
        e = cm.exception
        self.assertEqual(e.sqlite_errorcode, sqlite.SQLITE_CANTOPEN)
        self.assertEqual(e.sqlite_errorname, "SQLITE_CANTOPEN")
        self.assertEqual(str(e), "unable to open database file")

class ConnectionTests(unittest.TestCase):

    def setUp(self):
        self.cx = sqlite.connect(":memory:")
        cu = self.cx.cursor()
        cu.execute("create table test(id integer primary key, name text)")
        cu.execute("insert into test(name) values (?)", ("foo",))

    def tearDown(self):
        self.cx.close()

    def CheckCommit(self):
        self.cx.commit()

    def CheckCommitAfterNoChanges(self):
        """
        A commit should also work when no changes were made to the database.
        """
        self.cx.commit()
        self.cx.commit()

    def CheckRollback(self):
        self.cx.rollback()

    def CheckRollbackAfterNoChanges(self):
        """
        A rollback should also work when no changes were made to the database.
        """
        self.cx.rollback()
        self.cx.rollback()

    def CheckCursor(self):
        cu = self.cx.cursor()

    def CheckFailedOpen(self):
        YOU_CANNOT_OPEN_THIS = "/foo/bar/bla/23534/mydb.db"
        with self.assertRaises(sqlite.OperationalError):
            con = sqlite.connect(YOU_CANNOT_OPEN_THIS)

    def CheckClose(self):
        self.cx.close()

    def CheckExceptions(self):
        # Optional DB-API extension.
        self.assertEqual(self.cx.Warning, sqlite.Warning)
        self.assertEqual(self.cx.Error, sqlite.Error)
        self.assertEqual(self.cx.InterfaceError, sqlite.InterfaceError)
        self.assertEqual(self.cx.DatabaseError, sqlite.DatabaseError)
        self.assertEqual(self.cx.DataError, sqlite.DataError)
        self.assertEqual(self.cx.OperationalError, sqlite.OperationalError)
        self.assertEqual(self.cx.IntegrityError, sqlite.IntegrityError)
        self.assertEqual(self.cx.InternalError, sqlite.InternalError)
        self.assertEqual(self.cx.ProgrammingError, sqlite.ProgrammingError)
        self.assertEqual(self.cx.NotSupportedError, sqlite.NotSupportedError)

    def CheckInTransaction(self):
        # Can't use db from setUp because we want to test initial state.
        cx = sqlite.connect(":memory:")
        cu = cx.cursor()
        self.assertEqual(cx.in_transaction, False)
        cu.execute("create table transactiontest(id integer primary key, name text)")
        self.assertEqual(cx.in_transaction, False)
        cu.execute("insert into transactiontest(name) values (?)", ("foo",))
        self.assertEqual(cx.in_transaction, True)
        cu.execute("select name from transactiontest where name=?", ["foo"])
        row = cu.fetchone()
        self.assertEqual(cx.in_transaction, True)
        cx.commit()
        self.assertEqual(cx.in_transaction, False)
        cu.execute("select name from transactiontest where name=?", ["foo"])
        row = cu.fetchone()
        self.assertEqual(cx.in_transaction, False)

    def CheckInTransactionRO(self):
        with self.assertRaises(AttributeError):
            self.cx.in_transaction = True

    def CheckOpenWithPathLikeObject(self):
        """ Checks that we can successfully connect to a database using an object that
            is PathLike, i.e. has __fspath__(). """
        self.addCleanup(unlink, TESTFN)
        class Path:
            def __fspath__(self):
                return TESTFN
        path = Path()
        with sqlite.connect(path) as cx:
            cx.execute('create table test(id integer)')

    def CheckOpenUri(self):
        if sqlite.sqlite_version_info < (3, 7, 7):
            with self.assertRaises(sqlite.NotSupportedError):
                sqlite.connect(':memory:', uri=True)
            return
        self.addCleanup(unlink, TESTFN)
        with sqlite.connect(TESTFN) as cx:
            cx.execute('create table test(id integer)')
        with sqlite.connect('file:' + TESTFN, uri=True) as cx:
            cx.execute('insert into test(id) values(0)')
        with sqlite.connect('file:' + TESTFN + '?mode=ro', uri=True) as cx:
            with self.assertRaises(sqlite.OperationalError):
                cx.execute('insert into test(id) values(1)')

    @unittest.skipIf(sqlite.sqlite_version_info >= (3, 3, 1),
                     'needs sqlite versions older than 3.3.1')
    def CheckSameThreadErrorOnOldVersion(self):
        with self.assertRaises(sqlite.NotSupportedError) as cm:
            sqlite.connect(':memory:', check_same_thread=False)
        self.assertEqual(str(cm.exception), 'shared connections not available')

class CursorTests(unittest.TestCase):
    def setUp(self):
        self.cx = sqlite.connect(":memory:")
        self.cu = self.cx.cursor()
        self.cu.execute(
            "create table test(id integer primary key, name text, "
            "income number, unique_test text unique)"
        )
        self.cu.execute("insert into test(name) values (?)", ("foo",))

    def tearDown(self):
        self.cu.close()
        self.cx.close()

    def CheckExecuteNoArgs(self):
        self.cu.execute("delete from test")

    def CheckExecuteIllegalSql(self):
        with self.assertRaises(sqlite.OperationalError):
            self.cu.execute("select asdf")

    def CheckExecuteTooMuchSql(self):
        with self.assertRaises(sqlite.Warning):
            self.cu.execute("select 5+4; select 4+5")

    def CheckExecuteTooMuchSql2(self):
        self.cu.execute("select 5+4; -- foo bar")

    def CheckExecuteTooMuchSql3(self):
        self.cu.execute("""
            select 5+4;

            /*
            foo
            */
            """)

    def CheckExecuteWrongSqlArg(self):
        with self.assertRaises(TypeError):
            self.cu.execute(42)

    def CheckExecuteArgInt(self):
        self.cu.execute("insert into test(id) values (?)", (42,))

    def CheckExecuteArgFloat(self):
        self.cu.execute("insert into test(income) values (?)", (2500.32,))

    def CheckExecuteArgString(self):
        self.cu.execute("insert into test(name) values (?)", ("Hugo",))

    def CheckExecuteArgStringWithZeroByte(self):
        self.cu.execute("insert into test(name) values (?)", ("Hu\x00go",))

        self.cu.execute("select name from test where id=?", (self.cu.lastrowid,))
        row = self.cu.fetchone()
        self.assertEqual(row[0], "Hu\x00go")

    def CheckExecuteNonIterable(self):
        with self.assertRaises(ValueError) as cm:
            self.cu.execute("insert into test(id) values (?)", 42)
        self.assertEqual(str(cm.exception), 'parameters are of unsupported type')

    def CheckExecuteWrongNoOfArgs1(self):
        # too many parameters
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("insert into test(id) values (?)", (17, "Egon"))

    def CheckExecuteWrongNoOfArgs2(self):
        # too little parameters
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("insert into test(id) values (?)")

    def CheckExecuteWrongNoOfArgs3(self):
        # no parameters, parameters are needed
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("insert into test(id) values (?)")

    def CheckExecuteParamList(self):
        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("select name from test where name=?", ["foo"])
        row = self.cu.fetchone()
        self.assertEqual(row[0], "foo")

    def CheckExecuteParamSequence(self):
        class L(object):
            def __len__(self):
                return 1
            def __getitem__(self, x):
                assert x == 0
                return "foo"

        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("select name from test where name=?", L())
        row = self.cu.fetchone()
        self.assertEqual(row[0], "foo")

    def CheckExecuteDictMapping(self):
        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("select name from test where name=:name", {"name": "foo"})
        row = self.cu.fetchone()
        self.assertEqual(row[0], "foo")

    def CheckExecuteDictMapping_Mapping(self):
        class D(dict):
            def __missing__(self, key):
                return "foo"

        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("select name from test where name=:name", D())
        row = self.cu.fetchone()
        self.assertEqual(row[0], "foo")

    def CheckExecuteDictMappingTooLittleArgs(self):
        self.cu.execute("insert into test(name) values ('foo')")
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("select name from test where name=:name and id=:id", {"name": "foo"})

    def CheckExecuteDictMappingNoArgs(self):
        self.cu.execute("insert into test(name) values ('foo')")
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("select name from test where name=:name")

    def CheckExecuteDictMappingUnnamed(self):
        self.cu.execute("insert into test(name) values ('foo')")
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("select name from test where name=?", {"name": "foo"})

    def CheckClose(self):
        self.cu.close()

    def CheckRowcountExecute(self):
        self.cu.execute("delete from test")
        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("update test set name='bar'")
        self.assertEqual(self.cu.rowcount, 2)

    def CheckRowcountSelect(self):
        """
        pysqlite does not know the rowcount of SELECT statements, because we
        don't fetch all rows after executing the select statement. The rowcount
        has thus to be -1.
        """
        self.cu.execute("select 5 union select 6")
        self.assertEqual(self.cu.rowcount, -1)

    def CheckRowcountExecutemany(self):
        self.cu.execute("delete from test")
        self.cu.executemany("insert into test(name) values (?)", [(1,), (2,), (3,)])
        self.assertEqual(self.cu.rowcount, 3)

    def CheckTotalChanges(self):
        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("insert into test(name) values ('foo')")
        self.assertLess(2, self.cx.total_changes, msg='total changes reported wrong value')

    # Checks for executemany:
    # Sequences are required by the DB-API, iterators
    # enhancements in pysqlite.

    def CheckExecuteManySequence(self):
        self.cu.executemany("insert into test(income) values (?)", [(x,) for x in range(100, 110)])

    def CheckExecuteManyIterator(self):
        class MyIter:
            def __init__(self):
                self.value = 5

            def __next__(self):
                if self.value == 10:
                    raise StopIteration
                else:
                    self.value += 1
                    return (self.value,)

        self.cu.executemany("insert into test(income) values (?)", MyIter())

    def CheckExecuteManyGenerator(self):
        def mygen():
            for i in range(5):
                yield (i,)

        self.cu.executemany("insert into test(income) values (?)", mygen())

    def CheckExecuteManyWrongSqlArg(self):
        with self.assertRaises(TypeError):
            self.cu.executemany(42, [(3,)])

    def CheckExecuteManySelect(self):
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.executemany("select ?", [(3,)])

    def CheckExecuteManyNotIterable(self):
        with self.assertRaises(TypeError):
            self.cu.executemany("insert into test(income) values (?)", 42)

    def CheckFetchIter(self):
        # Optional DB-API extension.
        self.cu.execute("delete from test")
        self.cu.execute("insert into test(id) values (?)", (5,))
        self.cu.execute("insert into test(id) values (?)", (6,))
        self.cu.execute("select id from test order by id")
        lst = []
        for row in self.cu:
            lst.append(row[0])
        self.assertEqual(lst[0], 5)
        self.assertEqual(lst[1], 6)

    def CheckFetchone(self):
        self.cu.execute("select name from test")
        row = self.cu.fetchone()
        self.assertEqual(row[0], "foo")
        row = self.cu.fetchone()
        self.assertEqual(row, None)

    def CheckFetchoneNoStatement(self):
        cur = self.cx.cursor()
        row = cur.fetchone()
        self.assertEqual(row, None)

    def CheckArraySize(self):
        # must default ot 1
        self.assertEqual(self.cu.arraysize, 1)

        # now set to 2
        self.cu.arraysize = 2

        # now make the query return 3 rows
        self.cu.execute("delete from test")
        self.cu.execute("insert into test(name) values ('A')")
        self.cu.execute("insert into test(name) values ('B')")
        self.cu.execute("insert into test(name) values ('C')")
        self.cu.execute("select name from test")
        res = self.cu.fetchmany()

        self.assertEqual(len(res), 2)

    def CheckFetchmany(self):
        self.cu.execute("select name from test")
        res = self.cu.fetchmany(100)
        self.assertEqual(len(res), 1)
        res = self.cu.fetchmany(100)
        self.assertEqual(res, [])

    def CheckFetchmanyKwArg(self):
        """Checks if fetchmany works with keyword arguments"""
        self.cu.execute("select name from test")
        res = self.cu.fetchmany(size=100)
        self.assertEqual(len(res), 1)

    def CheckFetchall(self):
        self.cu.execute("select name from test")
        res = self.cu.fetchall()
        self.assertEqual(len(res), 1)
        res = self.cu.fetchall()
        self.assertEqual(res, [])

    def CheckSetinputsizes(self):
        self.cu.setinputsizes([3, 4, 5])

    def CheckSetoutputsize(self):
        self.cu.setoutputsize(5, 0)

    def CheckSetoutputsizeNoColumn(self):
        self.cu.setoutputsize(42)

    def CheckCursorConnection(self):
        # Optional DB-API extension.
        self.assertEqual(self.cu.connection, self.cx)

    def CheckWrongCursorCallable(self):
        with self.assertRaises(TypeError):
            def f(): pass
            cur = self.cx.cursor(f)

    def CheckCursorWrongClass(self):
        class Foo: pass
        foo = Foo()
        with self.assertRaises(TypeError):
            cur = sqlite.Cursor(foo)

    def CheckLastRowIDOnReplace(self):
        """
        INSERT OR REPLACE and REPLACE INTO should produce the same behavior.
        """
        sql = '{} INTO test(id, unique_test) VALUES (?, ?)'
        for statement in ('INSERT OR REPLACE', 'REPLACE'):
            with self.subTest(statement=statement):
                self.cu.execute(sql.format(statement), (1, 'foo'))
                self.assertEqual(self.cu.lastrowid, 1)

    def CheckLastRowIDOnIgnore(self):
        self.cu.execute(
            "insert or ignore into test(unique_test) values (?)",
            ('test',))
        self.assertEqual(self.cu.lastrowid, 2)
        self.cu.execute(
            "insert or ignore into test(unique_test) values (?)",
            ('test',))
        self.assertEqual(self.cu.lastrowid, 2)

    def CheckLastRowIDInsertOR(self):
        results = []
        for statement in ('FAIL', 'ABORT', 'ROLLBACK'):
            sql = 'INSERT OR {} INTO test(unique_test) VALUES (?)'
            with self.subTest(statement='INSERT OR {}'.format(statement)):
                self.cu.execute(sql.format(statement), (statement,))
                results.append((statement, self.cu.lastrowid))
                with self.assertRaises(sqlite.IntegrityError):
                    self.cu.execute(sql.format(statement), (statement,))
                results.append((statement, self.cu.lastrowid))
        expected = [
            ('FAIL', 2), ('FAIL', 2),
            ('ABORT', 3), ('ABORT', 3),
            ('ROLLBACK', 4), ('ROLLBACK', 4),
        ]
        self.assertEqual(results, expected)

class BlobTests(unittest.TestCase):
    def setUp(self):
        self.cx = sqlite.connect(":memory:")
        self.cx.execute("create table test(id integer primary key, blob_col blob)")
        self.blob_data = b"a" * 100
        self.cx.execute("insert into test(blob_col) values (?)", (self.blob_data, ))
        self.blob = self.cx.open_blob("test", "blob_col", 1)
        self.second_data = b"b" * 100

    def tearDown(self):
        self.blob.close()
        self.cx.close()

    def CheckLength(self):
        self.assertEqual(len(self.blob), 100)

    def CheckTell(self):
        self.assertEqual(self.blob.tell(), 0)

    def CheckSeekFromBlobStart(self):
        self.blob.seek(10)
        self.assertEqual(self.blob.tell(), 10)
        self.blob.seek(10, 0)
        self.assertEqual(self.blob.tell(), 10)

    def CheckSeekFromCurrentPosition(self):
        self.blob.seek(10, 1)
        self.blob.seek(10, 1)
        self.assertEqual(self.blob.tell(), 20)

    def CheckSeekFromBlobEnd(self):
        self.blob.seek(-10, 2)
        self.assertEqual(self.blob.tell(), 90)

    def CheckBlobSeekOverBlobSize(self):
        with self.assertRaises(ValueError):
            self.blob.seek(1000)

    def CheckBlobSeekUnderBlobSize(self):
        with self.assertRaises(ValueError):
            self.blob.seek(-10)

    def CheckBlobRead(self):
        self.assertEqual(self.blob.read(), self.blob_data)

    def CheckBlobReadSize(self):
        self.assertEqual(len(self.blob.read(10)), 10)

    def CheckBlobReadAdvanceOffset(self):
        self.blob.read(10)
        self.assertEqual(self.blob.tell(), 10)

    def CheckBlobReadStartAtOffset(self):
        self.blob.seek(10)
        self.blob.write(self.second_data[:10])
        self.blob.seek(10)
        self.assertEqual(self.blob.read(10), self.second_data[:10])

    def CheckBlobWrite(self):
        self.blob.write(self.second_data)
        self.assertEqual(self.cx.execute("select blob_col from test").fetchone()[0], self.second_data)

    def CheckBlobWriteAtOffset(self):
        self.blob.seek(50)
        self.blob.write(self.second_data[:50])
        self.assertEqual(self.cx.execute("select blob_col from test").fetchone()[0],
                         self.blob_data[:50] + self.second_data[:50])

    def CheckBlobWriteAdvanceOffset(self):
        self.blob.write(self.second_data[:50])
        self.assertEqual(self.blob.tell(), 50)

    def CheckBlobWriteMoreThenBlobSize(self):
        with self.assertRaises(ValueError):
            self.blob.write(b"a" * 1000)

    def CheckBlobReadAfterRowChange(self):
        self.cx.execute("UPDATE test SET blob_col='aaaa' where id=1")
        with self.assertRaises(sqlite.OperationalError):
            self.blob.read()

    def CheckBlobWriteAfterRowChange(self):
        self.cx.execute("UPDATE test SET blob_col='aaaa' where id=1")
        with self.assertRaises(sqlite.OperationalError):
            self.blob.write(b"aaa")

    def CheckBlobWriteWhenReadOnly(self):
        read_only_blob = \
            self.cx.open_blob("test", "blob_col", 1, readonly=True)
        with self.assertRaises(sqlite.OperationalError):
            read_only_blob.write(b"aaa")
        read_only_blob.close()

    def CheckBlobOpenWithBadDb(self):
        with self.assertRaises(sqlite.OperationalError):
            self.cx.open_blob("test", "blob_col", 1, dbname="notexisintg")

    def CheckBlobOpenWithBadTable(self):
        with self.assertRaises(sqlite.OperationalError):
            self.cx.open_blob("notexisintg", "blob_col", 1)

    def CheckBlobOpenWithBadColumn(self):
        with self.assertRaises(sqlite.OperationalError):
            self.cx.open_blob("test", "notexisting", 1)

    def CheckBlobOpenWithBadRow(self):
        with self.assertRaises(sqlite.OperationalError):
            self.cx.open_blob("test", "blob_col", 2)

    def CheckBlobGetItem(self):
        self.assertEqual(self.blob[5], b"a")

    def CheckBlobGetItemIndexOutOfRange(self):
        with self.assertRaises(IndexError):
            self.blob[105]
        with self.assertRaises(IndexError):
            self.blob[-105]

    def CheckBlobGetItemNegativeIndex(self):
        self.assertEqual(self.blob[-5], b"a")

    def CheckBlobGetItemInvalidIndex(self):
        with self.assertRaises(TypeError):
            self.blob[b"a"]

    def CheckBlobGetSlice(self):
        self.assertEqual(self.blob[5:10], b"aaaaa")

    def CheckBlobGetSliceNegativeIndex(self):
        self.assertEqual(self.blob[5:-5], self.blob_data[5:-5])

    def CheckBlobGetSliceInvalidIndex(self):
        with self.assertRaises(TypeError):
            self.blob[5:b"a"]

    def CheckBlobGetSliceWithSkip(self):
        self.blob.write(b"abcdefghij")
        self.assertEqual(self.blob[0:10:2], b"acegi")

    def CheckBlobSetItem(self):
        self.blob[0] = b"b"
        self.assertEqual(self.cx.execute("select blob_col from test").fetchone()[0], b"b" + self.blob_data[1:])

    def CheckBlobSetSlice(self):
        self.blob[0:5] = b"bbbbb"
        self.assertEqual(self.cx.execute("select blob_col from test").fetchone()[0], b"bbbbb" + self.blob_data[5:])

    def CheckBlobSetSliceWithSkip(self):
        self.blob[0:10:2] = b"bbbbb"
        self.assertEqual(self.cx.execute("select blob_col from test").fetchone()[0], b"bababababa" + self.blob_data[10:])

    def CheckBlobGetEmptySlice(self):
        self.assertEqual(self.blob[5:5], b"")

    def CheckBlobSetSliceWrongLength(self):
        with self.assertRaises(IndexError):
            self.blob[5:10] = b"a"

    def CheckBlobConcatNotSupported(self):
        with self.assertRaises(SystemError):
            self.blob + self.blob

    def CheckBlobRepeateNotSupported(self):
        with self.assertRaises(SystemError):
            self.blob * 5

    def CheckBlobContainsNotSupported(self):
        with self.assertRaises(SystemError):
            b"aaaaa" in self.blob

@unittest.skipUnless(threading, 'This test requires threading.')
class ThreadTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")
        self.cur = self.con.cursor()
        self.cur.execute("create table test(id integer primary key, name text, bin binary, ratio number, ts timestamp)")

    def tearDown(self):
        self.cur.close()
        self.con.close()

    def CheckConCursor(self):
        def run(con, errors):
            try:
                cur = con.cursor()
                errors.append("did not raise ProgrammingError")
                return
            except sqlite.ProgrammingError:
                return
            except:
                errors.append("raised wrong exception")

        errors = []
        t = threading.Thread(target=run, kwargs={"con": self.con, "errors": errors})
        t.start()
        t.join()
        if len(errors) > 0:
            self.fail("\n".join(errors))

    def CheckConCommit(self):
        def run(con, errors):
            try:
                con.commit()
                errors.append("did not raise ProgrammingError")
                return
            except sqlite.ProgrammingError:
                return
            except:
                errors.append("raised wrong exception")

        errors = []
        t = threading.Thread(target=run, kwargs={"con": self.con, "errors": errors})
        t.start()
        t.join()
        if len(errors) > 0:
            self.fail("\n".join(errors))

    def CheckConRollback(self):
        def run(con, errors):
            try:
                con.rollback()
                errors.append("did not raise ProgrammingError")
                return
            except sqlite.ProgrammingError:
                return
            except:
                errors.append("raised wrong exception")

        errors = []
        t = threading.Thread(target=run, kwargs={"con": self.con, "errors": errors})
        t.start()
        t.join()
        if len(errors) > 0:
            self.fail("\n".join(errors))

    def CheckConClose(self):
        def run(con, errors):
            try:
                con.close()
                errors.append("did not raise ProgrammingError")
                return
            except sqlite.ProgrammingError:
                return
            except:
                errors.append("raised wrong exception")

        errors = []
        t = threading.Thread(target=run, kwargs={"con": self.con, "errors": errors})
        t.start()
        t.join()
        if len(errors) > 0:
            self.fail("\n".join(errors))

    def CheckCurImplicitBegin(self):
        def run(cur, errors):
            try:
                cur.execute("insert into test(name) values ('a')")
                errors.append("did not raise ProgrammingError")
                return
            except sqlite.ProgrammingError:
                return
            except:
                errors.append("raised wrong exception")

        errors = []
        t = threading.Thread(target=run, kwargs={"cur": self.cur, "errors": errors})
        t.start()
        t.join()
        if len(errors) > 0:
            self.fail("\n".join(errors))

    def CheckCurClose(self):
        def run(cur, errors):
            try:
                cur.close()
                errors.append("did not raise ProgrammingError")
                return
            except sqlite.ProgrammingError:
                return
            except:
                errors.append("raised wrong exception")

        errors = []
        t = threading.Thread(target=run, kwargs={"cur": self.cur, "errors": errors})
        t.start()
        t.join()
        if len(errors) > 0:
            self.fail("\n".join(errors))

    def CheckCurExecute(self):
        def run(cur, errors):
            try:
                cur.execute("select name from test")
                errors.append("did not raise ProgrammingError")
                return
            except sqlite.ProgrammingError:
                return
            except:
                errors.append("raised wrong exception")

        errors = []
        self.cur.execute("insert into test(name) values ('a')")
        t = threading.Thread(target=run, kwargs={"cur": self.cur, "errors": errors})
        t.start()
        t.join()
        if len(errors) > 0:
            self.fail("\n".join(errors))

    def CheckCurIterNext(self):
        def run(cur, errors):
            try:
                row = cur.fetchone()
                errors.append("did not raise ProgrammingError")
                return
            except sqlite.ProgrammingError:
                return
            except:
                errors.append("raised wrong exception")

        errors = []
        self.cur.execute("insert into test(name) values ('a')")
        self.cur.execute("select name from test")
        t = threading.Thread(target=run, kwargs={"cur": self.cur, "errors": errors})
        t.start()
        t.join()
        if len(errors) > 0:
            self.fail("\n".join(errors))

class ConstructorTests(unittest.TestCase):
    def CheckDate(self):
        d = sqlite.Date(2004, 10, 28)

    def CheckTime(self):
        t = sqlite.Time(12, 39, 35)

    def CheckTimestamp(self):
        ts = sqlite.Timestamp(2004, 10, 28, 12, 39, 35)

    def CheckDateFromTicks(self):
        d = sqlite.DateFromTicks(42)

    def CheckTimeFromTicks(self):
        t = sqlite.TimeFromTicks(42)

    def CheckTimestampFromTicks(self):
        ts = sqlite.TimestampFromTicks(42)

    def CheckBinary(self):
        b = sqlite.Binary(b"\0'")

class ExtensionTests(unittest.TestCase):
    def CheckScriptStringSql(self):
        con = sqlite.connect(":memory:")
        cur = con.cursor()
        cur.executescript("""
            -- bla bla
            /* a stupid comment */
            create table a(i);
            insert into a(i) values (5);
            """)
        cur.execute("select i from a")
        res = cur.fetchone()[0]
        self.assertEqual(res, 5)

    def CheckScriptSyntaxError(self):
        con = sqlite.connect(":memory:")
        cur = con.cursor()
        with self.assertRaises(sqlite.OperationalError):
            cur.executescript("create table test(x); asdf; create table test2(x)")

    def CheckScriptErrorNormal(self):
        con = sqlite.connect(":memory:")
        cur = con.cursor()
        with self.assertRaises(sqlite.OperationalError):
            cur.executescript("create table test(sadfsadfdsa); select foo from hurz;")

    def CheckCursorExecutescriptAsBytes(self):
        con = sqlite.connect(":memory:")
        cur = con.cursor()
        with self.assertRaises(ValueError) as cm:
            cur.executescript(b"create table test(foo); insert into test(foo) values (5);")
        self.assertEqual(str(cm.exception), 'script argument must be unicode.')

    def CheckConnectionExecute(self):
        con = sqlite.connect(":memory:")
        result = con.execute("select 5").fetchone()[0]
        self.assertEqual(result, 5, "Basic test of Connection.execute")

    def CheckConnectionExecutemany(self):
        con = sqlite.connect(":memory:")
        con.execute("create table test(foo)")
        con.executemany("insert into test(foo) values (?)", [(3,), (4,)])
        result = con.execute("select foo from test order by foo").fetchall()
        self.assertEqual(result[0][0], 3, "Basic test of Connection.executemany")
        self.assertEqual(result[1][0], 4, "Basic test of Connection.executemany")

    def CheckConnectionExecutescript(self):
        con = sqlite.connect(":memory:")
        con.executescript("create table test(foo); insert into test(foo) values (5);")
        result = con.execute("select foo from test").fetchone()[0]
        self.assertEqual(result, 5, "Basic test of Connection.executescript")

class ClosedConTests(unittest.TestCase):
    def CheckClosedConCursor(self):
        con = sqlite.connect(":memory:")
        con.close()
        with self.assertRaises(sqlite.ProgrammingError):
            cur = con.cursor()

    def CheckClosedConCommit(self):
        con = sqlite.connect(":memory:")
        con.close()
        with self.assertRaises(sqlite.ProgrammingError):
            con.commit()

    def CheckClosedConRollback(self):
        con = sqlite.connect(":memory:")
        con.close()
        with self.assertRaises(sqlite.ProgrammingError):
            con.rollback()

    def CheckClosedCurExecute(self):
        con = sqlite.connect(":memory:")
        cur = con.cursor()
        con.close()
        with self.assertRaises(sqlite.ProgrammingError):
            cur.execute("select 4")

    def CheckClosedBlobRead(self):
        con = sqlite.connect(":memory:")
        con.execute("create table test(id integer primary key, blob_col blob)")
        con.execute("insert into test(blob_col) values (zeroblob(100))")
        blob = con.open_blob("test", "blob_col", 1)
        con.close()
        with self.assertRaises(sqlite.ProgrammingError):
            blob.read()

    def CheckClosedCreateFunction(self):
        con = sqlite.connect(":memory:")
        con.close()
        def f(x): return 17
        with self.assertRaises(sqlite.ProgrammingError):
            con.create_function("foo", 1, f)

    def CheckClosedCreateAggregate(self):
        con = sqlite.connect(":memory:")
        con.close()
        class Agg:
            def __init__(self):
                pass
            def step(self, x):
                pass
            def finalize(self):
                return 17
        with self.assertRaises(sqlite.ProgrammingError):
            con.create_aggregate("foo", 1, Agg)

    def CheckClosedSetAuthorizer(self):
        con = sqlite.connect(":memory:")
        con.close()
        def authorizer(*args):
            return sqlite.DENY
        with self.assertRaises(sqlite.ProgrammingError):
            con.set_authorizer(authorizer)

    def CheckClosedSetProgressCallback(self):
        con = sqlite.connect(":memory:")
        con.close()
        def progress(): pass
        with self.assertRaises(sqlite.ProgrammingError):
            con.set_progress_handler(progress, 100)

    def CheckClosedCall(self):
        con = sqlite.connect(":memory:")
        con.close()
        with self.assertRaises(sqlite.ProgrammingError):
            con()

class ClosedCurTests(unittest.TestCase):
    def CheckClosed(self):
        con = sqlite.connect(":memory:")
        cur = con.cursor()
        cur.close()

        for method_name in ("execute", "executemany", "executescript", "fetchall", "fetchmany", "fetchone"):
            if method_name in ("execute", "executescript"):
                params = ("select 4 union select 5",)
            elif method_name == "executemany":
                params = ("insert into foo(bar) values (?)", [(3,), (4,)])
            else:
                params = []

            with self.assertRaises(sqlite.ProgrammingError):
                method = getattr(cur, method_name)
                method(*params)


class SqliteOnConflictTests(unittest.TestCase):
    """
    Tests for SQLite's "insert on conflict" feature.

    See https://www.sqlite.org/lang_conflict.html for details.
    """

    def setUp(self):
        self.cx = sqlite.connect(":memory:")
        self.cu = self.cx.cursor()
        self.cu.execute("""
          CREATE TABLE test(
            id INTEGER PRIMARY KEY, name TEXT, unique_name TEXT UNIQUE
          );
        """)

    def tearDown(self):
        self.cu.close()
        self.cx.close()

    def CheckOnConflictRollbackWithExplicitTransaction(self):
        self.cx.isolation_level = None  # autocommit mode
        self.cu = self.cx.cursor()
        # Start an explicit transaction.
        self.cu.execute("BEGIN")
        self.cu.execute("INSERT INTO test(name) VALUES ('abort_test')")
        self.cu.execute("INSERT OR ROLLBACK INTO test(unique_name) VALUES ('foo')")
        with self.assertRaises(sqlite.IntegrityError):
            self.cu.execute("INSERT OR ROLLBACK INTO test(unique_name) VALUES ('foo')")
        # Use connection to commit.
        self.cx.commit()
        self.cu.execute("SELECT name, unique_name from test")
        # Transaction should have rolled back and nothing should be in table.
        self.assertEqual(self.cu.fetchall(), [])

    def CheckOnConflictAbortRaisesWithExplicitTransactions(self):
        # Abort cancels the current sql statement but doesn't change anything
        # about the current transaction.
        self.cx.isolation_level = None  # autocommit mode
        self.cu = self.cx.cursor()
        # Start an explicit transaction.
        self.cu.execute("BEGIN")
        self.cu.execute("INSERT INTO test(name) VALUES ('abort_test')")
        self.cu.execute("INSERT OR ABORT INTO test(unique_name) VALUES ('foo')")
        with self.assertRaises(sqlite.IntegrityError):
            self.cu.execute("INSERT OR ABORT INTO test(unique_name) VALUES ('foo')")
        self.cx.commit()
        self.cu.execute("SELECT name, unique_name FROM test")
        # Expect the first two inserts to work, third to do nothing.
        self.assertEqual(self.cu.fetchall(), [('abort_test', None), (None, 'foo',)])

    def CheckOnConflictRollbackWithoutTransaction(self):
        # Start of implicit transaction
        self.cu.execute("INSERT INTO test(name) VALUES ('abort_test')")
        self.cu.execute("INSERT OR ROLLBACK INTO test(unique_name) VALUES ('foo')")
        with self.assertRaises(sqlite.IntegrityError):
            self.cu.execute("INSERT OR ROLLBACK INTO test(unique_name) VALUES ('foo')")
        self.cu.execute("SELECT name, unique_name FROM test")
        # Implicit transaction is rolled back on error.
        self.assertEqual(self.cu.fetchall(), [])

    def CheckOnConflictAbortRaisesWithoutTransactions(self):
        # Abort cancels the current sql statement but doesn't change anything
        # about the current transaction.
        self.cu.execute("INSERT INTO test(name) VALUES ('abort_test')")
        self.cu.execute("INSERT OR ABORT INTO test(unique_name) VALUES ('foo')")
        with self.assertRaises(sqlite.IntegrityError):
            self.cu.execute("INSERT OR ABORT INTO test(unique_name) VALUES ('foo')")
        # Make sure all other values were inserted.
        self.cu.execute("SELECT name, unique_name FROM test")
        self.assertEqual(self.cu.fetchall(), [('abort_test', None), (None, 'foo',)])

    def CheckOnConflictFail(self):
        self.cu.execute("INSERT OR FAIL INTO test(unique_name) VALUES ('foo')")
        with self.assertRaises(sqlite.IntegrityError):
            self.cu.execute("INSERT OR FAIL INTO test(unique_name) VALUES ('foo')")
        self.assertEqual(self.cu.fetchall(), [])

    def CheckOnConflictIgnore(self):
        self.cu.execute("INSERT OR IGNORE INTO test(unique_name) VALUES ('foo')")
        # Nothing should happen.
        self.cu.execute("INSERT OR IGNORE INTO test(unique_name) VALUES ('foo')")
        self.cu.execute("SELECT unique_name FROM test")
        self.assertEqual(self.cu.fetchall(), [('foo',)])

    def CheckOnConflictReplace(self):
        self.cu.execute("INSERT OR REPLACE INTO test(name, unique_name) VALUES ('Data!', 'foo')")
        # There shouldn't be an IntegrityError exception.
        self.cu.execute("INSERT OR REPLACE INTO test(name, unique_name) VALUES ('Very different data!', 'foo')")
        self.cu.execute("SELECT name, unique_name FROM test")
        self.assertEqual(self.cu.fetchall(), [('Very different data!', 'foo')])


class ClosedBlobTests(unittest.TestCase):
    def setUp(self):
        self.cx = sqlite.connect(":memory:")
        self.cx.execute("create table test(id integer primary key, blob_col blob)")
        self.cx.execute("insert into test(blob_col) values (zeroblob(100))")

    def tearDown(self):
        self.cx.close()

    def CheckClosedRead(self):
        self.blob = self.cx.open_blob("test", "blob_col", 1)
        self.blob.close()
        with self.assertRaises(sqlite.ProgrammingError):
            self.blob.read()

    def CheckClosedWrite(self):
        self.blob = self.cx.open_blob("test", "blob_col", 1)
        self.blob.close()
        with self.assertRaises(sqlite.ProgrammingError):
            self.blob.write(b"aaaaaaaaa")

    def CheckClosedSeek(self):
        self.blob = self.cx.open_blob("test", "blob_col", 1)
        self.blob.close()
        with self.assertRaises(sqlite.ProgrammingError):
            self.blob.seek(10)

    def CheckClosedTell(self):
        self.blob = self.cx.open_blob("test", "blob_col", 1)
        self.blob.close()
        with self.assertRaises(sqlite.ProgrammingError):
            self.blob.tell()

    def CheckClosedClose(self):
        self.blob = self.cx.open_blob("test", "blob_col", 1)
        self.blob.close()
        with self.assertRaises(sqlite.ProgrammingError):
            self.blob.close()


class BlobContextManagerTests(unittest.TestCase):
    def setUp(self):
        self.cx = sqlite.connect(":memory:")
        self.cx.execute("create table test(id integer primary key, blob_col blob)")
        self.cx.execute("insert into test(blob_col) values (zeroblob(100))")

    def tearDown(self):
        self.cx.close()

    def CheckContextExecute(self):
        data = b"a" * 100
        with self.cx.open_blob("test", "blob_col", 1) as blob:
            blob.write(data)
        self.assertEqual(self.cx.execute("select blob_col from test").fetchone()[0], data)

    def CheckContextCloseBlob(self):
        with self.cx.open_blob("test", "blob_col", 1) as blob:
            blob.seek(10)
        with self.assertRaises(sqlite.ProgrammingError):
            blob.close()


def suite():
    module_suite = unittest.makeSuite(ModuleTests, "Check")
    connection_suite = unittest.makeSuite(ConnectionTests, "Check")
    cursor_suite = unittest.makeSuite(CursorTests, "Check")
    thread_suite = unittest.makeSuite(ThreadTests, "Check")
    constructor_suite = unittest.makeSuite(ConstructorTests, "Check")
    ext_suite = unittest.makeSuite(ExtensionTests, "Check")
    closed_con_suite = unittest.makeSuite(ClosedConTests, "Check")
    closed_cur_suite = unittest.makeSuite(ClosedCurTests, "Check")
    on_conflict_suite = unittest.makeSuite(SqliteOnConflictTests, "Check")
    blob_suite = unittest.makeSuite(BlobTests, "Check")
    closed_blob_suite = unittest.makeSuite(ClosedBlobTests, "Check")
    blob_context_manager_suite = unittest.makeSuite(BlobContextManagerTests, "Check")
    return unittest.TestSuite((
        module_suite, connection_suite, cursor_suite, thread_suite,
        constructor_suite, ext_suite, closed_con_suite, closed_cur_suite,
        on_conflict_suite, blob_suite, closed_blob_suite,
        blob_context_manager_suite,
    ))

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

test_suite_list.append(suite)
"""


/*
file https://github.com/coleifer/pysqlite3/blob/0.5.0/test/factory.py
*/

"""
#-*- coding: iso-8859-1 -*-
# pysqlite2/test/factory.py: tests for the various factories in pysqlite
#
# Copyright (C) 2005-2007 Gerhard H�ring <gh@ghaering.de>
#
# This file is part of pysqlite.
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

import unittest
from . import sqlmath_dbapi2 as sqlite
from collections.abc import Sequence

class MyConnection(sqlite.Connection):
    def __init__(self, *args, **kwargs):
        sqlite.Connection.__init__(self, *args, **kwargs)

def dict_factory(cursor, row):
    d = {}
    for idx, col in enumerate(cursor.description):
        d[col[0]] = row[idx]
    return d

class MyCursor(sqlite.Cursor):
    def __init__(self, *args, **kwargs):
        sqlite.Cursor.__init__(self, *args, **kwargs)
        self.row_factory = dict_factory

class ConnectionFactoryTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:", factory=MyConnection)

    def tearDown(self):
        self.con.close()

    def CheckIsInstance(self):
        self.assertIsInstance(self.con, MyConnection)

class CursorFactoryTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")

    def tearDown(self):
        self.con.close()

    def CheckIsInstance(self):
        cur = self.con.cursor()
        self.assertIsInstance(cur, sqlite.Cursor)
        cur = self.con.cursor(MyCursor)
        self.assertIsInstance(cur, MyCursor)
        cur = self.con.cursor(factory=lambda con: MyCursor(con))
        self.assertIsInstance(cur, MyCursor)

    def CheckInvalidFactory(self):
        # not a callable at all
        self.assertRaises(TypeError, self.con.cursor, None)
        # invalid callable with not exact one argument
        self.assertRaises(TypeError, self.con.cursor, lambda: None)
        # invalid callable returning non-cursor
        self.assertRaises(TypeError, self.con.cursor, lambda con: None)

class RowFactoryTestsBackwardsCompat(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")

    def CheckIsProducedByFactory(self):
        cur = self.con.cursor(factory=MyCursor)
        cur.execute("select 4+5 as foo")
        row = cur.fetchone()
        self.assertIsInstance(row, dict)
        cur.close()

    def tearDown(self):
        self.con.close()

class RowFactoryTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")

    def CheckCustomFactory(self):
        self.con.row_factory = lambda cur, row: list(row)
        row = self.con.execute("select 1, 2").fetchone()
        self.assertIsInstance(row, list)

    def CheckSqliteRowIndex(self):
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1 as a_1, 2 as b").fetchone()
        self.assertIsInstance(row, sqlite.Row)

        self.assertEqual(row["a_1"], 1, "by name: wrong result for column 'a_1'")
        self.assertEqual(row["b"], 2, "by name: wrong result for column 'b'")

        self.assertEqual(row["A_1"], 1, "by name: wrong result for column 'A_1'")
        self.assertEqual(row["B"], 2, "by name: wrong result for column 'B'")

        self.assertEqual(row[0], 1, "by index: wrong result for column 0")
        self.assertEqual(row[1], 2, "by index: wrong result for column 1")
        self.assertEqual(row[-1], 2, "by index: wrong result for column -1")
        self.assertEqual(row[-2], 1, "by index: wrong result for column -2")

        with self.assertRaises(IndexError):
            row['c']
        with self.assertRaises(IndexError):
            row['a_\x11']
        with self.assertRaises(IndexError):
            row['a_\x7f1']
        with self.assertRaises(IndexError):
            row[2]
        with self.assertRaises(IndexError):
            row[-3]
        with self.assertRaises(IndexError):
            row[2**1000]

    def CheckSqliteRowIndexUnicode(self):
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1 as \xff").fetchone()
        self.assertEqual(row["\xff"], 1)
        with self.assertRaises(IndexError):
            row['\u0178']
        with self.assertRaises(IndexError):
            row['\xdf']

    def CheckSqliteRowSlice(self):
        # A sqlite.Row can be sliced like a list.
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1, 2, 3, 4").fetchone()
        self.assertEqual(row[0:0], ())
        self.assertEqual(row[0:1], (1,))
        self.assertEqual(row[1:3], (2, 3))
        self.assertEqual(row[3:1], ())
        # Explicit bounds are optional.
        self.assertEqual(row[1:], (2, 3, 4))
        self.assertEqual(row[:3], (1, 2, 3))
        # Slices can use negative indices.
        self.assertEqual(row[-2:-1], (3,))
        self.assertEqual(row[-2:], (3, 4))
        # Slicing supports steps.
        self.assertEqual(row[0:4:2], (1, 3))
        self.assertEqual(row[3:0:-2], (4, 2))

    def CheckSqliteRowIter(self):
        """Checks if the row object is iterable"""
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1 as a, 2 as b").fetchone()
        for col in row:
            pass

    def CheckSqliteRowAsTuple(self):
        """Checks if the row object can be converted to a tuple"""
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1 as a, 2 as b").fetchone()
        t = tuple(row)
        self.assertEqual(t, (row['a'], row['b']))

    def CheckSqliteRowAsDict(self):
        """Checks if the row object can be correctly converted to a dictionary"""
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1 as a, 2 as b").fetchone()
        d = dict(row)
        self.assertEqual(d["a"], row["a"])
        self.assertEqual(d["b"], row["b"])

    def CheckSqliteRowHashCmp(self):
        """Checks if the row object compares and hashes correctly"""
        self.con.row_factory = sqlite.Row
        row_1 = self.con.execute("select 1 as a, 2 as b").fetchone()
        row_2 = self.con.execute("select 1 as a, 2 as b").fetchone()
        row_3 = self.con.execute("select 1 as a, 3 as b").fetchone()
        row_4 = self.con.execute("select 1 as b, 2 as a").fetchone()
        row_5 = self.con.execute("select 2 as b, 1 as a").fetchone()

        self.assertTrue(row_1 == row_1)
        self.assertTrue(row_1 == row_2)
        self.assertFalse(row_1 == row_3)
        self.assertFalse(row_1 == row_4)
        self.assertFalse(row_1 == row_5)
        self.assertFalse(row_1 == object())

        self.assertFalse(row_1 != row_1)
        self.assertFalse(row_1 != row_2)
        self.assertTrue(row_1 != row_3)
        self.assertTrue(row_1 != row_4)
        self.assertTrue(row_1 != row_5)
        self.assertTrue(row_1 != object())

        with self.assertRaises(TypeError):
            row_1 > row_2
        with self.assertRaises(TypeError):
            row_1 < row_2
        with self.assertRaises(TypeError):
            row_1 >= row_2
        with self.assertRaises(TypeError):
            row_1 <= row_2

        self.assertEqual(hash(row_1), hash(row_2))

    def CheckSqliteRowAsSequence(self):
        """ Checks if the row object can act like a sequence """
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1 as a, 2 as b").fetchone()

        as_tuple = tuple(row)
        self.assertEqual(list(reversed(row)), list(reversed(as_tuple)))
        self.assertIsInstance(row, Sequence)

    def CheckFakeCursorClass(self):
        # Issue #24257: Incorrect use of PyObject_IsInstance() caused
        # segmentation fault.
        # Issue #27861: Also applies for cursor factory.
        class FakeCursor(str):
            __class__ = sqlite.Cursor
        self.con.row_factory = sqlite.Row
        self.assertRaises(TypeError, self.con.cursor, FakeCursor)
        self.assertRaises(TypeError, sqlite.Row, FakeCursor(), ())

    def tearDown(self):
        self.con.close()

class TextFactoryTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")

    def CheckUnicode(self):
        austria = "�sterreich"
        row = self.con.execute("select ?", (austria,)).fetchone()
        self.assertEqual(type(row[0]), str, "type of row[0] must be unicode")

    def CheckString(self):
        self.con.text_factory = bytes
        austria = "�sterreich"
        row = self.con.execute("select ?", (austria,)).fetchone()
        self.assertEqual(type(row[0]), bytes, "type of row[0] must be bytes")
        self.assertEqual(row[0], austria.encode("utf-8"), "column must equal original data in UTF-8")

    def CheckCustom(self):
        self.con.text_factory = lambda x: str(x, "utf-8", "ignore")
        austria = "�sterreich"
        row = self.con.execute("select ?", (austria,)).fetchone()
        self.assertEqual(type(row[0]), str, "type of row[0] must be unicode")
        self.assertTrue(row[0].endswith("reich"), "column must contain original data")

    def CheckOptimizedUnicode(self):
        # In py3k, str objects are always returned when text_factory
        # is OptimizedUnicode
        self.con.text_factory = sqlite.OptimizedUnicode
        austria = "�sterreich"
        germany = "Deutchland"
        a_row = self.con.execute("select ?", (austria,)).fetchone()
        d_row = self.con.execute("select ?", (germany,)).fetchone()
        self.assertEqual(type(a_row[0]), str, "type of non-ASCII row must be str")
        self.assertEqual(type(d_row[0]), str, "type of ASCII-only row must be str")

    def tearDown(self):
        self.con.close()

class TextFactoryTestsWithEmbeddedZeroBytes(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")
        self.con.execute("create table test (value text)")
        self.con.execute("insert into test (value) values (?)", ("a\x00b",))

    def CheckString(self):
        # text_factory defaults to str
        row = self.con.execute("select value from test").fetchone()
        self.assertIs(type(row[0]), str)
        self.assertEqual(row[0], "a\x00b")

    def CheckBytes(self):
        self.con.text_factory = bytes
        row = self.con.execute("select value from test").fetchone()
        self.assertIs(type(row[0]), bytes)
        self.assertEqual(row[0], b"a\x00b")

    def CheckBytearray(self):
        self.con.text_factory = bytearray
        row = self.con.execute("select value from test").fetchone()
        self.assertIs(type(row[0]), bytearray)
        self.assertEqual(row[0], b"a\x00b")

    def CheckCustom(self):
        # A custom factory should receive a bytes argument
        self.con.text_factory = lambda x: x
        row = self.con.execute("select value from test").fetchone()
        self.assertIs(type(row[0]), bytes)
        self.assertEqual(row[0], b"a\x00b")

    def tearDown(self):
        self.con.close()

def suite():
    connection_suite = unittest.makeSuite(ConnectionFactoryTests, "Check")
    cursor_suite = unittest.makeSuite(CursorFactoryTests, "Check")
    row_suite_compat = unittest.makeSuite(RowFactoryTestsBackwardsCompat, "Check")
    row_suite = unittest.makeSuite(RowFactoryTests, "Check")
    text_suite = unittest.makeSuite(TextFactoryTests, "Check")
    text_zero_bytes_suite = unittest.makeSuite(TextFactoryTestsWithEmbeddedZeroBytes, "Check")
    return unittest.TestSuite((connection_suite, cursor_suite, row_suite_compat, row_suite, text_suite, text_zero_bytes_suite))

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

test_suite_list.append(suite)
"""


/*
file https://github.com/coleifer/pysqlite3/blob/0.5.0/test/hooks.py
*/

"""
#-*- coding: iso-8859-1 -*-
# pysqlite2/test/hooks.py: tests for various SQLite-specific hooks
#
# Copyright (C) 2006-2007 Gerhard H�ring <gh@ghaering.de>
#
# This file is part of pysqlite.
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

import unittest
from . import sqlmath_dbapi2 as sqlite


class CollationTests(unittest.TestCase):
    def CheckCreateCollationNotString(self):
        con = sqlite.connect(":memory:")
        with self.assertRaises(TypeError):
            con.create_collation(None, lambda x, y: (x > y) - (x < y))

    def CheckCreateCollationNotCallable(self):
        con = sqlite.connect(":memory:")
        with self.assertRaises(TypeError) as cm:
            con.create_collation("X", 42)
        self.assertEqual(str(cm.exception), 'parameter must be callable')

    def CheckCreateCollationNotAscii(self):
        con = sqlite.connect(":memory:")
        with self.assertRaises(sqlite.ProgrammingError):
            con.create_collation("coll�", lambda x, y: (x > y) - (x < y))

    def CheckCreateCollationBadUpper(self):
        class BadUpperStr(str):
            def upper(self):
                return None
        con = sqlite.connect(":memory:")
        mycoll = lambda x, y: -((x > y) - (x < y))
        con.create_collation(BadUpperStr("mycoll"), mycoll)
        result = con.execute("""
            select x from (
            select 'a' as x
            union
            select 'b' as x
            ) order by x collate mycoll
            """).fetchall()
        self.assertEqual(result[0][0], 'b')
        self.assertEqual(result[1][0], 'a')

    @unittest.skipIf(sqlite.sqlite_version_info < (3, 2, 1),
                     'old SQLite versions crash on this test')
    def CheckCollationIsUsed(self):
        def mycoll(x, y):
            # reverse order
            return -((x > y) - (x < y))

        con = sqlite.connect(":memory:")
        con.create_collation("mycoll", mycoll)
        sql = """
            select x from (
            select 'a' as x
            union
            select 'b' as x
            union
            select 'c' as x
            ) order by x collate mycoll
            """
        result = con.execute(sql).fetchall()
        self.assertEqual(result, [('c',), ('b',), ('a',)],
                         msg='the expected order was not returned')

        con.create_collation("mycoll", None)
        with self.assertRaises(sqlite.OperationalError) as cm:
            result = con.execute(sql).fetchall()
        self.assertEqual(str(cm.exception), 'no such collation sequence: mycoll')

    def CheckCollationReturnsLargeInteger(self):
        def mycoll(x, y):
            # reverse order
            return -((x > y) - (x < y)) * 2**32
        con = sqlite.connect(":memory:")
        con.create_collation("mycoll", mycoll)
        sql = """
            select x from (
            select 'a' as x
            union
            select 'b' as x
            union
            select 'c' as x
            ) order by x collate mycoll
            """
        result = con.execute(sql).fetchall()
        self.assertEqual(result, [('c',), ('b',), ('a',)],
                         msg="the expected order was not returned")

    def CheckCollationRegisterTwice(self):
        """
        Register two different collation functions under the same name.
        Verify that the last one is actually used.
        """
        con = sqlite.connect(":memory:")
        con.create_collation("mycoll", lambda x, y: (x > y) - (x < y))
        con.create_collation("mycoll", lambda x, y: -((x > y) - (x < y)))
        result = con.execute("""
            select x from (select 'a' as x union select 'b' as x) order by x collate mycoll
            """).fetchall()
        self.assertEqual(result[0][0], 'b')
        self.assertEqual(result[1][0], 'a')

    def CheckDeregisterCollation(self):
        """
        Register a collation, then deregister it. Make sure an error is raised if we try
        to use it.
        """
        con = sqlite.connect(":memory:")
        con.create_collation("mycoll", lambda x, y: (x > y) - (x < y))
        con.create_collation("mycoll", None)
        with self.assertRaises(sqlite.OperationalError) as cm:
            con.execute("select 'a' as x union select 'b' as x order by x collate mycoll")
        self.assertEqual(str(cm.exception), 'no such collation sequence: mycoll')

class ProgressTests(unittest.TestCase):
    def CheckProgressHandlerUsed(self):
        """
        Test that the progress handler is invoked once it is set.
        """
        con = sqlite.connect(":memory:")
        progress_calls = []
        def progress():
            progress_calls.append(None)
            return 0
        con.set_progress_handler(progress, 1)
        con.execute("""
            create table foo(a, b)
            """)
        self.assertTrue(progress_calls)


    def CheckOpcodeCount(self):
        """
        Test that the opcode argument is respected.
        """
        con = sqlite.connect(":memory:")
        progress_calls = []
        def progress():
            progress_calls.append(None)
            return 0
        con.set_progress_handler(progress, 1)
        curs = con.cursor()
        curs.execute("""
            create table foo (a, b)
            """)
        first_count = len(progress_calls)
        progress_calls = []
        con.set_progress_handler(progress, 2)
        curs.execute("""
            create table bar (a, b)
            """)
        second_count = len(progress_calls)
        self.assertGreaterEqual(first_count, second_count)

    def CheckCancelOperation(self):
        """
        Test that returning a non-zero value stops the operation in progress.
        """
        con = sqlite.connect(":memory:")
        def progress():
            return 1
        con.set_progress_handler(progress, 1)
        curs = con.cursor()
        self.assertRaises(
            sqlite.OperationalError,
            curs.execute,
            "create table bar (a, b)")

    def CheckClearHandler(self):
        """
        Test that setting the progress handler to None clears the previously set handler.
        """
        con = sqlite.connect(":memory:")
        action = 0
        def progress():
            nonlocal action
            action = 1
            return 0
        con.set_progress_handler(progress, 1)
        con.set_progress_handler(None, 1)
        con.execute("select 1 union select 2 union select 3").fetchall()
        self.assertEqual(action, 0, "progress handler was not cleared")

class TraceCallbackTests(unittest.TestCase):
    def CheckTraceCallbackUsed(self):
        """
        Test that the trace callback is invoked once it is set.
        """
        con = sqlite.connect(":memory:")
        traced_statements = []
        def trace(statement):
            traced_statements.append(statement)
        con.set_trace_callback(trace)
        con.execute("create table foo(a, b)")
        self.assertTrue(traced_statements)
        self.assertTrue(any("create table foo" in stmt for stmt in traced_statements))

    def CheckClearTraceCallback(self):
        """
        Test that setting the trace callback to None clears the previously set callback.
        """
        con = sqlite.connect(":memory:")
        traced_statements = []
        def trace(statement):
            traced_statements.append(statement)
        con.set_trace_callback(trace)
        con.set_trace_callback(None)
        con.execute("create table foo(a, b)")
        self.assertFalse(traced_statements, "trace callback was not cleared")

    def CheckUnicodeContent(self):
        """
        Test that the statement can contain unicode literals.
        """
        unicode_value = '\xf6\xe4\xfc\xd6\xc4\xdc\xdf\u20ac'
        con = sqlite.connect(":memory:")
        traced_statements = []
        def trace(statement):
            traced_statements.append(statement)
        con.set_trace_callback(trace)
        con.execute("create table foo(x)")
        # Can't execute bound parameters as their values don't appear
        # in traced statements before SQLite 3.6.21
        # (cf. http://www.sqlite.org/draft/releaselog/3_6_21.html)
# hack-pysqlite3 - enforce SQLITE_DQS
        con.execute('insert into foo(x) values (\'%s\')' % unicode_value)
        con.commit()
        self.assertTrue(any(unicode_value in stmt for stmt in traced_statements),
                        "Unicode data %s garbled in trace callback: %s"
                        % (ascii(unicode_value), ', '.join(map(ascii, traced_statements))))

    #@unittest.skipIf(sqlite.sqlite_version_info < (3, 3, 9), "sqlite3_prepare_v2 is not available")
    #def CheckTraceCallbackContent(self):
    #    # set_trace_callback() shouldn't produce duplicate content (bpo-26187)
    #    traced_statements = []
    #    def trace(statement):
    #        traced_statements.append(statement)

    #    queries = ["create table foo(x)",
    #               "insert into foo(x) values(1)"]
    #    self.addCleanup(unlink, TESTFN)
    #    con1 = sqlite.connect(TESTFN, isolation_level=None)
    #    con2 = sqlite.connect(TESTFN)
    #    con1.set_trace_callback(trace)
    #    cur = con1.cursor()
    #    cur.execute(queries[0])
    #    con2.execute("create table bar(x)")
    #    cur.execute(queries[1])
    #    self.assertEqual(traced_statements, queries)


def suite():
    collation_suite = unittest.makeSuite(CollationTests, "Check")
    progress_suite = unittest.makeSuite(ProgressTests, "Check")
    trace_suite = unittest.makeSuite(TraceCallbackTests, "Check")
    return unittest.TestSuite((collation_suite, progress_suite, trace_suite))

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

test_suite_list.append(suite)
"""


/*
file https://github.com/coleifer/pysqlite3/blob/0.5.0/test/regression.py
*/

"""
#-*- coding: iso-8859-1 -*-
# pysqlite2/test/regression.py: pysqlite regression tests
#
# Copyright (C) 2006-2010 Gerhard H�ring <gh@ghaering.de>
#
# This file is part of pysqlite.
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

import datetime
import functools
import unittest
from . import sqlmath_dbapi2 as sqlite
import weakref
#from test import support

class RegressionTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")

    def tearDown(self):
        self.con.close()

    def CheckPragmaUserVersion(self):
        # This used to crash pysqlite because this pragma command returns NULL for the column name
        cur = self.con.cursor()
        cur.execute("pragma user_version")

    def CheckPragmaSchemaVersion(self):
        # This still crashed pysqlite <= 2.2.1
        con = sqlite.connect(":memory:", detect_types=sqlite.PARSE_COLNAMES)
        try:
            cur = self.con.cursor()
            cur.execute("pragma schema_version")
        finally:
            cur.close()
            con.close()

    def CheckStatementReset(self):
        # pysqlite 2.1.0 to 2.2.0 have the problem that not all statements are
        # reset before a rollback, but only those that are still in the
        # statement cache. The others are not accessible from the connection object.
        con = sqlite.connect(":memory:", cached_statements=5)
        cursors = [con.cursor() for x in range(5)]
        cursors[0].execute("create table test(x)")
        for i in range(10):
            cursors[0].executemany("insert into test(x) values (?)", [(x,) for x in range(10)])

        for i in range(5):
            cursors[i].execute(" " * i + "select x from test")

        con.rollback()

    def CheckColumnNameWithSpaces(self):
        cur = self.con.cursor()
        cur.execute('select 1 as "foo bar [datetime]"')
        self.assertEqual(cur.description[0][0], "foo bar [datetime]")

        cur.execute('select 1 as "foo baz"')
        self.assertEqual(cur.description[0][0], "foo baz")

    def CheckStatementFinalizationOnCloseDb(self):
        # pysqlite versions <= 2.3.3 only finalized statements in the statement
        # cache when closing the database. statements that were still
        # referenced in cursors weren't closed and could provoke "
        # "OperationalError: Unable to close due to unfinalised statements".
        con = sqlite.connect(":memory:")
        cursors = []
        # default statement cache size is 100
        for i in range(105):
            cur = con.cursor()
            cursors.append(cur)
            cur.execute("select 1 x union select " + str(i))
        con.close()

    @unittest.skipIf(sqlite.sqlite_version_info < (3, 2, 2), 'needs sqlite 3.2.2 or newer')
    def CheckOnConflictRollback(self):
        con = sqlite.connect(":memory:")
        con.execute("create table foo(x, unique(x) on conflict rollback)")
        con.execute("insert into foo(x) values (1)")
        try:
            con.execute("insert into foo(x) values (1)")
        except sqlite.DatabaseError:
            pass
        con.execute("insert into foo(x) values (2)")
        try:
            con.commit()
        except sqlite.OperationalError:
            self.fail("pysqlite knew nothing about the implicit ROLLBACK")

    def CheckWorkaroundForBuggySqliteTransferBindings(self):
        """
        pysqlite would crash with older SQLite versions unless
        a workaround is implemented.
        """
        self.con.execute("create table foo(bar)")
        self.con.execute("drop table foo")
        self.con.execute("create table foo(bar)")

    def CheckEmptyStatement(self):
        """
        pysqlite used to segfault with SQLite versions 3.5.x. These return NULL
        for "no-operation" statements
        """
        self.con.execute("")

    def CheckTypeMapUsage(self):
        """
        pysqlite until 2.4.1 did not rebuild the row_cast_map when recompiling
        a statement. This test exhibits the problem.
        """
        SELECT = "select * from foo"
        con = sqlite.connect(":memory:",detect_types=sqlite.PARSE_DECLTYPES)
        con.execute("create table foo(bar timestamp)")
        con.execute("insert into foo(bar) values (?)", (datetime.datetime.now(),))
        con.execute(SELECT)
        con.execute("drop table foo")
        con.execute("create table foo(bar integer)")
        con.execute("insert into foo(bar) values (5)")
        con.execute(SELECT)

    def CheckErrorMsgDecodeError(self):
        # When porting the module to Python 3.0, the error message about
        # decoding errors disappeared. This verifies they're back again.
        with self.assertRaises(sqlite.OperationalError) as cm:
            self.con.execute("select 'xxx' || ? || 'yyy' colname",
                             (bytes(bytearray([250])),)).fetchone()
        msg = "Could not decode to UTF-8 column 'colname' with text 'xxx"
        self.assertIn(msg, str(cm.exception))

    def CheckRegisterAdapter(self):
        """
        See issue 3312.
        """
        self.assertRaises(TypeError, sqlite.register_adapter, {}, None)

    def CheckSetIsolationLevel(self):
        # See issue 27881.
        class CustomStr(str):
            def upper(self):
                return None
            def __del__(self):
                con.isolation_level = ""

        con = sqlite.connect(":memory:")
        con.isolation_level = None
        for level in "", "DEFERRED", "IMMEDIATE", "EXCLUSIVE":
            with self.subTest(level=level):
                con.isolation_level = level
                con.isolation_level = level.lower()
                con.isolation_level = level.capitalize()
                con.isolation_level = CustomStr(level)

        # setting isolation_level failure should not alter previous state
        con.isolation_level = None
        con.isolation_level = "DEFERRED"
        pairs = [
            (1, TypeError), (b'', TypeError), ("abc", ValueError),
            ("IMMEDIATE\0EXCLUSIVE", ValueError), ("\xe9", ValueError),
        ]
        for value, exc in pairs:
            with self.subTest(level=value):
                with self.assertRaises(exc):
                    con.isolation_level = value
                self.assertEqual(con.isolation_level, "DEFERRED")

    def CheckCursorConstructorCallCheck(self):
        """
        Verifies that cursor methods check whether base class __init__ was
        called.
        """
        class Cursor(sqlite.Cursor):
            def __init__(self, con):
                pass

        con = sqlite.connect(":memory:")
        cur = Cursor(con)
        with self.assertRaises(sqlite.ProgrammingError):
            cur.execute("select 4+5").fetchall()
        with self.assertRaisesRegex(sqlite.ProgrammingError,
                                    r'^Base Cursor\.__init__ not called\.$'):
            cur.close()

    def CheckStrSubclass(self):
        """
        The Python 3.0 port of the module didn't cope with values of subclasses of str.
        """
        class MyStr(str): pass
        self.con.execute("select ?", (MyStr("abc"),))

    def CheckConnectionConstructorCallCheck(self):
        """
        Verifies that connection methods check whether base class __init__ was
        called.
        """
        class Connection(sqlite.Connection):
            def __init__(self, name):
                pass

        con = Connection(":memory:")
        with self.assertRaises(sqlite.ProgrammingError):
            cur = con.cursor()

    def CheckCursorRegistration(self):
        """
        Verifies that subclassed cursor classes are correctly registered with
        the connection object, too.  (fetch-across-rollback problem)
        """
        class Connection(sqlite.Connection):
            def cursor(self):
                return Cursor(self)

        class Cursor(sqlite.Cursor):
            def __init__(self, con):
                sqlite.Cursor.__init__(self, con)

        con = Connection(":memory:")
        cur = con.cursor()
        cur.execute("create table foo(x)")
        cur.executemany("insert into foo(x) values (?)", [(3,), (4,), (5,)])
        cur.execute("select x from foo")
        con.rollback()
        with self.assertRaises(sqlite.InterfaceError):
            cur.fetchall()

    def CheckAutoCommit(self):
        """
        Verifies that creating a connection in autocommit mode works.
        2.5.3 introduced a regression so that these could no longer
        be created.
        """
        con = sqlite.connect(":memory:", isolation_level=None)

    def CheckPragmaAutocommit(self):
        """
        Verifies that running a PRAGMA statement that does an autocommit does
        work. This did not work in 2.5.3/2.5.4.
        """
        cur = self.con.cursor()
        cur.execute("create table foo(bar)")
        cur.execute("insert into foo(bar) values (5)")

        cur.execute("pragma page_size")
        row = cur.fetchone()

    def CheckConnectionCall(self):
        """
        Call a connection with a non-string SQL request: check error handling
        of the statement constructor.
        """
        self.assertRaises(TypeError, self.con, 1)

    def CheckCollation(self):
        def collation_cb(a, b):
            return 1
        self.assertRaises(sqlite.ProgrammingError, self.con.create_collation,
            # Lone surrogate cannot be encoded to the default encoding (utf8)
            "\uDC80", collation_cb)

    def CheckRecursiveCursorUse(self):
        """
        http://bugs.python.org/issue10811

        Recursively using a cursor, such as when reusing it from a generator led to segfaults.
        Now we catch recursive cursor usage and raise a ProgrammingError.
        """
        con = sqlite.connect(":memory:")

        cur = con.cursor()
        cur.execute("create table a (bar)")
        cur.execute("create table b (baz)")

        def foo():
            cur.execute("insert into a (bar) values (?)", (1,))
            yield 1

        with self.assertRaises(sqlite.ProgrammingError):
            cur.executemany("insert into b (baz) values (?)",
                            ((i,) for i in foo()))

    def CheckConvertTimestampMicrosecondPadding(self):
        """
        http://bugs.python.org/issue14720

        The microsecond parsing of convert_timestamp() should pad with zeros,
        since the microsecond string "456" actually represents "456000".
        """

        con = sqlite.connect(":memory:", detect_types=sqlite.PARSE_DECLTYPES)
        cur = con.cursor()
        cur.execute("CREATE TABLE t (x TIMESTAMP)")

        # Microseconds should be 456000
        cur.execute("INSERT INTO t (x) VALUES ('2012-04-04 15:06:00.456')")

        # Microseconds should be truncated to 123456
        cur.execute("INSERT INTO t (x) VALUES ('2012-04-04 15:06:00.123456789')")

        cur.execute("SELECT * FROM t")
        values = [x[0] for x in cur.fetchall()]

        self.assertEqual(values, [
            datetime.datetime(2012, 4, 4, 15, 6, 0, 456000),
            datetime.datetime(2012, 4, 4, 15, 6, 0, 123456),
        ])

    def CheckInvalidIsolationLevelType(self):
        # isolation level is a string, not an integer
        self.assertRaises(TypeError,
                          sqlite.connect, ":memory:", isolation_level=123)


    def CheckNullCharacter(self):
        # Issue #21147
        con = sqlite.connect(":memory:")
        self.assertRaises(ValueError, con, "\0select 1")
        self.assertRaises(ValueError, con, "select 1\0")
        cur = con.cursor()
        self.assertRaises(ValueError, cur.execute, " \0select 2")
        self.assertRaises(ValueError, cur.execute, "select 2\0")

    def CheckCommitCursorReset(self):
        """
        Connection.commit() did reset cursors, which made sqlite3
        to return rows multiple times when fetched from cursors
        after commit. See issues 10513 and 23129 for details.
        """
        con = sqlite.connect(":memory:")
        con.executescript("""
        create table t(c);
        create table t2(c);
        insert into t values(0);
        insert into t values(1);
        insert into t values(2);
        """)

        self.assertEqual(con.isolation_level, "")

        counter = 0
        for i, row in enumerate(con.execute("select c from t")):
            with self.subTest(i=i, row=row):
                con.execute("insert into t2(c) values (?)", (i,))
                con.commit()
                if counter == 0:
                    self.assertEqual(row[0], 0)
                elif counter == 1:
                    self.assertEqual(row[0], 1)
                elif counter == 2:
                    self.assertEqual(row[0], 2)
                counter += 1
        self.assertEqual(counter, 3, "should have returned exactly three rows")

    def CheckBpo31770(self):
        """
        The interpreter shouldn't crash in case Cursor.__init__() is called
        more than once.
        """
        def callback(*args):
            pass
        con = sqlite.connect(":memory:")
        cur = sqlite.Cursor(con)
        ref = weakref.ref(cur, callback)
        cur.__init__(con)
        del cur
        # The interpreter shouldn't crash when ref is collected.
        del ref
        #support.gc_collect()

    def CheckDelIsolation_levelSegfault(self):
        with self.assertRaises(AttributeError):
            del self.con.isolation_level

    def CheckBpo37347(self):
        class Printer:
            def log(self, *args):
                return sqlite.SQLITE_OK

        for method in [self.con.set_trace_callback,
                       functools.partial(self.con.set_progress_handler, n=1),
                       self.con.set_authorizer]:
            printer_instance = Printer()
            method(printer_instance.log)
            method(printer_instance.log)  # Register twice, incref twice.
            self.con.execute('select 1')  # Triggers segfault.
            method(None)


def suite():
    regression_suite = unittest.makeSuite(RegressionTests, "Check")
    return unittest.TestSuite((
        regression_suite,
    ))

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

test_suite_list.append(suite)
"""


/*
file https://github.com/coleifer/pysqlite3/blob/0.5.0/test/transactions.py
*/

"""
#-*- coding: iso-8859-1 -*-
# pysqlite2/test/transactions.py: tests transactions
#
# Copyright (C) 2005-2007 Gerhard H�ring <gh@ghaering.de>
#
# This file is part of pysqlite.
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

import os, unittest
from . import sqlmath_dbapi2 as sqlite

def get_db_path():
    return "sqlite_testdb"

class TransactionTests(unittest.TestCase):
    def setUp(self):
        try:
            os.remove(get_db_path())
        except OSError:
            pass

        self.con1 = sqlite.connect(get_db_path(), timeout=0.1)
        self.cur1 = self.con1.cursor()

        self.con2 = sqlite.connect(get_db_path(), timeout=0.1)
        self.cur2 = self.con2.cursor()

    def tearDown(self):
        self.cur1.close()
        self.con1.close()

        self.cur2.close()
        self.con2.close()

        try:
            os.unlink(get_db_path())
        except OSError:
            pass

    def CheckDMLDoesNotAutoCommitBefore(self):
        self.cur1.execute("create table test(i)")
        self.cur1.execute("insert into test(i) values (5)")
        self.cur1.execute("create table test2(j)")
        self.cur2.execute("select i from test")
        res = self.cur2.fetchall()
        self.assertEqual(len(res), 0)

    def CheckInsertStartsTransaction(self):
        self.cur1.execute("create table test(i)")
        self.cur1.execute("insert into test(i) values (5)")
        self.cur2.execute("select i from test")
        res = self.cur2.fetchall()
        self.assertEqual(len(res), 0)

    def CheckUpdateStartsTransaction(self):
        self.cur1.execute("create table test(i)")
        self.cur1.execute("insert into test(i) values (5)")
        self.con1.commit()
        self.cur1.execute("update test set i=6")
        self.cur2.execute("select i from test")
        res = self.cur2.fetchone()[0]
        self.assertEqual(res, 5)

    def CheckDeleteStartsTransaction(self):
        self.cur1.execute("create table test(i)")
        self.cur1.execute("insert into test(i) values (5)")
        self.con1.commit()
        self.cur1.execute("delete from test")
        self.cur2.execute("select i from test")
        res = self.cur2.fetchall()
        self.assertEqual(len(res), 1)

    def CheckReplaceStartsTransaction(self):
        self.cur1.execute("create table test(i)")
        self.cur1.execute("insert into test(i) values (5)")
        self.con1.commit()
        self.cur1.execute("replace into test(i) values (6)")
        self.cur2.execute("select i from test")
        res = self.cur2.fetchall()
        self.assertEqual(len(res), 1)
        self.assertEqual(res[0][0], 5)

    def CheckToggleAutoCommit(self):
        self.cur1.execute("create table test(i)")
        self.cur1.execute("insert into test(i) values (5)")
        self.con1.isolation_level = None
        self.assertEqual(self.con1.isolation_level, None)
        self.cur2.execute("select i from test")
        res = self.cur2.fetchall()
        self.assertEqual(len(res), 1)

        self.con1.isolation_level = "DEFERRED"
        self.assertEqual(self.con1.isolation_level , "DEFERRED")
        self.cur1.execute("insert into test(i) values (5)")
        self.cur2.execute("select i from test")
        res = self.cur2.fetchall()
        self.assertEqual(len(res), 1)

    @unittest.skipIf(sqlite.sqlite_version_info < (3, 2, 2),
                     'test hangs on sqlite versions older than 3.2.2')
    def CheckRaiseTimeout(self):
        self.cur1.execute("create table test(i)")
        self.cur1.execute("insert into test(i) values (5)")
        with self.assertRaises(sqlite.OperationalError):
            self.cur2.execute("insert into test(i) values (5)")

    @unittest.skipIf(sqlite.sqlite_version_info < (3, 2, 2),
                     'test hangs on sqlite versions older than 3.2.2')
    def CheckLocking(self):
        """
        This tests the improved concurrency with pysqlite 2.3.4. You needed
        to roll back con2 before you could commit con1.
        """
        self.cur1.execute("create table test(i)")
        self.cur1.execute("insert into test(i) values (5)")
        with self.assertRaises(sqlite.OperationalError):
            self.cur2.execute("insert into test(i) values (5)")
        # NO self.con2.rollback() HERE!!!
        self.con1.commit()

    def CheckRollbackCursorConsistency(self):
        """
        Checks if cursors on the connection are set into a "reset" state
        when a rollback is done on the connection.
        """
        con = sqlite.connect(":memory:")
        cur = con.cursor()
        cur.execute("create table test(x)")
        cur.execute("insert into test(x) values (5)")
        cur.execute("select 1 union select 2 union select 3")

        con.rollback()
        with self.assertRaises(sqlite.InterfaceError):
            cur.fetchall()

class SpecialCommandTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")
        self.cur = self.con.cursor()

    def CheckDropTable(self):
        self.cur.execute("create table test(i)")
        self.cur.execute("insert into test(i) values (5)")
        self.cur.execute("drop table test")

    def CheckPragma(self):
        self.cur.execute("create table test(i)")
        self.cur.execute("insert into test(i) values (5)")
        self.cur.execute("pragma count_changes=1")

    def tearDown(self):
        self.cur.close()
        self.con.close()

class TransactionalDDL(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")

    def CheckDdlDoesNotAutostartTransaction(self):
        # For backwards compatibility reasons, DDL statements should not
        # implicitly start a transaction.
        self.con.execute("create table test(i)")
        self.con.rollback()
        result = self.con.execute("select * from test").fetchall()
        self.assertEqual(result, [])

        self.con.execute("alter table test rename to test2")
        self.con.rollback()
        result = self.con.execute("select * from test2").fetchall()
        self.assertEqual(result, [])

    def CheckImmediateTransactionalDDL(self):
        # You can achieve transactional DDL by issuing a BEGIN
        # statement manually.
        self.con.execute("begin immediate")
        self.con.execute("create table test(i)")
        self.con.rollback()
        with self.assertRaises(sqlite.OperationalError):
            self.con.execute("select * from test")

    def CheckTransactionalDDL(self):
        # You can achieve transactional DDL by issuing a BEGIN
        # statement manually.
        self.con.execute("begin")
        self.con.execute("create table test(i)")
        self.con.rollback()
        with self.assertRaises(sqlite.OperationalError):
            self.con.execute("select * from test")

    def tearDown(self):
        self.con.close()


class DMLStatementDetectionTestCase(unittest.TestCase):
    """
    https://bugs.python.org/issue36859

    Use sqlite3_stmt_readonly to determine if the statement is DML or not.
    """
    @unittest.skipIf(sqlite.sqlite_version_info < (3, 8, 3),
                     'needs sqlite 3.8.3 or newer')
    def test_dml_detection_cte(self):
        conn = sqlite.connect(':memory:')
        conn.execute('create table kv ("key" text, "val" integer)')
        self.assertFalse(conn.in_transaction)
        conn.execute('insert into kv (key, val) values (?, ?), (?, ?)',
                     ('k1', 1, 'k2', 2))
        self.assertTrue(conn.in_transaction)

        conn.commit()
        self.assertFalse(conn.in_transaction)

        rc = conn.execute('update kv set val=val + ?', (10,))
        self.assertEqual(rc.rowcount, 2)
        self.assertTrue(conn.in_transaction)
        conn.commit()
        self.assertFalse(conn.in_transaction)

        rc = conn.execute('with c(k, v) as (select key, val + ? from kv) '
                          'update kv set val=(select v from c where k=kv.key)',
                          (100,))
        self.assertEqual(rc.rowcount, 2)
        self.assertTrue(conn.in_transaction)

        curs = conn.execute('select key, val from kv order by key')
        self.assertEqual(curs.fetchall(), [('k1', 111), ('k2', 112)])

    def test_dml_detection_sql_comment(self):
        conn = sqlite.connect(':memory:')
        conn.execute('create table kv ("key" text, "val" integer)')
        conn.execute('insert into kv (key, val) values (?, ?), (?, ?)',
                     ('k1', 1, 'k2', 2))
        conn.commit()

        self.assertFalse(conn.in_transaction)
        rc = conn.execute('-- a comment\nupdate kv set val=val + ?', (10,))
        self.assertEqual(rc.rowcount, 2)
        self.assertTrue(conn.in_transaction)

        curs = conn.execute('select key, val from kv order by key')
        self.assertEqual(curs.fetchall(), [('k1', 11), ('k2', 12)])
        conn.rollback()

    def test_dml_detection_begin_exclusive(self):
        conn = sqlite.connect(':memory:')
        conn.execute('begin exclusive')
        self.assertTrue(conn.in_transaction)
        conn.execute('rollback')
        self.assertFalse(conn.in_transaction)

    def test_dml_detection_vacuum(self):
        conn = sqlite.connect(':memory:')
        conn.execute('vacuum')
        self.assertFalse(conn.in_transaction)


def suite():
    default_suite = unittest.makeSuite(TransactionTests, "Check")
    special_command_suite = unittest.makeSuite(SpecialCommandTests, "Check")
    ddl_suite = unittest.makeSuite(TransactionalDDL, "Check")
    dml_suite = unittest.makeSuite(DMLStatementDetectionTestCase)
    return unittest.TestSuite((default_suite, special_command_suite, ddl_suite, dml_suite))

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

test_suite_list.append(suite)
"""


/*
file https://github.com/coleifer/pysqlite3/blob/0.5.0/test/ttypes.py
*/

"""
#-*- coding: iso-8859-1 -*-
# pysqlite2/test/types.py: tests for type conversion and detection
#
# Copyright (C) 2005 Gerhard H�ring <gh@ghaering.de>
#
# This file is part of pysqlite.
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

import datetime
import unittest
from . import sqlmath_dbapi2 as sqlite
try:
    import zlib
except ImportError:
    zlib = None


class SqliteTypeTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")
        self.cur = self.con.cursor()
        self.cur.execute("create table test(i integer, s varchar, f number, b blob)")

    def tearDown(self):
        self.cur.close()
        self.con.close()

    def CheckString(self):
        self.cur.execute("insert into test(s) values (?)", ("�sterreich",))
        self.cur.execute("select s from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], "�sterreich")

    def CheckSmallInt(self):
        self.cur.execute("insert into test(i) values (?)", (42,))
        self.cur.execute("select i from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], 42)

    def CheckLargeInt(self):
        num = 2**40
        self.cur.execute("insert into test(i) values (?)", (num,))
        self.cur.execute("select i from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], num)

    def CheckFloat(self):
        val = 3.14
        self.cur.execute("insert into test(f) values (?)", (val,))
        self.cur.execute("select f from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], val)

    def CheckBlob(self):
        sample = b"Guglhupf"
        val = memoryview(sample)
        self.cur.execute("insert into test(b) values (?)", (val,))
        self.cur.execute("select b from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], sample)

    def CheckUnicodeExecute(self):
        self.cur.execute("select '�sterreich'")
        row = self.cur.fetchone()
        self.assertEqual(row[0], "�sterreich")

class DeclTypesTests(unittest.TestCase):
    class Foo:
        def __init__(self, _val):
            if isinstance(_val, bytes):
                # sqlite3 always calls __init__ with a bytes created from a
                # UTF-8 string when __conform__ was used to store the object.
                _val = _val.decode('utf-8')
            self.val = _val

        def __eq__(self, other):
            if not isinstance(other, DeclTypesTests.Foo):
                return NotImplemented
            return self.val == other.val

        def __conform__(self, protocol):
            if protocol is sqlite.PrepareProtocol:
                return self.val
            else:
                return None

        def __str__(self):
            return "<%s>" % self.val

    class BadConform:
        def __init__(self, exc):
            self.exc = exc
        def __conform__(self, protocol):
            raise self.exc

    def setUp(self):
        self.con = sqlite.connect(":memory:", detect_types=sqlite.PARSE_DECLTYPES)
        self.cur = self.con.cursor()
        self.cur.execute("create table test(i int, s str, f float, b bool, u unicode, foo foo, bin blob, n1 number, n2 number(5), bad bad)")

        # override float, make them always return the same number
        sqlite.converters["FLOAT"] = lambda x: 47.2

        # and implement two custom ones
        sqlite.converters["BOOL"] = lambda x: bool(int(x))
        sqlite.converters["FOO"] = DeclTypesTests.Foo
        sqlite.converters["BAD"] = DeclTypesTests.BadConform
        sqlite.converters["WRONG"] = lambda x: "WRONG"
        sqlite.converters["NUMBER"] = float

    def tearDown(self):
        del sqlite.converters["FLOAT"]
        del sqlite.converters["BOOL"]
        del sqlite.converters["FOO"]
        del sqlite.converters["BAD"]
        del sqlite.converters["WRONG"]
        del sqlite.converters["NUMBER"]
        self.cur.close()
        self.con.close()

    def CheckString(self):
        # default
        self.cur.execute("insert into test(s) values (?)", ("foo",))
        self.cur.execute('select s as "s [WRONG]" from test')
        row = self.cur.fetchone()
        self.assertEqual(row[0], "foo")

    def CheckSmallInt(self):
        # default
        self.cur.execute("insert into test(i) values (?)", (42,))
        self.cur.execute("select i from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], 42)

    def CheckLargeInt(self):
        # default
        num = 2**40
        self.cur.execute("insert into test(i) values (?)", (num,))
        self.cur.execute("select i from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], num)

    def CheckFloat(self):
        # custom
        val = 3.14
        self.cur.execute("insert into test(f) values (?)", (val,))
        self.cur.execute("select f from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], 47.2)

    def CheckBool(self):
        # custom
        self.cur.execute("insert into test(b) values (?)", (False,))
        self.cur.execute("select b from test")
        row = self.cur.fetchone()
        self.assertIs(row[0], False)

        self.cur.execute("delete from test")
        self.cur.execute("insert into test(b) values (?)", (True,))
        self.cur.execute("select b from test")
        row = self.cur.fetchone()
        self.assertIs(row[0], True)

    def CheckUnicode(self):
        # default
        val = "\xd6sterreich"
        self.cur.execute("insert into test(u) values (?)", (val,))
        self.cur.execute("select u from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], val)

    def CheckFoo(self):
        val = DeclTypesTests.Foo("bla")
        self.cur.execute("insert into test(foo) values (?)", (val,))
        self.cur.execute("select foo from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], val)

    def CheckErrorInConform(self):
        val = DeclTypesTests.BadConform(TypeError)
        with self.assertRaises(sqlite.InterfaceError):
            self.cur.execute("insert into test(bad) values (?)", (val,))
        with self.assertRaises(sqlite.InterfaceError):
            self.cur.execute("insert into test(bad) values (:val)", {"val": val})

        val = DeclTypesTests.BadConform(KeyboardInterrupt)
        with self.assertRaises(KeyboardInterrupt):
            self.cur.execute("insert into test(bad) values (?)", (val,))
        with self.assertRaises(KeyboardInterrupt):
            self.cur.execute("insert into test(bad) values (:val)", {"val": val})

    def CheckUnsupportedSeq(self):
        class Bar: pass
        val = Bar()
        with self.assertRaises(sqlite.InterfaceError):
            self.cur.execute("insert into test(f) values (?)", (val,))

    def CheckUnsupportedDict(self):
        class Bar: pass
        val = Bar()
        with self.assertRaises(sqlite.InterfaceError):
            self.cur.execute("insert into test(f) values (:val)", {"val": val})

    def CheckBlob(self):
        # default
        sample = b"Guglhupf"
        val = memoryview(sample)
        self.cur.execute("insert into test(bin) values (?)", (val,))
        self.cur.execute("select bin from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], sample)

    def CheckNumber1(self):
        self.cur.execute("insert into test(n1) values (5)")
        value = self.cur.execute("select n1 from test").fetchone()[0]
        # if the converter is not used, it's an int instead of a float
        self.assertEqual(type(value), float)

    def CheckNumber2(self):
        """Checks whether converter names are cut off at '(' characters"""
        self.cur.execute("insert into test(n2) values (5)")
        value = self.cur.execute("select n2 from test").fetchone()[0]
        # if the converter is not used, it's an int instead of a float
        self.assertEqual(type(value), float)

class ColNamesTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:", detect_types=sqlite.PARSE_COLNAMES)
        self.cur = self.con.cursor()
        self.cur.execute("create table test(x foo)")

        sqlite.converters["FOO"] = lambda x: "[%s]" % x.decode("ascii")
        sqlite.converters["BAR"] = lambda x: "<%s>" % x.decode("ascii")
        sqlite.converters["EXC"] = lambda x: 5/0
        sqlite.converters["B1B1"] = lambda x: "MARKER"

    def tearDown(self):
        del sqlite.converters["FOO"]
        del sqlite.converters["BAR"]
        del sqlite.converters["EXC"]
        del sqlite.converters["B1B1"]
        self.cur.close()
        self.con.close()

    def CheckDeclTypeNotUsed(self):
        """
        Assures that the declared type is not used when PARSE_DECLTYPES
        is not set.
        """
        self.cur.execute("insert into test(x) values (?)", ("xxx",))
        self.cur.execute("select x from test")
        val = self.cur.fetchone()[0]
        self.assertEqual(val, "xxx")

    def CheckNone(self):
        self.cur.execute("insert into test(x) values (?)", (None,))
        self.cur.execute("select x from test")
        val = self.cur.fetchone()[0]
        self.assertEqual(val, None)

    def CheckColName(self):
        self.cur.execute("insert into test(x) values (?)", ("xxx",))
        self.cur.execute('select x as "x y [bar]" from test')
        val = self.cur.fetchone()[0]
        self.assertEqual(val, "<xxx>")

        # Check if the stripping of colnames works. Everything after the first
        # whitespace should be stripped.
        self.assertEqual(self.cur.description[0][0], "x y")

    def CheckCaseInConverterName(self):
        self.cur.execute("select 'other' as \"x [b1b1]\"")
        val = self.cur.fetchone()[0]
        self.assertEqual(val, "MARKER")

    def CheckCursorDescriptionNoRow(self):
        """
        cursor.description should at least provide the column name(s), even if
        no row returned.
        """
        self.cur.execute("select * from test where 0 = 1")
        self.assertEqual(self.cur.description[0][0], "x")

    def CheckCursorDescriptionInsert(self):
        self.cur.execute("insert into test values (1)")
        self.assertIsNone(self.cur.description)


@unittest.skipIf(sqlite.sqlite_version_info < (3, 8, 3), "CTEs not supported")
class CommonTableExpressionTests(unittest.TestCase):

    def setUp(self):
        self.con = sqlite.connect(":memory:")
        self.cur = self.con.cursor()
        self.cur.execute("create table test(x foo)")

    def tearDown(self):
        self.cur.close()
        self.con.close()

    def CheckCursorDescriptionCTESimple(self):
        self.cur.execute("with one as (select 1) select * from one")
        self.assertIsNotNone(self.cur.description)
        self.assertEqual(self.cur.description[0][0], "1")

    def CheckCursorDescriptionCTESMultipleColumns(self):
        self.cur.execute("insert into test values(1)")
        self.cur.execute("insert into test values(2)")
        self.cur.execute("with testCTE as (select * from test) select * from testCTE")
        self.assertIsNotNone(self.cur.description)
        self.assertEqual(self.cur.description[0][0], "x")

    def CheckCursorDescriptionCTE(self):
        self.cur.execute("insert into test values (1)")
        self.cur.execute("with bar as (select * from test) select * from test where x = 1")
        self.assertIsNotNone(self.cur.description)
        self.assertEqual(self.cur.description[0][0], "x")
        self.cur.execute("with bar as (select * from test) select * from test where x = 2")
        self.assertIsNotNone(self.cur.description)
        self.assertEqual(self.cur.description[0][0], "x")


class ObjectAdaptationTests(unittest.TestCase):
    def cast(obj):
        return float(obj)
    cast = staticmethod(cast)

    def setUp(self):
        self.con = sqlite.connect(":memory:")
        try:
            del sqlite.adapters[int]
        except:
            pass
        sqlite.register_adapter(int, ObjectAdaptationTests.cast)
        self.cur = self.con.cursor()

    def tearDown(self):
        del sqlite.adapters[(int, sqlite.PrepareProtocol)]
        self.cur.close()
        self.con.close()

    def CheckCasterIsUsed(self):
        self.cur.execute("select ?", (4,))
        val = self.cur.fetchone()[0]
        self.assertEqual(type(val), float)

@unittest.skipUnless(zlib, "requires zlib")
class BinaryConverterTests(unittest.TestCase):
    def convert(s):
        return zlib.decompress(s)
    convert = staticmethod(convert)

    def setUp(self):
        self.con = sqlite.connect(":memory:", detect_types=sqlite.PARSE_COLNAMES)
        sqlite.register_converter("bin", BinaryConverterTests.convert)

    def tearDown(self):
        self.con.close()

    def CheckBinaryInputForConverter(self):
        testdata = b"abcdefg" * 10
        result = self.con.execute('select ? as "x [bin]"', (memoryview(zlib.compress(testdata)),)).fetchone()[0]
        self.assertEqual(testdata, result)

class DateTimeTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:", detect_types=sqlite.PARSE_DECLTYPES)
        self.cur = self.con.cursor()
        self.cur.execute("create table test(d date, ts timestamp)")

    def tearDown(self):
        self.cur.close()
        self.con.close()

    def CheckSqliteDate(self):
        d = sqlite.Date(2004, 2, 14)
        self.cur.execute("insert into test(d) values (?)", (d,))
        self.cur.execute("select d from test")
        d2 = self.cur.fetchone()[0]
        self.assertEqual(d, d2)

    def CheckSqliteTimestamp(self):
        ts = sqlite.Timestamp(2004, 2, 14, 7, 15, 0)
        self.cur.execute("insert into test(ts) values (?)", (ts,))
        self.cur.execute("select ts from test")
        ts2 = self.cur.fetchone()[0]
        self.assertEqual(ts, ts2)

    @unittest.skipIf(sqlite.sqlite_version_info < (3, 1),
                     'the date functions are available on 3.1 or later')
    def CheckSqlTimestamp(self):
        now = datetime.datetime.utcnow()
        self.cur.execute("insert into test(ts) values (current_timestamp)")
        self.cur.execute("select ts from test")
        ts = self.cur.fetchone()[0]
        self.assertEqual(type(ts), datetime.datetime)
        self.assertEqual(ts.year, now.year)

    def CheckDateTimeSubSeconds(self):
        ts = sqlite.Timestamp(2004, 2, 14, 7, 15, 0, 500000)
        self.cur.execute("insert into test(ts) values (?)", (ts,))
        self.cur.execute("select ts from test")
        ts2 = self.cur.fetchone()[0]
        self.assertEqual(ts, ts2)

    def CheckDateTimeSubSecondsFloatingPoint(self):
        ts = sqlite.Timestamp(2004, 2, 14, 7, 15, 0, 510241)
        self.cur.execute("insert into test(ts) values (?)", (ts,))
        self.cur.execute("select ts from test")
        ts2 = self.cur.fetchone()[0]
        self.assertEqual(ts, ts2)

def suite():
    sqlite_type_suite = unittest.makeSuite(SqliteTypeTests, "Check")
    decltypes_type_suite = unittest.makeSuite(DeclTypesTests, "Check")
    colnames_type_suite = unittest.makeSuite(ColNamesTests, "Check")
    adaptation_suite = unittest.makeSuite(ObjectAdaptationTests, "Check")
    bin_suite = unittest.makeSuite(BinaryConverterTests, "Check")
    date_suite = unittest.makeSuite(DateTimeTests, "Check")
    cte_suite = unittest.makeSuite(CommonTableExpressionTests, "Check")
    return unittest.TestSuite((sqlite_type_suite, decltypes_type_suite, colnames_type_suite, adaptation_suite, bin_suite, date_suite, cte_suite))

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

test_suite_list.append(suite)
"""


/*
file https://github.com/coleifer/pysqlite3/blob/0.5.0/test/userfunctions.py
*/

"""
#-*- coding: iso-8859-1 -*-
# pysqlite2/test/userfunctions.py: tests for user-defined functions and
#                                  aggregates.
#
# Copyright (C) 2005-2007 Gerhard H�ring <gh@ghaering.de>
#
# This file is part of pysqlite.
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.

import unittest
import unittest.mock
from . import sqlmath_dbapi2 as sqlite

def func_returntext():
    return "foo"
def func_returnunicode():
    return "bar"
def func_returnint():
    return 42
def func_returnfloat():
    return 3.14
def func_returnnull():
    return None
def func_returnblob():
    return b"blob"
def func_returnlonglong():
    return 1<<31
def func_raiseexception():
    5/0

def func_isstring(v):
    return type(v) is str
def func_isint(v):
    return type(v) is int
def func_isfloat(v):
    return type(v) is float
def func_isnone(v):
    return type(v) is type(None)
def func_isblob(v):
    return isinstance(v, (bytes, memoryview))
def func_islonglong(v):
    return isinstance(v, int) and v >= 1<<31

def func(*args):
    return len(args)

class AggrNoStep:
    def __init__(self):
        pass

    def finalize(self):
        return 1

class AggrNoFinalize:
    def __init__(self):
        pass

    def step(self, x):
        pass

class AggrExceptionInInit:
    def __init__(self):
        5/0

    def step(self, x):
        pass

    def finalize(self):
        pass

class AggrExceptionInStep:
    def __init__(self):
        pass

    def step(self, x):
        5/0

    def finalize(self):
        return 42

class AggrExceptionInFinalize:
    def __init__(self):
        pass

    def step(self, x):
        pass

    def finalize(self):
        5/0

class AggrCheckType:
    def __init__(self):
        self.val = None

    def step(self, whichType, val):
        theType = {"str": str, "int": int, "float": float, "None": type(None),
                   "blob": bytes}
        self.val = int(theType[whichType] is type(val))

    def finalize(self):
        return self.val

class AggrCheckTypes:
    def __init__(self):
        self.val = 0

    def step(self, whichType, *vals):
        theType = {"str": str, "int": int, "float": float, "None": type(None),
                   "blob": bytes}
        for val in vals:
            self.val += int(theType[whichType] is type(val))

    def finalize(self):
        return self.val

class AggrSum:
    def __init__(self):
        self.val = 0.0

    def step(self, val):
        self.val += val

    def finalize(self):
        return self.val

class FunctionTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")

        self.con.create_function("returntext", 0, func_returntext)
        self.con.create_function("returnunicode", 0, func_returnunicode)
        self.con.create_function("returnint", 0, func_returnint)
        self.con.create_function("returnfloat", 0, func_returnfloat)
        self.con.create_function("returnnull", 0, func_returnnull)
        self.con.create_function("returnblob", 0, func_returnblob)
        self.con.create_function("returnlonglong", 0, func_returnlonglong)
        self.con.create_function("raiseexception", 0, func_raiseexception)

        self.con.create_function("isstring", 1, func_isstring)
        self.con.create_function("isint", 1, func_isint)
        self.con.create_function("isfloat", 1, func_isfloat)
        self.con.create_function("isnone", 1, func_isnone)
        self.con.create_function("isblob", 1, func_isblob)
        self.con.create_function("islonglong", 1, func_islonglong)
        self.con.create_function("spam", -1, func)

    def tearDown(self):
        self.con.close()

    def CheckFuncErrorOnCreate(self):
        with self.assertRaises(sqlite.OperationalError):
            self.con.create_function("bla", -100, lambda x: 2*x)

    def CheckFuncRefCount(self):
        def getfunc():
            def f():
                return 1
            return f
        f = getfunc()
        globals()["foo"] = f
        # self.con.create_function("reftest", 0, getfunc())
        self.con.create_function("reftest", 0, f)
        cur = self.con.cursor()
        cur.execute("select reftest()")

    def CheckFuncReturnText(self):
        cur = self.con.cursor()
        cur.execute("select returntext()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), str)
        self.assertEqual(val, "foo")

    def CheckFuncReturnUnicode(self):
        cur = self.con.cursor()
        cur.execute("select returnunicode()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), str)
        self.assertEqual(val, "bar")

    def CheckFuncReturnInt(self):
        cur = self.con.cursor()
        cur.execute("select returnint()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), int)
        self.assertEqual(val, 42)

    def CheckFuncReturnFloat(self):
        cur = self.con.cursor()
        cur.execute("select returnfloat()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), float)
        if val < 3.139 or val > 3.141:
            self.fail("wrong value")

    def CheckFuncReturnNull(self):
        cur = self.con.cursor()
        cur.execute("select returnnull()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), type(None))
        self.assertEqual(val, None)

    def CheckFuncReturnBlob(self):
        cur = self.con.cursor()
        cur.execute("select returnblob()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), bytes)
        self.assertEqual(val, b"blob")

    def CheckFuncReturnLongLong(self):
        cur = self.con.cursor()
        cur.execute("select returnlonglong()")
        val = cur.fetchone()[0]
        self.assertEqual(val, 1<<31)

    def CheckFuncException(self):
        cur = self.con.cursor()
        with self.assertRaises(sqlite.OperationalError) as cm:
            cur.execute("select raiseexception()")
            cur.fetchone()
        self.assertEqual(str(cm.exception), 'user-defined function raised exception')

    def CheckParamString(self):
        cur = self.con.cursor()
        cur.execute("select isstring(?)", ("foo",))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def CheckParamInt(self):
        cur = self.con.cursor()
        cur.execute("select isint(?)", (42,))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def CheckParamFloat(self):
        cur = self.con.cursor()
        cur.execute("select isfloat(?)", (3.14,))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def CheckParamNone(self):
        cur = self.con.cursor()
        cur.execute("select isnone(?)", (None,))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def CheckParamBlob(self):
        cur = self.con.cursor()
        cur.execute("select isblob(?)", (memoryview(b"blob"),))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def CheckParamLongLong(self):
        cur = self.con.cursor()
        cur.execute("select islonglong(?)", (1<<42,))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def CheckAnyArguments(self):
        cur = self.con.cursor()
        cur.execute("select spam(?, ?)", (1, 2))
        val = cur.fetchone()[0]
        self.assertEqual(val, 2)

    def CheckFuncNonDeterministic(self):
        mock = unittest.mock.Mock(return_value=None)
        self.con.create_function("deterministic", 0, mock, deterministic=False)
        self.con.execute("select deterministic() = deterministic()")
        self.assertEqual(mock.call_count, 2)

    @unittest.skipIf(sqlite.sqlite_version_info < (3, 8, 3), "deterministic parameter not supported")
    def CheckFuncDeterministic(self):
        mock = unittest.mock.Mock(return_value=None)
        self.con.create_function("deterministic", 0, mock, True)
        self.con.execute("select 1 where deterministic() AND deterministic()")
        self.assertEqual(mock.call_count, 1)

    @unittest.skipIf(sqlite.sqlite_version_info >= (3, 8, 3), "SQLite < 3.8.3 needed")
    def CheckFuncDeterministicNotSupported(self):
        with self.assertRaises(sqlite.NotSupportedError):
            self.con.create_function("deterministic", 0, int, deterministic=True)


class AggregateTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")
        cur = self.con.cursor()
        cur.execute("""
            create table test(
                t text,
                i integer,
                f float,
                n,
                b blob
                )
            """)
        cur.execute("insert into test(t, i, f, n, b) values (?, ?, ?, ?, ?)",
            ("foo", 5, 3.14, None, memoryview(b"blob"),))

        self.con.create_aggregate("nostep", 1, AggrNoStep)
        self.con.create_aggregate("nofinalize", 1, AggrNoFinalize)
        self.con.create_aggregate("excInit", 1, AggrExceptionInInit)
        self.con.create_aggregate("excStep", 1, AggrExceptionInStep)
        self.con.create_aggregate("excFinalize", 1, AggrExceptionInFinalize)
        self.con.create_aggregate("checkType", 2, AggrCheckType)
        self.con.create_aggregate("checkTypes", -1, AggrCheckTypes)
        self.con.create_aggregate("mysum", 1, AggrSum)

    def tearDown(self):
        #self.cur.close()
        #self.con.close()
        pass

    def CheckAggrErrorOnCreate(self):
        with self.assertRaises(sqlite.OperationalError):
            self.con.create_function("bla", -100, AggrSum)

    def CheckAggrNoStep(self):
        cur = self.con.cursor()
        with self.assertRaises(AttributeError) as cm:
            cur.execute("select nostep(t) from test")
        self.assertEqual(str(cm.exception), "'AggrNoStep' object has no attribute 'step'")

    def CheckAggrNoFinalize(self):
        cur = self.con.cursor()
        with self.assertRaises(sqlite.OperationalError) as cm:
            cur.execute("select nofinalize(t) from test")
            val = cur.fetchone()[0]
        self.assertEqual(str(cm.exception), "user-defined aggregate's 'finalize' method raised error")

    def CheckAggrExceptionInInit(self):
        cur = self.con.cursor()
        with self.assertRaises(sqlite.OperationalError) as cm:
            cur.execute("select excInit(t) from test")
            val = cur.fetchone()[0]
        self.assertEqual(str(cm.exception), "user-defined aggregate's '__init__' method raised error")

    def CheckAggrExceptionInStep(self):
        cur = self.con.cursor()
        with self.assertRaises(sqlite.OperationalError) as cm:
            cur.execute("select excStep(t) from test")
            val = cur.fetchone()[0]
        self.assertEqual(str(cm.exception), "user-defined aggregate's 'step' method raised error")

    def CheckAggrExceptionInFinalize(self):
        cur = self.con.cursor()
        with self.assertRaises(sqlite.OperationalError) as cm:
            cur.execute("select excFinalize(t) from test")
            val = cur.fetchone()[0]
        self.assertEqual(str(cm.exception), "user-defined aggregate's 'finalize' method raised error")

    def CheckAggrCheckParamStr(self):
        cur = self.con.cursor()
        cur.execute("select checkType('str', ?)", ("foo",))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def CheckAggrCheckParamInt(self):
        cur = self.con.cursor()
        cur.execute("select checkType('int', ?)", (42,))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def CheckAggrCheckParamsInt(self):
        cur = self.con.cursor()
        cur.execute("select checkTypes('int', ?, ?)", (42, 24))
        val = cur.fetchone()[0]
        self.assertEqual(val, 2)

    def CheckAggrCheckParamFloat(self):
        cur = self.con.cursor()
        cur.execute("select checkType('float', ?)", (3.14,))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def CheckAggrCheckParamNone(self):
        cur = self.con.cursor()
        cur.execute("select checkType('None', ?)", (None,))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def CheckAggrCheckParamBlob(self):
        cur = self.con.cursor()
        cur.execute("select checkType('blob', ?)", (memoryview(b"blob"),))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def CheckAggrCheckAggrSum(self):
        cur = self.con.cursor()
        cur.execute("delete from test")
        cur.executemany("insert into test(i) values (?)", [(10,), (20,), (30,)])
        cur.execute("select mysum(i) from test")
        val = cur.fetchone()[0]
        self.assertEqual(val, 60)

    def CheckAggrNoMatch(self):
        cur = self.con.execute('select mysum(i) from (select 1 as i) where i == 0')
        val = cur.fetchone()[0]
        self.assertIsNone(val)


@unittest.skipIf(sqlite.sqlite_version_info < (3, 25, 0),
                 'requires sqlite with window-function support')
class WindowFunctionTests(unittest.TestCase):
    def setUp(self):
        self.conn = sqlite.connect(":memory:")
        self.conn.execute('create table sample (id integer primary key, '
                          'counter integer, value real);')
        self.conn.execute('insert into sample (counter, value) values '
                          '(?,?),(?,?),(?,?),(?,?),(?,?)', (
                              1, 10.,
                              1, 20.,
                              2, 1.,
                              2, 2.,
                              3, 100.))

    def test_user_defined_window_function(self):
        class MySum(object):
            def __init__(self): self._value = 0
            def step(self, value): self._value += value
            def inverse(self, value): self._value -= value
            def value(self): return self._value
            def finalize(self): return self._value

        self.conn.create_window_function('mysum', -1, MySum)

        q = ('select counter, value, mysum(value) over (partition by counter) '
             'from sample order by id')
        self.assertEqual(self.conn.execute(q).fetchall(), [
            (1, 10., 30.), (1, 20., 30.),
            (2, 1., 3.), (2, 2., 3.),
            (3, 100., 100.)])


class AuthorizerTests(unittest.TestCase):
    @staticmethod
    def authorizer_cb(action, arg1, arg2, dbname, source):
        if action != sqlite.SQLITE_SELECT:
            return sqlite.SQLITE_DENY
        if arg2 == 'c2' or arg1 == 't2':
            return sqlite.SQLITE_DENY
        return sqlite.SQLITE_OK

    def setUp(self):
        self.con = sqlite.connect(":memory:")
        self.con.executescript("""
            create table t1 (c1, c2);
            create table t2 (c1, c2);
            insert into t1 (c1, c2) values (1, 2);
            insert into t2 (c1, c2) values (4, 5);
            """)

        # For our security test:
        self.con.execute("select c2 from t2")

        self.con.set_authorizer(self.authorizer_cb)

    def tearDown(self):
        pass

    def test_table_access(self):
        with self.assertRaises(sqlite.DatabaseError) as cm:
            self.con.execute("select * from t2")
        self.assertIn('prohibited', str(cm.exception))

    def test_column_access(self):
        with self.assertRaises(sqlite.DatabaseError) as cm:
            self.con.execute("select c2 from t1")
        self.assertIn('prohibited', str(cm.exception))

    def test_clear_authorizer(self):
        self.con.set_authorizer(None)
        self.con.execute('select * from t2')
        self.con.execute('select c2 from t1')

class AuthorizerRaiseExceptionTests(AuthorizerTests):
    @staticmethod
    def authorizer_cb(action, arg1, arg2, dbname, source):
        if action != sqlite.SQLITE_SELECT:
            raise ValueError
        if arg2 == 'c2' or arg1 == 't2':
            raise ValueError
        return sqlite.SQLITE_OK

class AuthorizerIllegalTypeTests(AuthorizerTests):
    @staticmethod
    def authorizer_cb(action, arg1, arg2, dbname, source):
        if action != sqlite.SQLITE_SELECT:
            return 0.0
        if arg2 == 'c2' or arg1 == 't2':
            return 0.0
        return sqlite.SQLITE_OK

class AuthorizerLargeIntegerTests(AuthorizerTests):
    @staticmethod
    def authorizer_cb(action, arg1, arg2, dbname, source):
        if action != sqlite.SQLITE_SELECT:
            return 2**32
        if arg2 == 'c2' or arg1 == 't2':
            return 2**32
        return sqlite.SQLITE_OK


def suite():
    function_suite = unittest.makeSuite(FunctionTests, "Check")
    aggregate_suite = unittest.makeSuite(AggregateTests, "Check")
    window_suite = unittest.makeSuite(WindowFunctionTests)
    authorizer_suite = unittest.makeSuite(AuthorizerTests)
    return unittest.TestSuite((
            function_suite,
            aggregate_suite,
            window_suite,
            authorizer_suite,
            unittest.makeSuite(AuthorizerRaiseExceptionTests),
            unittest.makeSuite(AuthorizerIllegalTypeTests),
            unittest.makeSuite(AuthorizerLargeIntegerTests),
        ))

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

test_suite_list.append(suite)
"""


/*
file none
*/
/*jslint-enable*/


"""
