#ifndef PHP_OATH_H
#define PHP_OATH_H 1

#ifdef PHP_WIN32
#    define PHP_COUNTER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define PHP_COUNTER_API __attribute__ ((visibility("default")))
#else
#    define PHP_COUNTER_API
#endif

#ifdef ZTS
	#include "TSRM.h"
#endif

#include "liboath/oath.h"

#include <sys/types.h>
#include <time.h>
#include <stdlib.h>     /* strtoul */

#define PHP_OATH_VERSION "1.0"
#define PHP_OATH_EXTNAME "oath"

PHP_MINIT_FUNCTION(oath);
PHP_MSHUTDOWN_FUNCTION(oath);
PHP_RINIT_FUNCTION(oath);

static char* php_totp_generate(char* key, ulong length, ulong step_size);
static char* php_hotp_generate(char* key, ulong length, ulong step_size);
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