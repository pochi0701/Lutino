<?
   // String.sqlBind(params) は、":key" 形式のプレースホルダーを
   // params オブジェクトの値でSQL標準のエスケープ（文字列は '...' で
   // 囲み、内部の ' は '' に置換）を行いながら安全に置換します。
   // eval や文字列連結で値を直接埋め込む方式と違い、値に引用符が
   // 含まれていてもSQL文の構造が壊れません。

   let a = 10;
   let n = "O'Brien"; // シングルクォートを含む値
   let base = "select * from t where length>:a and name=:n";
   let sql = base.sqlBind({a: a, n: n});
   print("元となるSQL: " + base + "<br>\n");
   print("組み立てられたSQL: " + sql + "<br>\n");

   // DB接続がある場合は、そのまま実行できます:
   // var db = DBConnect("main");
   // if (db != "") {
   //     print(db.SQL(sql));
   //     db.DBDisConnect();
   // }

   // 未対応キーを指定するとエラーになることの確認:
   print("エラーとなる場合:"+'"select * from t where x=:missing".sqlBind({a: 1});'+"<br>\n");
   try {
       "select * from t where x=:missing".sqlBind({a: 1});
   } catch (e) {
       print("予想エラー: " + e + "<br>");
   }
?>
