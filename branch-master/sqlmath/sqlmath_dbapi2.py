test_suite_list = []
"""
/*jslint-disable*/
/*
shRollupFetch
{
    "fetchList": [
        {
            "comment": true,
            "url": "https://github.com/python/cpython/blob/v3.10.11/LICENSE"
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
            "url": "https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/dbapi2.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/backup.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/dbapi.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/dump.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/factory.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/hooks.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/regression.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/types.py"
        },
        {
            "footer": "\n\"\"\"\n",
            "header": "\n\"\"\"\n",
            "url": "https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/userfunctions.py"
        }
    ],
    "replaceList": [
        {
            "aa": "@unittest.skipIf.*?\\n *?def test_shared_cache_deprecated\\([\\S\\s]*?    def ",
            "bb": "def ",
            "flags": "g",
            "substr": ""
        },
        {
            "aa": "\\b_sqlite3\\b",
            "bb": "_sqlmath",
            "flags": "g",
            "substr": ""
        },
        {
            "aa": "\\nif __name__ == .__main__.:\\n    .*?test.*?\\n",
            "bb": "\n# hack-python - append test-suite\ntest_suite_list.append(suite)\n",
            "flags": "g",
            "substr": ""
        },
        {
            "aa": "def test_optimized_unicode\\([\\S\\s]*?    def ",
            "bb": "def ",
            "flags": "g",
            "substr": ""
        },
        {
            "aa": "import sqlite3 as sqlite",
            "bb": "from . import sqlmath_dbapi2 as sqlite",
            "flags": "g",
            "substr": ""
        }
    ]
}
-            import sqlite3
+# hack-python - fix import
+            import sqlmath as sqlite3

-    from _sqlmath import *
+# hack-python - fix import
+    from ._sqlmath import *
*/


/*
repo https://github.com/python/cpython/tree/v3.10.11
committed 2023-04-04T21:57:15Z
*/


/*
file https://github.com/python/cpython/blob/v3.10.11/LICENSE
*/
/*
A. HISTORY OF THE SOFTWARE
==========================

Python was created in the early 1990s by Guido van Rossum at Stichting
Mathematisch Centrum (CWI, see https://www.cwi.nl) in the Netherlands
as a successor of a language called ABC.  Guido remains Python's
principal author, although it includes many contributions from others.

In 1995, Guido continued his work on Python at the Corporation for
National Research Initiatives (CNRI, see https://www.cnri.reston.va.us)
in Reston, Virginia where he released several versions of the
software.

In May 2000, Guido and the Python core development team moved to
BeOpen.com to form the BeOpen PythonLabs team.  In October of the same
year, the PythonLabs team moved to Digital Creations, which became
Zope Corporation.  In 2001, the Python Software Foundation (PSF, see
https://www.python.org/psf/) was formed, a non-profit organization
created specifically to own Python-related Intellectual Property.
Zope Corporation was a sponsoring member of the PSF.

All Python releases are Open Source (see https://opensource.org for
the Open Source Definition).  Historically, most, but not all, Python
releases have also been GPL-compatible; the table below summarizes
the various releases.

    Release         Derived     Year        Owner       GPL-
                    from                                compatible? (1)

    0.9.0 thru 1.2              1991-1995   CWI         yes
    1.3 thru 1.5.2  1.2         1995-1999   CNRI        yes
    1.6             1.5.2       2000        CNRI        no
    2.0             1.6         2000        BeOpen.com  no
    1.6.1           1.6         2001        CNRI        yes (2)
    2.1             2.0+1.6.1   2001        PSF         no
    2.0.1           2.0+1.6.1   2001        PSF         yes
    2.1.1           2.1+2.0.1   2001        PSF         yes
    2.1.2           2.1.1       2002        PSF         yes
    2.1.3           2.1.2       2002        PSF         yes
    2.2 and above   2.1.1       2001-now    PSF         yes

Footnotes:

(1) GPL-compatible doesn't mean that we're distributing Python under
    the GPL.  All Python licenses, unlike the GPL, let you distribute
    a modified version without making your changes open source.  The
    GPL-compatible licenses make it possible to combine Python with
    other software that is released under the GPL; the others don't.

(2) According to Richard Stallman, 1.6.1 is not GPL-compatible,
    because its license has a choice of law clause.  According to
    CNRI, however, Stallman's lawyer has told CNRI's lawyer that 1.6.1
    is "not incompatible" with the GPL.

Thanks to the many outside volunteers who have worked under Guido's
direction to make these releases possible.


B. TERMS AND CONDITIONS FOR ACCESSING OR OTHERWISE USING PYTHON
===============================================================

Python software and documentation are licensed under the
Python Software Foundation License Version 2.

Starting with Python 3.8.6, examples, recipes, and other code in
the documentation are dual licensed under the PSF License Version 2
and the Zero-Clause BSD license.

Some software incorporated into Python is under different licenses.
The licenses are listed with code falling under that license.


PYTHON SOFTWARE FOUNDATION LICENSE VERSION 2
--------------------------------------------

1. This LICENSE AGREEMENT is between the Python Software Foundation
("PSF"), and the Individual or Organization ("Licensee") accessing and
otherwise using this software ("Python") in source or binary form and
its associated documentation.

2. Subject to the terms and conditions of this License Agreement, PSF hereby
grants Licensee a nonexclusive, royalty-free, world-wide license to reproduce,
analyze, test, perform and/or display publicly, prepare derivative works,
distribute, and otherwise use Python alone or in any derivative version,
provided, however, that PSF's License Agreement and PSF's notice of copyright,
i.e., "Copyright (c) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Python Software Foundation;
All Rights Reserved" are retained in Python alone or in any derivative version
prepared by Licensee.

3. In the event Licensee prepares a derivative work that is based on
or incorporates Python or any part thereof, and wants to make
the derivative work available to others as provided herein, then
Licensee hereby agrees to include in any such work a brief summary of
the changes made to Python.

4. PSF is making Python available to Licensee on an "AS IS"
basis.  PSF MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR
IMPLIED.  BY WAY OF EXAMPLE, BUT NOT LIMITATION, PSF MAKES NO AND
DISCLAIMS ANY REPRESENTATION OR WARRANTY OF MERCHANTABILITY OR FITNESS
FOR ANY PARTICULAR PURPOSE OR THAT THE USE OF PYTHON WILL NOT
INFRINGE ANY THIRD PARTY RIGHTS.

5. PSF SHALL NOT BE LIABLE TO LICENSEE OR ANY OTHER USERS OF PYTHON
FOR ANY INCIDENTAL, SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS AS
A RESULT OF MODIFYING, DISTRIBUTING, OR OTHERWISE USING PYTHON,
OR ANY DERIVATIVE THEREOF, EVEN IF ADVISED OF THE POSSIBILITY THEREOF.

6. This License Agreement will automatically terminate upon a material
breach of its terms and conditions.

7. Nothing in this License Agreement shall be deemed to create any
relationship of agency, partnership, or joint venture between PSF and
Licensee.  This License Agreement does not grant permission to use PSF
trademarks or trade name in a trademark sense to endorse or promote
products or services of Licensee, or any third party.

8. By copying, installing or otherwise using Python, Licensee
agrees to be bound by the terms and conditions of this License
Agreement.


BEOPEN.COM LICENSE AGREEMENT FOR PYTHON 2.0
-------------------------------------------

BEOPEN PYTHON OPEN SOURCE LICENSE AGREEMENT VERSION 1

1. This LICENSE AGREEMENT is between BeOpen.com ("BeOpen"), having an
office at 160 Saratoga Avenue, Santa Clara, CA 95051, and the
Individual or Organization ("Licensee") accessing and otherwise using
this software in source or binary form and its associated
documentation ("the Software").

2. Subject to the terms and conditions of this BeOpen Python License
Agreement, BeOpen hereby grants Licensee a non-exclusive,
royalty-free, world-wide license to reproduce, analyze, test, perform
and/or display publicly, prepare derivative works, distribute, and
otherwise use the Software alone or in any derivative version,
provided, however, that the BeOpen Python License is retained in the
Software, alone or in any derivative version prepared by Licensee.

3. BeOpen is making the Software available to Licensee on an "AS IS"
basis.  BEOPEN MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR
IMPLIED.  BY WAY OF EXAMPLE, BUT NOT LIMITATION, BEOPEN MAKES NO AND
DISCLAIMS ANY REPRESENTATION OR WARRANTY OF MERCHANTABILITY OR FITNESS
FOR ANY PARTICULAR PURPOSE OR THAT THE USE OF THE SOFTWARE WILL NOT
INFRINGE ANY THIRD PARTY RIGHTS.

4. BEOPEN SHALL NOT BE LIABLE TO LICENSEE OR ANY OTHER USERS OF THE
SOFTWARE FOR ANY INCIDENTAL, SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS
AS A RESULT OF USING, MODIFYING OR DISTRIBUTING THE SOFTWARE, OR ANY
DERIVATIVE THEREOF, EVEN IF ADVISED OF THE POSSIBILITY THEREOF.

5. This License Agreement will automatically terminate upon a material
breach of its terms and conditions.

6. This License Agreement shall be governed by and interpreted in all
respects by the law of the State of California, excluding conflict of
law provisions.  Nothing in this License Agreement shall be deemed to
create any relationship of agency, partnership, or joint venture
between BeOpen and Licensee.  This License Agreement does not grant
permission to use BeOpen trademarks or trade names in a trademark
sense to endorse or promote products or services of Licensee, or any
third party.  As an exception, the "BeOpen Python" logos available at
http://www.pythonlabs.com/logos.html may be used according to the
permissions granted on that web page.

7. By copying, installing or otherwise using the software, Licensee
agrees to be bound by the terms and conditions of this License
Agreement.


CNRI LICENSE AGREEMENT FOR PYTHON 1.6.1
---------------------------------------

1. This LICENSE AGREEMENT is between the Corporation for National
Research Initiatives, having an office at 1895 Preston White Drive,
Reston, VA 20191 ("CNRI"), and the Individual or Organization
("Licensee") accessing and otherwise using Python 1.6.1 software in
source or binary form and its associated documentation.

2. Subject to the terms and conditions of this License Agreement, CNRI
hereby grants Licensee a nonexclusive, royalty-free, world-wide
license to reproduce, analyze, test, perform and/or display publicly,
prepare derivative works, distribute, and otherwise use Python 1.6.1
alone or in any derivative version, provided, however, that CNRI's
License Agreement and CNRI's notice of copyright, i.e., "Copyright (c)
1995-2001 Corporation for National Research Initiatives; All Rights
Reserved" are retained in Python 1.6.1 alone or in any derivative
version prepared by Licensee.  Alternately, in lieu of CNRI's License
Agreement, Licensee may substitute the following text (omitting the
quotes): "Python 1.6.1 is made available subject to the terms and
conditions in CNRI's License Agreement.  This Agreement together with
Python 1.6.1 may be located on the internet using the following
unique, persistent identifier (known as a handle): 1895.22/1013.  This
Agreement may also be obtained from a proxy server on the internet
using the following URL: http://hdl.handle.net/1895.22/1013".

3. In the event Licensee prepares a derivative work that is based on
or incorporates Python 1.6.1 or any part thereof, and wants to make
the derivative work available to others as provided herein, then
Licensee hereby agrees to include in any such work a brief summary of
the changes made to Python 1.6.1.

4. CNRI is making Python 1.6.1 available to Licensee on an "AS IS"
basis.  CNRI MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR
IMPLIED.  BY WAY OF EXAMPLE, BUT NOT LIMITATION, CNRI MAKES NO AND
DISCLAIMS ANY REPRESENTATION OR WARRANTY OF MERCHANTABILITY OR FITNESS
FOR ANY PARTICULAR PURPOSE OR THAT THE USE OF PYTHON 1.6.1 WILL NOT
INFRINGE ANY THIRD PARTY RIGHTS.

5. CNRI SHALL NOT BE LIABLE TO LICENSEE OR ANY OTHER USERS OF PYTHON
1.6.1 FOR ANY INCIDENTAL, SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS AS
A RESULT OF MODIFYING, DISTRIBUTING, OR OTHERWISE USING PYTHON 1.6.1,
OR ANY DERIVATIVE THEREOF, EVEN IF ADVISED OF THE POSSIBILITY THEREOF.

6. This License Agreement will automatically terminate upon a material
breach of its terms and conditions.

7. This License Agreement shall be governed by the federal
intellectual property law of the United States, including without
limitation the federal copyright law, and, to the extent such
U.S. federal law does not apply, by the law of the Commonwealth of
Virginia, excluding Virginia's conflict of law provisions.
Notwithstanding the foregoing, with regard to derivative works based
on Python 1.6.1 that incorporate non-separable material that was
previously distributed under the GNU General Public License (GPL), the
law of the Commonwealth of Virginia shall govern this License
Agreement only as to issues arising under or with respect to
Paragraphs 4, 5, and 7 of this License Agreement.  Nothing in this
License Agreement shall be deemed to create any relationship of
agency, partnership, or joint venture between CNRI and Licensee.  This
License Agreement does not grant permission to use CNRI trademarks or
trade name in a trademark sense to endorse or promote products or
services of Licensee, or any third party.

8. By clicking on the "ACCEPT" button where indicated, or by copying,
installing or otherwise using Python 1.6.1, Licensee agrees to be
bound by the terms and conditions of this License Agreement.

        ACCEPT


CWI LICENSE AGREEMENT FOR PYTHON 0.9.0 THROUGH 1.2
--------------------------------------------------

Copyright (c) 1991 - 1995, Stichting Mathematisch Centrum Amsterdam,
The Netherlands.  All rights reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Stichting Mathematisch
Centrum or CWI not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

STICHTING MATHEMATISCH CENTRUM DISCLAIMS ALL WARRANTIES WITH REGARD TO
THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS, IN NO EVENT SHALL STICHTING MATHEMATISCH CENTRUM BE LIABLE
FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

ZERO-CLAUSE BSD LICENSE FOR CODE IN THE PYTHON DOCUMENTATION
----------------------------------------------------------------------

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
*/


/*
file https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/dbapi2.py
*/

"""
if __name__ != "__main__":
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

