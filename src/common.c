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

/** {{{ _php_foundation_db_close
 */
void _php_foundation_db_close(zend_resource *rsrc TSRMLS_DC)
{
    //
}
/* }}} */

/** {{{ fdb_check_error
 */
void fdb_check_error(fdb_error_t fdb_error)
{
    if (fdb_error != 0 && fdb_error != 2000) {
        zend_throw_exception(foundation_db_exception_ce, fdb_get_error(fdb_error), (int) fdb_error);
    }
}
/* }}} */

/** {{{ runNetwork
 */
void runNetwork()
{
    fdb_check_error(fdb_run_network());
}
/* }}} */