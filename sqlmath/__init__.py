"""sqlmath.py."""


import sys

from .sqlmath_dbapi2 import *  # noqa=F403


def debugInline(*argv): # noqa=N802
    """This function will print <arg> to stderr and then return <arg0>."""
    arg0 = argv[0] if argv else None
    print("\n\ndebugInline", file=sys.stderr)
    print(*argv, file=sys.stderr)
    print("\n")
    return arg0


def noop(arg=None):
    """This function will do nothing."""
    return arg


def test_python_run():
    """This function will run python tests."""
    import unittest

    from .sqlmath_dbapi2 import test_suite_list
    for test_suite in test_suite_list:
        results = unittest.TextTestRunner(
            verbosity=1,
            failfast=False,
        ).run(test_suite())
        if results.failures or results.errors:
            sys.exit(1)
