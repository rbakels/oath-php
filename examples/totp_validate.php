<?php
$key = '0123456789ABCDEF';
echo __FILE__ . ": \n";
$generatedValue = totp_generate($key, 6, 60);
echo "Generated time based value: " . $generatedValue . "\n";

$valid = totp_validate($key, $generatedValue, 6, 60);
$valid = ($valid) ? 'true' : 'false';

echo "Valid  test result: " . $valid . "\n";

$invalid = totp_validate($key, $generatedValue + 1, 6, 60);
$invalid = ($invalid) ? 'true' : 'false';

echo "Inalid  test result: " . $invalid . "\n";