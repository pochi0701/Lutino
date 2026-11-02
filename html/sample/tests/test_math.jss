<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>TinyJS 数学関数テスト</title>
    <link rel="stylesheet" href="/sample/tests/test-results.css">
</head>
<body>
<main class="test-page">
    <h1>TinyJS 数学関数テスト</h1>
    <section class="test-results">
<?
var total = 0;
var pass = 0;

function near(a, b, eps) {
    return Math.abs(a - b) < eps;
}

function t(label, ok) {
    total++;
    print("<div class='test-result'><span class='test-name'>" + label + "</span><span class='test-status " + (ok ? "status-pass" : "status-fail") + "'>" + (ok ? "PASS" : "FAIL") + "</span></div>\n");
    if (ok) pass++;
}

t("Math.abs", Math.abs(-5) == 5);
t("Math.round", Math.round(3.7) == 4);
t("Math.min", Math.min(2, 8) == 2);
t("Math.max", Math.max(2, 8) == 8);
t("Math.range upper clamp", Math.range(15, 0, 10) == 10);
t("Math.range lower clamp", Math.range(-5, 0, 10) == 0);
t("Math.sign neg", Math.sign(-123) == -1);
t("Math.sign zero", Math.sign(0) == 0);
t("Math.sign pos", Math.sign(123) == 1);

t("Math.PI", near(Math.PI(), 3.14159265, 0.0001));
t("Math.E", near(Math.E(), 2.71828182, 0.0001));
t("Math.toDegrees", near(Math.toDegrees(Math.PI()), 180, 0.0001));
t("Math.toRadians", near(Math.toRadians(180), Math.PI(), 0.0001));

t("Math.sin", near(Math.sin(Math.PI()/2), 1, 0.0001));
t("Math.asin", near(Math.asin(1), Math.PI()/2, 0.0001));
t("Math.cos", near(Math.cos(0), 1, 0.0001));
t("Math.acos", near(Math.acos(1), 0, 0.0001));
t("Math.tan", near(Math.tan(Math.PI()/4), 1, 0.001));
t("Math.atan", near(Math.atan(1), Math.PI()/4, 0.0001));

t("Math.sinh", near(Math.sinh(0), 0, 0.0001));
t("Math.asinh", near(Math.asinh(1), 0.881373, 0.001));
t("Math.cosh", near(Math.cosh(0), 1, 0.0001));
t("Math.acosh", near(Math.acosh(1), 0, 0.0001));
t("Math.tanh", near(Math.tanh(0), 0, 0.0001));
t("Math.atanh", near(Math.atanh(0.5), 0.549306, 0.001));

t("Math.log", near(Math.log(Math.E()), 1, 0.0001));
t("Math.log10", near(Math.log10(100), 2, 0.0001));
t("Math.exp", near(Math.exp(1), Math.E(), 0.0001));
t("Math.pow", Math.pow(2, 8) == 256);
t("Math.sqr", Math.sqr(5) == 25);
t("Math.sqrt", Math.sqrt(25) == 5);

var r = Math.rand();
t("Math.rand range", r >= 0 && r <= 1);
var ri = Math.randInt(1, 10);
t("Math.randInt range", ri >= 1 && ri <= 10);

t("isNaN(sqrt(-1))", isNaN(Math.sqrt(-1)) == 1);
t("isFinite(1.23)", isFinite(1.23) == 1);

print("<section class='summary'><h2>テスト結果</h2><p>合計: " + total + "　成功: <span class='status-pass'>" + pass + "</span>　失敗: <span class='status-fail'>" + (total-pass) + "</span></p></section>");
?>
</section>
<a class="menu-link" href="/sample/tests/?">ファイルメニューに戻る</a>
</main>
</body>
</html>