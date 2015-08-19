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

#include "php_oath.h"

/* {{{ utils */

/**
 * Convert hexadecimal string to binary encoded string.
 */
static inline int php_oath_hex2bin(const char *key, size_t keylen,
    char **secret, size_t *secretlen)
{
    size_t len = 1 + keylen / 2;
    char *sec = (char *)emalloc(len + 1);
    int ret = oath_hex2bin(key, sec, &len);
    if (ret >= 0) {
        *secret = sec;
        *secretlen = len;
    } else {
        efree(sec);
    }
    return ret;
}

/**
 * Convert current time to UTC time.
 */
static inline time_t php_oath_time2utc(time_t time)
{
    struct tm *ptm;
    
    ptm = gmtime(&time);
    return mktime(ptm);
}

/* }}} */


/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_count, 0, 0, 1)
    ZEND_ARG_INFO(0, var)
    ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ oath_functions */
static zend_function_entry oath_functions[] = {
    PHP_FE(totp_validate, arginfo_count)
    PHP_FE(totp_generate, arginfo_count)
    PHP_FE(hotp_validate, arginfo_count)
    PHP_FE(hotp_generate, arginfo_count)
    PHP_FE(google_authenticator_validate, arginfo_count)
    PHP_FE(google_authenticator_generate, arginfo_count)
    PHP_FE_END
};
/* }}} */

/* {{{ oath_module_entry */
zend_module_entry oath_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_OATH_EXTNAME,
    oath_functions,
    NULL, /* name of the MINIT function or NULL if not applicable */
    NULL, /* name of the MSHUTDOWN function or NULL if not applicable */
    NULL, /* name of the RINIT function or NULL if not applicable */
    NULL, /* name of the RSHUTDOWN function or NULL if not applicable */
    PHP_MINFO(oath), /* name of the MINFO function or NULL if not applicable */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_OATH_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_OATH
ZEND_GET_MODULE(oath)
#endif
/* }}} */

/* {{{ PHP_MINFO(oath) */
PHP_MINFO_FUNCTION(oath)
{
    php_info_print_table_start();
    php_info_print_table_row(2, "oath support", "enabled");
    php_info_print_table_row(2, "Oath Extension Version", PHP_OATH_VERSION);
    php_info_print_table_row(2, "Oath Library Version", oath_check_version(NULL));
    php_info_print_table_row(2, "Oath Header Version", OATH_VERSION);
    php_info_print_table_end();
}
/* }}} */

