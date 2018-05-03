dnl $Id$
dnl config.m4 for extension foundationdb

PHP_ARG_WITH(foundationdb, for foundationdb support,
[  --with-foundationdb             Include foundationdb support])

LIB_MIN_MAJOR=0
LIBFDBC_MIN_MINOR=28

if test "$PHP_FOUNDATIONDB" != "no"; then

  LIBNAME="fdb_c"
  LIBSYMBOL="fdb_setup_network"

  AC_MSG_CHECKING(for foundationdb headers)

  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/foundationdb/fdb_c.h"

  AC_MSG_CHECKING([for foundationdb files in default path])
  for i in $SEARCH_PATH ; do
    if test -r $i/$SEARCH_FOR; then
      FOUNDATION_DB_DIR=$i
    fi
  done

  if test -n "$FOUNDATION_DB_DIR"; then
    AC_MSG_RESULT(found in $FOUNDATION_DB_DIR/include/foundationdb)
  else
    AC_MSG_RESULT([not found $FOUNDATION_DB_DIR])
    AC_MSG_ERROR([Please reinstall the foundationdb distribution])
  fi
  PHP_ADD_INCLUDE($FOUNDATION_DB_DIR/include/foundationdb)

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $FOUNDATION_DB_DIR/$PHP_LIBDIR, FOUNDATIONDB_SHARED_LIBADD)
  ],[
   AC_MSG_ERROR([wrong fdb lib version or lib not found])
  ],[
    -L$CMARK_DIR/$PHP_LIBDIR -lm
  ])

  PHP_SUBST(FOUNDATIONDB_SHARED_LIBADD)

  PHP_NEW_EXTENSION(foundationdb, foundationdb.c src/common.c src/client.c src/exception.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)

  PHP_ADD_BUILD_DIR([$ext_builddir/src])
fi
