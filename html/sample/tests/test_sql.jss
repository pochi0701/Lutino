<!DOCTYPE html>
<html lang="ja">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>SQLテスト</title>
    <link rel="stylesheet" href="/sample/tests/test-results.css">
</head>
<body>
<main class="test-page">
    <h1>SQLテスト</h1>
    <section class="test-results">
<?
    // Database SQL Test Suite
    // Simplified based on initSQL.jss pattern to avoid memory leaks
    
    let database = DBConnect("_SYSTEM");
    let test_count = 0;
    let pass_count = 0;

    // Cleanup from previous runs (ignore errors if table does not exist)
    database.SQL("drop table t1;");
    database.SQL("drop table t2;");
    database.SQL("drop table t3;");
    database.SQL("drop table t_ins;");
    database.SQL("drop table t_where;");
    database.SQL("drop table t_right_l;");
    database.SQL("drop table t_right_r;");
    database.SQL("drop table t_sort;");
    
    function renderResult(label, passed) {
        print("<div class='test-result'><span class='test-name'>" + label + "</span><span class='test-status " + (passed ? "status-pass" : "status-fail") + "'>" + (passed ? "PASS" : "FAIL") + "</span></div>\n");
    }

    function testDDL(label, sql) {
        test_count++;
        var ret = database.SQL(sql);
        var passed = ret.indexOf("OK") >= 0;
        if (passed) pass_count++;
        renderResult(label, passed);
        return passed;
    }
    
    function testSelect(label, sql, fragment) {
        test_count++;
        var ret = database.SQL(sql);
        var passed = false;
        if (ret.indexOf("[") === 0 || ret.indexOf("{") === 0) {
            passed = ret.indexOf(fragment) >= 0;
        }
        if (passed) pass_count++;
        renderResult(label, passed);
        return passed;
    }
    
    // ============= Table Creation Tests =============
    print("<h3>1. Table Creation</h3>");
    testDDL("create t1", "create table t1(id number, v1 string);");
    testDDL("create t2", "create table t2(id number, v2 string);");
    testDDL("create t3", "create table t3(id number, v3 string);");
    
    // ============= INSERT Tests =============
    print("<h3>2. INSERT Operations</h3>");
    testDDL("insert t1-1", "insert into t1(id,v1) values(1,'a');");
    testDDL("insert t1-2", "insert into t1(id,v1) values(2,'b');");
    testDDL("insert t2-1", "insert into t2(id,v2) values(1,'x');");
    testDDL("insert t2-2", "insert into t2(id,v2) values(2,'y');");
    testDDL("insert t3-1", "insert into t3(id,v3) values(1,'m');");
    testDDL("insert t3-2", "insert into t3(id,v3) values(2,'n');");
    
    // ============= MULTI-JOIN Tests =============
    print("<h3>3. Multi-JOIN Operations</h3>");
    testSelect("multi join", "select t1.id,t1.v1,t2.v2,t3.v3 from t1 join t2 on t1.id=t2.id join t3 on t2.id=t3.id;", "b");
    
    // ============= Column Mapping Tests =============
    print("<h3>4. INSERT with Column Mapping</h3>");
    testDDL("create ins-map", "create table t_ins(a number, b number);");
    testDDL("insert swapped", "insert into t_ins(b,a) values(1,2);");
    testSelect("verify mapping", "select a,b from t_ins;", "2");
    
    // ============= WHERE Clause Tests =============
    print("<h3>5. WHERE Clause Tests</h3>");
    testDDL("create t_where", "create table t_where(id number, v string);");
    testDDL("insert where-1", "insert into t_where(id,v) values(1,'a');");
    testDDL("insert where-2", "insert into t_where(id,v) values(2,'b');");
    testSelect("select where", "select * from t_where where id=1;", "a");
    
    // ============= RIGHT JOIN Tests =============
    print("<h3>6. RIGHT JOIN Operations</h3>");
    testDDL("create right_l", "create table t_right_l(id number);");
    testDDL("create right_r", "create table t_right_r(id number);");
    testDDL("insert right_l", "insert into t_right_l(id) values(1);");
    testDDL("insert right_r-1", "insert into t_right_r(id) values(1);");
    testDDL("insert right_r-2", "insert into t_right_r(id) values(2);");
    testSelect("right join", 
    "select t_right_l.id,t_right_r.id from t_right_l right join t_right_r on t_right_l.id=t_right_r.id;"
    , "2");
    
    // ============= ORDER BY Tests =============
    print("<h3>7. ORDER BY Operations</h3>");
    testDDL("create sort", "create table t_sort(id number);");
    testDDL("insert -10", "insert into t_sort(id) values(-10);");
    testDDL("insert -2", "insert into t_sort(id) values(-2);");
    testDDL("insert 1", "insert into t_sort(id) values(1);");
    testSelect("order asc", "select id from t_sort order by id asc;", "-10");
    testSelect("order desc", "select id from t_sort order by id desc;", "1");
    
    // ============= Test Summary =============
    print("<section class='summary'><h2>テスト結果</h2><p>合計: " + test_count + "　成功: <span class='status-pass'>" + pass_count + "</span>　失敗: <span class='status-fail'>" + (test_count - pass_count) + "</span></p>");
    
    if (pass_count === test_count) {
        print("<p class='test-status status-pass'>すべて成功</p></section>");
    } else {
        print("<p class='test-status status-fail'>失敗したテストがあります</p></section>");
    }
    
    // Cleanup from previous runs (ignore errors if table does not exist)
    database.SQL("drop table t1;");
    database.SQL("drop table t2;");
    database.SQL("drop table t3;");
    database.SQL("drop table t_ins;");
    database.SQL("drop table t_where;");
    database.SQL("drop table t_right_l;");
    database.SQL("drop table t_right_r;");
    database.SQL("drop table t_sort;");
    // Close database connection
    database.DBDisConnect();
?>
    </section>
    <a class="menu-link" href="/sample/tests/?">ファイルメニューに戻る</a>
</main>
</body>
</html>
