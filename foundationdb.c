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
  | Author: dev@service.viest.me                                         |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_foundationdb.h"

#include "src/common.h"
#include "src/client.h"
#include "src/exception.h"

zend_object_handlers foundationdb_handlers;

static zend_always_inline void *vtiful_object_alloc(size_t obj_size, zend_class_entry *ce) {
	void *obj = emalloc(obj_size + zend_object_properties_size(ce));
	memset(obj, 0, obj_size);
	return obj;
}

/* {{{ foundationdb_objects_new
 */
PHP_FOUNDATIONDB_API zend_object *foundation_db_objects_new(zend_class_entry *ce)
{
    foundation_db_object *intern = vtiful_object_alloc(sizeof(foundation_db_object), ce);

    zend_object_std_init(&intern->zo, ce);
    object_properties_init(&intern->zo, ce);

    intern->zo.handlers = &foundationdb_handlers;

    return &intern->zo;
}
/* }}} */

/* {{{ foundationdb_objects_free
 */
void foundation_db_objects_free(zend_object *object)
{
    foundation_db_object *intern = php_foundation_db_fetch_object(object);

	if (intern->get_future_ptr != NULL) {
		fdb_future_destroy(intern->get_future_ptr);
	}

    if (intern->transaction != NULL) {
		fdb_transaction_destroy(intern->transaction);
	}

	if (intern->db_future_ptr != NULL) {
		fdb_future_destroy(intern->db_future_ptr);
	}

	zend_object_std_dtor(&intern->zo);
}
/* }}} */

int le_foundationdb;


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(foundationdb)
{
    FOUNDATIONDB_STARTUP_MODULE(foundation_client);
    FOUNDATIONDB_STARTUP_MODULE(foundation_exception);

    le_foundationdb = zend_register_list_destructors_ex(_php_foundationdb_close, NULL, FOUNDATION_DB_RESOURCE_NAME, module_number);

	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(foundationdb)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(foundationdb)
{
#if defined(COMPILE_DL_FOUNDATIONDB) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(foundationdb)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(foundationdb)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "foundationdb support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ foundationdb_functions[]
 *
 * Every user visible function must have an entry in foundationdb_functions[].
 */
const zend_function_entry foundationdb_functions[] = {
	PHP_FE_END
};
/* }}} */

/* {{{ foundationdb_module_entry
 */
zend_module_entry foundationdb_module_entry = {
	STANDARD_MODULE_HEADER,
	"foundationdb",
	foundationdb_functions,
	PHP_MINIT(foundationdb),
	PHP_MSHUTDOWN(foundationdb),
	PHP_RINIT(foundationdb),
	PHP_RSHUTDOWN(foundationdb),
	PHP_MINFO(foundationdb),
	PHP_FOUNDATIONDB_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_FOUNDATIONDB
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(foundationdb)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