# hack-python - fix import
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

    # bpo-24464: enable_shared_cache was deprecated in Python 3.10.  It's
    # scheduled for removal in Python 3.12.
    def enable_shared_cache(enable):
        from _sqlmath import enable_shared_cache as _old_enable_shared_cache
        import warnings
        msg = (
            "enable_shared_cache is deprecated and will be removed in Python 3.12. "
            "Shared cache is strongly discouraged by the SQLite 3 documentation. "
            "If shared cache must be used, open the database in URI mode using"
            "the cache=shared query parameter."
        )
        warnings.warn(msg, DeprecationWarning, stacklevel=2)
        return _old_enable_shared_cache(enable)

    # Clean up namespace

    del(register_adapters_and_converters)
"""


/*
file https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/backup.py
*/

"""
from . import sqlmath_dbapi2 as sqlite
import unittest


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

    def test_bad_target(self):
        with self.assertRaises(TypeError):
            self.cx.backup(None)
        with self.assertRaises(TypeError):
            self.cx.backup()

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

    def test_bad_source_closed_connection(self):
        bck = sqlite.connect(':memory:')
        source = sqlite.connect(":memory:")
        source.close()
        with self.assertRaises(sqlite.ProgrammingError):
            source.backup(bck)

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
        self.assertIn("unknown database", str(cm.exception))

        self.cx.execute("ATTACH DATABASE ':memory:' AS attached_db")
        self.cx.execute('CREATE TABLE attached_db.foo (key INTEGER)')
        self.cx.executemany('INSERT INTO attached_db.foo (key) VALUES (?)', [(3,), (4,)])
        self.cx.commit()
        with sqlite.connect(':memory:') as bck:
            self.cx.backup(bck, name='attached_db')
            self.verify_backup(bck)


def suite():
    return unittest.TestLoader().loadTestsFromTestCase(BackupTests)

# hack-python - append test-suite
test_suite_list.append(suite)
"""


/*
file https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/dbapi.py
*/

"""
# pysqlite2/test/dbapi.py: tests for DB-API compliance
#
# Copyright (C) 2004-2010 Gerhard Häring <gh@ghaering.de>
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

import subprocess
import threading
import unittest
from . import sqlmath_dbapi2 as sqlite
import sys

from test.support import check_disallow_instantiation, SHORT_TIMEOUT
from test.support.os_helper import TESTFN, unlink


class ModuleTests(unittest.TestCase):
    def test_api_level(self):
        self.assertEqual(sqlite.apilevel, "2.0",
                         "apilevel is %s, should be 2.0" % sqlite.apilevel)

    def test_thread_safety(self):
        self.assertEqual(sqlite.threadsafety, 1,
                         "threadsafety is %d, should be 1" % sqlite.threadsafety)

    def test_param_style(self):
        self.assertEqual(sqlite.paramstyle, "qmark",
                         "paramstyle is '%s', should be 'qmark'" %
                         sqlite.paramstyle)

    def test_warning(self):
        self.assertTrue(issubclass(sqlite.Warning, Exception),
                     "Warning is not a subclass of Exception")

    def test_error(self):
        self.assertTrue(issubclass(sqlite.Error, Exception),
                        "Error is not a subclass of Exception")

    def test_interface_error(self):
        self.assertTrue(issubclass(sqlite.InterfaceError, sqlite.Error),
                        "InterfaceError is not a subclass of Error")

    def test_database_error(self):
        self.assertTrue(issubclass(sqlite.DatabaseError, sqlite.Error),
                        "DatabaseError is not a subclass of Error")

    def test_data_error(self):
        self.assertTrue(issubclass(sqlite.DataError, sqlite.DatabaseError),
                        "DataError is not a subclass of DatabaseError")

    def test_operational_error(self):
        self.assertTrue(issubclass(sqlite.OperationalError, sqlite.DatabaseError),
                        "OperationalError is not a subclass of DatabaseError")

    def test_integrity_error(self):
        self.assertTrue(issubclass(sqlite.IntegrityError, sqlite.DatabaseError),
                        "IntegrityError is not a subclass of DatabaseError")

    def test_internal_error(self):
        self.assertTrue(issubclass(sqlite.InternalError, sqlite.DatabaseError),
                        "InternalError is not a subclass of DatabaseError")

    def test_programming_error(self):
        self.assertTrue(issubclass(sqlite.ProgrammingError, sqlite.DatabaseError),
                        "ProgrammingError is not a subclass of DatabaseError")

    def test_not_supported_error(self):
        self.assertTrue(issubclass(sqlite.NotSupportedError,
                                   sqlite.DatabaseError),
                        "NotSupportedError is not a subclass of DatabaseError")

    # sqlite3_enable_shared_cache() is deprecated on macOS and calling it may raise
    # OperationalError on some buildbots.
    def test_disallow_instantiation(self):
        cx = sqlite.connect(":memory:")
        check_disallow_instantiation(self, type(cx("select 1")))


class ConnectionTests(unittest.TestCase):

    def setUp(self):
        self.cx = sqlite.connect(":memory:")
        cu = self.cx.cursor()
        cu.execute("create table test(id integer primary key, name text)")
        cu.execute("insert into test(name) values (?)", ("foo",))

    def tearDown(self):
        self.cx.close()

    def test_commit(self):
        self.cx.commit()

    def test_commit_after_no_changes(self):
        """
        A commit should also work when no changes were made to the database.
        """
        self.cx.commit()
        self.cx.commit()

    def test_rollback(self):
        self.cx.rollback()

    def test_rollback_after_no_changes(self):
        """
        A rollback should also work when no changes were made to the database.
        """
        self.cx.rollback()
        self.cx.rollback()

    def test_cursor(self):
        cu = self.cx.cursor()

    def test_failed_open(self):
        YOU_CANNOT_OPEN_THIS = "/foo/bar/bla/23534/mydb.db"
        with self.assertRaises(sqlite.OperationalError):
            con = sqlite.connect(YOU_CANNOT_OPEN_THIS)

    def test_close(self):
        self.cx.close()

    def test_exceptions(self):
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

    def test_in_transaction(self):
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

    def test_in_transaction_ro(self):
        with self.assertRaises(AttributeError):
            self.cx.in_transaction = True

    def test_open_with_path_like_object(self):
        """ Checks that we can successfully connect to a database using an object that
            is PathLike, i.e. has __fspath__(). """
        self.addCleanup(unlink, TESTFN)
        class Path:
            def __fspath__(self):
                return TESTFN
        path = Path()
        with sqlite.connect(path) as cx:
            cx.execute('create table test(id integer)')

    def test_open_uri(self):
        self.addCleanup(unlink, TESTFN)
        with sqlite.connect(TESTFN) as cx:
            cx.execute('create table test(id integer)')
        with sqlite.connect('file:' + TESTFN, uri=True) as cx:
            cx.execute('insert into test(id) values(0)')
        with sqlite.connect('file:' + TESTFN + '?mode=ro', uri=True) as cx:
            with self.assertRaises(sqlite.OperationalError):
                cx.execute('insert into test(id) values(1)')


