/*
  +----------------------------------------------------------------------+
  | FoundationDB Extension                                               |
  +----------------------------------------------------------------------+
  | Copyright (c) 2018-2018 The Viest                                    |
  +----------------------------------------------------------------------+
  | http://www.viest.me                                                  |
  +----------------------------------------------------------------------+
  | Author: viest <dev@service.viest.me>                                 |
  +----------------------------------------------------------------------+
*/


#ifndef FOUNDATIONDB_COMMON_H
#define FOUNDATIONDB_COMMON_H

#include <pthread.h>

#include "php.h"

#include "zend.h"
#include "zend_API.h"
#include "zend_exceptions.h"

#include "../php_foundationdb.h"

#define FOUNDATIONDB_STARTUP_MODULE(module) ZEND_MODULE_STARTUP_N(foundationdb_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define FOUNDATIONDB_STARTUP_FUNCTION(module) ZEND_MINIT_FUNCTION(foundationdb_##module)

#define FOUNDATIONDB_CREATE_TRANSACTION(object) fdb_create_transaction(object)
#define FOUNDATIONDB_COMMIT_TRANSACTION(object) fdb_commit_transaction(object)

#define FOUNDATIONDB_GET_KEYVAL(get_future, ret_val) fdb_keyvalue_array(get_future, ret_val)

void runNetwork();
void fdb_check_error(fdb_error_t fdb_error);
void fdb_wait_check_error(FDBFuture *future);
void fdb_create_transaction(foundation_db_object *obj);
void fdb_commit_transaction(foundation_db_object *obj);
void fdb_keyvalue_array(FDBFuture *get_future, zval *ret_val);



#endif
