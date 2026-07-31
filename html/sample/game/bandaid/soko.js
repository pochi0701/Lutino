//ウィンドウサイズ
var WIDTH=420;
var HEIGHT=290;

// URLパラメータからレベルを取得 (?level=N、デフォルト1、範囲1-90)
function getLevel() {
    var m = window.location.search.match(/[?&]level=(\d+)/);
    var n = m ? parseInt(m[1]) : 1;
    return (n >= 1 && n <= 90) ? n : 1;
}
var currentLevel = getLevel();

var xmlHttp;
var mX;
var mY;
var Fd;
var xml;
var mB=0;
var X;
var Y;
var gl;
var bg;
var wGap=20;
var hGap=20;
var Cood = function(xx, yy) {
    this.x = xx;
    this.y = yy;
};
let ctx;
var GoalCHK=0;
var change=0;

var img;
        // onload イベント
        window.onload = function() {
    ctx = document.getElementById('mainWindow').getContext('2d'); //描画するコンテキスト
    img = new Image();
    img.src = "./game.gif";
            img.onload = function() {
              loadText();
            };
            img.onerror = function() {
                alert("画像読み込みに失敗しました。");
            };
                
        };

        // mousedown イベント
        document.getElementById('mainWindow').addEventListener('mousedown', function(event) {
            var rect = event.currentTarget.getBoundingClientRect();
            var x = (event.clientX - rect.left) * event.currentTarget.width / rect.width;
            var y = (event.clientY - rect.top) * event.currentTarget.height / rect.height;
            mouseDown(x, y);
        });

        // 矢印キー イベント
        document.addEventListener('keydown', function(event) {
            var directions = {
                ArrowUp: [-1, 0],
                ArrowDown: [1, 0],
                ArrowLeft: [0, -1],
                ArrowRight: [0, 1]
            };
            var direction = directions[event.key];
            if (!direction) return;

            event.preventDefault();
            movePlayer(direction[0], direction[1]);
        });
        
function loadText(){
    GoalCHK = 0;
    // ステージラベルを更新
    var lbl = document.getElementById('stage-label');
    if (lbl) lbl.textContent = 'STAGE ' + currentLevel;
    if (window.XMLHttpRequest){
        xmlHttp = new XMLHttpRequest();
    }else{
        if (window.ActiveXObject){
            xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
        }else{
            xmlHttp = null;
        }
    }
    xmlHttp.onreadystatechange = checkStatus;
    xmlHttp.open("GET", "./screen/screen." + currentLevel, true);
    xmlHttp.send(null);
}
//読み込み完了した
function checkStatus(){
    var i, j;
    if (xmlHttp.readyState == 4 && xmlHttp.status == 200){
        var raw = xmlHttp.responseText.split("\n");

        // フォーマット自動検出:
        // Format A (screen.1,2): 先頭2行が数値(幅・高さ)
        // Format B (screen.3+) : マップデータが直接始まる
        var n0 = parseInt(raw[0], 10);
        var n1 = parseInt(raw[1], 10);
        var mapLines;
        if (!isNaN(n0) && !isNaN(n1) && n0 > 0 && n0 < 200 && n1 > 0 && n1 < 200) {
            mX = n0; mY = n1;
            mapLines = raw.slice(2, 2 + mY);
        } else {
            // 末尾の空行を除去してmX/mYを計算
            mapLines = raw.slice();
            while (mapLines.length > 0 && mapLines[mapLines.length - 1].trim() === '') {
                mapLines.pop();
            }
            mY = mapLines.length;
            mX = 0;
            for (i = 0; i < mY; i++) {
                if (mapLines[i].length > mX) mX = mapLines[i].length;
            }
        }

        // フィールド初期化(壁で埋める)
        mB = 0;
        Fd = new Array(mY);
        for (i = 0; i < mY; i++) {
            Fd[i] = new Array(mX);
            for (j = 0; j < mX; j++) Fd[i][j] = 1;
        }

        // マップ解析
        // 値: 0=道 1=壁 2=ボール 3=プレイヤー 4=ゴール 5=ボール+ゴール(*)  6=プレイヤー+ゴール(+)
        for (i = 0; i < mY; i++) {
            var s2 = mapLines[i] || '';
            for (j = 0; j < mX; j++) {
                var tch = s2.charAt(j);
                switch(tch){
                    case ' ': Fd[i][j] = 0; break;
                    case '#': Fd[i][j] = 1; break;
                    case '$': Fd[i][j] = 2; mB++; break;
                    case '@': Fd[i][j] = 3; X = i; Y = j; break;
                    case '.': Fd[i][j] = 4; break;
                    case '*': Fd[i][j] = 5; mB++; break; // ボール+ゴール
                    case '+': Fd[i][j] = 6; X = i; Y = j; break; // プレイヤー+ゴール
                }
            }
        }

        // ゴール・ボール位置を配列に登録
        gl = new Array(mB);
        bg = new Array(mB);
        for (i = 0; i < mB; i++) {
            gl[i] = new Cood(0,0);
            bg[i] = new Cood(0,0);
        }
        var j1 = 0, k1 = 0;
        for (var l1 = 0; l1 < mX; l1++) {
            for (var i2 = 0; i2 < mY; i2++) {
                // ゴール位置(4=ゴール空, 5=ボール+ゴール, 6=プレイヤー+ゴール)
                if (Fd[i2][l1] == 4 || Fd[i2][l1] == 5 || Fd[i2][l1] == 6) {
                    if (j1 < mB) { gl[j1].y = i2; gl[j1].x = l1; j1++; }
                }
                // ボール位置(2=ボール, 5=ボール+ゴール)
                if (Fd[i2][l1] == 2 || Fd[i2][l1] == 5) {
                    if (k1 < mB) { bg[k1].y = i2; bg[k1].x = l1; k1++; }
                }
            }
        }
        // 5(ボール+ゴール)→2(ボール), 6(プレイヤー+ゴール)→3(プレイヤー) に正規化
        for (i = 0; i < mY; i++) {
            for (j = 0; j < mX; j++) {
                if (Fd[i][j] == 5) Fd[i][j] = 2;
                if (Fd[i][j] == 6) Fd[i][j] = 3;
            }
        }

        draw();
        if (ctx) {
            setInterval(main, 500);
        }
    } else {
        if (xmlHttp.status > 0 && xmlHttp.status != 200){
            alert("reload. this shuould be rewrite use axios."+xmlHttp.status);
        }
    }
}

