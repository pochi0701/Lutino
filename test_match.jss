<?
print("Testing match() function:\n");
print("test1: ");
let result = match("/hello/", "hello world");
print((result == 1)?"PASS":"FAIL (expected 1, got " + result + ")\n");

print("test2: ");
result = match("/goodbye/", "hello world");
print((result == 0)?"PASS":"FAIL (expected 0, got " + result + ")\n");

print("test3: ");
result = match("/HELLO/i", "hello world");
print((result == 1)?"PASS":"FAIL (expected 1 for case-insensitive, got " + result + ")\n");

print("test4: ");
result = match("invalid", "test");
print((result == 0)?"PASS":"FAIL (expected 0 for invalid pattern, got " + result + ")\n");

print("test5: ");
result = match("/[a-z]+/", "abc");
print((result == 1)?"PASS":"FAIL (expected 1 for character class, got " + result + ")\n");

print("\nAll tests completed!");
?>
