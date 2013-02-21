<?php
$key = '0123456789ABCDEF';
echo __FILE__ . ": \n";
echo "Generated time based value: " . hotp_generate($key, 2, 6) . "\n";