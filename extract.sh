(set -e
    curl https://www.sqlite.org/2021/sqlite-autoconf-3360000.tar.gz | tar -xz
    mv sqlite-autoconf-3360000/* "$2"
    cp "$1/extension-functions.c" "$2"
    #!! ls -l
    # debug $2
    find "$(node -p 'require("path").resolve(process.argv[2])' "$@")"
    printf "\n\n\n\n"
    node --input-type=module -e '
import moduleFs from "fs";
(async function () {
    let fileDict = {};
    process.chdir(process.argv[2]);
    await Promise.all([
        "extension-functions.c",
        "sqlite3.c"
    ].map(async function (file) {
        fileDict[file] = await moduleFs.promises.readFile(file, "utf8");
    }));
    fileDict["sqlite3.c"] = fileDict["sqlite3.c"].replace((
        "/************** End of json1.c "
        + "***********************************************/"
    ), function (match0) {
        return match0 + "\n" + fileDict["extension-functions.c"];
    }).replace(
        "SQLITE_PRIVATE int sqlite3Json1Init(sqlite3*);",
        "$&\nSQLITE_PRIVATE int RegisterExtensionFunctions(sqlite3*);"
    ).replace(
        "sqlite3Json1Init,",
        "$&\nRegisterExtensionFunctions,"
    );
    await moduleFs.promises.writeFile("sqlite3.c", fileDict["sqlite3.c"]);
}());
' "$@" # '

)
