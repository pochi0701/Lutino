<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="utf-8">
    <title>JSSテスト</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
<?
    print("<br>\ntest01 ");
    {
        let result = 1;
        print( (result == 1)?"OK":"NG" );
    }
    print("<br>\ntest02 ");
    {
        var a  = 42;
        print( (a == 42)?"OK":"NG" );
    }
    print("<br>\ntest03 ");
    {
        let a = 0;
        let i ;
        for (i=1;i<10;i++) a = a + i;
        print( (a == 45)?"OK":"NG" );
    }
    print("<br>\ntest04 ");
    {
        let a = 42;
        print( (a < 43)?"OK":"NG" );
    }
    print("<br>\ntest05 ");
    {
        let a = 0;
        for (let i=1;i<10;i++) a += i;
        print( (a == 45)?"OK":"NG" );
    }
    print("<br>\ntest06 ");
    {
        // simple function
        function add(x,y) { return x+y; }
        print( (add(3,6)==9)?"OK":"NG" );
    }
    print("<br>\ntest07 ");
    {
        // simple function scoping test
        let a = 7;
        function add(x,y) { var a=x+y; return a; }
        print( (add(3,6)==9 && a==7)?"OK":"NG");
    }
    print("<br>\ntest08 ");
    {
        // functions in variables
        let bob = {};
        bob.add = function(x,y) { return x+y; };
        print( (bob.add(3,6)==9)?"OK":"NG");
    }
    print("<br>\ntest09 ");
    {
        // functions in variables using JSON-style initialisation
        let bob = { add : function(x,y) { return x+y; } };
        print( (bob.add(3,6)==9)?"OK":"NG");
    }
    print("<br>\ntest10 ");
    {
        // double function calls
        function a(x) { return x+2; }
        function b(x) { return a(x)+1; }
        print( (a(3)==5 && b(3)==6)?"OK":"NG");
    }
    print("<br>\ntest11 ");
    {
        // recursion
        function a(x) { 
            if (x>1)
                return x*a(x-1);
            return 1;
        }
        print( (a(5)==1*2*3*4*5)?"OK":"NG");
    }

    print("<br>\ntest12 ");
    {
        // if .. else
        let a = 42;
        let result = -1;
        if (a != 42)
            result = 0;
        else
            result = 1;
        print( (result == 1)?"OK":"NG");
    }
    print("<br>\ntest13 ");
    {
        // if .. else with blocks
        let a = 42;
        let result = -1;
        if (a != 42) {
            result = 0;
        } else {
            result = 1;
        }
        print( (result == 1)?"OK":"NG");
    }
    print("<br>\ntest14 ");
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
        print( (blah==0 && z!='foxes' && twenty==20)?"OK":"NG");
    }
    print("<br>\ntest15 ");
    {
        // Number definition from http://en.wikipedia.org/wiki/JavaScript_syntax
        let a = 345;    // an "integer", although there is only one numeric type in JavaScript
        let b = 34.5;   // a floating-point number
        let c = 3.45e2; // another floating-point, equivalent to 345
        let d = 0377;   // an octal integer equal to 255
        let e = 0xFF;   // a hexadecimal integer equal to 255, digits represented by the letters A-F may be upper or lowercase
        print((a==345 && b*10==345 && c==345 && d==255 && e==255)?"OK":"NG");
    }
    print("<br>\ntest16 ");
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

        print((result == 1)?"OK":"NG");
    }
    print("<br>\ntest17 ");
    {
        // references for arrays
        let a = [];
        a[0] = 10;
        a[1] = 22;

        let b = a;
        b[0] = 5;

        print((a[0]==5 && a[1]==22 && b[1]==22)?"OK":"NG");
    }
    print("<br>\ntest18 ");
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
        print((a==42 && b[0]==44)?"OK":"NG");
    }
    print("<br>\ntest19 ");
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

        print((foo.length==13 && foo.indexOf("bar")==4 && foo.substring(8,13)=="stuff" && parsed==42 && Integer.valueOf(aChar)==65 && obj1.food=="cake" && obj2.desert=="pie")?"OK":"NG");
    }
    print("<br>\ntest20 ");
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
        print((true)?"OK":"NG");
    }
    print("<br>\ntest21 ");
    {
        /* Javascript eval */
        // 42-tiny-js change begin --->
        // in JavaScript eval is not JSON.parse
        // use parentheses or JSON.parse instead
        // myfoo = eval("{ foo: 42 }");
        let myfoo = eval("("+"{ foo: 42 }"+")");
        print((eval("4*10+2")==42 && myfoo.foo==42)?"OK":"NG");
    }
    print("<br>\ntest22 ");
    {
        // test for array remove
        let a = [1,2,4,5,7];
        a.remove(2);
        a.remove(5);

        print((a.length==3 && a[0]==1 && a[1]==4 && a[2]==7)?"OK":"NG");
    }
    print("<br>\ntest23 ");
    {
        // mikael.kindborg@mobilesorcery.com - Function symbol is evaluated in bracket-less body of false if-statement
        let foo; // a var is only created automated by assignment

        if (foo !== undefined) foo();

        print((true)?"OK":"NG");
    }
    print("<br>\ntest24 ");
    {
        // 省略
        print((true)?"OK":"NG");
    }
    print("<br>\ntest25 ");
    {
        // Array length test
        myArray = [ 1, 2, 3, 4, 5 ];
        myArray2 = [ 1, 2, 3, 4, 5 ];
        myArray2[8] = 42;
        print((myArray.length == 5 && myArray2.length == 9)?"OK":"NG");
    }
    print("<br>\ntest26 ");
    {
        // check for undefined-ness
        let a = undefined;
        let b = "foo";
        print((a==undefined && b!=undefined)?"OK":"NG");
    }
    print("<br>\ntest27 ");
    {
        // test for postincrement working as expected
        let foo = 5;
        let bar = 5; 
        print(((foo++)==5 && (++bar)==6)?"OK":"NG");
    }
    print("<br>\ntest28 ");
    {
        // test for array contains
        let a = [1,2,4,5,7];
        let b = ["bread","cheese","sandwich"];
        print((a.contains(1) && !a.contains(42) && b.contains("cheese") && !b.contains("eggs"))?"OK":"NG");
    }
    print("<br>\ntest29 ");
    {
        let a = [1,2,4,5,7];
        a.remove(2);
        a.remove(5);
        print((a.length==3 && a[0]==1 && a[1]==4 && a[2]==7)?"OK":"NG");
    }
    print("<br>\ntest30 ");
    {
        let a = [1,2,4,5,7];
        print((a.join(",")=="1,2,4,5,7")?"OK":"NG");
    }
    print("<br>\ntest31 ");
    {
        // test for string split
        let b = "1,4,7";
        let a = b.split(",");
        print((a.length==3 && a[0]==1 && a[1]==4 && a[2]==7)?"OK":"NG");
    }
    print("<br>\ntest32 ");
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

        print((result1==3 && result2==5)?"OK":"NG");
    }
    print("<br>\ntest33 ");
    {
        // test for shift
        var a = (2<<2);
        var b = (16>>3);
        var c = (-1 >>> 16);

        print((a==8 && b==2 && c == 0xFFFF)?"OK":"NG");
    }
    print("<br>\ntest34 ");
    {
        // test for ternary
        print(((true?3:4)==3 && (false?5:6)==6)?"OK":"NG");

    }
    print("<br>\ntest35 ");
    {
        function Person(name) {
          this.name = name;
          this.kill = function() { this.name += " is dead"; };
        }
        let a = new Person("Kenny");
        a.kill();

        print((a.name == "Kenny is dead")?"OK":"NG");
    }
    print("<br>\ntest36 ");
    {
        // the 'lf' in the printf caused issues writing doubles on some compilers
        var a=5.0/10.0*100.0;
        var b=5.0*110.0;
        var c=50.0/10.0;
        a.dump();
        b.dump();
        c.dump();

        print((a==50 && b==550 && c==5)?"OK":"NG");
    }

    print("<br>\ntest37 ");
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

        print(( a==45)?"OK":"NG");
    }
    print("<br>\ntest38 ");
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
        print((a==3)?"OK":"NG");
    }
    print("<br>\ntest39 ");
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

        print((test01 == 1 && test02 == 1)?"OK":"NG");
    }
    print("<br>\ntest40 ");
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
        print(((sum1 == 45) && (sum2 == 45) && (sum3 == 45) && (sum4 == 45))?"OK":"NG");
    }
    print("<br>\ntest41 ");
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
        print((result01 && result02 && result03 && result04 && result05 && result06)?"OK":"NG");
    }
    print("<br>\ntest42 ");
    {
        // Test for regex match - basic pattern
        let result = match("/hello/", "hello world");
        print((result == 1)?"OK":"NG");
    }
    print("<br>\ntest43 ");
    {
        // Test for regex match - no match
        let result = match("/goodbye/", "hello world");
        print((result == 0)?"OK":"NG");
    }
    print("<br>\ntest44 ");
    {
        // Test for regex match - case insensitive
        let result = match("/HELLO/i", "hello world");
        print((result == 1)?"OK":"NG");
    }
    print("<br>\ntest45 ");
    {
        // Test for regex match - invalid pattern (should not crash)
        let result = match("invalid", "test");
        print((result == 0)?"OK":"NG");
    }
    print("<br>\ntest46 ");
    {
        // Test for regex replace - basic
        var result = replace("hello world", "/world/", "universe");
        print((result == "hello universe")?"OK":"NG");
    }
    print("<br>\ntest47 ");
    {
        // Test for regex replace - multiple patterns
        var result = replace("hello world", ["/hello/", "/world/"], ["hi", "earth"]);
        print((result == "hi earth")?"OK":"NG");
    }
    print("<br>\ntest48 ");
    {
        // Test for regex replace - case insensitive
        var result = replace("HELLO world", "/hello/i", "hi");
        print((result == "hi world")?"OK":"NG");
    }
    print("<br>\ntest49 ");
    {
        // Test for regex replace - invalid pattern (should return original)
        var result = replace("hello world", "invalid", "replacement");
        print((result == "hello world")?"OK":"NG");
    }
    print("<br>\ntest50 ");
    {
        // Test for regex - complex pattern with special chars
        let result = match("/[a-z]+/", "abc");
        print((result == 1)?"OK":"NG");
    }
    print("<br>\ntest51 ");
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

        print((a == 4 && b == 5 && c == 9)?"OK":"NG");
    }
    print("<br>\ntest52 ");
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
        print(result?"OK":"NG");
    }
    print("<br>\ntest53 ");
    {
        // instanceof
        let Base = {};
        let Other = {};
        let obj = new Base();
        print(((obj instanceof Base) && !(obj instanceof Other))?"OK":"NG");
    }
    print("<br>\ntest54 ");
    {
        // bitwise not
        let a = ~0;
        let b = ~5;
        print((a == -1 && b == -6)?"OK":"NG");
    }
?>
</body>
</html>
