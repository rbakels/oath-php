<?php
$key = '0123456789ABCDEF';
echo __FILE__ . ": \n";
$generatedValue = google_authenticator_generate($key);
echo "Generated time based value: " . $generatedValue . "\n";

$valid = google_authenticator_validate($key, $generatedValue);
$valid = ($valid) ? 'true' : 'false';

echo "Valid  test result: " . $valid . "\n";

$invalid = google_authenticator_validate($key, $generatedValue - 1);
$invalid = ($invalid) ? 'true' : 'false';

echo "Inalid  test result: " . $invalid . "\n";