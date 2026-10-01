# Lutino

軽量な Web / Application Server です。サーバー側スクリプトとして TinyJS を内蔵し、組み込み SQL と HTTPS/TLS に対応しています。

## Quick Start

### Windows
1. `Lutino.exe` と `ltn.conf` を同じ構成で配置します（このリポジトリの `html` / `database` / `system` / `skin` も利用）。
2. `Lutino.exe` を起動します。
3. `http://localhost:8000/` にアクセスします（既定設定）。

PDF viewer を利用する場合は、起動前に Node.js 22.13 以降を用意して `cd MyPDF` → `npm install` → `npm run build` を実行してください。生成物の `pdfView.jss` とアセットは `system\pdf\` に配置されます（git 管理外）。配布時は `system\pdf\` も `system\` と一緒にコピーします。旧 `html\mypdf\` および `system\pdf\index.html` は配布物から撤去してください。

### Linux (CMake)
```sh
cmake -S . -B build
cmake --build build
cd MyPDF && npm install && npm run build && cd ..
cmake --install build
```

`cmake --install build` の `build` は、`cmake -B build` で指定した**ビルドディレクトリ**です（フォルダー名は任意）。
`make -C source install` を使う場合も、先に同じ MyPDF ビルドが必要です。viewer の生成物がない場合はインストールを中止します。

## PDF viewer

PDF を閲覧するときは `/system/common/cheat-sheet-markdown.pdf` のように元の PDF URL を開きます。Lutino はリダイレクトせずサーバー内部で `pdfView.jss` を実行し、viewer は同一オリジンの `/system/common/cheat-sheet-markdown.pdf?mypdf_raw=1` から PDF を取得します。PDF のクエリーは `&` 区切りで完全一致する `mypdf_raw=1` のみを残し、旧 action やその他のパラメータは内部で除去します。有効な raw 指定が1つでもあれば PDF 本体を返し、それ以外は viewer を表示します。ブラウザーのアドレスバーのクエリーは変更しません。PDF ファイル一覧やヘルプも通常の PDF URL にリンクします。PDF の編集・ダウンロード操作は表示しません。PDF 以外のファイル配信は従来どおりです。

既存の `alias /html/system /system` が有効な間、viewer を含む `/system` 全体が公開されます。この alias を解除すると通常 PDF URL からの viewer 表示はエラーになりますが、`html` 配下の PDF の raw 取得を禁止するものではありません。また他の `/system` のページも利用できなくなります。JSS を実行するため、`flag_execute_cgi` を無効にしないでください。

コピー、印刷、保存の抑止は **viewer の UI に限る補助措置**です。`?mypdf_raw=1` の PDF URL は公開されており、ブラウザーへの手入力、HTTP クライアントや開発者ツール等で PDF 本体を取得できます。Cookie や User-Agent では viewer の取得と手入力を確実に区別できません。閲覧権限の追加や複製防止を保証するものではありません。実際のアクセス制限が必要なら、別途サーバー側の認証・認可と画像配信方式を検討してください。

サーバーを起動した状態で `cd MyPDF` → `LUTINO_BASE_URL=http://127.0.0.1:8000 npm test`（PowerShell では `$env:LUTINO_BASE_URL='http://127.0.0.1:8000'; npm test`）により HTTP 統合テストを実行できます。

## 主な機能
- TinyJS によるサーバーサイドスクリプト実行
- 組み込み SQL（結果は JSON 形式で取得可能）
- Web Editor
- Markdown ファイル表示
- 動画ストリーミング
- HTTPS/TLS（BearSSL ベンダリング実装）

## 設定ファイル (`ltn.conf`)
主な設定項目:
- `server_port` : HTTP ポート（既定: `8000`）
- `server_tls_port` : HTTPS ポート（既定: `8443`、`0` で無効）
- `document_root` / `skin_root` : コンテンツのルート
- `skin_name` / `skin_menu` : スキン設定

## TinyJS
- 組み込み関数リファレンス: `TINYJS_BUILTINS.md`
- 現在反映済みの主な言語拡張:
  - `let` / `const` / `typeof` / `instanceof`
  - 厳密比較 `===` / `!==`
  - 符号なしシフト `>>>` / `>>>=`
  - `const` 再代入時の例外化
  - `String.substring(lo, hi)` の `hi` 省略対応
  - `Math.atanh(a)` の `std::atanh` 実装
  - バッククォート文字列 `` `...` ``（`${...}` 補完は未対応）

## TLS / HTTPS
- HTTP に加えて HTTPS を利用できます。
- TLS は `source\bearssl\` の BearSSL ソースを利用しています。
- 開発用として埋め込みテスト証明書を使用する実装段階です。

## Linux インストール補足
- Makefile と CMake の既定インストール先は `/var/www/html9` です。
- Makefile は `INSTALL_DIR`、CMake は `CMAKE_INSTALL_PREFIX` で変更できます。
- インストール先が存在しなければ自動的に作成します。
- インストール先が空でなければ、既存内容を変更せず警告して終了します。
- `lutino`、`ltn.conf`、`html/`、`database/`、`skin/`、`system/` を配置します。
- 詳細: `CMAKE_INSTALL.md`

## History

### ver. 1.00
- プロジェクト名を `wizd` / `Cybele` から `Lutino` に変更
- MFC 依存を削除
