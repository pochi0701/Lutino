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
    print("<br>\ntest07<br>\n");
    print("<br>\ntest08<br>\n");
    print("<br>\ntest09<br>\n");
    print("<br>\ntest10<br>\n");
    print("<br>\ntest11<br>\n");
    print("<br>\ntest12<br>\n");
    print("<br>\ntest13<br>\n");
    print("<br>\ntest14<br>\n");
    print("<br>\ntest15<br>\n");
    print("<br>\ntest16<br>\n");
    
?>
</body>
</html>
