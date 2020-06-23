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

zend_class_entry *foundation_db_transaction_ce;

static zend_object_handlers foundation_db_transaction_handlers;

/** {{{ foundation_db_transaction_object_alloc
 */
static zend_always_inline void *foundation_db_transaction_object_alloc(size_t obj_size, zend_class_entry *ce) {
    void *obj = emalloc(obj_size);
    memset(obj, 0, obj_size);
    return obj;
}
/* }}} */

/** {{{ foundation_db_transaction_objects_create
 */
void foundation_db_transaction_objects_create(zval *instance, FDBTransaction *transaction)
{
    fdb_transaction_object *object = foundation_db_transaction_object_alloc(sizeof(fdb_transaction_object), foundation_db_transaction_ce);

    zend_object_std_init(&object->zo, foundation_db_transaction_ce);
    object_properties_init(&object->zo, foundation_db_transaction_ce);

    object->transaction = transaction;
    object->zo.handlers = &foundation_db_transaction_handlers;

    ZVAL_OBJ(instance, &object->zo);
}
/* }}} */

/** {{{ foundation_db_transaction_objects_new
 */
PHP_FOUNDATIONDB_API zend_object *foundation_db_transaction_objects_new(zend_class_entry *ce)
{
    fdb_transaction_object *object = foundation_db_transaction_object_alloc(sizeof(fdb_transaction_object), ce);

    zend_object_std_init(&object->zo, ce);
    object_properties_init(&object->zo, ce);

    object->transaction = NULL;
    object->zo.handlers = &foundation_db_transaction_handlers;

    return &object->zo;
}
/* }}} */

/** {{{ foundation_db_transaction_objects_free
 */
static void foundation_db_transaction_objects_free(zend_object *object)
{
    fdb_transaction_object *fdb_transaction_object = php_foundation_db_transaction_fetch_object(object);

    if (fdb_transaction_object->transaction != NULL) {
        fdb_transaction_destroy(fdb_transaction_object->transaction);
    }

    zend_object_std_dtor(&fdb_transaction_object->zo);
}
/* }}} */

/* {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(fdb_arg_transaction_destroy, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_transaction_set_option, 0, 0, 2)
    ZEND_ARG_INFO(0, option)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_transaction_set_read_version, 0, 0, 1)
    ZEND_ARG_INFO(0, version)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_transaction_get_read_version, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_transaction_get, 0, 0, 1)
    ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_transaction_set, 0, 0, 2)
    ZEND_ARG_INFO(0, key)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_transaction_commit, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_transaction_reset, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ \Foundation\Transaction::destroy()
 */
PHP_METHOD (foundation_db_transaction, destroy)
{
    fdb_transaction_object *transaction = FDB_TRANSACTION_THIS_OBJECT();

    fdb_transaction_destroy(transaction->transaction);
    transaction->transaction = NULL;

    RETURN_TRUE;
}
/* }}} */

/** {{{ \Foundation\Transaction::setOption()
 */
PHP_METHOD (foundation_db_transaction, setOption)
{
    zval *value;
    zend_long option;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_LONG(option)
        Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    RETURN_SELF;

    fdb_transaction_object *object = FDB_TRANSACTION_THIS_OBJECT();

    FDB_TRANSACTION_IS_NULL_RETURN(object);

    if (Z_TYPE_P(value) == IS_STRING) {
        fdb_check_error(fdb_transaction_set_option(object->transaction, option, (uint8_t const*)Z_STRVAL_P(value), Z_STRLEN_P(value)));

        return;
    }

    if (Z_TYPE_P(value) == IS_LONG) {
        fdb_check_error(fdb_transaction_set_option(object->transaction, option, (uint8_t const*)Z_LVAL_P(value), sizeof(uint8_t)));

        return;
    }
}
/* }}} */

/** {{{ \Foundation\Transaction::setReadVersion()
 */