class UninitialisedConnectionTests(unittest.TestCase):
    def setUp(self):
        self.cx = sqlite.Connection.__new__(sqlite.Connection)

    def test_uninit_operations(self):
        funcs = (
            lambda: self.cx.isolation_level,
            lambda: self.cx.total_changes,
            lambda: self.cx.in_transaction,
            lambda: self.cx.iterdump(),
            lambda: self.cx.cursor(),
            lambda: self.cx.close(),
        )
        for func in funcs:
            with self.subTest(func=func):
                self.assertRaisesRegex(sqlite.ProgrammingError,
                                       "Base Connection.__init__ not called",
                                       func)


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

    def test_execute_no_args(self):
        self.cu.execute("delete from test")

    def test_execute_illegal_sql(self):
        with self.assertRaises(sqlite.OperationalError):
            self.cu.execute("select asdf")

    def test_execute_too_much_sql(self):
        with self.assertRaises(sqlite.Warning):
            self.cu.execute("select 5+4; select 4+5")

    def test_execute_too_much_sql2(self):
        self.cu.execute("select 5+4; -- foo bar")

    def test_execute_too_much_sql3(self):
        self.cu.execute("""
            select 5+4;

            /*
            foo
            */
            """)

    def test_execute_wrong_sql_arg(self):
        with self.assertRaises(TypeError):
            self.cu.execute(42)

    def test_execute_arg_int(self):
        self.cu.execute("insert into test(id) values (?)", (42,))

    def test_execute_arg_float(self):
        self.cu.execute("insert into test(income) values (?)", (2500.32,))

    def test_execute_arg_string(self):
        self.cu.execute("insert into test(name) values (?)", ("Hugo",))

    def test_execute_arg_string_with_zero_byte(self):
        self.cu.execute("insert into test(name) values (?)", ("Hu\x00go",))

        self.cu.execute("select name from test where id=?", (self.cu.lastrowid,))
        row = self.cu.fetchone()
        self.assertEqual(row[0], "Hu\x00go")

    def test_execute_non_iterable(self):
        with self.assertRaises(ValueError) as cm:
            self.cu.execute("insert into test(id) values (?)", 42)
        self.assertEqual(str(cm.exception), 'parameters are of unsupported type')

    def test_execute_wrong_no_of_args1(self):
        # too many parameters
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("insert into test(id) values (?)", (17, "Egon"))

    def test_execute_wrong_no_of_args2(self):
        # too little parameters
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("insert into test(id) values (?)")

    def test_execute_wrong_no_of_args3(self):
        # no parameters, parameters are needed
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("insert into test(id) values (?)")

    def test_execute_param_list(self):
        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("select name from test where name=?", ["foo"])
        row = self.cu.fetchone()
        self.assertEqual(row[0], "foo")

    def test_execute_param_sequence(self):
        class L:
            def __len__(self):
                return 1
            def __getitem__(self, x):
                assert x == 0
                return "foo"

        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("select name from test where name=?", L())
        row = self.cu.fetchone()
        self.assertEqual(row[0], "foo")

    def test_execute_param_sequence_bad_len(self):
        # Issue41662: Error in __len__() was overridden with ProgrammingError.
        class L:
            def __len__(self):
                1/0
            def __getitem__(slf, x):
                raise AssertionError

        self.cu.execute("insert into test(name) values ('foo')")
        with self.assertRaises(ZeroDivisionError):
            self.cu.execute("select name from test where name=?", L())

    def test_execute_dict_mapping(self):
        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("select name from test where name=:name", {"name": "foo"})
        row = self.cu.fetchone()
        self.assertEqual(row[0], "foo")

    def test_execute_dict_mapping_mapping(self):
        class D(dict):
            def __missing__(self, key):
                return "foo"

        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("select name from test where name=:name", D())
        row = self.cu.fetchone()
        self.assertEqual(row[0], "foo")

    def test_execute_dict_mapping_too_little_args(self):
        self.cu.execute("insert into test(name) values ('foo')")
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("select name from test where name=:name and id=:id", {"name": "foo"})

    def test_execute_dict_mapping_no_args(self):
        self.cu.execute("insert into test(name) values ('foo')")
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("select name from test where name=:name")

    def test_execute_dict_mapping_unnamed(self):
        self.cu.execute("insert into test(name) values ('foo')")
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.execute("select name from test where name=?", {"name": "foo"})

    def test_close(self):
        self.cu.close()

    def test_rowcount_execute(self):
        self.cu.execute("delete from test")
        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("update test set name='bar'")
        self.assertEqual(self.cu.rowcount, 2)

    def test_rowcount_select(self):
        """
        pysqlite does not know the rowcount of SELECT statements, because we
        don't fetch all rows after executing the select statement. The rowcount
        has thus to be -1.
        """
        self.cu.execute("select 5 union select 6")
        self.assertEqual(self.cu.rowcount, -1)

    def test_rowcount_executemany(self):
        self.cu.execute("delete from test")
        self.cu.executemany("insert into test(name) values (?)", [(1,), (2,), (3,)])
        self.assertEqual(self.cu.rowcount, 3)

    @unittest.skipIf(sqlite.sqlite_version_info < (3, 35, 0),
                     "Requires SQLite 3.35.0 or newer")
    def test_rowcount_update_returning(self):
        # gh-93421: rowcount is updated correctly for UPDATE...RETURNING queries
        self.cu.execute("update test set name='bar' where name='foo' returning 1")
        self.assertEqual(self.cu.fetchone()[0], 1)
        self.assertEqual(self.cu.rowcount, 1)

    def test_total_changes(self):
        self.cu.execute("insert into test(name) values ('foo')")
        self.cu.execute("insert into test(name) values ('foo')")
        self.assertLess(2, self.cx.total_changes, msg='total changes reported wrong value')

    # Checks for executemany:
    # Sequences are required by the DB-API, iterators
    # enhancements in pysqlite.

    def test_execute_many_sequence(self):
        self.cu.executemany("insert into test(income) values (?)", [(x,) for x in range(100, 110)])

    def test_execute_many_iterator(self):
        class MyIter:
            def __init__(self):
                self.value = 5

            def __iter__(self):
                return self

            def __next__(self):
                if self.value == 10:
                    raise StopIteration
                else:
                    self.value += 1
                    return (self.value,)

        self.cu.executemany("insert into test(income) values (?)", MyIter())

    def test_execute_many_generator(self):
        def mygen():
            for i in range(5):
                yield (i,)

        self.cu.executemany("insert into test(income) values (?)", mygen())

    def test_execute_many_wrong_sql_arg(self):
        with self.assertRaises(TypeError):
            self.cu.executemany(42, [(3,)])

    def test_execute_many_select(self):
        with self.assertRaises(sqlite.ProgrammingError):
            self.cu.executemany("select ?", [(3,)])

    def test_execute_many_not_iterable(self):
        with self.assertRaises(TypeError):
            self.cu.executemany("insert into test(income) values (?)", 42)

    def test_fetch_iter(self):
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

    def test_fetchone(self):
        self.cu.execute("select name from test")
        row = self.cu.fetchone()
        self.assertEqual(row[0], "foo")
        row = self.cu.fetchone()
        self.assertEqual(row, None)

    def test_fetchone_no_statement(self):
        cur = self.cx.cursor()
        row = cur.fetchone()
        self.assertEqual(row, None)

    def test_array_size(self):
        # must default to 1
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

    def test_fetchmany(self):
        self.cu.execute("select name from test")
        res = self.cu.fetchmany(100)
        self.assertEqual(len(res), 1)
        res = self.cu.fetchmany(100)
        self.assertEqual(res, [])

    def test_fetchmany_kw_arg(self):
        """Checks if fetchmany works with keyword arguments"""
        self.cu.execute("select name from test")
        res = self.cu.fetchmany(size=100)
        self.assertEqual(len(res), 1)

    def test_fetchall(self):
        self.cu.execute("select name from test")
        res = self.cu.fetchall()
        self.assertEqual(len(res), 1)
        res = self.cu.fetchall()
        self.assertEqual(res, [])

    def test_setinputsizes(self):
        self.cu.setinputsizes([3, 4, 5])

    def test_setoutputsize(self):
        self.cu.setoutputsize(5, 0)

    def test_setoutputsize_no_column(self):
        self.cu.setoutputsize(42)

    def test_cursor_connection(self):
        # Optional DB-API extension.
        self.assertEqual(self.cu.connection, self.cx)

    def test_wrong_cursor_callable(self):
        with self.assertRaises(TypeError):
            def f(): pass
            cur = self.cx.cursor(f)

    def test_cursor_wrong_class(self):
        class Foo: pass
        foo = Foo()
        with self.assertRaises(TypeError):
            cur = sqlite.Cursor(foo)

    def test_last_row_id_on_replace(self):
        """
        INSERT OR REPLACE and REPLACE INTO should produce the same behavior.
        """
        sql = '{} INTO test(id, unique_test) VALUES (?, ?)'
        for statement in ('INSERT OR REPLACE', 'REPLACE'):
            with self.subTest(statement=statement):
                self.cu.execute(sql.format(statement), (1, 'foo'))
                self.assertEqual(self.cu.lastrowid, 1)

    def test_last_row_id_on_ignore(self):
        self.cu.execute(
            "insert or ignore into test(unique_test) values (?)",
            ('test',))
        self.assertEqual(self.cu.lastrowid, 2)
        self.cu.execute(
            "insert or ignore into test(unique_test) values (?)",
            ('test',))
        self.assertEqual(self.cu.lastrowid, 2)

    def test_last_row_id_insert_o_r(self):
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


class ThreadTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")
        self.cur = self.con.cursor()
        self.cur.execute("create table test(id integer primary key, name text, bin binary, ratio number, ts timestamp)")

    def tearDown(self):
        self.cur.close()
        self.con.close()

    def test_con_cursor(self):
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

    def test_con_commit(self):
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

    def test_con_rollback(self):
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

    def test_con_close(self):
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

    def test_cur_implicit_begin(self):
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

    def test_cur_close(self):
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

    def test_cur_execute(self):
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

    def test_cur_iter_next(self):
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
    def test_date(self):
        d = sqlite.Date(2004, 10, 28)

    def test_time(self):
        t = sqlite.Time(12, 39, 35)

    def test_timestamp(self):
        ts = sqlite.Timestamp(2004, 10, 28, 12, 39, 35)

    def test_date_from_ticks(self):
        d = sqlite.DateFromTicks(42)

    def test_time_from_ticks(self):
        t = sqlite.TimeFromTicks(42)

    def test_timestamp_from_ticks(self):
        ts = sqlite.TimestampFromTicks(42)

    def test_binary(self):
        b = sqlite.Binary(b"\0'")

