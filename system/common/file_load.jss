<?
path = _GET.path;
//読み込み処理
if (file_exists(path)) {
    code = loadFromFile(path);
    //BOM(UTF-8: EF BB BF)判定。Aceエディタは文字のみ扱いBOMを区別できないため、
    //BOMはcodeから取り除き、ヘッダでフラグとして別送する(本文はcode等の中身に
    //タブ等の制御文字が含まれてもそのまま安全に渡せるようJSON化しない)。
    bom = false;
    if (code.length >= 3 && code.substring(0,3) == '\xef\xbb\xbf') {
        bom = true;
        code = code.substring(3);
    }
    header("X-Bom: " + (bom ? "1" : "0"));
    print(code);
}
?>