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

zend_class_entry *foundation_db_future_ce;

static zend_object_handlers foundation_db_future_handlers;

/** {{{ foundation_db_future_object_alloc
 */
static zend_always_inline void *foundation_db_future_object_alloc(size_t obj_size, zend_class_entry *ce) {
    void *obj = emalloc(obj_size);
    memset(obj, 0, obj_size);
    return obj;
}
/* }}} */

/** {{{ foundation_db_future_objects_create
 */
void foundation_db_future_objects_create(zval *instance, FDBFuture *future)
{
    fdb_future_object *object = foundation_db_future_object_alloc(sizeof(fdb_transaction_object), foundation_db_future_ce);

    zend_object_std_init(&object->zo, foundation_db_future_ce);
    object_properties_init(&object->zo, foundation_db_future_ce);

    object->future = future;
    object->zo.handlers = &foundation_db_future_handlers;

    ZVAL_OBJ(instance, &object->zo);
}
/* }}} */

/** {{{ foundation_db_future_objects_new
 */
PHP_FOUNDATIONDB_API zend_object *foundation_db_future_objects_new(zend_class_entry *ce)
{
    fdb_future_object *object = foundation_db_future_object_alloc(sizeof(fdb_transaction_object), ce);

    zend_object_std_init(&object->zo, ce);
    object_properties_init(&object->zo, ce);

    object->future = NULL;
    object->zo.handlers = &foundation_db_future_handlers;

    return &object->zo;
}
/* }}} */

/** {{{ foundation_db_future_objects_free
 */
static void foundation_db_future_objects_free(zend_object *object)
{
    fdb_future_object *fdb_future_object = php_foundation_db_future_fetch_object(object);

    if (fdb_future_object->future != NULL) {
        fdb_future_destroy(fdb_future_object->future);
    }

    zend_object_std_dtor(&fdb_future_object->zo);
}
/* }}} */

/* {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(fdb_arg_future_cancel, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_future_destroy, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_future_consturct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_future_block_until_ready, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_future_is_ready, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_future_get_value, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_future_get_error, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ \Foundation\Future::__construct()
 */
PHP_METHOD (foundation_db_future, __construct)
{
    //
}
/* }}} */

/** {{{ \Foundation\Future::cancel()
 */
PHP_METHOD (foundation_db_future, cancel)
{
    fdb_future_object *object = FDB_FUTURE_THIS_OBJECT();

    FDB_FUTURE_IS_NULL_RETURN(object);

    fdb_future_cancel(object->future);

    RETURN_SELF;
}
/* }}} */

/** {{{ \Foundation\Future::destroy()
 */
PHP_METHOD (foundation_db_future, destroy)
{
    fdb_future_object *object = FDB_FUTURE_THIS_OBJECT();

    FDB_FUTURE_IS_NULL_RETURN(object);

    fdb_future_destroy(object->future);
    object->future = NULL;

    RETURN_SELF;
}
/* }}} */

/** {{{ \Foundation\Future::blockUntilReady()
 */
PHP_METHOD (foundation_db_future, blockUntilReady)
{
    fdb_future_object *object = FDB_FUTURE_THIS_OBJECT();

    FDB_FUTURE_IS_NULL_RETURN(object);

    fdb_check_error(fdb_future_block_until_ready(object->future));

    RETURN_SELF;
}
/* }}} */

/** {{{ \Foundation\Future::isReady()
 */
PHP_METHOD (foundation_db_future, isReady)
{
    fdb_future_object *object = FDB_FUTURE_THIS_OBJECT();

    FDB_FUTURE_IS_NULL_RETURN(object);

    RETURN_BOOL(fdb_future_is_ready(object->future));
}
/* }}} */

/** {{{ \Foundation\Future::getValue()
 */
PHP_METHOD (foundation_db_future, getValue)
{
    fdb_future_object *object = FDB_FUTURE_THIS_OBJECT();

    FDB_FUTURE_IS_NULL_RETURN(object);

    int valueLength;
    fdb_bool_t valuePresent;
    const uint8_t *value = NULL;

    fdb_check_error(fdb_future_get_value(object->future, &valuePresent, &value, &valueLength));

    if (value == NULL) {
        RETURN_NULL();
    }

    RETURN_STRINGL((char *)value, valueLength);
}
/* }}} */

/** {{{ \Foundation\Future::getError()
 */
PHP_METHOD (foundation_db_future, getError)
{
    fdb_future_object *object = FDB_FUTURE_THIS_OBJECT();

    FDB_FUTURE_IS_NULL_RETURN(object);

    fdb_check_error(fdb_future_get_error(object->future));

    RETURN_NULL();
}
/* }}} */

/** {{{ future_methods
 */
zend_function_entry future_methods[] = {
        PHP_ME(foundation_db_future, __construct,     fdb_arg_future_consturct,         ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_future, cancel,          fdb_arg_future_cancel,            ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_future, destroy,         fdb_arg_future_destroy,           ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_future, blockUntilReady, fdb_arg_future_block_until_ready, ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_future, isReady,         fdb_arg_future_is_ready,          ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_future, getValue,        fdb_arg_future_get_value,         ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_future, getError,        fdb_arg_future_get_error,         ZEND_ACC_PUBLIC)
        PHP_FE_END
};
/* }}} */

FOUNDATION_DB_STARTUP_FUNCTION(foundation_future)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "Foundation", "Future", future_methods);
    ce.create_object = foundation_db_future_objects_new;
    foundation_db_future_ce = zend_register_internal_class(&ce);

    memcpy(&foundation_db_future_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    foundation_db_future_handlers.offset   = XtOffsetOf(fdb_database_object , zo);
    foundation_db_future_handlers.free_obj = foundation_db_future_objects_free;


    return SUCCESS;
}