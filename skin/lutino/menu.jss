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
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65" crossorigin="anonymous">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4" crossorigin="anonymous"></script>
    <link rel ="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.1.1/css/all.min.css">
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
        }
        .breadcrumb-item a {
            color: #0d6efd;
            text-decoration: none;
            font-weight: 500;
        }
        .breadcrumb-item a:hover {
            color: #0a58ca;
            text-decoration: underline;
        }
        .breadcrumb-item.active {
            color: #495057;
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
        .icon-folder { color: #ffc107; }
        .icon-music { color: #e83e8c; }
        .icon-image { color: #17a2b8; }
        .icon-movie { color: #ff6b6b; }
        .icon-document { color: #28a745; }
        .icon-markdown { color: #fd7e14; }
        .icon-script { color: #6f42c1; }
        .icon-unknown { color: #6c757d; }
        .file-name {
            font-weight: 500;
            color: #212529;
        }
        .file-name a {
            color: #0d6efd;
            text-decoration: none;
            transition: color 0.2s ease;
        }
        .file-name a:hover {
            color: #0a58ca;
            text-decoration: underline;
        }
        .folder-name {
            font-weight: 600;
            color: #0d6efd;
        }
        .folder-name a {
            color: #0d6efd;
            text-decoration: none;
            transition: color 0.2s ease;
        }
        .folder-name a:hover {
            color: #0a58ca;
            text-decoration: underline;
        }
        .file-size {
            text-align: right;
            color: #6c757d;
            font-size: 0.9rem;
            min-width: 80px;
        }
        .file-date {
            color: #6c757d;
            font-size: 0.9rem;
            min-width: 150px;
        }
        .nav-back {
            margin-bottom: 1rem;
        }
        .nav-back a {
            display: inline-flex;
            align-items: center;
            gap: 0.5rem;
            color: #0d6efd;
            text-decoration: none;
            font-weight: 500;
            transition: color 0.2s ease;
        }
        .nav-back a:hover {
            color: #0a58ca;
        }
        .empty-state {
            text-align: center;
            padding: 3rem 1rem;
            color: #6c757d;
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
            <thead>
                <tr>
                    <th style="width: 50px;"></th>
                    <th>Name</th>
                    <th style="width: 100px;">Size</th>
                    <th style="width: 180px;">Modified</th>
                </tr>
            </thead>
            <tbody>
        <?
            //親ディレクトリ
            filePath = dirname(root);
            if ( filePath != ""  && filePath.indexOf(base)>=0 ){
                url = "?root="+encodeURI(filePath);
                print( "<tr><td><a href=\""+url+"\" class=\"nav-back\"><i class=\"fas fa-level-up-alt\"></i></a></td><td><a href=\""+url+"\" class=\"folder-name\">..</a></td><td></td><td></td></tr>\n");
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
                    //check each folders
                    for( i = 0 ; i < files.length ; i++ ){
                        file = files[i];
                        filePath = root+"/"+basename(file);
                        if( dir_exists(file) ){
                            //make link tag
                            stat = eval(file_stat(file));
                            url1 = "?root="+encodeURI(filePath);
                            print( "<tr><td class=\"file-icon icon-folder\"><i class=\"fas fa-folder\"></i></td><td><a href=\""+url1+"\" class=\"folder-name\">"+basename(file)+"</a></td><td></td><td class=\"file-date\">"+stat.date+"</td></tr>\n");
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
                                    url2 = fl+basename(filePath)+"?action=view.jss";
                                }else if( mime.fileType == "TYPE_DOCUMENT" ){
                                    if( ext == "md"){
                                        icon = "fab fa-markdown";
                                        iconClass = "icon-markdown";
                                        url1 = fl+basename(filePath)+"?action=MarkDownv.jss";
                                        url2 = fl+basename(filePath)+"?action=MarkDown.jss";
                                    }else{
                                        icon = "fas fa-file";
                                        iconClass = "icon-document";
                                        url1 = fl+basename(filePath);
                                        url2 = fl+basename(filePath)+"?action=MarkDown.jss";
                                    }
                                }else if( mime.fileType == "TYPE_SCRIPT" ){
                                    icon = "fas fa-code";
                                    iconClass = "icon-script";
                                    url1 = fl+basename(filePath);
                                }else{
                                    icon = "fas fa-question-circle";
                                    iconClass = "icon-unknown";
                                    url1 = fl+basename(filePath);
                                }
                                print( "<tr><td class=\"file-icon "+iconClass+"\"><i class=\""+icon+"\"></i></td><td class=\"file-name\"><a href=\""+url1+"\">"+fname+"</a></td><td class=\"file-size\">"+fileSize+"</td><td class=\"file-date\">"+stat.date+"</td></tr>\n");
                            }
                        }
                    }
                    if(files.length == 0){
                        print( "<tr><td colspan=\"4\" class=\"empty-state\"><i class=\"fas fa-inbox\"></i><p>No files in this directory</p></td></tr>\n");
                    }
                }
            }
        ?>
            </tbody>
        </table>
    </div>
</body>
</html>
