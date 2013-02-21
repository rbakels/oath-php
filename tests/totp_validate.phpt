--TEST--
totp_validate() function - basic test for totp_validate()
--FILE--
<?php
$key = '0123456789ABCDEF';
$generatedValue = totp_generate($key, 6, 30);
$valid = totp_validate($key, $generatedValue, 6, 30);
var_dump($valid);
?>
--EXPECT--
bool(true)