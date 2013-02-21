/*
    +----------------------------------------------------------------------+
    | PHP Version 5                                                        |
    +----------------------------------------------------------------------+
    | Copyright (c) 1997-2012 The PHP Group                                |
    +----------------------------------------------------------------------+
    | This source file is subject to version 3.01 of the PHP license,      |
    | that is bundled with this package in the file LICENSE, and is        |
    | available through the world-wide-web at the following url:           |
    | http://www.php.net/license/3_01.txt                                  |
    | If you did not receive a copy of the PHP license and are unable to   |
    | obtain it through the world-wide-web, please send a note to          |
    | license@php.net so we can mail you a copy immediately.               |
    +----------------------------------------------------------------------+
    | Author: Robin Bakels <robin@xlab.nl>
    +----------------------------------------------------------------------+

    $Id$
*/
#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
#include "ext/spl/spl_exceptions.h"
#include "zend_extensions.h"
#include "php_oath.h"

/* Definition of the arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_count, 0, 0, 1)
    ZEND_ARG_INFO(0, var)
    ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

/* {{{ oath_functions[] */
static function_entry php_oath_functions[] = {
    PHP_FE(totp_validate, arginfo_count)
    PHP_FE(totp_generate, arginfo_count)
    PHP_FE(hotp_validate, arginfo_count)
    PHP_FE(hotp_generate, arginfo_count)
    PHP_FE(google_authenticator_validate, arginfo_count)
    PHP_FE(google_authenticator_generate, arginfo_count)
    {NULL, NULL, NULL}
};
/* }}} */

/**
 * the following code creates an entry for the module and registers it with Zend.
 */
zend_module_entry oath_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_OATH_EXTNAME,
    php_oath_functions,
    PHP_MINIT(oath), /* name of the MINIT function or NULL if not applicable */
    PHP_MSHUTDOWN(oath), /* name of the MSHUTDOWN function or NULL if not applicable */
    PHP_RINIT(oath), /* name of the RINIT function or NULL if not applicable */
    NULL, /* name of the RSHUTDOWN function or NULL if not applicable */
    PHP_MINFO(oath), /* name of the MINFO function or NULL if not applicable */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_OATH_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_OATH
ZEND_GET_MODULE(oath)
#define OATH_ZEND_EXT  ZEND_DLEXPORT
#else
#define OATH_ZEND_EXT
#endif

