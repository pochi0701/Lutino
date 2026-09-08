<?
url = _GET.url;
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Image View<? print(url); ?></title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB" crossorigin="anonymous">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/js/bootstrap.bundle.min.js" integrity="sha384-FKyoEForCGlyvwx9Hj09JcYn3nv7wiPVlz7YYwJrWVcXK/BmnVDxM+D2scQbITxI" crossorigin="anonymous"></script>
    <style>
        .thumbnail {
            cursor: pointer;
            transition: transform 0.3s;
        }
        .expanded {
            transform: scale(2);
        }
    </style>
</head>
<body>
    <div class="container" text-center mt-5">
        <button type="button" class="btn btn-primary" onClick="(window.history.length>1)?history.back():window.close();">戻る</button>
        <br>
        <img id="image" src="<?print(url);?>" alt="zoom" class="thumbnail img-thumbnail" style="width: 200px;" onclick="expandImage()">
    </div>
    <script>
        function expandImage() {
            const img = document.getElementById('image');
            img.classList.toggle('expanded');
        }
    </script>
</body>
</html>
