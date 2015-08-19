PHP_ARG_ENABLE(oath, for OATH support,
[	--enable-oath   Include OATH support])

if test "$PHP_OATH" = "yes"; then
  for i in $PHP_OATH_FILTER /usr/local /usr; do
    test -f $i/include/liboath/oath.h && PHP_OATH_FILTER_DIR=$i && break
  done

  if test -z "$PHP_OATH_FILTER_DIR"; then
    AC_MSG_ERROR(oath.h not found. Please (re)install liboath.)
  fi

  PHP_CHECK_LIBRARY(oath, oath_totp_generate, [], [
    AC_MSG_ERROR(oath lib not found. Please (re)install liboath.)
  ], [
    -L$PHP_OATH_FILTER_DIR/lib -loath
  ])

  PHP_ADD_LIBRARY_WITH_PATH(oath, $PHP_OATH_FILTER_DIR/lib, OATH_SHARED_LIBADD)
  PHP_ADD_INCLUDE($PHP_OATH_FILTER_DIR/include)
  PHP_NEW_EXTENSION(oath, oath.c, $ext_shared)
  PHP_SUBST(OATH_SHARED_LIBADD)
fi