class ExtensionTests(unittest.TestCase):
    def test_script_string_sql(self):
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

    def test_script_syntax_error(self):
        con = sqlite.connect(":memory:")
        cur = con.cursor()
        with self.assertRaises(sqlite.OperationalError):
            cur.executescript("create table test(x); asdf; create table test2(x)")

    def test_script_error_normal(self):
        con = sqlite.connect(":memory:")
        cur = con.cursor()
        with self.assertRaises(sqlite.OperationalError):
            cur.executescript("create table test(sadfsadfdsa); select foo from hurz;")

    def test_cursor_executescript_as_bytes(self):
        con = sqlite.connect(":memory:")
        cur = con.cursor()
        with self.assertRaises(ValueError) as cm:
            cur.executescript(b"create table test(foo); insert into test(foo) values (5);")
        self.assertEqual(str(cm.exception), 'script argument must be unicode.')

    def test_connection_execute(self):
        con = sqlite.connect(":memory:")
        result = con.execute("select 5").fetchone()[0]
        self.assertEqual(result, 5, "Basic test of Connection.execute")

    def test_connection_executemany(self):
        con = sqlite.connect(":memory:")
        con.execute("create table test(foo)")
        con.executemany("insert into test(foo) values (?)", [(3,), (4,)])
        result = con.execute("select foo from test order by foo").fetchall()
        self.assertEqual(result[0][0], 3, "Basic test of Connection.executemany")
        self.assertEqual(result[1][0], 4, "Basic test of Connection.executemany")

    def test_connection_executescript(self):
        con = sqlite.connect(":memory:")
        con.executescript("create table test(foo); insert into test(foo) values (5);")
        result = con.execute("select foo from test").fetchone()[0]
        self.assertEqual(result, 5, "Basic test of Connection.executescript")

class ClosedConTests(unittest.TestCase):
    def test_closed_con_cursor(self):
        con = sqlite.connect(":memory:")
        con.close()
        with self.assertRaises(sqlite.ProgrammingError):
            cur = con.cursor()

    def test_closed_con_commit(self):
        con = sqlite.connect(":memory:")
        con.close()
        with self.assertRaises(sqlite.ProgrammingError):
            con.commit()

    def test_closed_con_rollback(self):
        con = sqlite.connect(":memory:")
        con.close()
        with self.assertRaises(sqlite.ProgrammingError):
            con.rollback()

    def test_closed_cur_execute(self):
        con = sqlite.connect(":memory:")
        cur = con.cursor()
        con.close()
        with self.assertRaises(sqlite.ProgrammingError):
            cur.execute("select 4")

    def test_closed_create_function(self):
        con = sqlite.connect(":memory:")
        con.close()
        def f(x): return 17
        with self.assertRaises(sqlite.ProgrammingError):
            con.create_function("foo", 1, f)

    def test_closed_create_aggregate(self):
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

    def test_closed_set_authorizer(self):
        con = sqlite.connect(":memory:")
        con.close()
        def authorizer(*args):
            return sqlite.DENY
        with self.assertRaises(sqlite.ProgrammingError):
            con.set_authorizer(authorizer)

    def test_closed_set_progress_callback(self):
        con = sqlite.connect(":memory:")
        con.close()
        def progress(): pass
        with self.assertRaises(sqlite.ProgrammingError):
            con.set_progress_handler(progress, 100)

    def test_closed_call(self):
        con = sqlite.connect(":memory:")
        con.close()
        with self.assertRaises(sqlite.ProgrammingError):
            con()

class ClosedCurTests(unittest.TestCase):
    def test_closed(self):
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

    def test_on_conflict_rollback_with_explicit_transaction(self):
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

    def test_on_conflict_abort_raises_with_explicit_transactions(self):
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

    def test_on_conflict_rollback_without_transaction(self):
        # Start of implicit transaction
        self.cu.execute("INSERT INTO test(name) VALUES ('abort_test')")
        self.cu.execute("INSERT OR ROLLBACK INTO test(unique_name) VALUES ('foo')")
        with self.assertRaises(sqlite.IntegrityError):
            self.cu.execute("INSERT OR ROLLBACK INTO test(unique_name) VALUES ('foo')")
        self.cu.execute("SELECT name, unique_name FROM test")
        # Implicit transaction is rolled back on error.
        self.assertEqual(self.cu.fetchall(), [])

    def test_on_conflict_abort_raises_without_transactions(self):
        # Abort cancels the current sql statement but doesn't change anything
        # about the current transaction.
        self.cu.execute("INSERT INTO test(name) VALUES ('abort_test')")
        self.cu.execute("INSERT OR ABORT INTO test(unique_name) VALUES ('foo')")
        with self.assertRaises(sqlite.IntegrityError):
            self.cu.execute("INSERT OR ABORT INTO test(unique_name) VALUES ('foo')")
        # Make sure all other values were inserted.
        self.cu.execute("SELECT name, unique_name FROM test")
        self.assertEqual(self.cu.fetchall(), [('abort_test', None), (None, 'foo',)])

    def test_on_conflict_fail(self):
        self.cu.execute("INSERT OR FAIL INTO test(unique_name) VALUES ('foo')")
        with self.assertRaises(sqlite.IntegrityError):
            self.cu.execute("INSERT OR FAIL INTO test(unique_name) VALUES ('foo')")
        self.assertEqual(self.cu.fetchall(), [])

    def test_on_conflict_ignore(self):
        self.cu.execute("INSERT OR IGNORE INTO test(unique_name) VALUES ('foo')")
        # Nothing should happen.
        self.cu.execute("INSERT OR IGNORE INTO test(unique_name) VALUES ('foo')")
        self.cu.execute("SELECT unique_name FROM test")
        self.assertEqual(self.cu.fetchall(), [('foo',)])

    def test_on_conflict_replace(self):
        self.cu.execute("INSERT OR REPLACE INTO test(name, unique_name) VALUES ('Data!', 'foo')")
        # There shouldn't be an IntegrityError exception.
        self.cu.execute("INSERT OR REPLACE INTO test(name, unique_name) VALUES ('Very different data!', 'foo')")
        self.cu.execute("SELECT name, unique_name FROM test")
        self.assertEqual(self.cu.fetchall(), [('Very different data!', 'foo')])


class MultiprocessTests(unittest.TestCase):
    CONNECTION_TIMEOUT = SHORT_TIMEOUT / 1000.  # Defaults to 30 ms

    def tearDown(self):
        unlink(TESTFN)

    def test_ctx_mgr_rollback_if_commit_failed(self):
        # bpo-27334: ctx manager does not rollback if commit fails
        SCRIPT = f"""if 1:
# hack-python - fix import
            import sqlmath as sqlite3
            def wait():
                print("started")
                assert "database is locked" in input()

            cx = sqlite3.connect("{TESTFN}", timeout={self.CONNECTION_TIMEOUT})
            cx.create_function("wait", 0, wait)
            with cx:
                cx.execute("create table t(t)")
            try:
                # execute two transactions; both will try to lock the db
                cx.executescript('''
                    -- start a transaction and wait for parent
                    begin transaction;
                    select * from t;
                    select wait();
                    rollback;

                    -- start a new transaction; would fail if parent holds lock
                    begin transaction;
                    select * from t;
                    rollback;
                ''')
            finally:
                cx.close()
        """

        # spawn child process
        proc = subprocess.Popen(
            [sys.executable, "-c", SCRIPT],
            encoding="utf-8",
            bufsize=0,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
        )
        self.addCleanup(proc.communicate)

        # wait for child process to start
        self.assertEqual("started", proc.stdout.readline().strip())

        cx = sqlite.connect(TESTFN, timeout=self.CONNECTION_TIMEOUT)
        try:  # context manager should correctly release the db lock
            with cx:
                cx.execute("insert into t values('test')")
        except sqlite.OperationalError as exc:
            proc.stdin.write(str(exc))
        else:
            proc.stdin.write("no error")
        finally:
            cx.close()

        # terminate child process
        self.assertIsNone(proc.returncode)
        try:
            proc.communicate(input="end", timeout=SHORT_TIMEOUT)
        except subprocess.TimeoutExpired:
            proc.kill()
            proc.communicate()
            raise
        self.assertEqual(proc.returncode, 0)


def suite():
    tests = [
        ClosedConTests,
        ClosedCurTests,
        ConnectionTests,
        ConstructorTests,
        CursorTests,
        ExtensionTests,
        ModuleTests,
        MultiprocessTests,
        SqliteOnConflictTests,
        ThreadTests,
        UninitialisedConnectionTests,
    ]
    return unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(t) for t in tests]
    )

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

# hack-python - append test-suite
test_suite_list.append(suite)
"""


/*
file https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/dump.py
*/

"""
# Author: Paul Kippes <kippesp@gmail.com>

import unittest
from . import sqlmath_dbapi2 as sqlite


class DumpTests(unittest.TestCase):
    def setUp(self):
        self.cx = sqlite.connect(":memory:")
        self.cu = self.cx.cursor()

    def tearDown(self):
        self.cx.close()

    def test_table_dump(self):
        expected_sqls = [
                """CREATE TABLE "index"("index" blob);"""
                ,
                """INSERT INTO "index" VALUES(X'01');"""
                ,
                """CREATE TABLE "quoted""table"("quoted""field" text);"""
                ,
                """INSERT INTO "quoted""table" VALUES('quoted''value');"""
                ,
                "CREATE TABLE t1(id integer primary key, s1 text, " \
                "t1_i1 integer not null, i2 integer, unique (s1), " \
                "constraint t1_idx1 unique (i2));"
                ,
                "INSERT INTO \"t1\" VALUES(1,'foo',10,20);"
                ,
                "INSERT INTO \"t1\" VALUES(2,'foo2',30,30);"
                ,
                "CREATE TABLE t2(id integer, t2_i1 integer, " \
                "t2_i2 integer, primary key (id)," \
                "foreign key(t2_i1) references t1(t1_i1));"
                ,
                "CREATE TRIGGER trigger_1 update of t1_i1 on t1 " \
                "begin " \
                "update t2 set t2_i1 = new.t1_i1 where t2_i1 = old.t1_i1; " \
                "end;"
                ,
                "CREATE VIEW v1 as select * from t1 left join t2 " \
                "using (id);"
                ]
        [self.cu.execute(s) for s in expected_sqls]
        i = self.cx.iterdump()
        actual_sqls = [s for s in i]
        expected_sqls = ['BEGIN TRANSACTION;'] + expected_sqls + \
            ['COMMIT;']
        [self.assertEqual(expected_sqls[i], actual_sqls[i])
            for i in range(len(expected_sqls))]

    def test_dump_autoincrement(self):
        expected = [
            'CREATE TABLE "t1" (id integer primary key autoincrement);',
            'INSERT INTO "t1" VALUES(NULL);',
            'CREATE TABLE "t2" (id integer primary key autoincrement);',
        ]
        self.cu.executescript("".join(expected))

        # the NULL value should now be automatically be set to 1
        expected[1] = expected[1].replace("NULL", "1")
        expected.insert(0, "BEGIN TRANSACTION;")
        expected.extend([
            'DELETE FROM "sqlite_sequence";',
            'INSERT INTO "sqlite_sequence" VALUES(\'t1\',1);',
            'COMMIT;',
        ])

        actual = [stmt for stmt in self.cx.iterdump()]
        self.assertEqual(expected, actual)

    def test_dump_autoincrement_create_new_db(self):
        self.cu.execute("BEGIN TRANSACTION")
        self.cu.execute("CREATE TABLE t1 (id integer primary key autoincrement)")
        self.cu.execute("CREATE TABLE t2 (id integer primary key autoincrement)")
        self.cu.executemany("INSERT INTO t1 VALUES(?)", ((None,) for _ in range(9)))
        self.cu.executemany("INSERT INTO t2 VALUES(?)", ((None,) for _ in range(4)))
        self.cx.commit()

        cx2 = sqlite.connect(":memory:")
        query = "".join(self.cx.iterdump())
        cx2.executescript(query)
        cu2 = cx2.cursor()

        dataset = (
            ("t1", 9),
            ("t2", 4),
        )
        for table, seq in dataset:
            with self.subTest(table=table, seq=seq):
                res = cu2.execute("""
                    SELECT "seq" FROM "sqlite_sequence" WHERE "name" == ?
                """, (table,))
                rows = res.fetchall()
                self.assertEqual(rows[0][0], seq)

    def test_unorderable_row(self):
        # iterdump() should be able to cope with unorderable row types (issue #15545)
        class UnorderableRow:
            def __init__(self, cursor, row):
                self.row = row
            def __getitem__(self, index):
                return self.row[index]
        self.cx.row_factory = UnorderableRow
        CREATE_ALPHA = """CREATE TABLE "alpha" ("one");"""
        CREATE_BETA = """CREATE TABLE "beta" ("two");"""
        expected = [
            "BEGIN TRANSACTION;",
            CREATE_ALPHA,
            CREATE_BETA,
            "COMMIT;"
            ]
        self.cu.execute(CREATE_BETA)
        self.cu.execute(CREATE_ALPHA)
        got = list(self.cx.iterdump())
        self.assertEqual(expected, got)

def suite():
    tests = [
        DumpTests,
    ]
    return unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(t) for t in tests]
    )

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

# hack-python - append test-suite
test_suite_list.append(suite)
"""


/*
file https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/factory.py
*/

"""
# pysqlite2/test/factory.py: tests for the various factories in pysqlite
#
# Copyright (C) 2005-2007 Gerhard Häring <gh@ghaering.de>
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

    def test_is_instance(self):
        self.assertIsInstance(self.con, MyConnection)

class CursorFactoryTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")

    def tearDown(self):
        self.con.close()

    def test_is_instance(self):
        cur = self.con.cursor()
        self.assertIsInstance(cur, sqlite.Cursor)
        cur = self.con.cursor(MyCursor)
        self.assertIsInstance(cur, MyCursor)
        cur = self.con.cursor(factory=lambda con: MyCursor(con))
        self.assertIsInstance(cur, MyCursor)

    def test_invalid_factory(self):
        # not a callable at all
        self.assertRaises(TypeError, self.con.cursor, None)
        # invalid callable with not exact one argument
        self.assertRaises(TypeError, self.con.cursor, lambda: None)
        # invalid callable returning non-cursor
        self.assertRaises(TypeError, self.con.cursor, lambda con: None)

class RowFactoryTestsBackwardsCompat(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")

    def test_is_produced_by_factory(self):
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

    def test_custom_factory(self):
        self.con.row_factory = lambda cur, row: list(row)
        row = self.con.execute("select 1, 2").fetchone()
        self.assertIsInstance(row, list)

    def test_sqlite_row_index(self):
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
            row['a\x7f1']
        with self.assertRaises(IndexError):
            row[2]
        with self.assertRaises(IndexError):
            row[-3]
        with self.assertRaises(IndexError):
            row[2**1000]

    def test_sqlite_row_index_unicode(self):
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1 as \xff").fetchone()
        self.assertEqual(row["\xff"], 1)
        with self.assertRaises(IndexError):
            row['\u0178']
        with self.assertRaises(IndexError):
            row['\xdf']

    def test_sqlite_row_slice(self):
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

    def test_sqlite_row_iter(self):
        """Checks if the row object is iterable"""
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1 as a, 2 as b").fetchone()

        # Is iterable in correct order and produces valid results:
        items = [col for col in row]
        self.assertEqual(items, [1, 2])

        # Is iterable the second time:
        items = [col for col in row]
        self.assertEqual(items, [1, 2])

    def test_sqlite_row_as_tuple(self):
        """Checks if the row object can be converted to a tuple"""
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1 as a, 2 as b").fetchone()
        t = tuple(row)
        self.assertEqual(t, (row['a'], row['b']))

    def test_sqlite_row_as_dict(self):
        """Checks if the row object can be correctly converted to a dictionary"""
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1 as a, 2 as b").fetchone()
        d = dict(row)
        self.assertEqual(d["a"], row["a"])
        self.assertEqual(d["b"], row["b"])

    def test_sqlite_row_hash_cmp(self):
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

    def test_sqlite_row_as_sequence(self):
        """ Checks if the row object can act like a sequence """
        self.con.row_factory = sqlite.Row
        row = self.con.execute("select 1 as a, 2 as b").fetchone()

        as_tuple = tuple(row)
        self.assertEqual(list(reversed(row)), list(reversed(as_tuple)))
        self.assertIsInstance(row, Sequence)

    def test_fake_cursor_class(self):
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

    def test_unicode(self):
        austria = "Österreich"
        row = self.con.execute("select ?", (austria,)).fetchone()
        self.assertEqual(type(row[0]), str, "type of row[0] must be unicode")

    def test_string(self):
        self.con.text_factory = bytes
        austria = "Österreich"
        row = self.con.execute("select ?", (austria,)).fetchone()
        self.assertEqual(type(row[0]), bytes, "type of row[0] must be bytes")
        self.assertEqual(row[0], austria.encode("utf-8"), "column must equal original data in UTF-8")

    def test_custom(self):
        self.con.text_factory = lambda x: str(x, "utf-8", "ignore")
        austria = "Österreich"
        row = self.con.execute("select ?", (austria,)).fetchone()
        self.assertEqual(type(row[0]), str, "type of row[0] must be unicode")
        self.assertTrue(row[0].endswith("reich"), "column must contain original data")

    def tearDown(self):
        self.con.close()

class TextFactoryTestsWithEmbeddedZeroBytes(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")
        self.con.execute("create table test (value text)")
        self.con.execute("insert into test (value) values (?)", ("a\x00b",))

    def test_string(self):
        # text_factory defaults to str
        row = self.con.execute("select value from test").fetchone()
        self.assertIs(type(row[0]), str)
        self.assertEqual(row[0], "a\x00b")

    def test_bytes(self):
        self.con.text_factory = bytes
        row = self.con.execute("select value from test").fetchone()
        self.assertIs(type(row[0]), bytes)
        self.assertEqual(row[0], b"a\x00b")

    def test_bytearray(self):
        self.con.text_factory = bytearray
        row = self.con.execute("select value from test").fetchone()
        self.assertIs(type(row[0]), bytearray)
        self.assertEqual(row[0], b"a\x00b")

    def test_custom(self):
        # A custom factory should receive a bytes argument
        self.con.text_factory = lambda x: x
        row = self.con.execute("select value from test").fetchone()
        self.assertIs(type(row[0]), bytes)
        self.assertEqual(row[0], b"a\x00b")

    def tearDown(self):
        self.con.close()

def suite():
    tests = [
        ConnectionFactoryTests,
        CursorFactoryTests,
        RowFactoryTests,
        RowFactoryTestsBackwardsCompat,
        TextFactoryTests,
        TextFactoryTestsWithEmbeddedZeroBytes,
    ]
    return unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(t) for t in tests]
    )

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

# hack-python - append test-suite
test_suite_list.append(suite)
"""


/*
file https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/hooks.py
*/

"""
# pysqlite2/test/hooks.py: tests for various SQLite-specific hooks
#
# Copyright (C) 2006-2007 Gerhard Häring <gh@ghaering.de>
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

import contextlib
import unittest
from . import sqlmath_dbapi2 as sqlite

from test.support.os_helper import TESTFN, unlink


