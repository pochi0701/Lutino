<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>例外処理テスト</title>
    <link rel="stylesheet" href="/sample/tests/test-results.css">
</head>
<body>
<main class="test-page">
    <h1>例外処理テスト</h1>
    <section class="test-results">
<?
var failures = 0;
var total = 0;
function check(name, passed) {
    total++;
    if (!passed) failures++;
    print("<div class='test-result'><span class='test-name'>" + name + "</span><span class='test-status " + (passed ? "status-pass" : "status-fail") + "'>" + (passed ? "PASS" : "FAIL") + "</span></div>\n");
}

function nativeFailure() {
    restful("GET", "invalid-url", "");
}

function catchNative() {
    let marker = 17;
    var caught = false;
    try {
        let marker = 99;
        nativeFailure();
    } catch (e) {
        caught = e.indexOf("Invalid HTTP URL") >= 0;
    }
    return caught && marker == 17;
}

var repeated = true;
for (var i = 0; i < 50; i++) {
    if (!catchNative()) repeated = false;
}
check("Repeated native exceptions preserve scopes", repeated);

function catchArgumentFailure() {
    function identity(value) { return value; }
    let marker = 23;
    try {
        identity(nativeFailure());
    } catch (e) {
        return marker == 23;
    }
    return false;
}
check("Exception while evaluating an argument", catchArgumentFailure());

function catchEvalFailure() {
    let marker = 31;
    try {
        eval("restful('GET', 'invalid-url', '');");
    } catch (e) {
        return marker == 31;
    }
    return false;
}
check("Eval restores interpreter state after an exception", catchEvalFailure());

function nestedThrow() {
    try {
        nativeFailure();
    } catch (e) {
        throw {message: e, value: 42};
    }
}
var finalCount = 0;
function catchRethrow() {
    var caught = false;
    try {
        try {
            nestedThrow();
        } finally {
            finalCount++;
        }
    } catch (e) {
        caught = e.value == 42 && e.message.indexOf("Invalid HTTP URL") >= 0;
    }
    return caught;
}
check("Rethrow survives function and finally scopes", catchRethrow() && finalCount == 1);

function finallyAfterCatchFailure() {
    try {
        try { throw "first"; }
        catch (e) { nativeFailure(); }
        finally { finalCount++; }
    } catch (e) {
        return e.indexOf("Invalid HTTP URL") >= 0;
    }
    return false;
}
check("Finally runs when catch throws", finallyAfterCatchFailure() && finalCount == 2);

function add(a, b) { return a + b; }
check("Functions still work after exceptions", add(3, 4) == 7 && catchNative());
print("<section class='summary'><h2>テスト結果</h2><p>合計: " + total + "　成功: <span class='status-pass'>" + (total - failures) + "</span>　失敗: <span class='status-fail'>" + failures + "</span></p>");
if (failures == 0) {
    print("<p class='test-status status-pass'>すべて成功</p></section>");
} else {
    print("<p class='test-status status-fail'>失敗したテストがあります</p></section>");
}
?>
    </section>
    <a class="menu-link" href="/sample/tests/?">ファイルメニューに戻る</a>
</main>
</body>
</html>
