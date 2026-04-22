<?
print("Testing replace() function:\n");

print("test1: ");
var result = "";
replace(result, "hello world", "/world/", "universe");
print("result='" + result + "' (expected 'hello universe')\n");

print("test2: ");
result = "";
replace(result, "hello world", "/goodbye/", "bye");
print("result='" + result + "' (expected 'hello world')\n");

print("test3: ");
result = "";
replace(result, "HELLO world", "/hello/i", "hi");
print("result='" + result + "' (expected 'hi world')\n");
?>