/*  {{{ proto bool google_authenticator_validate(string secret_key, string user_input)
    Returns TRUE if the time based token for the given key matches the user inputted value. FALSE, otherwise.
*/
PHP_FUNCTION(google_authenticator_validate)
{
    char *secret_key;
    strsize_t secret_key_length;
    char *user_input;
    strsize_t user_input_length;
    char *output_buffer;
    int ret;

    /**
     * Parse parameters. Make the secret_key mandatory, and the length and time factor optional.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &secret_key, &secret_key_length, &user_input, &user_input_length) == FAILURE) {
        RETURN_NULL();
    }

    ret = php_totp_validate(secret_key, secret_key_length, 6, 30, user_input);

    if (ret >= 0) {
        RETURN_TRUE;
    } else {
        if( ret != OATH_INVALID_OTP ) {
            zend_error(
                E_WARNING,
                "%s: %s",
                oath_strerror_name(ret),
                oath_strerror(ret)
            );
        }
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto int google_authenticator_generate(string secret_key)
    Returns the generated current time based token value for the given secret key. */
PHP_FUNCTION(google_authenticator_generate)
{
    char *secret_key;
    strsize_t secret_key_length;
    char output_buffer[9] = {0};
    int ret;

    /**
     * Parse parameters.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &secret_key, &secret_key_length) == FAILURE) {
        RETURN_NULL();
    }

    ret = php_totp_generate(secret_key, secret_key_length, 6, 30, output_buffer);

    if (ret >= 0) {
        _RETURN_STRINGL(output_buffer, 6);
    } else {
        zend_error(
            E_WARNING,
            "%s: %s",
            oath_strerror_name(ret),
            oath_strerror(ret)
        );
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto bool totp_validate(string secret_key, string user_input [, int length = 6 [, int time_step_size = 60 ]])
    Validate the current time based token for the secret key against the given user inputted value. */
PHP_FUNCTION(totp_validate)
{
    char *secret_key;
    strsize_t secret_key_length;
    char *user_input;
    strsize_t user_input_length;
    ulong length = 0;
    ulong time_step_size = 0;
    char *output_buffer;
    int ret;

    /**
     * Parse parameters. Make the secret_key mandatory, and the length and time factor optional.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|ll", &secret_key, &secret_key_length, &user_input, &user_input_length, &length, &time_step_size) == FAILURE) {
        RETURN_NULL();
    }

    /**
     * Set default value of optional parameter 'length' to 6.
     */
    if (length == 0) {
        length = 6;
    }

    /**
     * Set default value of optional parameter 'time_step_size' to 60.
     */
    if (time_step_size == 0) {
        time_step_size = 60;
    }

    ret = php_totp_validate(secret_key, secret_key_length, length, time_step_size, user_input);

    if (ret >= 0) {
        RETURN_TRUE;
    } else {
        if( ret != OATH_INVALID_OTP ) {
            zend_error(
                E_WARNING,
                "%s: %s",
                oath_strerror_name(ret),
                oath_strerror(ret)
            );
        }
        RETURN_FALSE;
    }
}

PHPAPI int php_totp_validate(const char *key, size_t key_length, unsigned length, unsigned time_step_size, const char *otp)
{
    char *secret;
    size_t secret_length = 0;
    time_t current_time = time(NULL);
    time_t start_time = 0;
    size_t window = 0;
    int ret;

    ret = php_oath_hex2bin(key, key_length, &secret, &secret_length);
    if (ret < 0) {
        return ret;
    }

    current_time = php_oath_time2utc(current_time);
    start_time = php_oath_time2utc(start_time);
    
    oath_init();
    ret = oath_totp_validate(secret, secret_length, current_time, time_step_size, start_time, window, otp);
    oath_done();

    efree(secret);

    return ret;
}
/* }}} */

/* {{{ proto string totp_generate(string secret_key [, int length = 6 [, int time_step_size = 60 ]])
    Generate the current time based value for the secret_key */
PHP_FUNCTION(totp_generate)
{
    char *secret_key;
    strsize_t secret_key_length;
    ulong length;
    ulong time_step_size;
    char output_buffer[9] = {0};
    int ret;

    /**
     * Parse parameters. Make the secret_key mandatory, and the length and time factor optional.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ll", &secret_key, &secret_key_length, &length, &time_step_size) == FAILURE) {
        RETURN_NULL();
    }

    /**
     * Set default value of optional parameter 'length' to 6.
     */
    if (length == 0) {
        length = 6;
    }

    /**
     * Set default value of optional parameter 'time_step_size' to 60.
     */
    if (time_step_size == 0) {
        time_step_size = 60;
    }

    ret = php_totp_generate(secret_key, secret_key_length, length, time_step_size, output_buffer);

    if (ret >= 0) {
        _RETURN_STRINGL(output_buffer, length);
    } else {
        zend_error(
            E_WARNING,
            "%s: %s",
            oath_strerror_name(ret),
            oath_strerror(ret)
        );
        RETURN_FALSE;
    }
}

PHPAPI int php_totp_generate(const char *key, size_t key_length, unsigned digits, unsigned time_step_size, char *otp)
{
    char *secret;
    size_t secretlen = 0;
    time_t current_time = time(NULL);
    time_t start_time = 0;
    int ret;

    ret = php_oath_hex2bin(key, strlen(key), &secret, &secretlen);
    if (ret < 0) {
        return ret;
    }

    current_time = php_oath_time2utc(current_time);
    start_time = php_oath_time2utc(start_time);

    /**
     * Init oath library.
     */
    oath_init();

    /**
     * Generate the TOTP value from the given parameters.
     */
    ret = oath_totp_generate(secret, secretlen, current_time, time_step_size, start_time, digits, otp);

    /**
     * We're done using the Oath library. Close it.
     */
    oath_done();
    efree(secret);

    return ret;
}
/* }}} */

/* {{{ proto bool hotp_validate(string secret_key, string user_input, int moving_factor)
    Validate the current counter based token for the secret key against the given user inputted value. */
PHP_FUNCTION(hotp_validate)
{
    char *secret_key;
    strsize_t secret_key_length;
    char *user_input;
    strsize_t user_input_length;
    ulong moving_factor;
    char *output_buffer;
    int ret;

    /**
     * Parse parameters. Make the secret_key mandatory, and the length and time factor optional.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssl", &secret_key, &secret_key_length, &user_input, &user_input_length, &moving_factor) == FAILURE) {
        RETURN_NULL();
    }

    ret = php_hotp_validate(secret_key, secret_key_length, moving_factor, user_input);

    if (ret >= 0) {
        RETURN_TRUE;
    } else {
        if( ret != OATH_INVALID_OTP ) {
            zend_error(
                E_WARNING,
                "%s: %s",
                oath_strerror_name(ret),
                oath_strerror(ret)
            );
        }
        RETURN_FALSE;
    }
}

PHPAPI int php_hotp_validate(const char *key, size_t key_length, uint64_t moving_factor, const char *otp)
{
    char *secret;
    size_t secret_length = 0;
    char* output_buffer;
    size_t window = 0;
    int ret;

    ret = php_oath_hex2bin(key, key_length, &secret, &secret_length);
    if (ret < 0) {
        return ret;
    }

    oath_init();
    ret = oath_hotp_validate(secret, secret_length, moving_factor, window, otp);
    oath_done();

    efree(secret);

    return ret;
}
/* }}} */

/* {{{ proto string hotp_generate(string secret_key, int moving_factor [, int length = 6 ])
    Generate the current counter based value for the secret_key */
PHP_FUNCTION(hotp_generate)
{
    char *secret_key;
    strsize_t secret_key_length;
    ulong moving_factor;
    ulong length;
    char output_buffer[9] = {0};
    int ret;

    /**
     * Parse parameters. Make the secret_key mandatory, and the length and time factor optional.
     */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl|l", &secret_key, &secret_key_length, &moving_factor, &length) == FAILURE) {
        RETURN_NULL();
    }

    /**
     * Set default value of optional parameter 'length' to 6.
     */
    if (length == 0) {
        length = 6;
    }

    ret = php_hotp_generate(secret_key, secret_key_length, moving_factor, length, output_buffer);

    if (ret >= 0) {
        _RETURN_STRINGL(output_buffer, length);
    } else {
        zend_error(
            E_WARNING,
            "%s: %s",
            oath_strerror_name(ret),
            oath_strerror(ret)
        );
        RETURN_FALSE;
    }
}

PHPAPI int php_hotp_generate(const char *key, size_t key_length, uint64_t moving_factor, size_t digits, char *otp)
{
    char *secret;
    size_t secret_length = 0;
    int ret;

    ret = php_oath_hex2bin(key, key_length, &secret, &secret_length);
    if (ret < 0) {
        return ret;
    }

    /**
     * Init oath library.
     */
    oath_init();

    /**
     * Generate the HOTP value from the given parameters.
     */
    ret = oath_hotp_generate(secret, secret_length, moving_factor, digits, 0, OATH_HOTP_DYNAMIC_TRUNCATION, otp);

    /**
     * We're done using the Oath library. Close it.
     */
    oath_done();
    efree(secret);

    return ret;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */

