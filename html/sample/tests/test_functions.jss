<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="utf-8">
    <title>TinyJS 関数テスト</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
<?
var total = 0;
var pass = 0;

function t(label, ok) {
    total++;
    print("<br>\n" + label + " " + (ok ? "OK" : "NG"));
    if (ok) pass++;
}

print("<h2>TinyJS Native Function Tests</h2>");

// Basic globals / utility
t("charToInt", charToInt("A") == 65);
t("Integer.parseInt", Integer.parseInt("123") == 123);
t("Integer.valueOf", Integer.valueOf("A") == 65);
t("encodeURI", encodeURI("a b") == "a%20b");
t("btoa/atob", atob(btoa("hello")) == "hello");
t("dirname", dirname("/a/b/c.txt") == "/a/b");
t("basename", basename("/a/b/c.txt") == "c.txt");
t("isNaN", isNaN(Math.sqrt(-1)) == 1);
t("isFinite", isFinite(123.5) == 1);

// String methods
var s = "Hello, World!";
t("String.indexOf", s.indexOf("World") == 7);
t("String.substring", s.substring(0, 5) == "Hello");
t("String.substr", s.substr(7, 5) == "World");
t("String.startsWith", s.startsWith("Hello") == 1);
t("String.endsWith", s.endsWith("!") == 1);
t("String.charAt", s.charAt(1) == "e");
t("String.charCodeAt", s.charCodeAt(1) == 101);
t("String.fromCharCode", String.fromCharCode(65) == "A");
t("String.split+Array.join", s.split(",").join("|") == "Hello| World!");
t("String.replace", s.replace("World", "TinyJS") == "Hello, TinyJS!");
var sReplaceAll = "a-b-c";
var sCase = "AbC";
var sTrim = "  hi  ";
var sLTrim = "  hi";
var sRTrim = "hi  ";
var sSlash = "a\"b";
t("String.replaceAll", sReplaceAll.replaceAll("-", "_") == "a_b_c");
t("String.replaceAll literal", "a-b-c".replaceAll("-", "_") == "a_b_c");
t("String.toLowerCase", sCase.toLowerCase() == "abc");
t("String.toUpperCase", sCase.toUpperCase() == "ABC");
t("String.trim", sTrim.trim() == "hi");
t("String.ltrim", sLTrim.ltrim() == "hi");
t("String.rtrim", sRTrim.rtrim() == "hi");
t("String.addSlashes", sSlash.addSlashes() == "a\\\"b");

// Regex helpers
t("match hit", match("/hello/", "hello world") == 1);
t("match miss", match("/goodbye/", "hello world") == 0);
t("match ignoreCase", match("/HELLO/i", "hello world") == 1);
t("replace single", replace("hello world", "/world/", "tiny") == "hello tiny");
t("replace array", replace("hello world", ["/hello/", "/world/"], ["hi", "earth"]) == "hi earth");

// Array methods
var a = [1,2,3];
t("Array.contains", a.contains(2) == 1);
t("Array.indexOf", a.indexOf(3) == 2);
a.push(4);
t("Array.push", a.join(",") == "1,2,3,4");
var p = a.pop();
t("Array.pop", p == 4 && a.join(",") == "1,2,3");
a.unshift(0);
t("Array.unshift", a.join(",") == "0,1,2,3");
var sh = a.shift();
t("Array.shift", sh == 0 && a.join(",") == "1,2,3");
var sl = a.slice(1,3);
t("Array.slice", sl.join(",") == "2,3");
var sp = a.splice(1,1);
t("Array.splice", sp.join(",") == "2" && a.join(",") == "1,3");
a.remove(3);
t("Array.remove", a.join(",") == "1");

// Object helpers / JSON
var obj = {x:1, y:2};
var keys = Object.keys(obj).join(",");
t("Object.keys", keys.indexOf("x") >= 0 && keys.indexOf("y") >= 0);
var clone = obj.clone();
clone.x = 9;
t("Object.clone", obj.x == 1 && clone.x == 9);
var jsonObj = JSON.stringify({a:1});
t("JSON.stringify", jsonObj.indexOf("\"a\"") >= 0 && jsonObj.indexOf("1") >= 0);

// Environment-dependent but safe shape checks
t("Date", Date().length > 0);
t("randomUUID", randomUUID().length > 0);
t("biosUUID", biosUUID().length > 0);
t("file_exists returns number", file_exists("C:\\\\") == 0 || file_exists("C:\\\\") == 1);
t("dir_exists returns number", dir_exists("C:\\\\") == 0 || dir_exists("C:\\\\") == 1);

print("<br>\n<br>\nTotal: " + total + " Passed: " + pass + " Failed: " + (total-pass));
?>