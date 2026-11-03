<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="utf-8">
    <title>TinyJS基本テスト</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="/sample/tests/test-results.css">
</head>
<body>
<main class="test-page">
    <h1>TinyJS基本テスト</h1>
    <section class="test-results">
<?
    var totalTests = 0;
    var passedTests = 0;
    function startTest(name, description) {
        print("<div class='test-result'><span class='test-name'>" + name + " — " + htmlspecialchars(description) + "</span>");
    }
    function finishTest(passed) {
        totalTests++;
        if (passed) passedTests++;
        print("<span class='test-status " + (passed ? "status-pass" : "status-fail") + "'>" + (passed ? "PASS" : "FAIL") + "</span></div>\n");
    }
    startTest("test01", "letで代入した値が1になる");
    {
        let result = 1;
        finishTest( (result == 1));
    }
    startTest("test02", "varで代入した値が42になる");
    {
        var a  = 42;
        finishTest( (a == 42));
    }
    startTest("test03", "forと加算で1〜9の合計が45になる");
    {
        let a = 0;
        let i ;
        for (i=1;i<10;i++) a = a + i;
        finishTest( (a == 45));
    }
    startTest("test04", "比較演算子で42 < 43が成立する");
    {
        let a = 42;
        finishTest( (a < 43));
    }
    startTest("test05", "for内のletと+=で1〜9の合計が45になる");
    {
        let a = 0;
        for (let i=1;i<10;i++) a += i;
        finishTest( (a == 45));
    }
    startTest("test06", "関数の引数と戻り値でadd(3,6)が9になる");
    {
        // simple function
        function add(x,y) { return x+y; }
        finishTest( (add(3,6)==9));
    }
    startTest("test07", "関数内の同名変数が外側の変数を変更しない");
    {
        // simple function scoping test
        let a = 7;
        function add(x,y) { var a=x+y; return a; }
        finishTest( (add(3,6)==9 && a==7));
    }
    startTest("test08", "オブジェクトのプロパティに代入した関数を呼び出せる");
    {
        // functions in variables
        let bob = {};
        bob.add = function(x,y) { return x+y; };
        finishTest( (bob.add(3,6)==9));
    }
    startTest("test09", "オブジェクト初期化時に定義した関数を呼び出せる");
    {
        // functions in variables using JSON-style initialisation
        let bob = { add : function(x,y) { return x+y; } };
        finishTest( (bob.add(3,6)==9));
    }
    startTest("test10", "関数から別の関数を呼び出せる");
    {
        // double function calls
        function a(x) { return x+2; }
        function b(x) { return a(x)+1; }
        finishTest( (a(3)==5 && b(3)==6));
    }
    startTest("test11", "再帰呼び出しで5の階乗が120になる");
    {
        // recursion
        function a(x) { 
            if (x>1)
                return x*a(x-1);
            return 1;
        }
        finishTest( (a(5)==1*2*3*4*5));
    }

    startTest("test12", "ブロックなしのif/elseで正しい分岐を実行する");
    {
        // if .. else
        let a = 42;
        let result = -1;
        if (a != 42)
            result = 0;
        else
            result = 1;
        finishTest( (result == 1));
    }
    startTest("test13", "ブロック付きのif/elseで正しい分岐を実行する");
    {
        // if .. else with blocks
        let a = 42;
        let result = -1;
        if (a != 42) {
            result = 0;
        } else {
            result = 1;
        }
        finishTest( (result == 1));
    }
    startTest("test14", "関数のローカル変数とグローバル変数のスコープを区別する");
    {
        // Variable creation and scope from http://en.wikipedia.org/wiki/JavaScript_syntax
        x = 0; // A global variable
        var y = 'Hello!'; // Another global variable
        z = 0; // yet another global variable

        function f(){
            var z = 'foxes'; // A local variable
            twenty = 20; // Global because keyword var is not used
            return x; // We can use x here because it is global
        }
        // The value of z is no longer available
        // testing
        let blah = f();
        finishTest( (blah==0 && z!='foxes' && twenty==20));
    }
    startTest("test15", "整数・小数・指数・8進数・16進数リテラルを解釈する");
    {
        // Number definition from http://en.wikipedia.org/wiki/JavaScript_syntax
        let a = 345;    // an "integer", although there is only one numeric type in JavaScript
        let b = 34.5;   // a floating-point number
        let c = 3.45e2; // another floating-point, equivalent to 345
        let d = 0377;   // an octal integer equal to 255
        let e = 0xFF;   // a hexadecimal integer equal to 255, digits represented by the letters A-F may be upper or lowercase
        finishTest((a==345 && b*10==345 && c==345 && d==255 && e==255));
    }
    startTest("test16", "未定義値の文字列化とundefined/nullの通常・厳密比較");
    {
        // Undefined/null from http://en.wikipedia.org/wiki/JavaScript_syntax
        let testUndefined;        // variable declared but not defined, set to value of undefined
        let testObj = {};  

        let result = 1;
        if ((""+testUndefined) != "undefined") result = 0; // test variable exists but value not defined, displays undefined
        if ((""+testObj.myProp) != "undefined") result = 0; // testObj exists, property does not, displays undefined
        if (!(undefined == null)) result = 0;  // unenforced type during check, displays true
        if (undefined === null) result = 0;// enforce type during check, displays false

        if (null != undefined) result = 0;  // unenforced type during check, displays true
        if (null === undefined) result = 0; // enforce type during check, displays false

        finishTest((result == 1));
    }
    startTest("test17", "配列を代入すると参照を共有する");
    {
        // references for arrays
        let a = [];
        a[0] = 10;
        a[1] = 22;

        let b = a;
        b[0] = 5;

        finishTest((a[0]==5 && a[1]==22 && b[1]==22));
    }
    startTest("test18", "関数に渡した配列は変更され、数値の元の値は変わらない");
    {
        // references with functions
        let a = 42;
        let b = [];
        b[0] = 43;

        function foo(myarray) {
            myarray[0]++;
        }

        function bar(myvalue) {
            myvalue++;
        }

        foo(b);
        bar(a);
        finishTest((a==42 && b[0]==44));
    }
    startTest("test19", "文字列検索・部分文字列・整数変換・文字コード・オブジェクト複製");
    {
        // built-in functions
        let foo = "foo bar stuff";
        let r = Math.rand();

        let parsed = Integer.parseInt("42");
  
        let aStr = "ABCD";
        let aChar = aStr.charAt(0);

        let obj1 = new Object();
        obj1.food = "cake";
        obj1.desert = "pie";

        let obj2 = obj1.clone();
        obj2.food = "kittens";

        finishTest((foo.length==13 && foo.indexOf("bar")==4 && foo.substring(8,13)=="stuff" && parsed==42 && Integer.valueOf(aChar)==65 && obj1.food=="cake" && obj2.desert=="pie"));
    }
    startTest("test20", "同名のオブジェクトプロパティが既存の関数を上書きしない");
    {
        // Test reported by sterowang, Variable attribute defines conflict with function.
        /*
        What steps will reproduce the problem?
        1. function a (){};
        2. b = {};
        3. b.a = {};
        4. a();

        What is the expected output? What do you see instead?
        Function "a" should be called. But the error message "Error Expecting 'a' 
        to be a function at (line: 1, col: 1)" received.

        What version of the product are you using? On what operating system?
        Version 1.6 is used on Cent OS 5.4


        Please provide any additional information below.
        When using dump() to show symbols, found the function "a" is reassigned to 
        "{}" by "b.a = {};" call.
        */

        function a (){};
        let b = {};
        b.a = {};
        a();
        finishTest((true));
    }
    startTest("test21", "evalで数式と括弧付きオブジェクト式を評価する");
    {
        /* Javascript eval */
        // 42-tiny-js change begin --->
        // in JavaScript eval is not JSON.parse
        // use parentheses or JSON.parse instead
        // myfoo = eval("{ foo: 42 }");
        let myfoo = eval("("+"{ foo: 42 }"+")");
        finishTest((eval("4*10+2")==42 && myfoo.foo==42));
    }
    startTest("test22", "Array.removeで値2と5を削除し、残りの順序を保つ");
    {
        // test for array remove
        let a = [1,2,4,5,7];
        a.remove(2);
        a.remove(5);

        finishTest((a.length==3 && a[0]==1 && a[1]==4 && a[2]==7));
    }
    startTest("test23", "条件が偽の場合は未定義の関数を呼び出さない");
    {
        // mikael.kindborg@mobilesorcery.com - Function symbol is evaluated in bracket-less body of false if-statement
        let foo; // a var is only created automated by assignment

        if (foo !== undefined) foo();

        finishTest((true));
    }
    startTest("test24", "省略されたテスト（判定は常に成功）");
    {
        // 省略
        finishTest((true));
    }
    startTest("test25", "配列のlengthが末尾インデックスへの代入で拡張される");
    {
        // Array length test
        myArray = [ 1, 2, 3, 4, 5 ];
        myArray2 = [ 1, 2, 3, 4, 5 ];
        myArray2[8] = 42;
        finishTest((myArray.length == 5 && myArray2.length == 9));
    }
    startTest("test26", "undefinedと定義済み文字列を比較で区別する");
    {
        // check for undefined-ness
        let a = undefined;
        let b = "foo";
        finishTest((a==undefined && b!=undefined));
    }
    startTest("test27", "後置++は増加前、前置++は増加後の値を返す");
    {
        // test for postincrement working as expected
        let foo = 5;
        let bar = 5; 
        finishTest(((foo++)==5 && (++bar)==6));
    }
    startTest("test28", "Array.containsで数値・文字列の存在と不在を判定する");
    {
        // test for array contains
        let a = [1,2,4,5,7];
        let b = ["bread","cheese","sandwich"];
        finishTest((a.contains(1) && !a.contains(42) && b.contains("cheese") && !b.contains("eggs")));
    }
    startTest("test29", "Array.remove後の長さと残った各要素を確認する");
    {
        let a = [1,2,4,5,7];
        a.remove(2);
        a.remove(5);
        finishTest((a.length==3 && a[0]==1 && a[1]==4 && a[2]==7));
    }
    startTest("test30", "Array.joinで要素をカンマ区切りの文字列にする");
    {
        let a = [1,2,4,5,7];
        finishTest((a.join(",")=="1,2,4,5,7"));
    }
    startTest("test31", "String.splitでカンマ区切りの文字列を配列にする");
    {
        // test for string split
        let b = "1,4,7";
        let a = b.split(",");
        finishTest((a.length==3 && a[0]==1 && a[1]==4 && a[2]==7));
    }
    startTest("test32", "prototypeとnewでメソッドを継承し、thisを参照する");
    {
        let Foo = {
            value : function() { return this.x + this.y; }
        };

        let a = { prototype: Foo, x: 1, y: 2 };
        let b = new Foo(); 
        b.x = 2;
        b.y = 3;
        let result1 = a.value();
        let result2 = b.value();

        finishTest((result1==3 && result2==5));
    }
    startTest("test33", "左シフト・右シフト・符号なし右シフト");
    {
        // test for shift
        var a = (2<<2);
        var b = (16>>3);
        var c = (-1 >>> 16);

        finishTest((a==8 && b==2 && c == 0xFFFF));
    }
    startTest("test34", "三項演算子で真偽に応じた値を返す");
    {
        // test for ternary
        finishTest(((true?3:4)==3 && (false?5:6)==6));

    }
    startTest("test35", "コンストラクタとメソッドからthisのプロパティを変更する");
    {
        function Person(name) {
          this.name = name;
          this.kill = function() { this.name += " is dead"; };
        }
        let a = new Person("Kenny");
        a.kill();

        finishTest((a.name == "Kenny is dead"));
    }
    startTest("test36", "浮動小数点の乗除算が50・550・5になる");
    {
        // the 'lf' in the printf caused issues writing doubles on some compilers
        var a=5.0/10.0*100.0;
        var b=5.0*110.0;
        var c=50.0/10.0;
        a.dump();
        b.dump();
        c.dump();

        finishTest((a==50 && b==550 && c==5));
    }

    startTest("test37", "forをbreakで終了し、1〜9の合計が45になる");
    {
        // simple for loop containing initialisation, using +=
        let a = 0;
        for (let i=1;i<12;i++)
        {
           if(i==10){
              print("break ");
              break;
           }
           a += i;
        }

        finishTest(( a==45));
    }
    startTest("test38", "whileをbreakで終了し、1と2の合計が3になる");
    {
        // simple for loop containing initialisation, using +=
        let a = 0;
        let i = 1;
        while(i<10){
            if(i==3){
                print("break ");
                break;
            }
            a += i;
            i += 1;
        }
        finishTest((a==3));
    }
    startTest("test39", "letのブロックスコープと入れ子の同名変数");
    {
        // Test for let block scope
        let test01 = 0;
        let test02 = 0;
        var x = 0;
        {
            let x = 1;
        }
        if( x == 0 ){
            test01 = 1;
        }

        {
            let y = 2;
            {
                let y = 3;
                if( y == 3){
                    test02 = 1;
                }
            }
        }

        finishTest((test01 == 1 && test02 == 1));
    }
    startTest("test40", "for/whileのbreak・continueで合計がそれぞれ45になる");
    {
        // Test for for,while,break,continue
        let sum1 = 0;
        for( let i = 0 ; i < 12 ; i++ ){
            if( i >= 10) {
                print("continue ");
                continue;
            }
            sum1 += i;
        }

        let sum2 = 0;
        for( let j = 0 ; j < 12 ; j++ ){
            if( j == 10) {
                print("break ");
                break;
            }
            sum2 += j;
        }

        let sum3 = 0;
        let l = 0;
        while( l < 12 ){
            if( l >= 10 ) {
                l += 1;
                print("continue ");
                continue;
            }
            sum3 += l;
            l += 1;
        }

        let sum4 = 0;
        let k = 0;
        while( k < 12){
            if( k == 10 ) {
                print("break ");
                break;
            }
            sum4 += k;
            k += 1;
        }
        finishTest(((sum1 == 45) && (sum2 == 45) && (sum3 == 45) && (sum4 == 45)));
    }
    startTest("test41", "配列のpush・pop・shift・unshift・indexOf・slice");
    {
        // Test for for,while,break,continue
        let array = ['a','b','c','d','e'];

        // ['a','b','c','d','e','f'];
        array.push('f');
        let result01 = (array[5] == 'f');

        // ['a','b','c','d','e'];
        array.pop();
        let result02 = (array.length == 5);

        // ['b','c','d','e'];
        array.shift();
        let result03 = (array[0] == 'b');

        // ['a','b','c','d','e'];
        array.unshift('a');
        let result04 = (array[0] == 'a');

        // ['a','b','c','d','e'];
        let result05 = (array.indexOf('b') == 1);

        // ['b','c'];
        let array2 = array.slice(1,3);
        let result06 = (array2[1] == 'c');

        array.slice(0,3);
        let result07 = (array.length == 2 && array[0] == 'd');
        finishTest((result01 && result02 && result03 && result04 && result05 && result06));
    }
    startTest("test42", "正規表現matchで一致した場合は1を返す");
    {
        // Test for regex match - basic pattern
        let result = match("/hello/", "hello world");
        finishTest((result == 1));
    }
    startTest("test43", "正規表現matchで一致しない場合は0を返す");
    {
        // Test for regex match - no match
        let result = match("/goodbye/", "hello world");
        finishTest((result == 0));
    }
    startTest("test44", "正規表現matchのiフラグで大文字・小文字を区別しない");
    {
        // Test for regex match - case insensitive
        let result = match("/HELLO/i", "hello world");
        finishTest((result == 1));
    }
    startTest("test45", "不正な正規表現のmatchはクラッシュせず0を返す");
    {
        // Test for regex match - invalid pattern (should not crash)
        let result = match("invalid", "test");
        finishTest((result == 0));
    }
    startTest("test46", "typeofとNaN/Infinityに対するisNaN・isFinite");
    {
        // typeof + NaN/Infinity + isNaN/isFinite
        let u;
        let o = {};
        function f(x) { return x; }
        let result =
            (typeof u == "undefined") &&
            (typeof 1 == "number") &&
            (typeof "abc" == "string") &&
            (typeof o == "object") &&
            (typeof f == "function") &&
            isNaN(NaN) &&
            !isNaN(1) &&
            isFinite(123.5) &&
            !isFinite(Infinity);
        finishTest((result == 1));
    }
    startTest("test47", "instanceofで生成元のオブジェクトを区別する");
    {
        // instanceof
        let Base = {};
        let Other = {};
        let obj = new Base();
        let result = (obj instanceof Base) && !(obj instanceof Other);
        finishTest((result == 1));
    }
    startTest("test48", "正規表現replaceでworldをuniverseに置換する");
    {
        // Test for regex replace - basic
        var result = replace("hello world", "/world/", "universe");
        finishTest((result == "hello universe"));
    }
    startTest("test49", "正規表現replaceで複数パターンをそれぞれ置換する");
    {
        // Test for regex replace - multiple patterns
        var result = replace("hello world", ["/hello/", "/world/"], ["hi", "earth"]);
        finishTest((result == "hi earth"));
    }
    startTest("test50", "正規表現replaceのiフラグで大文字・小文字を区別しない");
    {
        // Test for regex replace - case insensitive
        var result = replace("HELLO world", "/hello/i", "hi");
        finishTest((result == "hi world"));
    }
    startTest("test51", "不正な正規表現のreplaceは元の文字列を返す");
    {
        // Test for regex replace - invalid pattern (should return original)
        var result = replace("hello world", "invalid", "replacement");
        finishTest((result == "hello world"));
    }
    startTest("test52", "正規表現の文字クラス[a-z]と繰り返し+");
    {
        // Test for regex - complex pattern with special chars
        let result = match("/[a-z]+/", "abc");
        finishTest((result == 1));
    }
    startTest("test53", "乗除算・剰余・シフト・ビット演算の複合代入");
    {
        // compound assignment operators
        let a = 6;
        a *= 3;
        a /= 2;
        a %= 5;

        let b = 5;
        b <<= 2;
        b >>= 1;
        b >>>= 1;

        let c = 6;
        c &= 3;
        c |= 8;
        c ^= 3;

        finishTest((a == 4 && b == 5 && c == 9));
    }
    startTest("test54", "typeofとsqrt(-1)で生成したNaNのisNaN・isFinite");
    {
        // typeof + isNaN/isFinite
        let u;
        let o = {};
        function f(x) { return x; }

        // TinyJS本体の /0 は 0 扱いになるため、NaN は sqrt(-1) で作る
        let vNaN = Math.sqrt(-1);

        let result =
            (typeof u == "undefined") &&
            (typeof 1 == "number") &&
            (typeof "abc" == "string") &&
            (typeof o == "object") &&
            (typeof f == "function") &&
            isNaN(vNaN) &&
            !isNaN(1) &&
            isFinite(123.5) &&
            !isFinite(vNaN);
        finishTest(result);
    }
    startTest("test55", "instanceofの一致と不一致を確認する");
    {
        // instanceof
        let Base = {};
        let Other = {};
        let obj = new Base();
        finishTest(((obj instanceof Base) && !(obj instanceof Other)));
    }
    startTest("test56", "ビット反転~で0が-1、5が-6になる");
    {
        // bitwise not
        let a = ~0;
        let b = ~5;
        finishTest((a == -1 && b == -6));
    }
    print("<section class='summary'><h2>テスト結果</h2><p>合計: " + totalTests + "　成功: <span class='status-pass'>" + passedTests + "</span>　失敗: <span class='status-fail'>" + (totalTests - passedTests) + "</span></p>");
    if (passedTests == totalTests) {
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