// 一度描画したものは残りつづける為、毎回初期化する
function draw(){
    var i;
    var j;
    //画面の初期化
    //ctx.clearRect(0,0,WIDTH,HEIGHT);
    //ctx.save();
    //ctx.beginPath();
    for(i = 0; i < mY; i++){
        for(j = 0; j < mX; j++){
            ctx.drawImage(img, Fd[i][j]*wGap, 0, wGap, hGap,   j*wGap, i*wGap, wGap, hGap);
        }
    }
    for(var k = 0; k < mB; k++){
        if(Fd[gl[k].y][gl[k].x] == 0){
            ctx.drawImage(img, 4*wGap, 0, wGap, hGap, gl[k].x * wGap, gl[k].y * hGap,wGap,hGap);
        }else if(Fd[gl[k].y][gl[k].x] == 2){
            ctx.drawImage(img, 5*wGap, 0, wGap, hGap, gl[k].x * wGap, gl[k].y * hGap,wGap,hGap);
        }else if(Fd[gl[k].y][gl[k].x] == 3){
            ctx.drawImage(img, 6*wGap, 0, wGap, hGap, gl[k].x * wGap, gl[k].y * hGap,wGap,hGap);
        }
    }
    
    if(GoalCHK == 1){
        change = 1;
    }
    //ctx.save();
    
    //ここにアニメーション
    //ctx.beginPath();
    //ctx.strokeStyle='rgb(0,0,0)';
    //ctx.arc(character.x,character.y,10,0, Math.PI*2 ,true);
    //ctx.stroke();
    
    //空画面を復元
    //ctx.restore();
    //ctx.closePath();
}
function main()
{
    if( change ){
        change = 0;
        // 全ゴールにボールが乗っているか判定
        var complete = true;
        for(var i = 0; i < mB; i++){
            if(Fd[gl[i].y][gl[i].x] != 2){
                complete = false;
                break;
            }
        }
        draw();
        if(complete && mB > 0){
            // クリア: 1秒後に次の面へ
            setTimeout(function(){
                var next = currentLevel + 1;
                if(next <= 90){
                    window.location.href = window.location.pathname + '?level=' + next;
                } else {
                    alert('CONGRATULATIONS! All 90 stages complete!');
                }
            }, 1000);
        }
    }
}
function isInside(row, column) {
    return row >= 0 && row < mY && column >= 0 && column < mX;
}

function movePlayer(rowDirection, columnDirection) {
    if (!Fd) return;

    var nextRow = X + rowDirection;
    var nextColumn = Y + columnDirection;
    if (!isInside(nextRow, nextColumn)) return;

    if (Fd[nextRow][nextColumn] == 2) {
        var ballRow = nextRow + rowDirection;
        var ballColumn = nextColumn + columnDirection;
        if (!isInside(ballRow, ballColumn) ||
            (Fd[ballRow][ballColumn] != 0 && Fd[ballRow][ballColumn] != 4)) {
            return;
        }
        Fd[ballRow][ballColumn] = 2;
    } else if (Fd[nextRow][nextColumn] != 0 && Fd[nextRow][nextColumn] != 4) {
        return;
    }

    Fd[X][Y] = 0;
    X = nextRow;
    Y = nextColumn;
    Fd[X][Y] = 3;
    change++;
    main();
}

function mouseDown(mouseX, mouseY){
    var clickedRow = Math.floor(mouseY / hGap);
    var clickedColumn = Math.floor(mouseX / wGap);
    var rowDistance = clickedRow - X;
    var columnDistance = clickedColumn - Y;

    if (rowDistance == 0 && columnDistance == 0) return;

    if (Math.abs(rowDistance) > Math.abs(columnDistance)) {
        movePlayer(rowDistance < 0 ? -1 : 1, 0);
    } else {
        movePlayer(0, columnDistance < 0 ? -1 : 1);
    }
}
