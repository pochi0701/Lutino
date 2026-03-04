<?  session_start();
    var database;
    var elm;
    var course_no = _GET.no;
    var content_no = _GET.contentno;
    var path = "/school/";//_SERVER.SCRIPT_NAME;
    database = DBConnect("_SYSTEM");
    var tmp1 = database.SQL("select * from content where no="+course_no+" and content_no="+content_no+";");
    if(tmp1.startsWith("[")){
        elm1 = eval(tmp1);
        content_name = elm1[0].name;
        path += course_no+'/'+elm1[0].path+'/';
    }
    var tmp = database.SQL("select * from subcontent where no="+course_no+" and content_no="+content_no+" order by sub_no;");
    if(tmp.startsWith("[")){
        elm = eval(tmp);
    }
    database.DBDisConnect();
    function formatDateTime(dt) {
        // 入力された文字列を部分に分割
        year = dt.substring(0, 4);
        month = dt.substring(4, 6);
        day = dt.substring(6, 8);
        hours = dt.substring(8, 10);
        minutes = dt.substring(10, 12);
        seconds = dt.substring(12, 14);
        // フォーマットされた文字列を返す
        return year+"/"+month+"/"+day+" "+hours+":"+minutes+":"+seconds;
    }
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>オンライン学習</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM" crossorigin="anonymous">
    <style>
        .subcontent-card-title{
            font-weight: 600;
        }
        .subcontent-card{
            height: 100%;
        }
        .subcontent-meta{
            color: #6c757d;
            font-size: 0.9rem;
        }
    </style>
</head>
<body>
    <!-- 1.ナビゲーションバーの設定 -->
    <nav class="navbar navbar-expand-lg navbar-dark bg-dark sticky-top">
        <div class="container-fluid">
            <a class="navbar-brand" href="/" target="_top">Lutino</a>
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
    <div class="container py-3">
        <div class="d-flex align-items-center justify-content-between mb-3">
            <a class="btn btn-info" href="content.jss?no=<?print(course_no);?>">コンテンツ一覧に戻る</a>
        </div>
        <h2>使い方</h2>
        <div class="card border-info mb-3">
            <div class="card-header bg-info text-white">無料コンテンツ</div>
            <div class="card-body">
                先頭のテキストが教科書となります。まず教科書を読んでください。<br>
                途中のコンテンツは例題です。テキストから見ることができますが、完了にならないときはクリックしてください。<br>
                最後に理解度テストがある教材はここから理解度テストを実施してください。
            </div>
        </div>
        <br>
        <h2><?print(content_name);?></h2>
        <div class="row row-cols-1 row-cols-md-2 row-cols-xl-3 g-3">
            <?
            for(var i = 0 ; i<elm.length ; i++){
                connect_char = (elm[i].file.indexOf("?")>=0)?"&":"?";
                if(elm[i].file.startsWith('/')){
                    path2 = "";
                }else{
                    path2 = path;
                }

                var badgeClass;
                var badgeText;
                var metaText = '';
                if(elm[i].done == 100){
                    badgeClass = 'text-bg-success';
                    badgeText = '実施完了';
                    metaText = '完了日：' + formatDateTime(elm[i].execution);
                }else if(elm[i].done >= 1) {
                    badgeClass = 'text-bg-warning';
                    badgeText = elm[i].done + '%完了';
                    metaText = '実施日：' + formatDateTime(elm[i].execution);
                }else{
                    badgeClass = 'text-bg-danger';
                    badgeText = '未完了';
                }

                var viewHref = path2+elm[i].file+connect_char+'no='+elm[i].no+'&contentno='+elm[i].content_no+'&subno='+elm[i].sub_no;

                print('<div class="col">\r\n');
                print('  <div class="card shadow-sm subcontent-card">\r\n');
                print('    <div class="card-body d-flex flex-column">\r\n');
                print('      <div class="d-flex align-items-start gap-2 mb-2">\r\n');
                print('        <div class="subcontent-card-title flex-grow-1">'+(i+1)+': '+htmlspecialchars(elm[i].name)+'</div>\r\n');
                print('        <span class="badge '+badgeClass+' text-nowrap">'+badgeText+'</span>\r\n');
                print('      </div>\r\n');
                if(metaText.length > 0){
                    print('      <div class="subcontent-meta mb-3">'+metaText+'</div>\r\n');
                }else{
                    print('      <div class="subcontent-meta mb-3">&nbsp;</div>\r\n');
                }
                print('      <div class="mt-auto d-flex justify-content-end">\r\n');
                print('        <a class="btn btn-primary" href="'+viewHref+'" target="_blank">閲覧</a>\r\n');
                print('      </div>\r\n');
                print('    </div>\r\n');
                print('  </div>\r\n');
                print('</div>\r\n');
            }
            ?>
        </div>
    </div>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js" integrity="sha384-geWF76RCwLtnZ8qwWowPQNguL3RmwHVBC9FhGdlKrxdiJJigb/j/68SIy3Te4Bkz" crossorigin="anonymous"></script>
    <script>
      (function () {
        var reloadedKey = 'lutino:reloaded:' + location.pathname + location.search;

        function isBackForwardNavigation() {
          var nav = performance.getEntriesByType && performance.getEntriesByType('navigation');
          if (nav && nav.length > 0) {
            return nav[0].type === 'back_forward';
          }
          return false;
        }

        window.addEventListener('pageshow', function () {
          if (!isBackForwardNavigation()) {
            return;
          }
          if (sessionStorage.getItem(reloadedKey) === '1') {
            return;
          }
          sessionStorage.setItem(reloadedKey, '1');
          location.reload();
        });
      })();
    </script>
  </body>
</html>