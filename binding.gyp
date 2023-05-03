{
    "targets": [
        {
            "cflags": [
                "-Wall",
                "-std=c99"
            ],
            "conditions": [
                [
                    "OS == 'win'",
                    {
                        "defines": [
                            "WIN32"
                        ]
                    },
                    {
                        "defines": [
                            "HAVE_UNISTD_H"
                        ],
                        "link_settings": {
                            "libraries": [
                                "-lm"
                            ]
                        }
                    }
                ]
            ],
            "defines": [
                "SQLMATH_NODEJS_C2"
            ],
            "libraries": [
                "sqlite3_rollup.lib"
            ],
            "msvs_settings": {
                "VCCLCompilerTool": {
                    "WarningLevel": 3
                }
            },
            "sources": [
                "sqlmath_base.c"
            ],
            "target_name": "binding",
            "xcode_settings": {
                "OTHER_CFLAGS": [
                    "-Wall",
                    "-std=c99"
                ]
            }
        },
        {
            "copies": [
                {
                    "destination": "build/",
                    "files": [
                        "<(PRODUCT_DIR)/binding.node"
                    ]
                }
            ],
            "dependencies": [
                "binding"
            ],
            "target_name": "target_copy",
            "type": "none"
        }
    ]
}
