/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_FOUNDATIONDB_H
#define PHP_FOUNDATIONDB_H

extern zend_module_entry foundationdb_module_entry;
#define phpext_foundationdb_ptr &foundationdb_module_entry

#define PHP_FOUNDATIONDB_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_FOUNDATIONDB_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_FOUNDATIONDB_API __attribute__ ((visibility("default")))
#else
#	define PHP_FOUNDATIONDB_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define FDB_API_VERSION 510
#include "foundationdb/fdb_c.h"

typedef struct _foundationdb_object {
    FDBFuture *cluster_future_ptr;
    FDBFuture *db_future_ptr;
    FDBFuture *get_future_ptr;
    FDBTransaction *transaction;
    zend_object zo;
} foundation_db_object;

static inline foundation_db_object *php_foundation_db_fetch_object(zend_object *obj) {
    return (foundation_db_object *)((char *)(obj) - XtOffsetOf(foundation_db_object, zo));
}

#define FOUNDATION_DB_RESOURCE_NAME "foundationdb"
#define Z_FOUNDATION_DB_P(zv) php_foundation_db_fetch_object(Z_OBJ_P(zv));

extern int le_foundationdb;
extern zend_object_handlers foundationdb_handlers;

PHP_FOUNDATIONDB_API zend_object *foundation_db_objects_new(zend_class_entry *ce);

void foundation_db_objects_free(zend_object *object);
void _php_foundationdb_close(zend_resource *rsrc TSRMLS_DC);



/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(foundationdb)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(foundationdb)
*/

/* Always refer to the globals in your function as FOUNDATIONDB_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define FOUNDATIONDB_G(v) FOUNDATIONDB_GFOUNDATIONDB_G(foundationdb, v)

#if defined(ZTS) && defined(COMPILE_DL_FOUNDATIONDB)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif    /* PHP_FOUNDATIONDB_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
