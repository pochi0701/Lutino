<html lang="ja">
<head>
    <meta charset="utf-8">
    <title>コンテンツ削除</title>
</head>
<body>
<?
var version = _GET.version;
if( version === undefined ){
    version = _GET.no;
}

if( version !== undefined && version.length > 0 ){
    var v_int = Integer.parseInt(version);
    if( !isNaN(v_int) && v_int >= 0 ){
        var database = DBConnect("_SYSTEM");
        // 1. subcontentレコード削除
        database.SQL("delete from subcontent where no=:version;".sqlBind({"version":v_int}));
        // 2. contentレコード削除
        database.SQL("delete from content where no=:version;".sqlBind({"version":v_int}));
        // 3. courseの購入状態を未購入('')に初期化
        database.SQL("update course set purchase='' where no=:version;".sqlBind({"version":v_int}));
        database.DBDisConnect();

        // 4. 教材フォルダ (html/school/{version}) の削除
        var targetDir = dirname(_SERVER.SCRIPT_FILENAME) + "/" + v_int;

        if( dir_exists(targetDir) ){
            if( _SERVER.OS == "WINDOWS" ){
                var winTarget = targetDir.replaceAll('/', '\\');
                command('cmd.exe /c rd /s /q "' + winTarget + '"');
            }else{
                command('rm -rf "' + targetDir + '"');
            }
        }
    }
}

header("Location: /school/");
?>
</body>
</html>
