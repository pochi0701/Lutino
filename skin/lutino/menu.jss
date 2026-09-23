<?
//URLエンコード
function size_num_read(size) {
    bytes = ["B","KB","MB","GB","TB"];
    let pivot = 0;
    for( let i=0 ; i < 5 ; i++) {
        pivot = i;
        if(size > 1024){
            size = size / 1024;
        }else{
            pivot = i;
            break;
        }
    }
    //return "<span style=\"font-size: small\">".	round($size, 2).$val."</span>";
    return Math.round((size*100)/100)+bytes[pivot];
}
//変数取得
base    = _SERVER.DOCUMENT_ROOT;
root    = _GET.root;

//ユーザ限定処理
//初回の処理
if( root.length==0){
    root = _SERVER.DOCUMENT_ROOT;
}
sf = root.substring(base.length,root.length);
if( sf == ""){
   sf = "/";
}
//右端の/をなくす
while( root[root.length-1] == "/" ){
    root = root.substring(0,root.length-1);
}
me=_SERVER.SCRIPT_NAME;
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>file tree - <? print(sf); ?></title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB" crossorigin="anonymous">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/js/bootstrap.bundle.min.js" integrity="sha384-FKyoEForCGlyvwx9Hj09JcYn3nv7wiPVlz7YYwJrWVcXK/BmnVDxM+D2scQbITxI" crossorigin="anonymous"></script>
    <link rel ="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/7.3.1/css/all.min.css">
    <style>
        body {
            background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
            min-height: 100vh;
        }
        .container {
            margin-top: 2rem;
            margin-bottom: 2rem;
        }
        .breadcrumb {
            background-color: rgba(255, 255, 255, 0.9);
            border-radius: 0.5rem;
            padding: 1rem;
            margin-bottom: 1.5rem;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
            /* 現在位置の色は Bootstrap の breadcrumb 変数で指定する */
            --bs-breadcrumb-item-active-color: #495057;
        }
        .breadcrumb-item a {
            color: var(--bs-link-color);
            text-decoration: none;
            font-weight: 500;
        }
        .breadcrumb-item a:hover {
            color: var(--bs-link-hover-color);
            text-decoration: underline;
        }
        .breadcrumb-item.active {
            font-weight: 600;
        }
        .file-table {
            background: white;
            border-radius: 0.5rem;
            overflow: hidden;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        }
        .file-table thead {
            background: linear-gradient(135deg, #2c3e50 0%, #34495e 100%);
            color: white;
            font-weight: 600;
        }
        .file-table thead th {
            border: none;
            padding: 1rem;
            vertical-align: middle;
        }
        .file-table tbody tr {
            border-bottom: 1px solid #e9ecef;
            transition: background-color 0.2s ease;
        }
        .file-table tbody tr:hover {
            background-color: #f8f9fa;
        }
        .file-table tbody tr:last-child {
            border-bottom: none;
        }
        .file-table td {
            padding: 0.75rem 1rem;
            vertical-align: middle;
        }
        .file-icon {
            font-size: 1.25rem;
            min-width: 2rem;
            text-align: center;
        }
        /*
          Bootstrap 5.3 はセル(th/td)へ color を直接指定するようになったため、
          セルに対する色は --bs-table-color を通して Bootstrap 側に渡す。
          色そのものも Bootstrap のテーマ変数を使う。
        */
        .icon-folder { --bs-table-color: var(--bs-warning); }
        .icon-music { --bs-table-color: var(--bs-pink); }
        .icon-image { --bs-table-color: var(--bs-info); }
        .icon-movie { --bs-table-color: var(--bs-danger); }
        .icon-document { --bs-table-color: var(--bs-success); }
        .icon-markdown { --bs-table-color: var(--bs-orange); }
        .icon-script { --bs-table-color: var(--bs-purple); }
        .icon-unknown { --bs-table-color: var(--bs-secondary); }
        .file-name {
            font-weight: 500;
            --bs-table-color: var(--bs-body-color);
        }
        .file-name a {
            color: var(--bs-link-color);
            text-decoration: none;
            transition: color 0.2s ease;
        }
        .file-name a:hover {
            color: var(--bs-link-hover-color);
            text-decoration: underline;
        }
        .folder-name {
            font-weight: 600;
            color: var(--bs-link-color);
        }
        .folder-name a {
            color: var(--bs-link-color);
            text-decoration: none;
            transition: color 0.2s ease;
        }
        .folder-name a:hover {
            color: var(--bs-link-hover-color);
            text-decoration: underline;
        }
        .file-size {
            text-align: right;
            font-size: 0.9rem;
            min-width: 80px;
            --bs-table-color: var(--bs-secondary-color);
        }
        .file-date {
            font-size: 0.9rem;
            min-width: 150px;
            --bs-table-color: var(--bs-secondary-color);
        }
        /* クラスは <a> 自身に付くので子孫セレクタにしない */
        .nav-back {
            display: inline-flex;
            align-items: center;
            gap: 0.5rem;
            color: var(--bs-link-color);
            text-decoration: none;
            font-weight: 500;
            transition: color 0.2s ease;
        }
        .nav-back:hover {
            color: var(--bs-link-hover-color);
        }
        .empty-state {
            text-align: center;
            padding: 3rem 1rem;
            --bs-table-color: var(--bs-secondary-color);
        }
        .empty-state i {
            font-size: 3rem;
            margin-bottom: 1rem;
            opacity: 0.5;
        }
    </style>
    <script type="text/javascript">
    <!--
        var root = "<? print( root ); ?>";
        function lutinoDownload(url){
            var a = document.createElement('a');
            a.href = url;
            a.setAttribute('download', '');
            a.style.display = 'none';
            document.body.appendChild(a);
            a.click();
            a.remove();
        }

        function lutinoCopy(text){
            if (navigator.clipboard && navigator.clipboard.writeText) {
                navigator.clipboard.writeText(text);
            } else {
                var ta = document.createElement('textarea');
                ta.value = text;
                ta.style.position = 'fixed';
                ta.style.left = '-1000px';
                document.body.appendChild(ta);
                ta.select();
                try { document.execCommand('copy'); } catch(e) {}
                ta.remove();
            }
        }

        function lutinoShowMenu(ev, id){
            ev.preventDefault();
            ev.stopPropagation();
            var el = document.getElementById(id);
            if(!el) return;
            var dd = bootstrap.Dropdown.getOrCreateInstance(el);
            dd.show();
        }
        // -->
    </script>
    <style>
        td.fileicon { width: 2.2rem; }
        .icon-btn { color: inherit; text-decoration: none; cursor: pointer; display: inline-block; padding: 0.15rem 0.25rem; }
        .dropdown-menu { min-width: 12rem; }
    </style>
</head>
<body>
    <!-- ナビゲーションバー -->
    <nav class="navbar navbar-expand-lg navbar-dark bg-dark sticky-top">
        <div class="container-fluid">
            <a class="navbar-brand" href="/" target="_top">
                <i class="fas fa-folder-open"></i> Lutino
            </a>
            <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
                <span class="navbar-toggler-icon"></span>
            </button>
            <div class="collapse navbar-collapse flex-grow-1 text-end" id="myNavbar">
                <ul class="navbar-nav ms-auto flex-nowrap">
                    <li><a href="/" class="nav-link m-2 menu-item" target="_top">Home</a></li>
                </ul>
            </div>
        </div>
    </nav>

    <div class="container">
        <!-- パンくずリスト -->
        <nav aria-label="breadcrumb">
            <ol class="breadcrumb mb-0">
                <?
                    pathParts = sf.split("/");
                    currentPath = "";
                    print("<li class=\"breadcrumb-item\"><a href=\"?root="+encodeURI(base)+"\"><i class=\"fas fa-home\"></i> Home</a></li>\n");
                    
                    for(let j = 1; j < pathParts.length; j++) {
                        if(pathParts[j] != "") {
                            currentPath = currentPath + "/" + pathParts[j];
                            if(j == pathParts.length - 1) {
                                print("<li class=\"breadcrumb-item active\">"+pathParts[j]+"</li>\n");
                            } else {
                                pathUrl = base + currentPath;
                                print("<li class=\"breadcrumb-item\"><a href=\"?root="+encodeURI(pathUrl)+"\">"+pathParts[j]+"</a></li>\n");
                            }
                        }
                    }
                ?>
            </ol>
        </nav>

        <!-- ファイルテーブル -->
        <table class="table file-table table-hover mb-0">
            <thead class="table-dark">
                <tr>
                    <th style="width: 50px;"></th>
                    <th>Name</th>
                    <th style="width: 100px;">Size</th>
                    <th style="width: 180px;">Modified</th>
                    <th style="width: 120px;">Actions</th>
                </tr>
            </thead>
            <tbody>
        <?
            //親ディレクトリ
            filePath = dirname(root);
            if ( filePath != ""  && filePath.indexOf(base)>=0 ){
                url = "?root="+encodeURI(filePath);
                print( "<tr><td><a href=\""+url+"\" class=\"nav-back\"><i class=\"fas fa-level-up-alt\"></i></a></td><td><a href=\""+url+"\" class=\"folder-name\">..</a></td><td></td><td></td><td></td></tr>\n");
            }
            //ディレクトリの場合
            if (dir_exists(root)) {
                //ディレクトリ読み込み
                files = eval(scandir(root));
                for( i = files.length-1; i>=0 ;i--){
                    if( basename(files[i]) == "." || basename(files[i]) == ".." ){
                        files.remove(files[i]);
                    }
                }
                if( files.length>0 ){
                    let url1 = "";//URL表示用
                    let url2 = "";//URL編集用
                    let url3 = "";//URLダウンロード用
                    //check each folders
                    for( i = 0 ; i < files.length ; i++ ){
                        file = files[i];
                        filePath = root+"/"+basename(file);
                        if( dir_exists(file) ){
                            //make link tag
                            stat = eval(file_stat(file));
                            //フォルダ内にindex.html/index.jss/index.mdがあれば、展開せずにそちらを実行する
                            fld = filePath;
                            if( fld.indexOf(base)>=0){
                                fld = fld.substring(base.length,fld.length);
                            }
                            if(fld.substr(fld.length-1,1) != "/"){
                                fld = fld+"/";
                            }
                            if( file_exists(filePath+"/index.html") ){
                                url1 = fld+"index.html";
                            }else if( file_exists(filePath+"/index.jss") ){
                                url1 = fld+"index.jss";
                            }else if( file_exists(filePath+"/index.md") ){
                                url1 = fld+"index.md?action=/system/MarkDownv.jss";
                            }else{
                                url1 = "?root="+encodeURI(filePath);
                            }
                            print( "<tr><td class=\"file-icon icon-folder\"><i class=\"fas fa-folder\"></i></td><td><a href=\""+url1+"\" class=\"folder-name\">"+basename(file)+"</a></td><td></td><td class=\"file-date\">"+stat.date+"</td><td></td></tr>\n");
                        }
                    }
                    //check each files.
                    for( i=0 ; i < files.length ; i++ ){
                        file = files[i];
                        filePath = root+"/"+basename(file);
                        if( file_exists(filePath) ){
                            fl = filePath;
                            if( fl.indexOf(base)>=0){
                                fl = dirname(fl.substring(base.length,fl.length));
                            }
                            if(fl.substr(fl.length-1,1) != "/"){
                                fl = fl+"/";
                            }
                            ext = extractFileExt(file).toLowerCase();
                            if( ext != "bak"){
                                fname = basename(file);//.nkfconv("Sw");
                                //make link tag
                                stat = eval(file_stat(filePath));
                                mime = eval(mimeInfo(filePath));
                                fileSize = size_num_read(stat.size);
                                url3 = fl+basename(filePath);
                                if( mime.fileType == "TYPE_MUSIC"){
                                    if( ext == "mp3" ){
                                        var mp3=eval(JSON.mp3id3tag(filePath));
                                        if( mp3.title.length > 0 ){
                                            fname = mp3.title;
                                        }
                                    }
                                    icon = "fas fa-music";
                                    iconClass = "icon-music";
                                    url1 = fl+basename(filePath)+"?action=audio.jss";
                                    url2 = "";
                                }else if( mime.fileType == "TYPE_IMAGE" ){
                                    icon = "fas fa-image";
                                    iconClass = "icon-image";
                                    url1 = fl+basename(filePath)+"?action=ImageView.jss";
                                    url2 = "";
                                }else if( mime.fileType == "TYPE_MOVIE" ){
                                    icon = "fas fa-film";
                                    iconClass = "icon-movie";
                                    url1 = fl+basename(filePath)+"?action=preview.jss";
                                    url2 = "";
                                }else if( mime.fileType == "TYPE_DOCUMENT" ){
                                    if( ext == "md"){
                                        icon = "fab fa-markdown";
                                        iconClass = "icon-markdown";
                                        url1 = fl+basename(filePath)+"?action=/system/MarkDownv.jss";
                                        url2 = fl+basename(filePath)+"?action=/system/MarkDown.jss";
                                    }else{
                                        icon = "fas fa-file";
                                        iconClass = "icon-document";
                                        url1 = fl+basename(filePath);
                                        url2 = fl+basename(filePath)+"?action=/system/edit.jss";
                                    }
                                }else if( mime.fileType == "TYPE_SCRIPT" ){
                                    icon = "fas fa-code";
                                    iconClass = "icon-script";
                                    url1 = fl+basename(filePath);
                                    url2 = fl+basename(filePath)+"?action=/system/edit.jss";
                                }else{
                                    icon = "fas fa-question-circle";
                                    iconClass = "icon-unknown";
                                    url1 = fl+basename(filePath);
                                    url2 = "";
                                }
                                let actions = "";
                                if( url2.length > 0 ){
                                    actions += "<a href=\""+url2+"\" class=\"icon-btn\" title=\"Edit\" target=\"_blank\"><i class=\"fas fa-edit\"></i></a> ";
                                }
                                actions += "<a href=\"javascript:void(0)\" onclick=\"lutinoDownload('"+url3+"')\" class=\"icon-btn\" title=\"Download\"><i class=\"fas fa-download\"></i></a>";
                                print( "<tr><td class=\"file-icon "+iconClass+"\"><i class=\""+icon+"\"></i></td><td class=\"file-name\"><a href=\""+url1+"\">"+fname+"</a></td><td class=\"file-size\">"+fileSize+"</td><td class=\"file-date\">"+stat.date+"</td><td>"+actions+"</td></tr>\n");
                            }
                        }
                    }
                    if(files.length == 0){
                        print( "<tr><td colspan=\"5\" class=\"empty-state\"><i class=\"fas fa-inbox\"></i><p>No files in this directory</p></td></tr>\n");
                    }
                }
            }
        ?>
            </tbody>
        </table>
    </div>
</body>
</html>
