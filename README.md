# sqlmath - sqlite for data-science


# Status
| Branch | [master<br>(v2023.4.22)](https://github.com/sqlmath/sqlmath/tree/master) | [beta<br>(Web Demo)](https://github.com/sqlmath/sqlmath/tree/beta) | [alpha<br>(Development)](https://github.com/sqlmath/sqlmath/tree/alpha) |
|--:|:--:|:--:|:--:|
| CI | [![ci](https://github.com/sqlmath/sqlmath/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/sqlmath/sqlmath/actions?query=branch%3Amaster) | [![ci](https://github.com/sqlmath/sqlmath/actions/workflows/ci.yml/badge.svg?branch=beta)](https://github.com/sqlmath/sqlmath/actions?query=branch%3Abeta) | [![ci](https://github.com/sqlmath/sqlmath/actions/workflows/ci.yml/badge.svg?branch=alpha)](https://github.com/sqlmath/sqlmath/actions?query=branch%3Aalpha) |
| Coverage | [![coverage](https://sqlmath.github.io/sqlmath/branch-master/.artifact/coverage/coverage_badge.svg)](https://sqlmath.github.io/sqlmath/branch-master/.artifact/coverage/index.html) | [![coverage](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/coverage/coverage_badge.svg)](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/coverage/index.html) | [![coverage](https://sqlmath.github.io/sqlmath/branch-alpha/.artifact/coverage/coverage_badge.svg)](https://sqlmath.github.io/sqlmath/branch-alpha/.artifact/coverage/index.html) |
| Demo | [<img src="asset_image_github_brands.svg" height="32">](https://sqlmath.github.io/sqlmath/branch-master/index.html) | [<img src="asset_image_github_brands.svg" height="32">](https://sqlmath.github.io/sqlmath/branch-beta/index.html) | [<img src="asset_image_github_brands.svg" height="32">](https://sqlmath.github.io/sqlmath/branch-alpha/index.html) |
| Artifacts | [<img src="asset_image_folder_open_solid.svg" height="30">](https://github.com/sqlmath/sqlmath/tree/gh-pages/branch-master/.artifact) | [<img src="asset_image_folder_open_solid.svg" height="30">](https://github.com/sqlmath/sqlmath/tree/gh-pages/branch-beta/.artifact) | [<img src="asset_image_folder_open_solid.svg" height="30">](https://github.com/sqlmath/sqlmath/tree/gh-pages/branch-alpha/.artifact) |


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
- SQLite is under [public domain](https://www.sqlite.org/copyright.html).
- JSLint is under [Unlicense License](https://github.com/jslint-org/jslint/blob/master/LICENSE).
- [cpplint.py](cpplint.py) is under [BSD 3-clause license](https://github.com/cpplint/cpplint/blob/develop/LICENSE).
- [indent.exe](indent.exe) is under [GPLv3 License](https://www.gnu.org/licenses/gpl-3.0.txt).
- [sqlmath_jenks.c](sqlmath_jenks.c) is derived from [GPLv3 licensed CalcNaturalBreaks](https://www.geodms.nl/CalcNaturalBreaks).
- Everything else is under MIT License.