class CollationTests(unittest.TestCase):
    def test_create_collation_not_string(self):
        con = sqlite.connect(":memory:")
        with self.assertRaises(TypeError):
            con.create_collation(None, lambda x, y: (x > y) - (x < y))

    def test_create_collation_not_callable(self):
        con = sqlite.connect(":memory:")
        with self.assertRaises(TypeError) as cm:
            con.create_collation("X", 42)
        self.assertEqual(str(cm.exception), 'parameter must be callable')

    def test_create_collation_not_ascii(self):
        con = sqlite.connect(":memory:")
        with self.assertRaises(sqlite.ProgrammingError):
            con.create_collation("collä", lambda x, y: (x > y) - (x < y))

    def test_create_collation_bad_upper(self):
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

    def test_collation_is_used(self):
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

    def test_collation_returns_large_integer(self):
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

    def test_collation_register_twice(self):
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

    def test_deregister_collation(self):
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
    def test_progress_handler_used(self):
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


    def test_opcode_count(self):
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

    def test_cancel_operation(self):
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

    def test_clear_handler(self):
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
    @contextlib.contextmanager
    def check_stmt_trace(self, cx, expected):
        try:
            traced = []
            cx.set_trace_callback(lambda stmt: traced.append(stmt))
            yield
        finally:
            self.assertEqual(traced, expected)
            cx.set_trace_callback(None)

    def test_trace_callback_used(self):
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

    def test_clear_trace_callback(self):
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

    def test_unicode_content(self):
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
        con.execute("insert into foo(x) values ('%s')" % unicode_value)
        con.commit()
        self.assertTrue(any(unicode_value in stmt for stmt in traced_statements),
                        "Unicode data %s garbled in trace callback: %s"
                        % (ascii(unicode_value), ', '.join(map(ascii, traced_statements))))

    def test_trace_callback_content(self):
        # set_trace_callback() shouldn't produce duplicate content (bpo-26187)
        traced_statements = []
        def trace(statement):
            traced_statements.append(statement)

        queries = ["create table foo(x)",
                   "insert into foo(x) values(1)"]
        self.addCleanup(unlink, TESTFN)
        con1 = sqlite.connect(TESTFN, isolation_level=None)
        con2 = sqlite.connect(TESTFN)
        con1.set_trace_callback(trace)
        cur = con1.cursor()
        cur.execute(queries[0])
        con2.execute("create table bar(x)")
        cur.execute(queries[1])
        self.assertEqual(traced_statements, queries)

    def test_trace_expanded_sql(self):
        expected = [
            "create table t(t)",
            "BEGIN ",
            "insert into t values(0)",
            "insert into t values(1)",
            "insert into t values(2)",
            "COMMIT",
        ]
        cx = sqlite.connect(":memory:")
        with self.check_stmt_trace(cx, expected):
            with cx:
                cx.execute("create table t(t)")
                cx.executemany("insert into t values(?)", ((v,) for v in range(3)))


def suite():
    tests = [
        CollationTests,
        ProgressTests,
        TraceCallbackTests,
    ]
    return unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(t) for t in tests]
    )

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

# hack-python - append test-suite
test_suite_list.append(suite)
"""


/*
file https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/regression.py
*/

"""
# pysqlite2/test/regression.py: pysqlite regression tests
#
# Copyright (C) 2006-2010 Gerhard Häring <gh@ghaering.de>
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
import weakref
import functools
from test import support

from unittest.mock import patch


class RegressionTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")

    def tearDown(self):
        self.con.close()

    def test_pragma_user_version(self):
        # This used to crash pysqlite because this pragma command returns NULL for the column name
        cur = self.con.cursor()
        cur.execute("pragma user_version")

    def test_pragma_schema_version(self):
        # This still crashed pysqlite <= 2.2.1
        con = sqlite.connect(":memory:", detect_types=sqlite.PARSE_COLNAMES)
        try:
            cur = self.con.cursor()
            cur.execute("pragma schema_version")
        finally:
            cur.close()
            con.close()

    def test_statement_reset(self):
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

    def test_column_name_with_spaces(self):
        cur = self.con.cursor()
        cur.execute('select 1 as "foo bar [datetime]"')
        self.assertEqual(cur.description[0][0], "foo bar [datetime]")

        cur.execute('select 1 as "foo baz"')
        self.assertEqual(cur.description[0][0], "foo baz")

    def test_statement_finalization_on_close_db(self):
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

    def test_on_conflict_rollback(self):
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

    def test_workaround_for_buggy_sqlite_transfer_bindings(self):
        """
        pysqlite would crash with older SQLite versions unless
        a workaround is implemented.
        """
        self.con.execute("create table foo(bar)")
        self.con.execute("drop table foo")
        self.con.execute("create table foo(bar)")

    def test_empty_statement(self):
        """
        pysqlite used to segfault with SQLite versions 3.5.x. These return NULL
        for "no-operation" statements
        """
        self.con.execute("")

    def test_type_map_usage(self):
        """
        pysqlite until 2.4.1 did not rebuild the row_cast_map when recompiling
        a statement. This test exhibits the problem.
        """
        SELECT = "select * from foo"
        con = sqlite.connect(":memory:",detect_types=sqlite.PARSE_DECLTYPES)
        con.execute("create table foo(bar timestamp)")
        con.execute("insert into foo(bar) values (?)", (datetime.datetime.now(),))
        con.execute(SELECT).close()
        con.execute("drop table foo")
        con.execute("create table foo(bar integer)")
        con.execute("insert into foo(bar) values (5)")
        con.execute(SELECT).close()

    def test_bind_mutating_list(self):
        # Issue41662: Crash when mutate a list of parameters during iteration.
        class X:
            def __conform__(self, protocol):
                parameters.clear()
                return "..."
        parameters = [X(), 0]
        con = sqlite.connect(":memory:",detect_types=sqlite.PARSE_DECLTYPES)
        con.execute("create table foo(bar X, baz integer)")
        # Should not crash
        with self.assertRaises(IndexError):
            con.execute("insert into foo(bar, baz) values (?, ?)", parameters)

    def test_error_msg_decode_error(self):
        # When porting the module to Python 3.0, the error message about
        # decoding errors disappeared. This verifies they're back again.
        with self.assertRaises(sqlite.OperationalError) as cm:
            self.con.execute("select 'xxx' || ? || 'yyy' colname",
                             (bytes(bytearray([250])),)).fetchone()
        msg = "Could not decode to UTF-8 column 'colname' with text 'xxx"
        self.assertIn(msg, str(cm.exception))

    def test_register_adapter(self):
        """
        See issue 3312.
        """
        self.assertRaises(TypeError, sqlite.register_adapter, {}, None)

    def test_set_isolation_level(self):
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

    def test_cursor_constructor_call_check(self):
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

    def test_str_subclass(self):
        """
        The Python 3.0 port of the module didn't cope with values of subclasses of str.
        """
        class MyStr(str): pass
        self.con.execute("select ?", (MyStr("abc"),))

    def test_connection_constructor_call_check(self):
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

    def test_cursor_registration(self):
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

    def test_auto_commit(self):
        """
        Verifies that creating a connection in autocommit mode works.
        2.5.3 introduced a regression so that these could no longer
        be created.
        """
        con = sqlite.connect(":memory:", isolation_level=None)

    def test_pragma_autocommit(self):
        """
        Verifies that running a PRAGMA statement that does an autocommit does
        work. This did not work in 2.5.3/2.5.4.
        """
        cur = self.con.cursor()
        cur.execute("create table foo(bar)")
        cur.execute("insert into foo(bar) values (5)")

        cur.execute("pragma page_size")
        row = cur.fetchone()

    def test_connection_call(self):
        """
        Call a connection with a non-string SQL request: check error handling
        of the statement constructor.
        """
        self.assertRaises(TypeError, self.con, 1)

    def test_collation(self):
        def collation_cb(a, b):
            return 1
        self.assertRaises(sqlite.ProgrammingError, self.con.create_collation,
            # Lone surrogate cannot be encoded to the default encoding (utf8)
            "\uDC80", collation_cb)

    def test_recursive_cursor_use(self):
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

    def test_convert_timestamp_microsecond_padding(self):
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

    def test_invalid_isolation_level_type(self):
        # isolation level is a string, not an integer
        self.assertRaises(TypeError,
                          sqlite.connect, ":memory:", isolation_level=123)


    def test_null_character(self):
        # Issue #21147
        con = sqlite.connect(":memory:")
        self.assertRaises(ValueError, con, "\0select 1")
        self.assertRaises(ValueError, con, "select 1\0")
        cur = con.cursor()
        self.assertRaises(ValueError, cur.execute, " \0select 2")
        self.assertRaises(ValueError, cur.execute, "select 2\0")

    def test_commit_cursor_reset(self):
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

    def test_bpo31770(self):
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
        support.gc_collect()

    def test_del_isolation_level_segfault(self):
        with self.assertRaises(AttributeError):
            del self.con.isolation_level

    def test_bpo37347(self):
        class Printer:
            def log(self, *args):
                return sqlite.SQLITE_OK

        for method in [self.con.set_trace_callback,
                       functools.partial(self.con.set_progress_handler, n=1),
                       self.con.set_authorizer]:
            printer_instance = Printer()
            method(printer_instance.log)
            method(printer_instance.log)
            self.con.execute("select 1")  # trigger seg fault
            method(None)

    def test_return_empty_bytestring(self):
        cur = self.con.execute("select X''")
        val = cur.fetchone()[0]
        self.assertEqual(val, b'')


class RecursiveUseOfCursors(unittest.TestCase):
    # GH-80254: sqlite3 should not segfault for recursive use of cursors.
    msg = "Recursive use of cursors not allowed"

    def setUp(self):
        self.con = sqlite.connect(":memory:",
                                  detect_types=sqlite.PARSE_COLNAMES)
        self.cur = self.con.cursor()
        self.cur.execute("create table test(x foo)")
        self.cur.executemany("insert into test(x) values (?)",
                             [("foo",), ("bar",)])

    def tearDown(self):
        self.cur.close()
        self.con.close()

    def test_recursive_cursor_init(self):
        conv = lambda x: self.cur.__init__(self.con)
        with patch.dict(sqlite.converters, {"INIT": conv}):
            with self.assertRaisesRegex(sqlite.ProgrammingError, self.msg):
                self.cur.execute(f'select x as "x [INIT]", x from test')

    def test_recursive_cursor_close(self):
        conv = lambda x: self.cur.close()
        with patch.dict(sqlite.converters, {"CLOSE": conv}):
            with self.assertRaisesRegex(sqlite.ProgrammingError, self.msg):
                self.cur.execute(f'select x as "x [CLOSE]", x from test')

    def test_recursive_cursor_fetch(self):
        conv = lambda x, l=[]: self.cur.fetchone() if l else l.append(None)
        with patch.dict(sqlite.converters, {"ITER": conv}):
            self.cur.execute(f'select x as "x [ITER]", x from test')
            with self.assertRaisesRegex(sqlite.ProgrammingError, self.msg):
                self.cur.fetchall()


def suite():
    tests = [
        RegressionTests,
        RecursiveUseOfCursors,
    ]
    return unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(t) for t in tests]
    )

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

