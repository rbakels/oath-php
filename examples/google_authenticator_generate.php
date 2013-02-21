<?php
$key = '0123456789ABCDEF';
echo __FILE__ . ": \n";
echo "Generated time based value: " . google_authenticator_generate($key) . "\n";