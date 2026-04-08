# sqlmath

**SQLite with data science superpowers.** Run statistics, machine learning, and analytics directly in SQL queries — from Python, JavaScript, or the browser.

[![PyPI](https://img.shields.io/badge/PyPI-Install_Package-3775A9?style=for-the-badge&logo=pypi&logoColor=white)](https://pypi.org/project/sqlmath/)
[![npm](https://img.shields.io/badge/npm-Install_Package-CB3837?style=for-the-badge&logo=npm&logoColor=white)](https://www.npmjs.com/package/sqlmath)<!--no-validate-->
[![Demo](https://img.shields.io/badge/Demo-Try_Live-4285F4?style=for-the-badge&logo=googlechrome&logoColor=white)](https://sqlmath.github.io/sqlmath/index.html)
[![API Docs](https://img.shields.io/badge/Docs-API_Reference-blue?style=for-the-badge&logo=readthedocs&logoColor=white)](https://sqlmath.github.io/sqlmath/apidoc.html)

# Status

| Branch | [master<br>(v2026.3.31)](https://github.com/sqlmath/sqlmath/tree/master) | [beta<br>(Web Demo)](https://github.com/sqlmath/sqlmath/tree/beta) | [alpha<br>(Development)](https://github.com/sqlmath/sqlmath/tree/alpha) |
|--:|:--:|:--:|:--:|
| CI | [![ci](https://github.com/sqlmath/sqlmath/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/sqlmath/sqlmath/actions?query=branch%3Amaster) | [![ci](https://github.com/sqlmath/sqlmath/actions/workflows/ci.yml/badge.svg?branch=beta)](https://github.com/sqlmath/sqlmath/actions?query=branch%3Abeta) | [![ci](https://github.com/sqlmath/sqlmath/actions/workflows/ci.yml/badge.svg?branch=alpha)](https://github.com/sqlmath/sqlmath/actions?query=branch%3Aalpha) |
| Coverage | [![coverage](https://sqlmath.github.io/sqlmath/branch-master/.artifact/coverage/coverage_badge.svg)](https://sqlmath.github.io/sqlmath/branch-master/.artifact/coverage/index.html) | [![coverage](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/coverage/coverage_badge.svg)](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/coverage/index.html) | [![coverage](https://sqlmath.github.io/sqlmath/branch-alpha/.artifact/coverage/coverage_badge.svg)](https://sqlmath.github.io/sqlmath/branch-alpha/.artifact/coverage/index.html) |
| Demo | [<img src="https://sqlmath.github.io/sqlmath/asset_image_github_brands.svg" height="32">](https://sqlmath.github.io/sqlmath/branch-master/index.html) | [<img src="https://sqlmath.github.io/sqlmath/asset_image_github_brands.svg" height="32">](https://sqlmath.github.io/sqlmath/branch-beta/index.html) | [<img src="https://sqlmath.github.io/sqlmath/asset_image_github_brands.svg" height="32">](https://sqlmath.github.io/sqlmath/branch-alpha/index.html) |
| Artifacts | [<img src="https://sqlmath.github.io/sqlmath/asset_image_folder_open_solid.svg" height="30">](https://github.com/sqlmath/sqlmath/tree/gh-pages/branch-master/.artifact) | [<img src="https://sqlmath.github.io/sqlmath/asset_image_folder_open_solid.svg" height="30">](https://github.com/sqlmath/sqlmath/tree/gh-pages/branch-beta/.artifact) | [<img src="https://sqlmath.github.io/sqlmath/asset_image_folder_open_solid.svg" height="30">](https://github.com/sqlmath/sqlmath/tree/gh-pages/branch-alpha/.artifact) |


<br><br>
# Table of Contents

1. [Web Demo](#web-demo)

2. [Why sqlmath?](#why-sqlmath)

3. [Quickstart](#quickstart)
    - [Python](#python)
    - [JavaScript (Node.js)](#javascript-nodejs)
    - [Browser (WebAssembly)](#browser-webassembly)

4. [Built-in SQL Functions](#built-in-sql-functions)
    - [Math](#math)
    - [Statistics (Aggregate)](#statistics-aggregate)
    - [Statistics (Scalar)](#statistics-scalar)
    - [Arrays](#arrays)
    - [Date/Time](#datetime)
    - [Time Series / Signal Processing](#time-series--signal-processing)
    - [Compression](#compression)
    - [Type Casting](#type-casting)
    - [Cryptography](#cryptography)

5. [Machine Learning with LightGBM](#machine-learning-with-lightgbm)
    - [Training from a Table](#training-from-a-table)
    - [Prediction](#prediction)
    - [Real-World Example: Credit Card Fraud Detection](#real-world-example-credit-card-fraud-detection)
    - [LightGBM Functions](#lightgbm-functions)

6. [Use Cases](#use-cases)
    - [Financial Data Analysis](#financial-data-analysis)
    - [Embedded ML Pipelines](#embedded-ml-pipelines)
    - [Data Compression & Hashing](#data-compression--hashing)

7. [Why sqlmath vs Alternatives?](#why-sqlmath-vs-alternatives)

8. [Python API Reference](#python-api-reference)
    - [db_exec()](#db_exec)

9. [Node.js API Reference](#nodejs-api-reference)

10. [Platform Notes](#platform-notes)

11. [Building from Source](#building-from-source)
    - [Prerequisites](#prerequisites)
    - [Build](#build)
    - [Run Tests](#run-tests)
    - [Serve Demo Locally](#serve-demo-locally)

12. [Package Listing](#package-listing)

13. [Changelog](#changelog)

14. [License](#license)

15. [Devops Instruction](#devops-instruction)
    - [python pypi publish](#python-pypi-publish)
    - [sqlite upgrade](#sqlite-upgrade)


<br><br>
# Web Demo
- https://sqlmath.github.io/sqlmath/index.html

[![screenshot](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/screenshot_browser__2fsqlmath_2fbranch-beta_2findex.html.png)](https://sqlmath.github.io/sqlmath/index.html)


<br><br>
# Why sqlmath?

SQLite is everywhere — it's the most deployed database in the world. But it lacks the statistical and ML functions needed for data science. sqlmath fixes that.

**What you get:**
- **20+ built-in functions** for math, statistics, arrays, dates, and compression
- **LightGBM integration** — train and predict ML models directly in SQL
- **Zero dependencies** — single binary, no external libraries needed
- **Multi-platform** — Python, Node.js, browser (WebAssembly)


<br><br>
# Quickstart


<br><br>
### Python

```shell
pip install sqlmath
```

```python
from sqlmath import db_open, db_exec, db_close

# Open an in-memory database
db = db_open(":memory:")

# Create sample data
db_exec(db=db, sql="""
    CREATE TABLE prices (symbol TEXT, price REAL);
    INSERT INTO prices VALUES ('AAPL', 150.0), ('GOOGL', 140.0), ('MSFT', 380.0);
""")

# Use built-in statistical functions
result = db_exec(db=db, sql="""
    SELECT
        AVG(price) AS mean_price,
        MEDIAN2(price) AS median_price,
        STDEV(price) AS stdev_price
    FROM prices
""")
print(result)
# [[{'mean_price': 223.33, 'median_price': 150.0, 'stdev_price': 135.77}]]

db_close(db)
```


<br><br>
### JavaScript (Node.js)

```shell
npm install sqlmath
```

```javascript
import { dbOpenAsync, dbExecAsync, dbCloseAsync } from "sqlmath";

const db = await dbOpenAsync({ filename: ":memory:" });

await dbExecAsync({
    db,
    sql: `
        CREATE TABLE prices (symbol TEXT, price REAL);
        INSERT INTO prices VALUES ('AAPL', 150.0), ('GOOGL', 140.0), ('MSFT', 380.0);
    `
});

const result = await dbExecAsync({
    db,
    sql: "SELECT AVG(price) AS mean_price FROM prices"
});
console.log(result);

await dbCloseAsync(db);
```


<br><br>
### Browser (WebAssembly)

Try it live: **[sqlmath.github.io/sqlmath](https://sqlmath.github.io/sqlmath/index.html)**


<br><br>
# Built-in SQL Functions


<br><br>
### Math

| Function | Description |
|----------|-------------|
| `cot(x)` | Cotangent |
| `coth(x)` | Hyperbolic cotangent |
| `fmod(x, y)` | Floating-point modulo |
| `squared(x)` | Square of x |
| `squaredwithsign(x)` | Square preserving sign (negative input → negative output) |
| `sqrtwithsign(x)` | Square root preserving sign |
| `normalizewithsqrt(x)` | Normalize using square root |
| `normalizewithsquared(x)` | Normalize using square |


<br><br>
### Statistics (Aggregate)

| Function | Description |
|----------|-------------|
| `MEDIAN2(x)` | Median value (aggregate) |
| `PERCENTILE(x, p)` | Percentile at p (aggregate) |
| `QUANTILE(x, q)` | Quantile at q (aggregate) |
| `STDEV(x)` | Standard deviation (sample, window-capable) |


<br><br>
### Statistics (Scalar)

| Function | Description |
|----------|-------------|
| `marginoferror95(p, n)` | 95% margin of error: sqrt(p*(1-p)/n) |
| `random1()` | Random float in [0, 1) |


<br><br>
### Arrays

| Function | Description |
|----------|-------------|
| `doublearray_array(...)` | Create double array from values |
| `doublearray_extract(arr, i)` | Extract element at index |
| `doublearray_jsonfrom(json)` | Create array from JSON |
| `doublearray_jsonto(arr)` | Convert array to JSON |


<br><br>
### Date/Time

sqlmath extends SQLite's date functions with integer-format conversions (YYYYMMDDHHMMSS).


<br><br>
### Time Series / Signal Processing

| Function | Description |
|----------|-------------|
| `WIN_SINEFIT2(...)` | Fit sine wave to data (aggregate window function) |
| `SINEFIT_REFITLAST(...)` | Refit sine wave with new data point |


<br><br>
### Compression

| Function | Description |
|----------|-------------|
| `GZIP_COMPRESS(blob)` | Compress BLOB with gzip |
| `GZIP_UNCOMPRESS(blob)` | Decompress gzip BLOB |

> **Note:** Input must be BLOB. Use `CAST(text AS BLOB)` for text data.


<br><br>
### Type Casting

| Function | Description |
|----------|-------------|
| `castrealornull(x)` | Cast to REAL or NULL |
| `castrealorzero(x)` | Cast to REAL or 0.0 |
| `casttextorempty(x)` | Cast to TEXT or '' |
| `roundorzero(x, n)` | Round or return 0 |


<br><br>
### Cryptography

| Function | Description |
|----------|-------------|
| `sha256(data)` | SHA-256 hash (returns BLOB) |

```sql
SELECT HEX(sha256('hello')) AS hash;
-- 2CF24DBA5FB0A30E26E83B2AC5B9E29E1B161E5C1FA7425E73043362938B9824
```


<br><br>
# Machine Learning with LightGBM

sqlmath embeds [LightGBM](https://lightgbm.readthedocs.io/) for gradient boosting directly in SQL queries. Train models on your data without leaving SQL — no data shuffle to Python needed.

> **Python users:** You must call `lgbm_dlopen()` before using any `lgbm_*` functions. See [Platform Notes](#platform-notes) for details.


<br><br>
### Training from a Table

`LGBM_TRAINFROMTABLE` is an **aggregate function** — it consumes rows like `SUM()` or `AVG()`, but outputs a trained model BLOB.

```sql
-- Create a table to store the model
CREATE TABLE model_store (model BLOB);

-- Train directly from your data table
INSERT INTO model_store(model)
SELECT
    LGBM_TRAINFROMTABLE(
        -- Training parameters
        (
            'objective=binary'
            || ' learning_rate=0.1'
            || ' metric=auc'
            || ' num_leaves=31'
            || ' verbosity=0'
        ),
        50,            -- num_iterations
        10,            -- eval_step (early stopping check interval)
        'max_bin=15',  -- data parameters
        NULL,          -- reference dataset (NULL for training set)
        -- Columns: first column MUST be the label
        label, feature1, feature2, feature3, feature4
    )
FROM training_data;
```


<br><br>
### Prediction

```sql
-- Predict on new data using the stored model
SELECT
    id,
    LGBM_PREDICTFORTABLE(
        (SELECT model FROM model_store),
        0,     -- predict_type (0 = normal probability)
        0,     -- start_iteration
        50,    -- num_iterations
        '',    -- prediction parameters
        feature1, feature2, feature3, feature4
    ) AS prediction
FROM test_data;
```


<br><br>
### Real-World Example: Credit Card Fraud Detection

From the [Kaggle notebook](https://www.kaggle.com/code/kaizhu256/sql-is-all-you-need) — training on 284,807 transactions with 0.17% fraud rate:

```python
from sqlmath import db_open, db_exec, db_table_import

db = db_open("fraud.sqlite")

# Import Kaggle credit card dataset
db_table_import(db=db, filename="creditcard.csv", table_name="transactions", mode="csv")

# Split 80/20 train/test
db_exec(db=db, sql="""
    CREATE TABLE train AS SELECT * FROM transactions WHERE RANDOM() % 5 != 0;
    CREATE TABLE test AS SELECT * FROM transactions WHERE RANDOM() % 5 = 0;
""")

# Train with automatic class imbalance handling
db_exec(db=db, sql="""
    CREATE TABLE model_store (model BLOB);

    INSERT INTO model_store(model)
    SELECT
        LGBM_TRAINFROMTABLE(
            (
                'objective=binary'
                || ' learning_rate=0.05'
                || ' metric=auc'
                || ' is_unbalance=true'  -- Auto-weight minority class
                || ' num_leaves=31'
                || ' verbosity=0'
            ),
            100,           -- num_iterations
            10,            -- eval_step
            'max_bin=255', -- data_params
            NULL,          -- reference
            -- First column = label, rest = features (V1-V28 + Amount)
            Class, V1, V2, V3, V4, V5, V6, V7, V8, V9, V10,
            V11, V12, V13, V14, V15, V16, V17, V18, V19, V20,
            V21, V22, V23, V24, V25, V26, V27, V28, Amount
        )
    FROM train;
""")
```


<br><br>
### LightGBM Functions

| Function | Type | Description |
|----------|------|-------------|
| `lgbm_dlopen(path)` | scalar | Load LightGBM library (required in Python) |
| `LGBM_TRAINFROMTABLE(params, n_iter, eval, data_params, ref, label, ...)` | aggregate | Train model from table columns |
| `LGBM_TRAINFROMFILE(params, n_iter, eval, train_file, data_params, test_file)` | scalar | Train from LibSVM file |
| `LGBM_PREDICTFORTABLE(model, type, start, n, params, ...)` | window | Predict per row |
| `LGBM_PREDICTFORFILE(model, type, start, n, params, file, header, out)` | scalar | Predict and save to file |
| `lgbm_extract(result, key)` | scalar | Extract value from result |

**Complete Example:**

[![Kaggle](https://img.shields.io/badge/Kaggle-SQL_is_All_You_Need-20BEFF?style=for-the-badge&logo=kaggle&logoColor=white)](https://www.kaggle.com/code/kaizhu256/sql-is-all-you-need)

Full notebook with fraud detection, intraday trading signals, and model persistence.

**Kaggle Environment:**
- Linux x64, Python 3.12, Node.js 24
- Datasets: [Credit Card Fraud](https://www.kaggle.com/datasets/mlg-ulb/creditcardfraud) (284K transactions), SPY intraday OHLCV
- sqlmath installs via `pip install sqlmath==2026.3.31`


<br><br>
# Use Cases


<br><br>
### Financial Data Analysis

```python
from sqlmath import db_open, db_exec, db_close, db_table_import

db = db_open(":memory:")

# Import OHLCV data
db_table_import(db=db, table_name="prices", filename="prices.csv", mode="csv")

# Calculate rolling statistics in SQL
result = db_exec(db=db, sql="""
    SELECT
        date,
        close,
        AVG(close) OVER (ORDER BY date ROWS 20 PRECEDING) AS sma_20,
        STDEV(close) OVER (ORDER BY date ROWS 20 PRECEDING) AS volatility,
        PERCENTILE(volume, 50) OVER (ORDER BY date ROWS 5 PRECEDING) AS median_volume
    FROM prices
    ORDER BY date DESC
    LIMIT 10
""")

db_close(db)
```


<br><br>
### Embedded ML Pipelines

Train and deploy models without data movement:

```python
# 1. Load data into SQLite
db_table_import(db=db, table_name="features", filename="training_data.csv", mode="csv")

# 2. Train model in-database
db_exec(db=db, sql="""
    CREATE TABLE models AS
    SELECT LGBM_TRAINFROMTABLE(...) AS model FROM features
""")

# 3. Score new data in-database
db_exec(db=db, sql="""
    SELECT id, LGBM_PREDICTFORTABLE(model, ...) AS score
    FROM new_data, models
""")

# 4. Export results
db_file_save(db=db, filename="scored_data.sqlite")
```


<br><br>
### Data Compression & Hashing

```sql
-- Compress large text fields
UPDATE documents SET
    content_compressed = GZIP_COMPRESS(CAST(content AS BLOB));

-- Generate content hashes for deduplication
SELECT HEX(sha256(content)) AS content_hash, COUNT(*)
FROM documents
GROUP BY content_hash
HAVING COUNT(*) > 1;
```


<br><br>
# Why sqlmath vs Alternatives?

| Feature | sqlmath | pandas | DuckDB | sqlite3 |
|---------|---------|--------|--------|---------|
| In-database ML | ✅ LightGBM | ❌ | ❌ | ❌ |
| Zero data shuffle | ✅ | ❌ | ✅ | ✅ |
| Statistical functions | ✅ 20+ | ✅ | ✅ | ❌ |
| Browser support | ✅ WASM | ❌ | ❌ | ❌ |
| Single file deployment | ✅ | ❌ | ✅ | ✅ |
| Memory efficiency | ✅ Streaming | ❌ In-memory | ✅ | ✅ |

**Choose sqlmath when:**
- You need ML training/inference inside the database
- Your data is already in SQLite
- You want browser-based analytics (WebAssembly)
- You need statistical functions beyond basic SQL

**Choose pandas when:**
- You need complex data transformations
- Interactive exploration with rich visualization
- Your workflow is Python-centric

**Choose DuckDB when:**
- You need fast analytical queries on large datasets
- You want pandas DataFrame interop
- OLAP workloads are primary


<br><br>
# Python API Reference

```python
from sqlmath import (
    db_open,           # Open database
    db_close,          # Close database
    db_exec,           # Execute SQL, return results
    db_exec_and_return_lastblob,  # Execute SQL, return last blob
    db_file_load,      # Load database from file
    db_file_save,      # Save database to file
    db_table_import,   # Import data into table
    db_noop,           # No-op for testing
)
```


<br><br>
### db_exec()

```python
result = db_exec(
    db=db,                    # Database connection
    sql="SELECT ...",         # SQL statement(s)
    bind_list=[...],          # Bind parameters (list or dict)
    response_type=None,       # None (default) returns list-of-dicts
                              # "list" | "lastblob" | "arraybuffer"
)
# Default: [[{'col1': val1, 'col2': val2}, ...]]
```


<br><br>
# Node.js API Reference
- https://sqlmath.github.io/sqlmath/apidoc.html

[![screenshot](https://sqlmath.github.io/sqlmath/branch-beta/.artifact/screenshot_browser__2f.artifact_2fapidoc.html.png)](https://sqlmath.github.io/sqlmath/apidoc.html)


<br><br>
# Platform Notes

The JavaScript (Node.js) binding is more mature than Python. Key differences:

| Feature | Node.js | Python |
|---------|---------|--------|
| Type hints | N/A | ❌ Not yet |
| Context manager | N/A | ❌ `with db_open()` not supported |

> **Note:** The Python wrapper is a work in progress. Contributions welcome!


<br><br>
# Building from Source


<br><br>
### Prerequisites

- Node.js 24+
- Python 3.12+
- C compiler (gcc, clang, or MSVC)


<br><br>
### Build

```shell
#!/bin/sh

# git clone sqlmath repo
git clone https://github.com/sqlmath/sqlmath --branch=beta --single-branch
cd sqlmath

# Build native binary
npm run test2

# Build WebAssembly (optional)
sh jslint_ci.sh shCiBuildWasm
```


<br><br>
### Run Tests

```shell
# Full test suite (includes full clean-build)
npm run test2

# Full test suite (includes partial re-build of modified files)
npm run test

# Quick test (skip build)
npm run test --fast
```


<br><br>
### Serve Demo Locally

```shell
PORT=8080 sh jslint_ci.sh shHttpFileServer
# Open http://localhost:8080/index.html
```


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
- [zlib](https://github.com/madler/zlib) is under [zlib License](https://github.com/madler/zlib/blob/v1.3.1/LICENSE).
- [cpplint.py](cpplint.py) is under [3-Clause BSD License](https://github.com/cpplint/cpplint/blob/2.0.0/LICENSE).
- [indent.exe](indent.exe) is under [GPLv3 License](https://www.gnu.org/licenses/gpl-3.0.txt)<!--no-validate-->.
- Everything else is under MIT License.


<br><br>
# Devops Instruction


<br><br>
### python pypi publish
```shell
python -m build
#
twine upload --repository testpypi dist/sqlmath-2026.3.31*
py -m pip install --index-url https://test.pypi.org/simple/ sqlmath==2026.3.31
#
twine upload dist/sqlmath-2026.3.31*
pip install sqlmath==2026.3.31
```


<br><br>
### sqlite upgrade
- goto https://www.sqlite.org/changes.html
```shell
    (set -e
    #
    # lgbm
    sh jslint_ci.sh shRollupUpgrade "v4.5.0" "v4.6.0" ".ci.sh sqlmath_base.h"
    #
    # sqlite
    sh jslint_ci.sh shRollupUpgrade "3.50.3" "3.50.4" ".ci.sh sqlmath_external_sqlite.c"
    sh jslint_ci.sh shRollupUpgrade "3500300" "3500400" ".ci.sh sqlmath_external_sqlite.c"
    #
    # shSqlmathUpdate
    read -p "Press Enter to shSqlmathUpdate:"
    sh jslint_ci.sh shSqlmathUpdate
    )
```
