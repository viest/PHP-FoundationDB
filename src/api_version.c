/*
  +----------------------------------------------------------------------+
  | FoundationDB Extension                                               |
  +----------------------------------------------------------------------+
  | Copyright (c) 2020-2020 The Viest                                    |
  +----------------------------------------------------------------------+
  | http://www.viest.me                                                  |
  +----------------------------------------------------------------------+
  | Author: viest <dev@service.viest.me>                                 |
  +----------------------------------------------------------------------+
*/

#include "foundation.h"

zend_class_entry *foundation_db_api_version_ce;

/* {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(fdb_arg_select_api_version, 0, 0, 1)
    ZEND_ARG_INFO(0, version)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_get_max_api_version, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ \Foundation\ApiVersion::selectApiVersion(): void
 */
PHP_METHOD (foundation_db_api_version, selectApiVersion)
{
    zend_long version = 0;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_LONG(version)
    ZEND_PARSE_PARAMETERS_END();

    fdb_check_error(fdb_select_api_version(version));
}
/* }}} */

/** {{{ \Foundation\ApiVersion::getMaxApiVersion(): int
 */
PHP_METHOD (foundation_db_api_version, maxApiVersion)
{
    RETURN_LONG(fdb_get_max_api_version());
}
/* }}} */

/** {{{ api_version_methods
*/
zend_function_entry api_version_methods[] = {
        PHP_ME(foundation_db_api_version, selectApiVersion,  fdb_arg_select_api_version,  ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_api_version, maxApiVersion,     fdb_arg_get_max_api_version, ZEND_ACC_PUBLIC)
        PHP_FE_END
};
/* }}} */

/** {{{ FOUNDATION_DB_STARTUP_FUNCTION
*/
FOUNDATION_DB_STARTUP_FUNCTION(foundation_api_version)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "Foundation", "ApiVersion", api_version_methods);
    foundation_db_api_version_ce = zend_register_internal_class(&ce);

    return SUCCESS;
}
/* }}} */