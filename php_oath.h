
#ifndef PHP_OATH_H
#define PHP_OATH_H 1

#define PHP_OATH_VERSION "1.0"
#define PHP_OATH_EXTNAME "oath"

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#ifdef ZTS
#include "TSRM.h"
#endif

#include <sys/types.h>
#include <time.h>
#include <stdlib.h>

#include "liboath/oath.h"

#ifdef ZTS
#define OATHG(v) TSRMG(oath_globals_id, zend_oath_globals *, v)
#else
#define OATHG(v) (oath_globals.v)
#endif

#ifdef ZEND_ENGINE_3
typedef size_t strsize_t;
#define _RETURN_STRINGL RETURN_STRINGL
#else
typedef int strsize_t;
#define _RETURN_STRINGL(s, l) RETURN_STRINGL(s, l, 1)
#endif

PHPAPI int php_totp_generate(const char *key, size_t key_length, unsigned digits, unsigned time_step_size, char *otp);
PHPAPI int php_totp_validate(const char *key, size_t key_length, unsigned digits, unsigned time_step_size, size_t window, const char *otp);
PHPAPI int php_hotp_generate(const char *key, size_t key_length, uint64_t moving_factor, size_t digits, char *otp);
PHPAPI int php_hotp_validate(const char *key, size_t key_length, uint64_t moving_factor, size_t window, const char *otp);

PHP_FUNCTION(totp_validate);
PHP_FUNCTION(totp_generate);
PHP_FUNCTION(hotp_validate);
PHP_FUNCTION(hotp_generate);
PHP_FUNCTION(google_authenticator_validate);
PHP_FUNCTION(google_authenticator_generate);

extern zend_module_entry oath_module_entry;
#define phpext_oath_ptr &oath_module_entry

ZEND_BEGIN_MODULE_GLOBALS(oath)
	int window;
ZEND_END_MODULE_GLOBALS(oath)

ZEND_EXTERN_MODULE_GLOBALS(oath);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */

