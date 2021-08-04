{
    "target_defaults": {
        "cflags": [
            "-std=c99"
        ],
        "conditions": [
            [
                "OS == 'win'",
                {
                    "defines": [
                        "WIN32"
                    ]
                }
            ]
        ],
        "default_configuration": "Release",
        "defines": [
            "NDEBUG"
        ],
        "include_dirs": [
            "<(SHARED_INTERMEDIATE_DIR)",
            "src"
        ],
        "msvs_settings": {
            "VCCLCompilerTool": {
                "ExceptionHandling": 1
            }
        },
        "xcode_settings": {
            "DEAD_CODE_STRIPPING": "YES",
            "GCC_GENERATE_DEBUGGING_SYMBOLS": "NO",
            "GCC_INLINES_ARE_PRIVATE_EXTERN": "YES",
            "GCC_OPTIMIZATION_LEVEL": "3",
            "OTHER_CPLUSPLUSFLAGS!": [
                "-Os",
                "-O2"
            ]
        }
    },
    "targets": [
        {
            "actions": [
                {
                    "action": [
                        "sh",
                        "./extract.sh",
                        ".",
                        "<(SHARED_INTERMEDIATE_DIR)"
                    ],
                    "action_name": "unpack_sqlite_dep",
                    "inputs": [
                        "."
                    ],
                    "outputs": [
                        "<(SHARED_INTERMEDIATE_DIR)/sqlite3.c"
                    ]
                }
            ],
            "direct_dependent_settings": {},
            "hard_dependency": 1,
            "target_name": "target_fetch",
            "type": "none"
        },
        {
            "cflags_cc": [
                "-Wno-unused-value"
            ],
            "defines": [
                "HAVE_USLEEP=1",
                "SQLITE_ENABLE_DBSTAT_VTAB=1",
                "SQLITE_ENABLE_FTS3",
                "SQLITE_ENABLE_FTS4",
                "SQLITE_ENABLE_FTS5",
                "SQLITE_ENABLE_JSON1",
                "SQLITE_ENABLE_MATH_FUNCTIONS",
                "SQLITE_ENABLE_RTREE",
                "SQLITE_THREADSAFE=1",
                "_REENTRANT=1"
            ],
            "dependencies": [
                "target_fetch"
            ],
            "sources": [
                "<(SHARED_INTERMEDIATE_DIR)/sqlite3.c"
            ],
            "target_name": "target_src",
            "type": "static_library"
        },
        {
            "cflags!": [
                "-fno-exceptions"
            ],
            "cflags_cc!": [
                "-fno-exceptions"
            ],
            "defines": [
                "NAPI_VERSION=<(napi_build_version)",
                "NAPI_DISABLE_CPP_EXCEPTIONS=1"
            ],
            "dependencies": [
                "target_src"
            ],
            "sources": [
                "src/backup.cc",
                "src/database.cc",
                "src/node_sqlite3.cc",
                "src/statement.cc"
            ],
            "target_name": "node_sqlite3",
            "xcode_settings": {
                "CLANG_CXX_LIBRARY": "libc++",
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                "MACOSX_DEPLOYMENT_TARGET": "10.7"
            }
        },
        {
            "copies": [
                {
                    "destination": "<(module_path)",
                    "files": [
                        "<(PRODUCT_DIR)/node_sqlite3.node"
                    ]
                }
            ],
            "dependencies": [
                "node_sqlite3"
            ],
            "target_name": "target_copy",
            "type": "none"
        }
    ]
}