OATH_ZEND_EXT zend_extension oath_extension_entry = {
    PHP_OATH_EXTNAME,
    PHP_OATH_VERSION,
    "Robin Bakels",
    "http://www.php.net",
    "2013",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

PHP_INI_BEGIN()
PHP_INI_END()

PHP_RINIT_FUNCTION(oath)
{
    return SUCCESS;
}

PHP_MINIT_FUNCTION(oath)
{
    REGISTER_INI_ENTRIES();

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(oath)
{
    UNREGISTER_INI_ENTRIES();

    return SUCCESS;
}

/* {{{ PHP_MINFO(oath) */
PHP_MINFO_FUNCTION(oath)
{
    char        buf[10];

    php_info_print_table_start();
    php_info_print_table_row(2, "oath support", "enabled");
    php_info_print_table_end();
}
/* }}} */

/*  {{{ proto bool google_authenticator_validate(string secret_key, string user_input)
    Returns TRUE if the time based token for the given key matches the user inputted value. FALSE, otherwise.
*/
PHP_FUNCTION(google_authenticator_validate)
{
    char *secret_key;
    int   secret_key_length;
    char *user_input;
    int   user_input_length;
    char *output_buffer;
    ulong result;
    ulong user_input_converted;

    /**
     * Parse parameters. Make the secret_key mandatory, and the length and time factor optional.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &secret_key, &secret_key_length, &user_input, &user_input_length) == FAILURE) {
        RETURN_NULL();
    }

    output_buffer = totp_generate(secret_key, 6, 30);
    result = strtoul(output_buffer, NULL, 0);
    user_input_converted = strtoul(user_input, NULL, 0);

    if (result == user_input_converted)
    {
        RETURN_TRUE;
    }
    else
    {
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto int google_authenticator_generate(string secret_key)
    Returns the generated current time based token value for the given secret key. */
PHP_FUNCTION(google_authenticator_generate)
{
    char *secret_key;
    int   secret_key_length;
    char *output_buffer;
    ulong result;

    /**
     * Parse parameters.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &secret_key, &secret_key_length) == FAILURE)
    {
        RETURN_NULL();
    }

    output_buffer = totp_generate(secret_key, 6, 30);
    result = strtoul(output_buffer, NULL, 0);

    RETURN_LONG(result);
}
/* }}} */

/* {{{ proto bool totp_validate(string secret_key, string user_input [, int length = 6 [, int time_step_size = 60 ]])
    Validate the current time based token for the secret key against the given user inputted value. */
PHP_FUNCTION(totp_validate)
{
    char *secret_key;
    int   secret_key_length;
    char *user_input;
    int   user_input_length;
    ulong length;
    ulong time_step_size;
    char *output_buffer;
    ulong result;
    ulong user_input_converted;

    /**
     * Parse parameters. Make the secret_key mandatory, and the length and time factor optional.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|ll", &secret_key, &secret_key_length, &user_input, &user_input_length, &length, &time_step_size) == FAILURE) {
        RETURN_NULL();
    }

    /**
     * Set default value of optional parameter 'length' to 6.
     */
    if (length == 0)
    {
        length = 6;
    }

    /**
     * Set default value of optional parameter 'time_step_size' to 60.
     */
    if (time_step_size == 0)
    {
        time_step_size = 60;
    }

    output_buffer = totp_generate(secret_key, length, time_step_size);
    result = strtoul(output_buffer, NULL, 0);
    user_input_converted = strtoul(user_input, NULL, 0);

    if (result == user_input_converted)
    {
        RETURN_TRUE;
    }
    else
    {
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto int totp_generate(string secret_key [, int length = 6 [, int time_step_size = 60 ]])
    Generate the current time based value for the secret_key */
PHP_FUNCTION(totp_generate)
{
    char *secret_key;
    int   secret_key_length;
    ulong length;
    ulong time_step_size;
    char *output_buffer;
    ulong result;

    /**
     * Parse parameters. Make the secret_key mandatory, and the length and time factor optional.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &secret_key, &secret_key_length, &length, &time_step_size) == FAILURE) {
        RETURN_NULL();
    }

    /**
     * Set default value of optional parameter 'length' to 6.
     */
    if (length == 0)
    {
        length = 6;
    }

    /**
     * Set default value of optional parameter 'time_step_size' to 60.
     */
    if (time_step_size == 0)
    {
        time_step_size = 60;
    }

    output_buffer = totp_generate(secret_key, length, time_step_size);
    result = strtoul(output_buffer, NULL, 0);
    RETURN_LONG(result);
}
/* }}} */

/* {{{ proto bool hotp_validate(string secret_key, string user_input, int moving_factor [, int length = 6 ])
    Validate the current counter based token for the secret key against the given user inputted value. */
PHP_FUNCTION(hotp_validate)
{
    char *secret_key;
    int   secret_key_length;
    char *user_input;
    int   user_input_length;
    ulong moving_factor;
    ulong length;
    char *output_buffer;
    ulong result;
    ulong user_input_converted;

    /**
     * Parse parameters. Make the secret_key mandatory, and the length and time factor optional.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssl|l", &secret_key, &secret_key_length, &user_input, &user_input_length, &moving_factor, &length) == FAILURE) {
        RETURN_NULL();
    }

    /**
     * Set default value of optional parameter 'length' to 6.
     */
    if (length == 0)
    {
        length = 6;
    }

    output_buffer = hotp_generate(secret_key, moving_factor, length);
    result = strtoul(output_buffer, NULL, 0);
    user_input_converted = strtoul(user_input, NULL, 0);

    if (result == user_input_converted)
    {
        RETURN_TRUE;
    }
    else
    {
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto int hotp_generate(string secret_key, int moving_factor [, int length = 6 ])
    Generate the current counter based value for the secret_key */
PHP_FUNCTION(hotp_generate)
{
    char *secret_key;
    int   secret_key_length;
    ulong moving_factor;
    ulong length;
    char *output_buffer;
    ulong result;

    /**
     * Parse parameters. Make the secret_key mandatory, and the length and time factor optional.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl|l", &secret_key, &secret_key_length, &moving_factor, &length) == FAILURE) {
        RETURN_NULL();
    }

    /**
     * Set default value of optional parameter 'length' to 6.
     */
    if (length == 0)
    {
        length = 6;
    }

    output_buffer = hotp_generate(secret_key, moving_factor, length);
    result = strtoul(output_buffer, NULL, 0);
    RETURN_LONG(result);
}
/* }}} */

static char* php_hotp_generate(char* key, ulong moving_factor, ulong length)
{
    char *secret;
    size_t secretlen = 0;
    char* output_buffer;
    time_t current_time = time(NULL);
    time_t start_time = 0;

    /**
     * Convert hexadecimal string to binary encoded string.
     */
    secretlen = 1 + strlen (key) / 2;
    secret = (char *)emalloc(secretlen+1);

    oath_hex2bin (key, secret, &secretlen);

    /**
     * Convert current time to UTC time.
     */
    struct tm * ptm;
    ptm = gmtime ( &current_time );
    current_time = mktime(ptm);

    /**
     * Create a start time, and convert it to UTC time.
     */
    ptm = gmtime ( &start_time );
    start_time = mktime(ptm);

    /**
     * Init oath library.
     */
    oath_init();

    /**
     * Calculate Base32 from the binary encoded hex string.
     */
    char* base32;
    base32 = (char *)emalloc(2*(size_t)length+1);

    /**
     * Encode the secret key as base32, since we need it to base the calculation on.
     */
    oath_base32_encode (secret, secretlen, &base32, NULL);

    /**
     * Allocate enough memory in output buffer.
     */
    output_buffer = (char *)emalloc(length+1);
    size_t secret_length = (size_t)length;

    /**
     * Generate the HOTP value from the given parameters.
     */
    oath_hotp_generate (secret, secretlen, moving_factor, secret_length, false, OATH_HOTP_DYNAMIC_TRUNCATION, output_buffer);

    /**
     * We're done using the Oath library. Close it.
     */
    oath_done();

    return output_buffer;
}

static char* php_totp_generate(char* key, ulong length, ulong time_step_size)
{
    char *secret;
    size_t secretlen = 0;
    char* output_buffer;
    time_t current_time = time(NULL);
    time_t start_time = 0;

    /**
     * Convert hexadecimal string to binary encoded string.
     */
    secretlen = 1 + strlen (key) / 2;
    secret = (char *)emalloc(secretlen+1);

    oath_hex2bin (key, secret, &secretlen);

    /**
     * Convert current time to UTC time.
     */
    struct tm * ptm;
    ptm = gmtime ( &current_time );
    current_time = mktime(ptm);

    /**
     * Create a start time, and convert it to UTC time.
     */
    ptm = gmtime ( &start_time );
    start_time = mktime(ptm);

    /**
     * Init oath library.
     */
    oath_init();

    /**
     * Calculate Base32 from the binary encoded hex string.
     */
    char* base32;
    base32 = (char *)emalloc(2*(size_t)length+1);

    /**
     * Encode the secret key as base32, since we need it to base the calculation on.
     */
    oath_base32_encode (secret, secretlen, &base32, NULL);

    /**
     * Allocate enough memory in output buffer.
     */
    output_buffer = (char *)emalloc(length+1);
    size_t secret_length = (size_t)length;

    /**
     * Generate the TOTP value from the given parameters.
     */
    oath_totp_generate(secret, secretlen, current_time, time_step_size, start_time, secret_length, output_buffer);

    /**
     * We're done using the Oath library. Close it.
     */
    oath_done();

    return output_buffer;
}