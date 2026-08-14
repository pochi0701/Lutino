<?
url = _GET.url;
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Image View<? print(url); ?></title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65" crossorigin="anonymous">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4" crossorigin="anonymous"></script>
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
