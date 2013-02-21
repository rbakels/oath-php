--TEST--
google_authenticator_validate() function - basic test for google_authenticator_validate()
--FILE--
<?php
$key = '0123456789ABCDEF';
$generatedValue = google_authenticator_generate($key);
$valid = google_authenticator_validate($key, $generatedValue);
var_dump($valid);
?>
--EXPECT--
bool(true)