<?
// print関数のテスト
print("Hello, TinyJS!<br>\n" + "<br>\n");

// Math.rand()のテスト
print("Math.rand() = " + Math.rand() + "<br>\n");

// Math.randInt(min, max)のテスト
print("Math.randInt(1, 10) = " + Math.randInt(1, 10) + "<br>\n" + "<br>\n");

// 文字列関数のテスト
var str = "Hello, World!";
var str2 = "    hello    ";
print("str.indexOf('World') = " + str.indexOf("World") + "<br>\n" + "<br>\n");
print("str.substring(0,5) = " + str.substring(0,5) + "<br>\n" + "<br>\n");
print("str.substr(7,5) = " + str.substr(7,5) + "<br>\n" + "<br>\n");
print("str.startsWith('Hello') = " + str.startsWith("Hello") + "<br>\n" + "<br>\n");
print("str.endsWith('!') = " + str.endsWith("!") + "<br>\n" + "<br>\n");
print("str.charAt(1) = " + str.charAt(1) + "<br>\n" + "<br>\n");
print("str.charCodeAt(1) = " + str.charCodeAt(1) + "<br>\n" + "<br>\n");
print("String.fromCharCode(65) = " + String.fromCharCode(65) + "<br>\n" + "<br>\n");
print("str.split(',') = " + str.split(",").join("|") + "<br>\n" + "<br>\n");
print("str.replace('World','TinyJS') = " + str.replace("World","TinyJS") + "<br>\n" + "<br>\n");
print("str.replaceAll('l','L') = " + str.replaceAll("l","L") + "<br>\n" + "<br>\n");
print("str.trim() = " + str2.trim() + "<br>\n" + "<br>\n");
print("str.toLowerCase() = " + str.toLowerCase() + "<br>\n" + "<br>\n");
print("str.toUpperCase() = " + str.toUpperCase() + "<br>\n" + "<br>\n");

// 配列関数のテスト
var arr = [1,2,3,4,5];
print("arr.contains(3) = " + arr.contains(3) + "<br>\n" + "<br>\n");
arr.remove(3 + "<br>\n");
print("arr.join(',') = " + arr.join(",") + "<br>\n" + "<br>\n");

// ファイル・ディレクトリ関数（パスは環境に合わせて変更してください）
print("file_exists('/index.jss') = " + file_exists("/index.jss") + "<br>\n" + "<br>\n");
print("dir_exists('c:\\/') = " + dir_exists("c:\\/") + "<br>\n" + "<br>\n");

// その他
print("randomUUID() = " + randomUUID() + "<br>\n" + "<br>\n");
print("biosUUID() = " + biosUUID() + "<br>\n" + "<br>\n");
?>