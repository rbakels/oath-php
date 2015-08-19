--TEST--
google_authenticator_validate() function - basic test for google_authenticator_validate()
--FILE--
<?php
$key = '0123456789ABCDEF';

$generatedValue = google_authenticator_generate($key);
var_dump(is_numeric($generatedValue) && is_string($generatedValue));
$valid = google_authenticator_validate($key, $generatedValue);
var_dump($valid);

var_dump(google_authenticator_validate('foobar', $generatedValue));
var_dump(google_authenticator_validate('a0b1c2', $generatedValue));

$generatedValue = sprintf("%06d", $generatedValue + 1);
$valid = google_authenticator_validate($key, $generatedValue);
var_dump($valid);
?>
--EXPECTF--
bool(true)
bool(true)

Warning: OATH_INVALID_HEX: Hex string is invalid %s
bool(false)
bool(false)
bool(false)