# hack-python - append test-suite
test_suite_list.append(suite)
"""


/*
file https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/types.py
*/

"""
# pysqlite2/test/types.py: tests for type conversion and detection
#
# Copyright (C) 2005 Gerhard Häring <gh@ghaering.de>
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

    def test_string(self):
        self.cur.execute("insert into test(s) values (?)", ("Österreich",))
        self.cur.execute("select s from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], "Österreich")

    def test_small_int(self):
        self.cur.execute("insert into test(i) values (?)", (42,))
        self.cur.execute("select i from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], 42)

    def test_large_int(self):
        num = 2**40
        self.cur.execute("insert into test(i) values (?)", (num,))
        self.cur.execute("select i from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], num)

    def test_float(self):
        val = 3.14
        self.cur.execute("insert into test(f) values (?)", (val,))
        self.cur.execute("select f from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], val)

    def test_blob(self):
        sample = b"Guglhupf"
        val = memoryview(sample)
        self.cur.execute("insert into test(b) values (?)", (val,))
        self.cur.execute("select b from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], sample)

    def test_unicode_execute(self):
        self.cur.execute("select 'Österreich'")
        row = self.cur.fetchone()
        self.assertEqual(row[0], "Österreich")

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
        self.cur.execute("""
            create table test(
                i int,
                s str,
                f float,
                b bool,
                u unicode,
                foo foo,
                bin blob,
                n1 number,
                n2 number(5),
                bad bad,
                cbin cblob)
        """)

        # override float, make them always return the same number
        sqlite.converters["FLOAT"] = lambda x: 47.2

        # and implement two custom ones
        sqlite.converters["BOOL"] = lambda x: bool(int(x))
        sqlite.converters["FOO"] = DeclTypesTests.Foo
        sqlite.converters["BAD"] = DeclTypesTests.BadConform
        sqlite.converters["WRONG"] = lambda x: "WRONG"
        sqlite.converters["NUMBER"] = float
        sqlite.converters["CBLOB"] = lambda x: b"blobish"

    def tearDown(self):
        del sqlite.converters["FLOAT"]
        del sqlite.converters["BOOL"]
        del sqlite.converters["FOO"]
        del sqlite.converters["BAD"]
        del sqlite.converters["WRONG"]
        del sqlite.converters["NUMBER"]
        del sqlite.converters["CBLOB"]
        self.cur.close()
        self.con.close()

    def test_string(self):
        # default
        self.cur.execute("insert into test(s) values (?)", ("foo",))
        self.cur.execute('select s as "s [WRONG]" from test')
        row = self.cur.fetchone()
        self.assertEqual(row[0], "foo")

    def test_small_int(self):
        # default
        self.cur.execute("insert into test(i) values (?)", (42,))
        self.cur.execute("select i from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], 42)

    def test_large_int(self):
        # default
        num = 2**40
        self.cur.execute("insert into test(i) values (?)", (num,))
        self.cur.execute("select i from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], num)

    def test_float(self):
        # custom
        val = 3.14
        self.cur.execute("insert into test(f) values (?)", (val,))
        self.cur.execute("select f from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], 47.2)

    def test_bool(self):
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

    def test_unicode(self):
        # default
        val = "\xd6sterreich"
        self.cur.execute("insert into test(u) values (?)", (val,))
        self.cur.execute("select u from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], val)

    def test_foo(self):
        val = DeclTypesTests.Foo("bla")
        self.cur.execute("insert into test(foo) values (?)", (val,))
        self.cur.execute("select foo from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], val)

    def test_error_in_conform(self):
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

    def test_unsupported_seq(self):
        class Bar: pass
        val = Bar()
        with self.assertRaises(sqlite.InterfaceError):
            self.cur.execute("insert into test(f) values (?)", (val,))

    def test_unsupported_dict(self):
        class Bar: pass
        val = Bar()
        with self.assertRaises(sqlite.InterfaceError):
            self.cur.execute("insert into test(f) values (:val)", {"val": val})

    def test_blob(self):
        # default
        sample = b"Guglhupf"
        val = memoryview(sample)
        self.cur.execute("insert into test(bin) values (?)", (val,))
        self.cur.execute("select bin from test")
        row = self.cur.fetchone()
        self.assertEqual(row[0], sample)

    def test_number1(self):
        self.cur.execute("insert into test(n1) values (5)")
        value = self.cur.execute("select n1 from test").fetchone()[0]
        # if the converter is not used, it's an int instead of a float
        self.assertEqual(type(value), float)

    def test_number2(self):
        """Checks whether converter names are cut off at '(' characters"""
        self.cur.execute("insert into test(n2) values (5)")
        value = self.cur.execute("select n2 from test").fetchone()[0]
        # if the converter is not used, it's an int instead of a float
        self.assertEqual(type(value), float)

    def test_convert_zero_sized_blob(self):
        self.con.execute("insert into test(cbin) values (?)", (b"",))
        cur = self.con.execute("select cbin from test")
        # Zero-sized blobs with converters returns None.  This differs from
        # blobs without a converter, where b"" is returned.
        self.assertIsNone(cur.fetchone()[0])


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

    def test_decl_type_not_used(self):
        """
        Assures that the declared type is not used when PARSE_DECLTYPES
        is not set.
        """
        self.cur.execute("insert into test(x) values (?)", ("xxx",))
        self.cur.execute("select x from test")
        val = self.cur.fetchone()[0]
        self.assertEqual(val, "xxx")

    def test_none(self):
        self.cur.execute("insert into test(x) values (?)", (None,))
        self.cur.execute("select x from test")
        val = self.cur.fetchone()[0]
        self.assertEqual(val, None)

    def test_col_name(self):
        self.cur.execute("insert into test(x) values (?)", ("xxx",))
        self.cur.execute('select x as "x y [bar]" from test')
        val = self.cur.fetchone()[0]
        self.assertEqual(val, "<xxx>")

        # Check if the stripping of colnames works. Everything after the first
        # '[' (and the preceeding space) should be stripped.
        self.assertEqual(self.cur.description[0][0], "x y")

    def test_case_in_converter_name(self):
        self.cur.execute("select 'other' as \"x [b1b1]\"")
        val = self.cur.fetchone()[0]
        self.assertEqual(val, "MARKER")

    def test_cursor_description_no_row(self):
        """
        cursor.description should at least provide the column name(s), even if
        no row returned.
        """
        self.cur.execute("select * from test where 0 = 1")
        self.assertEqual(self.cur.description[0][0], "x")

    def test_cursor_description_insert(self):
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

    def test_cursor_description_cte_simple(self):
        self.cur.execute("with one as (select 1) select * from one")
        self.assertIsNotNone(self.cur.description)
        self.assertEqual(self.cur.description[0][0], "1")

    def test_cursor_description_cte_multiple_columns(self):
        self.cur.execute("insert into test values(1)")
        self.cur.execute("insert into test values(2)")
        self.cur.execute("with testCTE as (select * from test) select * from testCTE")
        self.assertIsNotNone(self.cur.description)
        self.assertEqual(self.cur.description[0][0], "x")

    def test_cursor_description_cte(self):
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

    def test_caster_is_used(self):
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

    def test_binary_input_for_converter(self):
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

    def test_sqlite_date(self):
        d = sqlite.Date(2004, 2, 14)
        self.cur.execute("insert into test(d) values (?)", (d,))
        self.cur.execute("select d from test")
        d2 = self.cur.fetchone()[0]
        self.assertEqual(d, d2)

    def test_sqlite_timestamp(self):
        ts = sqlite.Timestamp(2004, 2, 14, 7, 15, 0)
        self.cur.execute("insert into test(ts) values (?)", (ts,))
        self.cur.execute("select ts from test")
        ts2 = self.cur.fetchone()[0]
        self.assertEqual(ts, ts2)

    def test_sql_timestamp(self):
        now = datetime.datetime.utcnow()
        self.cur.execute("insert into test(ts) values (current_timestamp)")
        self.cur.execute("select ts from test")
        ts = self.cur.fetchone()[0]
        self.assertEqual(type(ts), datetime.datetime)
        self.assertEqual(ts.year, now.year)

    def test_date_time_sub_seconds(self):
        ts = sqlite.Timestamp(2004, 2, 14, 7, 15, 0, 500000)
        self.cur.execute("insert into test(ts) values (?)", (ts,))
        self.cur.execute("select ts from test")
        ts2 = self.cur.fetchone()[0]
        self.assertEqual(ts, ts2)

    def test_date_time_sub_seconds_floating_point(self):
        ts = sqlite.Timestamp(2004, 2, 14, 7, 15, 0, 510241)
        self.cur.execute("insert into test(ts) values (?)", (ts,))
        self.cur.execute("select ts from test")
        ts2 = self.cur.fetchone()[0]
        self.assertEqual(ts, ts2)

def suite():
    tests = [
        BinaryConverterTests,
        ColNamesTests,
        CommonTableExpressionTests,
        DateTimeTests,
        DeclTypesTests,
        ObjectAdaptationTests,
        SqliteTypeTests,
    ]
    return unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(t) for t in tests]
    )

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

# hack-python - append test-suite
test_suite_list.append(suite)
"""


/*
file https://github.com/python/cpython/blob/v3.10.11/Lib/sqlite3/test/userfunctions.py
*/

"""
# pysqlite2/test/userfunctions.py: tests for user-defined functions and
#                                  aggregates.
#
# Copyright (C) 2005-2007 Gerhard Häring <gh@ghaering.de>
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

from test.support import gc_collect


def func_returntext():
    return "foo"
def func_returntextwithnull():
    return "1\x002"
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

class AggrText:
    def __init__(self):
        self.txt = ""
    def step(self, txt):
        self.txt = self.txt + txt
    def finalize(self):
        return self.txt


class FunctionTests(unittest.TestCase):
    def setUp(self):
        self.con = sqlite.connect(":memory:")

        self.con.create_function("returntext", 0, func_returntext)
        self.con.create_function("returntextwithnull", 0, func_returntextwithnull)
        self.con.create_function("returnunicode", 0, func_returnunicode)
        self.con.create_function("returnint", 0, func_returnint)
        self.con.create_function("returnfloat", 0, func_returnfloat)
        self.con.create_function("returnnull", 0, func_returnnull)
        self.con.create_function("returnblob", 0, func_returnblob)
        self.con.create_function("returnlonglong", 0, func_returnlonglong)
        self.con.create_function("returnnan", 0, lambda: float("nan"))
        self.con.create_function("returntoolargeint", 0, lambda: 1 << 65)
        self.con.create_function("raiseexception", 0, func_raiseexception)

        self.con.create_function("isblob", 1, lambda x: isinstance(x, bytes))
        self.con.create_function("isnone", 1, lambda x: x is None)
        self.con.create_function("spam", -1, lambda *x: len(x))
        self.con.execute("create table test(t text)")

    def tearDown(self):
        self.con.close()

    def test_func_error_on_create(self):
        with self.assertRaises(sqlite.OperationalError):
            self.con.create_function("bla", -100, lambda x: 2*x)

    def test_func_ref_count(self):
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

    def test_func_return_text(self):
        cur = self.con.cursor()
        cur.execute("select returntext()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), str)
        self.assertEqual(val, "foo")

    def test_func_return_text_with_null_char(self):
        cur = self.con.cursor()
        res = cur.execute("select returntextwithnull()").fetchone()[0]
        self.assertEqual(type(res), str)
        self.assertEqual(res, "1\x002")

    def test_func_return_unicode(self):
        cur = self.con.cursor()
        cur.execute("select returnunicode()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), str)
        self.assertEqual(val, "bar")

    def test_func_return_int(self):
        cur = self.con.cursor()
        cur.execute("select returnint()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), int)
        self.assertEqual(val, 42)

    def test_func_return_float(self):
        cur = self.con.cursor()
        cur.execute("select returnfloat()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), float)
        if val < 3.139 or val > 3.141:
            self.fail("wrong value")

    def test_func_return_null(self):
        cur = self.con.cursor()
        cur.execute("select returnnull()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), type(None))
        self.assertEqual(val, None)

    def test_func_return_blob(self):
        cur = self.con.cursor()
        cur.execute("select returnblob()")
        val = cur.fetchone()[0]
        self.assertEqual(type(val), bytes)
        self.assertEqual(val, b"blob")

    def test_func_return_long_long(self):
        cur = self.con.cursor()
        cur.execute("select returnlonglong()")
        val = cur.fetchone()[0]
        self.assertEqual(val, 1<<31)

    def test_func_return_nan(self):
        cur = self.con.cursor()
        cur.execute("select returnnan()")
        self.assertIsNone(cur.fetchone()[0])

    def test_func_return_too_large_int(self):
        cur = self.con.cursor()
        with self.assertRaises(sqlite.OperationalError):
            self.con.execute("select returntoolargeint()")

    def test_func_exception(self):
        cur = self.con.cursor()
        with self.assertRaises(sqlite.OperationalError) as cm:
            cur.execute("select raiseexception()")
            cur.fetchone()
        self.assertEqual(str(cm.exception), 'user-defined function raised exception')

    def test_any_arguments(self):
        cur = self.con.cursor()
        cur.execute("select spam(?, ?)", (1, 2))
        val = cur.fetchone()[0]
        self.assertEqual(val, 2)

    def test_empty_blob(self):
        cur = self.con.execute("select isblob(x'')")
        self.assertTrue(cur.fetchone()[0])

    def test_nan_float(self):
        cur = self.con.execute("select isnone(?)", (float("nan"),))
        # SQLite has no concept of nan; it is converted to NULL
        self.assertTrue(cur.fetchone()[0])

    def test_too_large_int(self):
        err = "Python int too large to convert to SQLite INTEGER"
        self.assertRaisesRegex(OverflowError, err, self.con.execute,
                               "select spam(?)", (1 << 65,))

    def test_non_contiguous_blob(self):
        self.assertRaisesRegex(ValueError, "could not convert BLOB to buffer",
                               self.con.execute, "select spam(?)",
                               (memoryview(b"blob")[::2],))

    def test_param_surrogates(self):
        self.assertRaisesRegex(UnicodeEncodeError, "surrogates not allowed",
                               self.con.execute, "select spam(?)",
                               ("\ud803\ude6d",))

    def test_func_params(self):
        results = []
        def append_result(arg):
            results.append((arg, type(arg)))
        self.con.create_function("test_params", 1, append_result)

        dataset = [
            (42, int),
            (-1, int),
            (1234567890123456789, int),
            (4611686018427387905, int),  # 63-bit int with non-zero low bits
            (3.14, float),
            (float('inf'), float),
            ("text", str),
            ("1\x002", str),
            ("\u02e2q\u02e1\u2071\u1d57\u1d49", str),
            (b"blob", bytes),
            (bytearray(range(2)), bytes),
            (memoryview(b"blob"), bytes),
            (None, type(None)),
        ]
        for val, _ in dataset:
            cur = self.con.execute("select test_params(?)", (val,))
            cur.fetchone()
        self.assertEqual(dataset, results)

    # Regarding deterministic functions:
    #
    # Between 3.8.3 and 3.15.0, deterministic functions were only used to
    # optimize inner loops, so for those versions we can only test if the
    # sqlite machinery has factored out a call or not. From 3.15.0 and onward,
    # deterministic functions were permitted in WHERE clauses of partial
    # indices, which allows testing based on syntax, iso. the query optimizer.
    @unittest.skipIf(sqlite.sqlite_version_info < (3, 8, 3), "Requires SQLite 3.8.3 or higher")
    def test_func_non_deterministic(self):
        mock = unittest.mock.Mock(return_value=None)
        self.con.create_function("nondeterministic", 0, mock, deterministic=False)
        if sqlite.sqlite_version_info < (3, 15, 0):
            self.con.execute("select nondeterministic() = nondeterministic()")
            self.assertEqual(mock.call_count, 2)
        else:
            with self.assertRaises(sqlite.OperationalError):
                self.con.execute("create index t on test(t) where nondeterministic() is not null")

    @unittest.skipIf(sqlite.sqlite_version_info < (3, 8, 3), "Requires SQLite 3.8.3 or higher")
    def test_func_deterministic(self):
        mock = unittest.mock.Mock(return_value=None)
        self.con.create_function("deterministic", 0, mock, deterministic=True)
        if sqlite.sqlite_version_info < (3, 15, 0):
            self.con.execute("select deterministic() = deterministic()")
            self.assertEqual(mock.call_count, 1)
        else:
            try:
                self.con.execute("create index t on test(t) where deterministic() is not null")
            except sqlite.OperationalError:
                self.fail("Unexpected failure while creating partial index")

    @unittest.skipIf(sqlite.sqlite_version_info >= (3, 8, 3), "SQLite < 3.8.3 needed")
    def test_func_deterministic_not_supported(self):
        with self.assertRaises(sqlite.NotSupportedError):
            self.con.create_function("deterministic", 0, int, deterministic=True)

    def test_func_deterministic_keyword_only(self):
        with self.assertRaises(TypeError):
            self.con.create_function("deterministic", 0, int, True)

    def test_function_destructor_via_gc(self):
        # See bpo-44304: The destructor of the user function can
        # crash if is called without the GIL from the gc functions
        dest = sqlite.connect(':memory:')
        def md5sum(t):
            return

        dest.create_function("md5", 1, md5sum)
        x = dest("create table lang (name, first_appeared)")
        del md5sum, dest

        y = [x]
        y.append(y)

        del x,y
        gc_collect()

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
        self.con.create_aggregate("aggtxt", 1, AggrText)

    def tearDown(self):
        #self.cur.close()
        #self.con.close()
        pass

    def test_aggr_error_on_create(self):
        with self.assertRaises(sqlite.OperationalError):
            self.con.create_function("bla", -100, AggrSum)

    def test_aggr_no_step(self):
        cur = self.con.cursor()
        with self.assertRaises(AttributeError) as cm:
            cur.execute("select nostep(t) from test")
        self.assertEqual(str(cm.exception), "'AggrNoStep' object has no attribute 'step'")

    def test_aggr_no_finalize(self):
        cur = self.con.cursor()
        with self.assertRaises(sqlite.OperationalError) as cm:
            cur.execute("select nofinalize(t) from test")
            val = cur.fetchone()[0]
        self.assertEqual(str(cm.exception), "user-defined aggregate's 'finalize' method raised error")

    def test_aggr_exception_in_init(self):
        cur = self.con.cursor()
        with self.assertRaises(sqlite.OperationalError) as cm:
            cur.execute("select excInit(t) from test")
            val = cur.fetchone()[0]
        self.assertEqual(str(cm.exception), "user-defined aggregate's '__init__' method raised error")

    def test_aggr_exception_in_step(self):
        cur = self.con.cursor()
        with self.assertRaises(sqlite.OperationalError) as cm:
            cur.execute("select excStep(t) from test")
            val = cur.fetchone()[0]
        self.assertEqual(str(cm.exception), "user-defined aggregate's 'step' method raised error")

    def test_aggr_exception_in_finalize(self):
        cur = self.con.cursor()
        with self.assertRaises(sqlite.OperationalError) as cm:
            cur.execute("select excFinalize(t) from test")
            val = cur.fetchone()[0]
        self.assertEqual(str(cm.exception), "user-defined aggregate's 'finalize' method raised error")

    def test_aggr_check_param_str(self):
        cur = self.con.cursor()
        cur.execute("select checkTypes('str', ?, ?)", ("foo", str()))
        val = cur.fetchone()[0]
        self.assertEqual(val, 2)

    def test_aggr_check_param_int(self):
        cur = self.con.cursor()
        cur.execute("select checkType('int', ?)", (42,))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def test_aggr_check_params_int(self):
        cur = self.con.cursor()
        cur.execute("select checkTypes('int', ?, ?)", (42, 24))
        val = cur.fetchone()[0]
        self.assertEqual(val, 2)

    def test_aggr_check_param_float(self):
        cur = self.con.cursor()
        cur.execute("select checkType('float', ?)", (3.14,))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def test_aggr_check_param_none(self):
        cur = self.con.cursor()
        cur.execute("select checkType('None', ?)", (None,))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def test_aggr_check_param_blob(self):
        cur = self.con.cursor()
        cur.execute("select checkType('blob', ?)", (memoryview(b"blob"),))
        val = cur.fetchone()[0]
        self.assertEqual(val, 1)

    def test_aggr_check_aggr_sum(self):
        cur = self.con.cursor()
        cur.execute("delete from test")
        cur.executemany("insert into test(i) values (?)", [(10,), (20,), (30,)])
        cur.execute("select mysum(i) from test")
        val = cur.fetchone()[0]
        self.assertEqual(val, 60)

    def test_aggr_no_match(self):
        cur = self.con.execute("select mysum(i) from (select 1 as i) where i == 0")
        val = cur.fetchone()[0]
        self.assertIsNone(val)

    def test_aggr_text(self):
        cur = self.con.cursor()
        for txt in ["foo", "1\x002"]:
            with self.subTest(txt=txt):
                cur.execute("select aggtxt(?) from test", (txt,))
                val = cur.fetchone()[0]
                self.assertEqual(val, txt)


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
    tests = [
        AggregateTests,
        AuthorizerIllegalTypeTests,
        AuthorizerLargeIntegerTests,
        AuthorizerRaiseExceptionTests,
        AuthorizerTests,
        FunctionTests,
    ]
    return unittest.TestSuite(
        [unittest.TestLoader().loadTestsFromTestCase(t) for t in tests]
    )

def test():
    runner = unittest.TextTestRunner()
    runner.run(suite())

# hack-python - append test-suite
test_suite_list.append(suite)
"""


/*
file none
*/
/*jslint-enable*/


"""
