# TinyJS 組み込み関数一覧

Lutino に組み込まれている TinyJS のネイティブ関数一覧です。実装は主に `source\TinyJS_Functions.cpp` と `source\TinyJS_MathFunctions.cpp` にあります。

この文書は**このリポジトリで実際に登録されている関数**を説明したものです。一般的な JavaScript と同名でも、挙動が完全には一致しないものがあります。

## 前提

- `String.xxx()` / `Array.xxx()` / `Object.xxx()` / `Math.xxx()` の形で登録されているものは、基本的にその値に対するメソッドです。
- 真偽値は多くの関数で **`1` = true / `0` = false** の整数で返ります。
- いくつかの関数は **JSON 文字列** を返します。TinyJS には `JSON.parse` がないため、必要なら `eval(...)` で扱います。**信頼できる入力だけ**に使ってください。
- `Math.PI` と `Math.E` は定数ではなく **関数**です。`Math.PI()` / `Math.E()` と呼びます。

## 実装上の注意

- `String.charAt` / `String.charCodeAt` はコメント上も実装上も**バイト単位**です。
- `String.replace` は**先頭 1 件だけ**、`String.replaceAll` は**全件**置換です。
- `String.split(separator)` は末尾が空要素になるケースで、最後の空文字を返しません。
- `Array.splice(start, deleteCount)` と登録されていますが、実装上は **`Array.splice(start, deleteCount, ...items)`** に対応しています。
- `Math.atanh(a)` は、現状の実装では `atanh` ではなく **`atan` を呼んでいます**。
- `randomUUID()` は UUID v4 風の文字列を返しますが、実装上 `y` 桁は常に `9` になります。

## グローバル関数

| 関数 | 戻り値 | 説明 |
| --- | --- | --- |
| `print(text)` | なし | HTTP 出力または内部出力バッファへ文字列を書き込みます。 |
| `exec(jsCode)` | なし | 文字列として渡した TinyJS コードを実行します。 |
| `eval(jsCode)` | 任意 | 文字列として渡した式を評価し、その結果を返します。JSON 文字列の評価にも使えます。 |
| `trace()` | なし | ルートオブジェクト全体をトレース出力します。デバッグ向けです。 |
| `charToInt(ch)` | `int` | 文字列の先頭 1 文字の文字コードを返します。空文字なら `0`。 |
| `command(path)` | `int` | OS コマンドを実行します。成功時 `1`、失敗時 `0`。 |
| `header(str)` | `int` | HTTP ヘッダ 1 行を追加します。成功時 `1`。 |
| `session_start()` | `int` | セッションを開始し、`_SESSION` を初期化します。新規 Cookie を発行したときは `1`、既存セッション再利用時は現状 `0` を返します。 |
| `setCookie(name,value,expire)` | `int` | Cookie を設定します。`expire` は現在時刻からの秒数です。 |
| `encodeURI(uri)` | `string` | URI エンコードします。 |
| `atob(str)` | `string` | Base64 文字列をデコードします。 |
| `btoa(str)` | `string` | 文字列を Base64 エンコードします。 |
| `dirname(uri)` | `string` | `/` 区切りの親パスを返します。 |
| `basename(uri)` | `string` | `/` 区切りの末尾要素を返します。 |
| `match(pattern,text)` | `int` | 正規表現マッチを行います。結果は `dregex::match` の戻り値です。 |
| `replace(text,pattern,replacement)` | `string` | 正規表現置換を行います。`pattern` / `replacement` は配列でも指定できます。 |
| `getLocalAddress()` | `string` | ローカル IP アドレスを返します。 |
| `getLocalPort()` | `int` | ローカルポート番号を返します。 |
| `Date()` | `string` | 現在時刻の Unix 時間を文字列で返します。 |
| `DBConnect(dbn)` | `string` | DB 接続キーを返します。失敗時は空文字。 |
| `htmlspecialchars(uri)` | `string` | HTML エスケープします。 |
| `file_exists(path)` | `int` | 通常ファイルが存在すれば `1`。 |
| `dir_exists(path)` | `int` | ディレクトリが存在すれば `1`。 |
| `scandir(path)` | `string` | **JSON 文字列**としてディレクトリ内のフルパス配列を返します。 |
| `extractFileExt(uri)` | `string` | 拡張子を返します。 |
| `mimeInfo(uri)` | `string` | **JSON 文字列** `{"mimeType":"...","fileType":"..."}` を返します。未知なら空文字。 |
| `file_stat(path)` | `string` | **JSON 文字列** `{"permission":"...","size":...,"date":"..."}` を返します。存在しない場合は文字列 `"undefined"`。 |
| `filedate(path)` | `string` | 更新時刻の Unix 時間を文字列で返します。存在しない場合は文字列 `"undefined"`。 |
| `loadFromFile(path)` | `string` | ファイル内容をそのまま文字列で読み込みます。 |
| `loadFromCSV(path)` | `string` | CSV を **JSON 文字列の二次元配列**として返します。 |
| `unlink(path)` | `int` | ファイル削除。成功時 `1`。 |
| `touch(path)` | `int` | 空ファイルを作成します。成功時 `1`。 |
| `rename(pathf,patht)` | `int` | リネームします。成功時 `1`。 |
| `mkdir(path)` | `int` | ディレクトリを作成します。成功時 `1`。 |
| `rmdir(path)` | `int` | ディレクトリを削除します。成功時 `1`。 |
| `saveToFile(path,data)` | `int` | 文字列をファイルへ保存します。成功時 `1`。 |
| `copy(pathf,patht)` | `int` | ファイルをコピーします。成功時 `1`。 |
| `shutdown(password)` | なし | システムパスワードが一致した場合、Lutino の終了処理を開始します。 |
| `ssdp()` | `string` | SSDP 検索結果を文字列で返します。 |
| `restful(method,url,send)` | `string` | HTTP REST 呼び出しを行い、応答文字列を返します。 |
| `randomUUID()` | `string` | UUID v4 風の文字列を返します。 |
| `biosUUID()` | `string` | BIOS UUID を返します。 |
| `die(msg)` | 例外 | `CScriptException` を投げて処理を中断します。 |

