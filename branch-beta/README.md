# sqlmath - sqlite for data-science


# Status
| Branch | [master<br>(v2023.11.22)](https://github.com/sqlmath/sqlmath/tree/master) | [beta<br>(Web Demo)](https://github.com/sqlmath/sqlmath/tree/beta) | [alpha<br>(Development)](https://github.com/sqlmath/sqlmath/tree/alpha) |
|--:|:--:|:--:|:--:|
| CI | [![ci](https://github.com/sqlmath/sqlmath/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/sqlmath/sqlmath/actions?query=branch%3Amaster) | [![ci](https://github.com/sqlmath/sqlmath/actions/workflows/ci.yml/badge.svg?branch=beta)](https://github.com/sqlmath/sqlmath/actions?query=branch%3Abeta) | [![ci](https://github.com/sqlmath/sqlmath/actions/workflows/ci.yml/badge.svg?branch=alpha)](https://github.com/sqlmath/sqlmath/actions?query=branch%3Aalpha) |
| Coverage | [![coverage](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/coverage/coverage_badge.svg)](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/coverage/index.html) | [![coverage](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/coverage/coverage_badge.svg)](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/coverage/index.html) | [![coverage](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/coverage/coverage_badge.svg)](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/coverage/index.html) |
| Demo | [<img src="https://sqlmath.github.io/sqlmath/asset_image_github_brands.svg" height="32">](https://sqlmath.github.io/sqlmath/branch-beta/index.html) | [<img src="https://sqlmath.github.io/sqlmath/asset_image_github_brands.svg" height="32">](https://sqlmath.github.io/sqlmath/branch-beta/index.html) | [<img src="https://sqlmath.github.io/sqlmath/asset_image_github_brands.svg" height="32">](https://sqlmath.github.io/sqlmath/branch-beta/index.html) |
| Artifacts | [<img src="https://sqlmath.github.io/sqlmath/asset_image_folder_open_solid.svg" height="30">](https://github.com/sqlmath/sqlmath/tree/gh-pages/branch-beta/.artifact) | [<img src="https://sqlmath.github.io/sqlmath/asset_image_folder_open_solid.svg" height="30">](https://github.com/sqlmath/sqlmath/tree/gh-pages/branch-beta/.artifact) | [<img src="https://sqlmath.github.io/sqlmath/asset_image_folder_open_solid.svg" height="30">](https://github.com/sqlmath/sqlmath/tree/gh-pages/branch-beta/.artifact) |


<br><br>
# Table of Contents

1. [Web Demo](#web-demo)

2. [Quickstart Build](#quickstart-build)
    - [To build sqlmath:](#to-build-sqlmath)

3. [Quickstart Website](#quickstart-website)
    - [To serve your own sqlmath website:](#to-serve-your-own-sqlmath-website)

4. [Documentation](#documentation)
    - [API Doc](#api-doc)

5. [Package Listing](#package-listing)

6. [Changelog](#changelog)

7. [License](#license)

8. [Devops Instruction](#devops-instruction)
    - [python pypi publish](#python-pypi-publish)
    - [sqlite upgrade](#sqlite-upgrade)


<br><br>
# Web Demo
- https://sqlmath.github.io/sqlmath/index.html

[![screenshot](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/screenshot_browser__2fsqlmath_2fbranch-beta_2findex.html.png)](https://sqlmath.github.io/sqlmath/index.html)


<br><br>
# Quickstart Build


<br><br>
### To build sqlmath:
```shell
#!/bin/sh

# git clone sqlmath repo
git clone https://github.com/sqlmath/sqlmath --branch=beta --single-branch
cd sqlmath

# build nodejs binary ./_binary_sqlmath_napi8_xxx_x64.node
npm run test2

# build webassembly binary ./sqlmath_wasm.wasm
sh jslint_ci.sh shCiBuildWasm
```


<br><br>
# Quickstart Website


<br><br>
### To serve your own sqlmath website:
```shell
#!/bin/sh

# cd <sqlmath repo>

# serve website at http://localhost:8080/index.html
PORT=8080 sh jslint_ci.sh shHttpFileServer
```


<br><br>
# Documentation


<br><br>
### API Doc
- https://sqlmath.github.io/sqlmath/apidoc.html

[![screenshot](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/screenshot_browser__2f.artifact_2fapidoc.html.png)](https://sqlmath.github.io/sqlmath/apidoc.html)


<br><br>
# Package Listing
![screenshot_package_listing.svg](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/screenshot_package_listing.svg)


<br><br>
# Changelog
- [Full CHANGELOG.md](CHANGELOG.md)

![screenshot_changelog.svg](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/screenshot_changelog.svg)


<br><br>
# License
- [sqlite](https://github.com/sqlite/sqlite) is under [public domain](https://www.sqlite.org/copyright.html).
- [jslint](https://github.com/jslint-org/jslint) is under [Unlicense License](https://github.com/jslint-org/jslint/blob/master/LICENSE).
- [pcre2](https://github.com/PCRE2Project/pcre2) is under [3-Clause BSD License](https://github.com/PCRE2Project/pcre2/blob/pcre2-10.42/LICENCE)
- [zlib](https://github.com/madler/zlib) is under [zlib License](https://github.com/madler/zlib/blob/v1.2.13/LICENSE).
- [cpplint.py](cpplint.py) is under [3-Clause BSD License](https://github.com/cpplint/cpplint/blob/1.5.5/LICENSE).
- [indent.exe](indent.exe) is under [GPLv3 License](https://www.gnu.org/licenses/gpl-3.0.txt).
- Everything else is under MIT License.


<br><br>
# Devops Instruction


<br><br>
### python pypi publish
```shell
python -m build
#
twine upload --repository testpypi dist/sqlmath-2023.11.22*
py -m pip install --index-url https://test.pypi.org/simple/ sqlmath==2023.11.22
#
twine upload dist/sqlmath-2023.11.22*
pip install sqlmath==2023.11.22
```


<br><br>
### sqlite upgrade
- goto https://www.sqlite.org/changes.html
```shell
curl -L https://www.sqlite.org/2023/sqlite-autoconf-3420000.tar.gz | tar -xz
git grep "3\.39\.4\|3390400"
for FILE in .ci.sh sqlite_rollup.c
do
    sed -i -e "s|\<3\.39\.4\>|3.42.0|g" "$FILE"
    sed -i -e "s|\<3390400\>|3420000|g" "$FILE"
done
git grep "3\.39\.4\|3390400"
shRollupFetch sqlite_rollup.c
```