
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

#ifdef ZEND_ENGINE_3
typedef size_t strsize_t;
#else
typedef int strsize_t;
#endif

PHPAPI char* php_totp_generate(char* key, ulong length, ulong step_size);
PHPAPI int php_totp_validate(char* key, ulong length, ulong time_step_size, const char *otp);
PHPAPI char* php_hotp_generate(char* key, ulong length, ulong step_size);
PHPAPI int php_hotp_validate(char* key, uint64_t moving_factor, const char *otp);

PHP_FUNCTION(totp_validate);
PHP_FUNCTION(totp_generate);
PHP_FUNCTION(hotp_validate);
PHP_FUNCTION(hotp_generate);
PHP_FUNCTION(google_authenticator_validate);
PHP_FUNCTION(google_authenticator_generate);

PHP_MINFO_FUNCTION(oath);

extern zend_module_entry oath_module_entry;
#define phpext_oath_ptr &oath_module_entry

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: fdm=marker
 * vim: noet sw=4 ts=4
 */

