<?php
$key = '0123456789ABCDEF';
echo __FILE__ . ": \n";
$generatedValue = hotp_generate($key, 1, 6);
echo "Generated time based value: " . $generatedValue . "\n";

$valid = hotp_validate($key, $generatedValue, 1, 6);
$valid = ($valid) ? 'true' : 'false';

echo "Valid  test result: " . $valid . "\n";

$invalid = hotp_validate($key, $generatedValue + 1, 1, 6);
$invalid = ($invalid) ? 'true' : 'false';

echo "Inalid  test result: " . $invalid . "\n";