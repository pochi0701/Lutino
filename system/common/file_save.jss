<?
path = _POST.path;
errMsg = '<!-- no error -->';
code = _POST.code;
bom = _POST.bom;
if( code.length > 0 ){
    //BOM(UTF-8: EF BB BF)付加。Aceエディタで編集された文字列にはBOMが
    //含まれていないため、読み込み時のBOM有無フラグに従いここで復元する。
    if( (bom == '1' || bom == 'true') && code.substring(0,3) != '\xef\xbb\xbf' ){
        code = '\xef\xbb\xbf' + code;
    }
    //bakファイル作成
    bak = path+".bak";
    if( path.indexOf('/../') == -1 ){
        if( file_exists(path) ){
            copy(path,bak);
        }
        //保存
        if (! saveToFile(path, code)) {
            print( "<html><head></head><body><script type=\"text/javascript\">alert( 'error! ("+path+" not saved)' );</script></body><html>");
        }
    }
}
?>
