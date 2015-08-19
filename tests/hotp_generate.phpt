--TEST--
hotp_generate() function - basic test for hotp_generate()
--FILE--
<?php
$key = '0123456789ABCDEF';

var_dump(is_numeric(hotp_generate($key, 1, 6)));

var_dump(hotp_generate('foobar', 1, 6));

var_dump(hotp_generate($key, 1, 3));
var_dump(hotp_generate($key, 1, 20));

?>
--EXPECTF--
bool(true)

Warning: OATH_INVALID_HEX: Hex string is invalid %s
bool(false)

Warning: OATH_INVALID_DIGITS: Unsupported number of OTP digits %s
bool(false)

Warning: OATH_INVALID_DIGITS: Unsupported number of OTP digits %s
bool(false)
