--TEST--
totp_validate() function - basic test for totp_validate()
--FILE--
<?php
$key = '0123456789ABCDEF';

$generatedValue = totp_generate($key, 6, 30);
var_dump(is_numeric($generatedValue) && is_string($generatedValue));
$valid = totp_validate($key, $generatedValue, 6, 30);
var_dump($valid);

var_dump(totp_validate('foobar', $generatedValue, 6, 30));
var_dump(totp_validate('a0b1c2', $generatedValue, 6, 30));

$generatedValue = sprintf("%06d", $generatedValue + 1);
$valid = totp_validate($key, $generatedValue, 6, 30);
var_dump($valid);
?>
--EXPECTF--
bool(true)
bool(true)

Warning: OATH_INVALID_HEX: Hex string is invalid %s
bool(false)
bool(false)
bool(false)
