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

zend_class_entry *foundation_db_exception_ce;

/** {{{ exception_methods
*/
zend_function_entry exception_methods[] = {
        PHP_FE_END
};
/* }}} */

/** {{{ FOUNDATION_DB_STARTUP_FUNCTION
*/
FOUNDATION_DB_STARTUP_FUNCTION(foundation_exception) {
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "Foundation", "Exception", exception_methods);
    foundation_db_exception_ce = zend_register_internal_class_ex(&ce, zend_ce_exception);

    return SUCCESS;
}
/* }}} */
