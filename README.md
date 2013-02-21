OATH-PHP
=========

OATH-PHP is a PHP extension for handling OATH related technology. OATH-php provides methods  for two types of one time passwords (OTPs): event based (HOTP) or time based (TOTP). As a bonus a wrapper for the Google Authenticator is provided as well.

The library is tested against the [Google Authenticator Application](http://support.google.com/accounts/bin/answer.py?hl=en&answer=1066447), available for many smartphone platforms, and the C100/C200 OTP Hardware Tokens available at [Gooze.eu](http://www.gooze.eu/catalog/otp-tokens-oath-0).

OATH-PHP is based on the [liboath](http://www.nongnu.org/oath-toolkit/) library.

Version
-

1.0

License
-
[PHP License 3.01](http://www.php.net/license/3_01.txt)

Author
-----------
Robin Bakels [<<robin@xlab.nl>>](robin@xlab.nl).

Requirements
-----------

OATH-PHP requires PHP5+ and liboath installed. Liboath can be found [here](http://www.nongnu.org/oath-toolkit/). Installation instructions for the library are provided on the same website.


Installation
--------------

1. Clone the repo
2. `cd oath-php`
3. `phpize`
4. `./configure`
5. `make`
6. `make test`
7. `make install`

Don't forget to enable the extension in your php.ini file, by adding:

    extension=oath.so

Usage
-----
Very basic usage, which generates a time based token value based on Google Authenticator:

    <?php
    $secret_key = '0123456789ABCDEF';
    $generatedValue = google_authenticator_generate($secret_key);
    echo "Generated time based value: " . $generatedValue . "\n";

The `*_generate` functions always return the generated token value for the given secret key.

As a convenience, some validation functions are defined as well. The following example validates against a given user input for Google Authenticator:

    <?php
    $secret_key = '0123456789ABCDEF';
    $user_input = 123456;
    $valid = google_authenticator_validate($secret_key, $user_input);

The `*_validate` functions always return `true` if the given user input matches the current token value (event based or time based).

Quick Reference
----
You find a quick reference of all the functions provided by the extension below.

Please make note of the following:

 * Every secret key will be translated to the corresponding base32 equivalent key within the functions. You only have to pass the hex key as a secret key (for convenience purposes).
 * The Google Authenticator functions are just aliased for the `totp_*` functions with the settings set to: lenght=6, time_step_size=30. The functions are added for your convenience (you still can use the `totp_*` functions for Google Authenticator devices though).

### google_authenticator_generate

    int google_authenticator_generate(string secret_key)

*google_authenticator_generate()* generates a time based token value, for the Google Authenticator application.

Returns an integer with the time based token value.

##### Parameters

 * *secret_key* : The key for the authentication device to generate the time based token for.

##### Example

    <?php
    $secret_key = '0123456789ABCDEF';
    echo "Generated time based value: " . google_authenticator_generate($secret_key) . "\n";

### google_authenticator_validate

    bool google_authenticator_validate(string secret_key, string user_input)

*google_authenticator_validate()* generates a time based token value, for the Google Authenticator application, and validates it against the given user input string.

Returns TRUE if the inputted user value matches the generated time based token value. FALSE, otherwise.

##### Parameters

 * *secret_key* : The key for the authentication device to generate the time based token for.
 * *user_input* : The user input containing the generated time based token on their Google Authenticator application.

##### Example

    <?php
    $secret_key = '0123456789ABCDEF';
    $user_input = 123456;
    $valid = google_authenticator_validate($secret_key, $user_input);

### totp_generate

    int totp_generate(string secret_key [, int length = 6 [, int time_step_size = 60 ]])

*totp_generate()* generates a time based token value for the given secret key.

Returns an integer with the time based token value.

##### Parameters

 * *secret_key* : The key for the authentication device to generate the time based token for.
 * *length* : The length of the time based token value to return. Currently supported are a length of 6, 7 or 8 digits. Default: 6.
 * *time_step_size* : The time before a new token is being generated. Default: 60.

##### Example

    <?php
    $secret_key = '0123456789ABCDEF';
    $generated_value = totp_generate($secret_key);
    echo "Generated time based value: " . $generated_value . "\n";

### totp_validate

    bool totp_validate(string secret_key, string user_input [, int length = 6 [, int time_step_size = 60 ]])

*totp_validate()* generates a time based token value, for the given secret key, and validates it against the given user input string.

Returns TRUE if the inputted user value matches the generated time based token value. FALSE, otherwise.

##### Parameters

 * *secret_key* : The key for the authentication device to generate the time based token for.
 * *user_input* : The user input containing the generated time based token on their Google Authenticator application.
 * *length* : The length of the time based token value to return. Currently supported are a length of 6, 7 or 8 digits. Default: 6.
 * *time_step_size* : The time before a new token is being generated. Default: 60.

##### Example

    <?php
    $secret_key = '0123456789ABCDEF';
    $user_input = 123456;
    $valid = totp_validate($secret_key, $user_input);

### hotp_generate

    int hotp_generate(string secret_key, int moving_factor [, int length = 6 [, int time_step_size = 60 ]])

*hotp_generate()* generates a counter based token value for the given secret key.

Returns an integer with the counter based token value.

##### Parameters

 * *secret_key* : The key for the authentication device to generate the counter based token for.
 * *moving_factor* : The current counter value.
 * *length* : The length of the counter based token value to return. Currently supported are a length of 6, 7 or 8 digits. Default: 6.

##### Example

    <?php
    $secret_key = '0123456789ABCDEF';
    $counter_value = 10;
    $generated_value = hotp_generate($secret_key, $counter_value);
    echo "Generated time based value: " . $generated_value . "\n";

### hotp_validate

    bool hotp_validate(string secret_key, string user_input, int moving_factor [, int length = 6 [, int time_step_size = 60 ]])

*hotp_validate()* generates a counter based token value, for the given secret key, and validates it against the given user input string.

Returns TRUE if the inputted user value matches the generated counter based token value. FALSE, otherwise.

##### Parameters

 * *secret_key* : The key for the authentication device to generate the counter based token for.
 * *user_input* : The user input containing the generated counter based token on their Google Authenticator application.
 * *moving_factor* : The current counter value.

##### Example

    <?php
    $secret_key = '0123456789ABCDEF';
    $user_input = 123456;
    $counter_value = 10;
    $valid = hotp_validate($secret_key, $user_input, $counter_value);


TODO
-----
 * A Win32 build.
 * Better error handling within the extension.
