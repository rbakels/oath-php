--TEST--
totp_generate() function - basic test for totp_generate()
--FILE--
<?php
$key = '0123456789ABCDEF';

var_dump(is_numeric(totp_generate($key, 6, 30)));

var_dump(totp_generate('foobar', 6, 30));

var_dump(totp_generate($key, 3, 30));
var_dump(totp_generate($key, 20, 30));

?>
--EXPECTF--
bool(true)

Warning: OATH_INVALID_HEX: Hex string is invalid %s
bool(false)

Warning: OATH_INVALID_DIGITS: Unsupported number of OTP digits %s
bool(false)

Warning: OATH_INVALID_DIGITS: Unsupported number of OTP digits %s
bool(false)