## Object / JSON / Integer

| 関数 | 戻り値 | 説明 |
| --- | --- | --- |
| `Object.dump()` | なし | `this` オブジェクトをトレース出力します。 |
| `Object.clone()` | object | `this` の**深いコピー**を返します。 |
| `Object.keys(obj)` | array | `obj` のキー一覧を配列で返します。順序は内部保持順です。 |
| `Integer.parseInt(str)` | `int` | `strtol(..., 0, 0)` 相当で整数化します。`0x` 付き 16 進にも対応します。 |
| `Integer.valueOf(str)` | `int` | 長さ 1 の文字列ならその文字コード、そうでなければ `0`。 |
| `JSON.stringify(obj, replacer)` | `string` | JSON 文字列へ変換します。`replacer` 引数は**無視**されます。 |
| `JSON.mp3id3tag(path)` | `string` | MP3 の ID3 タグを **JSON 文字列**で返します。 |

## String メソッド

| 関数 | 戻り値 | 説明 |
| --- | --- | --- |
| `String.indexOf(search)` | `int` | 部分文字列の位置を返します。見つからなければ `-1`。 |
| `String.substring(lo,hi)` | `string` | `lo` から `hi` 手前までを返します。`hi` は省略可です。範囲外は丸められ、`lo > hi` なら入れ替えられます。 |
| `String.substr(lo,hi)` | `string` | `lo` から長さ `hi` 分を返します。範囲外は空文字。 |
| `String.startsWith(lo)` | `int` | 前方一致なら `1`。 |
| `String.endsWith(lo)` | `int` | 後方一致なら `1`。 |
| `String.charAt(pos)` | `string` | 指定位置の 1 文字を返します。範囲外は空文字。**バイト単位**です。 |
| `String.charCodeAt(pos)` | `int` | 指定位置の文字コードを返します。範囲外は `0`。**バイト単位**です。 |
| `String.fromCharCode(char)` | `string` | 数値から 1 文字を生成します。`this` は使いません。 |
| `String.split(separator)` | array | 区切り文字で分割した配列を返します。 |
| `String.replace(before,after)` | `string` | 最初の 1 件だけ置換します。 |
| `String.replaceAll(before,after)` | `string` | すべて置換します。 |
| `String.preg_replace(pattern,replace)` | `string` | 正規表現置換を行います。`pattern` / `replace` は配列でも指定できます。 |
| `String.addSlashes()` | `string` | `'` / `"` / `\` / `NUL` をバックスラッシュでエスケープします。 |
| `String.toLowerCase()` | `string` | ASCII ベースで小文字化します。 |
| `String.toUpperCase()` | `string` | ASCII ベースで大文字化します。 |
| `String.toDateString(format)` | `string` | `this` を Unix 時間文字列とみなし、`strftime` 形式で整形します。 |
| `String.nkfconv(format)` | `string` | 文字コード変換を行います。内部で `nkfcnv` を呼びます。 |
| `String.DBDisConnect()` | `int` | DB 接続キー文字列に対して切断します。成功時 `0`、キー未登録時 `-1`。 |
| `String.SQL(sqltext)` | `string` | DB 接続キー文字列に対して SQL を実行します。結果は JSON 文字列または `OK` / エラー文字列です。 |
| `String.trim()` | `string` | 前後の空白を除去します。 |
| `String.rtrim()` | `string` | 末尾の空白を除去します。 |
| `String.ltrim()` | `string` | 先頭の空白を除去します。 |

## Array メソッド

| 関数 | 戻り値 | 説明 |
| --- | --- | --- |
| `Array.contains(obj)` | `int` | 同値要素を含めば `1`。 |
| `Array.remove(obj)` | なし | 一致する要素をすべて削除し、配列を詰めます。 |
| `Array.join(separator)` | `string` | 要素を結合します。 |
| `Array.push(val)` | `int` | 末尾に追加し、新しい長さを返します。 |
| `Array.pop()` | 任意 | 末尾要素を返して削除します。空配列なら `undefined`。 |
| `Array.shift()` | 任意 | 先頭要素を返して削除します。空配列なら `undefined`。 |
| `Array.unshift(val)` | `int` | 先頭に追加し、新しい長さを返します。 |
| `Array.indexOf(val)` | `int` | 最初に一致した位置を返します。見つからなければ `-1`。 |
| `Array.slice(start,end)` | array | 部分配列を返します。`end` は省略可、負数インデックス対応です。 |
| `Array.splice(start,deleteCount, ...items)` | array | 要素を削除し、必要なら挿入します。戻り値は削除された要素配列です。 |

## Math

### 乱数

| 関数 | 戻り値 | 説明 |
| --- | --- | --- |
| `Math.rand()` | `double` | `0.0` 以上 `1.0` 以下の乱数を返します。 |
| `Math.randInt(min,max)` | `int` | `min` 以上 `max` 以下の整数乱数を返します。 |

### 基本演算

| 関数 | 戻り値 | 説明 |
| --- | --- | --- |
| `Math.abs(a)` | int/double | 絶対値。 |
| `Math.round(a)` | int/double | 四捨五入。 |
| `Math.min(a,b)` | int/double | 小さい方。 |
| `Math.max(a,b)` | int/double | 大きい方。 |
| `Math.range(x,a,b)` | int/double | `x` を `a` 以上 `b` 以下に丸めます。 |
| `Math.sign(a)` | int/double | 負なら `-1`、ゼロなら `0`、正なら `1`。 |
| `Math.sqr(a)` | `double` | 二乗。 |
| `Math.sqrt(a)` | `double` | 平方根。 |
| `Math.pow(a,b)` | `double` | べき乗。 |
| `Math.log(a)` | `double` | 自然対数。 |
| `Math.log10(a)` | `double` | 常用対数。 |
| `Math.exp(a)` | `double` | `e^a`。 |
| `Math.PI()` | `double` | 円周率。 |
| `Math.E()` | `double` | ネイピア数。 |

### 角度・三角関数

| 関数 | 戻り値 | 説明 |
| --- | --- | --- |
| `Math.toDegrees(a)` | `double` | ラジアンを度へ変換します。 |
| `Math.toRadians(a)` | `double` | 度をラジアンへ変換します。 |
| `Math.sin(a)` | `double` | 正弦。 |
| `Math.asin(a)` | `double` | 逆正弦。 |
| `Math.cos(a)` | `double` | 余弦。 |
| `Math.acos(a)` | `double` | 逆余弦。 |
| `Math.tan(a)` | `double` | 正接。 |
| `Math.atan(a)` | `double` | 逆正接。 |
| `Math.sinh(a)` | `double` | 双曲線正弦。 |
| `Math.asinh(a)` | `double` | 双曲線逆正弦。 |
| `Math.cosh(a)` | `double` | 双曲線余弦。 |
| `Math.acosh(a)` | `double` | 双曲線逆余弦。 |
| `Math.tanh(a)` | `double` | 双曲線正接。 |
| `Math.atanh(a)` | `double` | 名前は `atanh` ですが、現状の実装は `atan(a)` を返します。 |

## JSON 文字列を返す関数

そのままでは**文字列**なので、配列やオブジェクトとして使いたい場合は `eval` を使います。

| 関数 | 返るもの |
| --- | --- |
| `scandir(path)` | パス配列の JSON 文字列 |
| `mimeInfo(uri)` | MIME 情報オブジェクトの JSON 文字列 |
| `file_stat(path)` | ファイル属性オブジェクトの JSON 文字列 |
| `loadFromCSV(path)` | 二次元配列の JSON 文字列 |
| `JSON.mp3id3tag(path)` | MP3 タグ情報オブジェクトの JSON 文字列 |
| `String.SQL(sqltext)` | SELECT 系では JSON 文字列になることがあります |

例:

```javascript
var files = eval(scandir("html"));
var stat = eval("(" + file_stat("README.md") + ")");
var rows = eval(loadFromCSV("data.csv"));
```

## 使用例

### 文字列

```javascript
print("  Lutino  ".trim());                 // "Lutino"
print("abcabc".replace("ab", "XY"));       // "XYcabc"
print("abcabc".replaceAll("ab", "XY"));    // "XYcXYc"
print("hello.txt".endsWith(".txt"));       // 1
```

### 配列

```javascript
var a = [1, 2, 3];
a.push(4);                 // 4
print(a.join(","));        // "1,2,3,4"
var removed = a.splice(1, 2);
print(removed.join(","));  // "2,3"
print(a.join(","));        // "1,4"
```

### 日付

```javascript
var now = Date();
print(now);                                  // Unix 時間文字列
print(now.toDateString("%Y-%m-%d %H:%M:%S"));
```

### ファイル

```javascript
if (file_exists("README.md")) {
  var text = loadFromFile("README.md");
  saveToFile("work\\copy.txt", text);
}
```

### データベース

```javascript
var db = DBConnect("main");
if (db != "") {
  var rows = db.SQL("select * from sample");
  print(rows);
  db.DBDisConnect();
}
```

## 未実装・非搭載

- `JSON.parse` は登録されていません。
- この文書にない JavaScript 標準 API は、TinyJS 本体では使えない可能性があります。

