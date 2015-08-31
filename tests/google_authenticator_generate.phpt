--TEST--
google_authenticator_generate() function - basic test for google_authenticator_generate()
--FILE--
<?php
var_dump(is_numeric(google_authenticator_generate('a0b1c2')));
var_dump(google_authenticator_generate('foobar'));
?>
--EXPECTF--
bool(true)

Warning: OATH_INVALID_HEX: Hex string is invalid %s
bool(false)
