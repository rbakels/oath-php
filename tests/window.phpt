--TEST--
oath.window ini entry
--FILE--
<?php
$key = '0123456789ABCDEF';

$generatedValue = totp_generate($key, 6, 1);
$valid = totp_validate($key, $generatedValue, 6, 1);
var_dump($valid);

sleep(1);

$valid = totp_validate($key, $generatedValue, 6, 1);
var_dump($valid);

ini_set('oath.window', 2);

$valid = totp_validate($key, $generatedValue, 6, 1);
var_dump($valid);

?>
--EXPECTF--
bool(true)
bool(false)
bool(true)
