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


#include "common.h"
#include "exception.h"

/* {{{ fdb_check_error
 * */
void fdb_check_error(fdb_error_t fdb_error)
{
    if (fdb_error != 0 && fdb_error != 2000) {
        zend_throw_exception(foundation_exception_ce, fdb_get_error(fdb_error), (int) fdb_error);
    }
}
/* }}} */

/* {{{ fdb_wait_check_error
 * */
void fdb_wait_check_error(FDBFuture *future)
{
    fdb_check_error(fdb_future_block_until_ready(future));
    fdb_check_error(fdb_future_get_error(future));
}
/* }}} */

/* {{{ runNetwork
 * */
void runNetwork()
{
    fdb_check_error(fdb_run_network());
}
/* }}} */

/* {{{ fdb_create_transaction
 * */
void fdb_create_transaction(foundation_db_object *obj)
{
    FDBDatabase *db;

    fdb_check_error(fdb_future_get_database(FOUNDATION_DB_G(db_future_ptr), &db));
    fdb_check_error(fdb_database_create_transaction(db, &obj->transaction));
}
/* }}} */

/* {{{ fdb_commit_transaction
 * */
void fdb_commit_transaction(foundation_db_object *obj)
{
    FDBFuture *commitFuture = fdb_transaction_commit(obj->transaction);

    fdb_check_error(fdb_future_block_until_ready(commitFuture));
    fdb_wait_check_error(fdb_transaction_on_error(obj->transaction, fdb_future_get_error(commitFuture)));

    fdb_future_destroy(commitFuture);
}
/* }}} */

/* {{{ fdb_commit_transaction
 * */
void fdb_keyvalue_array(FDBFuture *get_future, zval *ret_val)
{
    const FDBKeyValue* out_kv;
    int out_count, index = 0;

    fdb_bool_t out_more;
    fdb_check_error(fdb_future_get_keyvalue_array(get_future, &out_kv, &out_count, &out_more));

    const FDBKeyValue *kv = out_kv;

    for(index = 0; index < (int)out_count; index++)
    {
        add_assoc_stringl_ex(ret_val, kv->key, (size_t)kv->key_length, (char *)kv->value, (size_t)kv->value_length);
        kv++;
    }
}
/* }}} */

void _php_foundationdb_close(zend_resource *rsrc TSRMLS_DC)
{

}