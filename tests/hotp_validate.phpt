--TEST--
hotp_validate() function - basic test for hotp_validate()
--FILE--
<?php
$key = '0123456789ABCDEF';
$generatedValue = hotp_generate($key, 1, 6);
$valid = hotp_validate($key, $generatedValue, 1, 6);
var_dump($valid);
?>
--EXPECT--
bool(true)