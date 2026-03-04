<?  session_start();
    var scriptn  = _SERVER.SCRIPT_NAME;
    var base = "http://"+_SERVER.SERVER_ADDR+":"+_SERVER.SERVER_PORT+dirname(scriptn)+"/initSQL.jss";
    if(file_exists(_SERVER.DOCUMENT_ROOT+dirname(scriptn)+"/initSQL.jss")){
        loadFromFile(base);
    }
    var course_no;
    var database;
    database = DBConnect("_SYSTEM");
    var tmp = database.SQL("select * from course order by no;");
    var elm;
    if(tmp.startsWith("[") ){
        elm = eval(tmp);
    }else{
        elm = undefined;
    }
    database.DBDisConnect();
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>コース一覧[オンライン学習]</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM" crossorigin="anonymous">
    <style>
        .content-card-title{
            font-weight: 600;
        }
        .content-card-detail{
            color: #6c757d;
        }
        .content-card{
            height: 100%;
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

        <h2>お知らせ</h2>
        <div class="card border-info mb-3">
            <div class="card-header bg-info text-white">コンテンツについて</div>
            <div class="card-body">
                「Lutinoの使い方紹介」は無料コンテンツとなっています。<br>
                ご自由にご利用ください。タイトルの下部をクリックすると、コンテンツに移動します。
            </div>
        </div>

        <h2>コース一覧</h2>
        <div class="row row-cols-1 row-cols-md-2 row-cols-xl-3 g-3">
            <?
            for(var i = 0 ; i < elm.length ; i++){
                var isPurchased = (elm[i].purchase.length > 0);
                var badgeClass = isPurchased ? 'text-bg-success' : 'text-bg-secondary';
                var badgeText = isPurchased ? '購入済' : '未購入';
                var actionHref = isPurchased ? ('content.jss?no='+elm[i].no) : ('get_content.jss?version='+elm[i].no);
                var actionText = isPurchased ? 'コンテンツを開く' : '購入';

                print('<div class="col">\r\n');
                print('  <div class="card shadow-sm content-card">\r\n');
                print('    <div class="card-body d-flex flex-column">\r\n');
                print('      <div class="d-flex align-items-start gap-2 mb-2">\r\n');
                print('        <div class="content-card-title flex-grow-1">'+htmlspecialchars(elm[i].name)+'</div>\r\n');
                print('        <span class="badge '+badgeClass+' text-nowrap">'+badgeText+'</span>\r\n');
                print('      </div>\r\n');
                print('      <div class="mt-auto d-flex justify-content-end">\r\n');
                print('        <a class="btn btn-primary" href="'+actionHref+'">'+actionText+'</a>\r\n');
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