<?
// ==========================================================================
// change_skin.jss
// ltn.conf の skin_name を変更し、change_config() でメモリ上の設定を
// 即時反映させる管理画面(reboot不要)。
// ==========================================================================
function extractValue(line, key)
{
    var rest = line.substring(key.length, line.length);
    return rest.trim();
}

var base = dirname(_SERVER.DOCUMENT_ROOT);
var confPath = base + "/ltn.conf";
var confText = loadFromFile(confPath);
var lines = confText.split("\n");

// 現在の設定値を取得
var currentSkin = "";
var currentPassword = "";
var skinRootValue = "";
for (var i = 0; i < lines.length; i++) {
    var line = lines[i].trim();
    if (line.startsWith("skin_name")) {
        currentSkin = extractValue(line, "skin_name");
    } else if (line.startsWith("skin_root")) {
        skinRootValue = extractValue(line, "skin_root");
    } else if (line.startsWith("system_password")) {
        currentPassword = extractValue(line, "system_password");
    }
}

// スキンフォルダの一覧を取得
var skinDir;
if (skinRootValue.indexOf(":") >= 0) {
    // ドライブ指定(絶対パス)
    skinDir = skinRootValue;
} else {
    // アプリ実行フォルダからの相対パス
    skinDir = base + skinRootValue;
}
var skinList = [];
if (dir_exists(skinDir)) {
    var files = eval(scandir(skinDir));
    for (var i = 0; i < files.length; i++) {
        var name = basename(files[i]);
        if (name != "." && name != ".." && dir_exists(files[i])) {
            skinList.push(name);
        }
    }
}

// POSTされた値
var newSkin = _POST.skin_name;
var password = _POST.password;
if (newSkin == undefined) { newSkin = ""; }
if (password == undefined) { password = ""; }

var message = "";
var messageClass = "";

if (newSkin.length > 0 || password.length > 0) {
    if (currentPassword.length == 0) {
        message = "system_password が設定されていないため変更できません。";
        messageClass = "text-danger";
    } else if (password != currentPassword) {
        message = "パスワードが違います。";
        messageClass = "text-danger";
    } else if (!skinList.contains(newSkin)) {
        message = "指定されたスキン '" + newSkin + "' は存在しません。";
        messageClass = "text-danger";
    } else {
        // skin_name行を書き換えて保存
        for (var i = 0; i < lines.length; i++) {
            if (lines[i].trim().startsWith("skin_name")) {
                lines[i] = "skin_name              " + newSkin;
            }
        }
        // ltn.confの末尾(system_password等)が改行なしで終わっていると、
        // config_file_read_line()の仕様上、最終行が読み込まれない
        // (=system_passwordが空になり、次回起動時の読み込みが失敗する)ため、
        // 末尾に改行を必ず付与して保存する。
        var newConfText = lines.join("\n") + "\n";
        saveToFile(confPath, newConfText);
        // メモリ上のskin_nameを書き換え、reboot無しで即時反映させる
        change_config("skin_name", newSkin, password);
        currentSkin = newSkin;
        message = "スキンを '" + newSkin + "' に変更しました。";
        messageClass = "text-success";
        header("Location: /");
    }
}
?>
<!doctype html>
<html lang="ja">
<head>
  <meta charset="utf-8">
  <title>SKIN CHANGE</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM" crossorigin="anonymous">
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js" integrity="sha384-geWF76RCwLtnZ8qwWowPQNguL3RmwHVBC9FhGdlKrxdiJJigb/j/68SIy3Te4Bkz" crossorigin="anonymous"></script>
</head>
<body>
  <div class="text-center">
  <h1>スキン変更：スキンとパスワードを入力してください。</h1>
  </div>
  <div class="container">
<?
if (message.length > 0) {
    print("<div class=\"" + messageClass + "\"><p>" + htmlspecialchars(message) + "</p></div>\n");
}
?>
    <form class="form-horizontal col-sm-4 col-sm-offset-4" role="form" method="post" action="">
    <div class="form-group">
        <label for="skin_name">スキン</label>
        <select name="skin_name" id="skin_name" class="form-control">
<?
for (var i = 0; i < skinList.length; i++) {
    var name = skinList[i];
    var selected = (name == currentSkin) ? " selected" : "";
    print("<option value=\"" + htmlspecialchars(name) + "\"" + selected + ">" + htmlspecialchars(name) + "</option>\n");
}
?>
        </select>
    </div>
    <div class="form-group">
        <label class="sr-only" for="password">パスワード</label>
        <input name="password" id="password" type="password" class="form-control" placeholder="Password" />
    </div>
    <div class="form-group">
        <button type="submit" name="submit" value="submit" class="btn btn-primary">変更して再起動</button>
    </div>
    </form>
  </div>
</body>
</html>
