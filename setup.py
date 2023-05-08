"""
setup.py.

python -m build
rm -rf build/ && python setup.py build_ext
"""

import subprocess
import sys

import setuptools
import setuptools.command.build_ext
import setuptools.command.install_lib


def build_ext():
    """This function will build c-extension."""
    subprocess.run(["sh", "jslint_ci.sh", "shCiBuildext"], check=True)
    setuptools.setup(ext_modules=[setuptools.Extension("_sqlmath", [])])


# monkey-patch setuptools to accept c-extension compiled in nodejs
setuptools.command.build_ext.build_ext.run = lambda self: self
setuptools.command.install_lib.install_lib.install = lambda self: self

if __name__ == "__main__":
    match sys.argv[1]:
        case "build_ext":
            build_ext()
        case "bdist_wheel":
            build_ext()
        case "test":
            import sqlmath
            sqlmath.test_python_run()
        case _:
            setuptools.setup()
