<? header("Cache-Control: no-store"); ?>
<!doctype html>
<html lang="ja">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <? print("<meta name=\"pdf-file-url\" content=\"" + htmlspecialchars(_GET.url) + "\" />"); ?>
    <meta
      http-equiv="Content-Security-Policy"
      content="default-src 'self'; script-src 'self'; style-src 'self' 'unsafe-inline'; img-src 'self' data: blob:; font-src 'self' data:; connect-src 'self' https:; worker-src 'self' blob:; object-src 'none'; base-uri 'self'; form-action 'none'"
    />
    <meta
      name="description"
      content="PDF.js を使った、閲覧とリンク遷移に機能を絞った PDF viewer"
    />
    <title>PDF Viewer</title>
    <script type="module" crossorigin src="/system/pdf/assets/index-B-OR0nzh.js"></script>
    <link rel="stylesheet" crossorigin href="/system/pdf/assets/index-DXq0A9ti.css">
  </head>
  <body>
      <header class="toolbar">
      <h1>PDF Viewer</h1>
      <p id="status" role="status" aria-live="polite">PDF を指定してください。</p>
    </header>
    <main id="viewer" aria-label="PDF viewer">
      <section id="message" class="message">
        PDF の URL を開いてください。
      </section>
      <div id="pages" class="pages" aria-label="PDF pages" hidden></div>
    </main>

  </body>
</html>
