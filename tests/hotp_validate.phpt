--TEST--
hotp_validate() function - basic test for hotp_validate()
--FILE--
<?php
$key = '0123456789ABCDEF';

$generatedValue = hotp_generate($key, 1, 6);
var_dump(is_numeric($generatedValue) && is_string($generatedValue));
$valid = hotp_validate($key, $generatedValue, 1);
var_dump($valid);

var_dump(hotp_validate('foobar', $generatedValue, 1));
var_dump(hotp_validate('a0b1c2', $generatedValue, 1));

$generatedValue = sprintf("%06d", $generatedValue + 1);
$valid = hotp_validate($key, $generatedValue, 1);
var_dump($valid);
?>
--EXPECTF--
bool(true)
bool(true)

Warning: OATH_INVALID_HEX: Hex string is invalid %s
bool(false)
bool(false)
bool(false)
