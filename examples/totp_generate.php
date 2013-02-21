<?php
$key = '0123456789ABCDEF';
echo __FILE__ . ": \n";
echo "Generated time based value: " . totp_generate($key, 6, 60) . "\n";