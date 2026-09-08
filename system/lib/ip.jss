<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>ip address</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB" crossorigin="anonymous">
    <script src="https://cdnjs.cloudflare.com/ajax/libs/qrcodejs/1.0.0/qrcode.min.js"></script>
</head>
<body>
    <div class="container">
        <button type="button" class="btn btn-primary" onClick="(window.history.length>1)?history.back():window.close();">戻る</button><br/>
        <?
        la = getLocalAddress();
        lp = getLocalPort();
        ga = loadFromFile("http://neon.cx/lutino/ip.php");
        ?>
        Local Address<br />
        <?print(la + ":" + lp + "/"); ?><br />
        <div id="qrcodeCanvas"></div>
        <br />
        <br />
        <br />
        Global Address<br />
        グローバルアドレスは、サーバーが外部に開放されてないと見ることができません<br/>
        <? print(ga + ":" + lp + "/"); ?><br />
        <div id="qrcodeCanvas2"></div>
        <script>
             new QRCode(document.getElementById("qrcodeCanvas"), { width: 96, height: 96, text: "http://<? print(la + ":" + lp + "/"); ?>" });
             new QRCode(document.getElementById("qrcodeCanvas2"), { width: 96, height: 96, text: "http://<? print(ga + ":" + lp + "/"); ?>" });
        </script>
    </div>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/js/bootstrap.bundle.min.js" integrity="sha384-FKyoEForCGlyvwx9Hj09JcYn3nv7wiPVlz7YYwJrWVcXK/BmnVDxM+D2scQbITxI" crossorigin="anonymous"></script>
</body>
</html>
