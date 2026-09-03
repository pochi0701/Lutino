<?
   var data = "ddd";
   var dat = loadFromFile("template.html");
   var html = dat.replaceAll("{{data}}", data);
   print(html);
?>