PHP_METHOD (foundation_db_transaction, setReadVersion)
{
    zend_long version;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(version)
    ZEND_PARSE_PARAMETERS_END();

    fdb_transaction_object *object = FDB_TRANSACTION_THIS_OBJECT();

    FDB_TRANSACTION_IS_NULL_RETURN(object);

    fdb_transaction_set_read_version(object->transaction, version);

    RETURN_SELF;
}
/* }}} */

/** {{{ \Foundation\Transaction::getReadVersion()
 */
PHP_METHOD (foundation_db_transaction, getReadVersion)
{
    fdb_transaction_object *object = FDB_TRANSACTION_THIS_OBJECT();

    FDB_TRANSACTION_IS_NULL_RETURN(object);

    FDBFuture *future = fdb_transaction_get_read_version(object->transaction);

    foundation_db_future_objects_create(return_value, future);
}
/* }}} */

/** {{{ \Foundation\Transaction::get()
 */
PHP_METHOD (foundation_db_transaction, get)
{
    zend_bool snapshot = 0;
    zend_string *key = NULL;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(key)
        Z_PARAM_BOOL(snapshot)
    ZEND_PARSE_PARAMETERS_END();

    fdb_transaction_object *object = FDB_TRANSACTION_THIS_OBJECT();

    FDB_TRANSACTION_IS_NULL_RETURN(object);

    FDBFuture *future = fdb_transaction_get(object->transaction, (uint8_t const*)ZSTR_VAL(key), ZSTR_LEN(key), snapshot);

    foundation_db_future_objects_create(return_value, future);
}
/* }}} */

/** {{{ \Foundation\Transaction::set()
 */
PHP_METHOD (foundation_db_transaction, set)
{
    zend_string *key = NULL, *value = NULL;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(key)
        Z_PARAM_STR(value)
    ZEND_PARSE_PARAMETERS_END();

    fdb_transaction_object *object = FDB_TRANSACTION_THIS_OBJECT();

    FDB_TRANSACTION_IS_NULL_RETURN(object);

    fdb_transaction_set(object->transaction,  (uint8_t const*)ZSTR_VAL(key), ZSTR_LEN(key),  (uint8_t const*)ZSTR_VAL(value), ZSTR_LEN(value));

    RETURN_SELF;
}
/* }}} */

/** {{{ \Foundation\Transaction::commit()
 */
PHP_METHOD (foundation_db_transaction, commit)
{
    fdb_transaction_object *object = FDB_TRANSACTION_THIS_OBJECT();

    FDBFuture *future = fdb_transaction_commit(object->transaction);

    foundation_db_future_objects_create(return_value, future);
}
/* }}} */

/** {{{ \Foundation\Transaction::reset()
 */
PHP_METHOD (foundation_db_transaction, reset)
{
    fdb_transaction_object *object = FDB_TRANSACTION_THIS_OBJECT();

    fdb_transaction_reset(object->transaction);

    RETURN_SELF;
}
/* }}} */

/** {{{ transaction_methods
 */
zend_function_entry transaction_methods[] = {
        PHP_ME(foundation_db_transaction, destroy,        fdb_arg_transaction_destroy,          ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_transaction, setOption,      fdb_arg_transaction_set_option,       ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_transaction, setReadVersion, fdb_arg_transaction_set_read_version, ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_transaction, getReadVersion, fdb_arg_transaction_get_read_version, ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_transaction, get,            fdb_arg_transaction_get,              ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_transaction, set,            fdb_arg_transaction_set,              ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_transaction, commit,         fdb_arg_transaction_commit,           ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_transaction, reset,          fdb_arg_transaction_reset,            ZEND_ACC_PUBLIC)
        PHP_FE_END
};
/* }}} */

FOUNDATION_DB_STARTUP_FUNCTION(foundation_transaction)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "Foundation", "Transaction", transaction_methods);
    ce.create_object = foundation_db_transaction_objects_new;
    foundation_db_transaction_ce = zend_register_internal_class(&ce);

    memcpy(&foundation_db_transaction_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    foundation_db_transaction_handlers.offset   = XtOffsetOf(fdb_database_object , zo);
    foundation_db_transaction_handlers.free_obj = foundation_db_transaction_objects_free;

    return SUCCESS;
}