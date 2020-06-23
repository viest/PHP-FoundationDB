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

zend_class_entry *foundation_db_database_ce;

static zend_object_handlers foundation_db_database_handlers;

/** {{{ foundation_db_database_object_alloc
 */
static zend_always_inline void *foundation_db_database_object_alloc(size_t obj_size, zend_class_entry *ce) {
    void *obj = emalloc(obj_size);
    memset(obj, 0, obj_size);
    return obj;
}

/** {{{ foundation_db_database_objects_new
 */
PHP_FOUNDATIONDB_API zend_object *foundation_db_database_objects_new(zend_class_entry *ce)
{
    fdb_database_object *object = foundation_db_database_object_alloc(sizeof(fdb_database_object), ce);

    zend_object_std_init(&object->zo, ce);
    object_properties_init(&object->zo, ce);

    object->database    = NULL;
    object->zo.handlers = &foundation_db_database_handlers;

    return &object->zo;
}
/* }}} */

/** {{{ foundation_db_database_objects_free
 */
static void foundation_db_database_objects_free(zend_object *object)
{
    fdb_database_object *fdb_database_object = php_foundation_db_database_fetch_object(object);

    if (fdb_database_object->database != NULL) {
        fdb_database_destroy(fdb_database_object->database);
    }

    zend_object_std_dtor(&fdb_database_object->zo);
}
/* }}} */

/* {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(fdb_arg_database_consturct, 0, 0, 1)
    ZEND_ARG_INFO(0, cluster_file_path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_database_destruct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_database_set_option, 0, 0, 2)
    ZEND_ARG_INFO(0, option)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_database_create_transaction, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ \Foundation\Database::__construct()
 */
PHP_METHOD (foundation_db_database, __construct)
{
    FDBDatabase *database = NULL;
    zend_string *cluster_file_path = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_STR(cluster_file_path)
    ZEND_PARSE_PARAMETERS_END();

    fdb_create_database(ZSTR_VAL(cluster_file_path), &database);

    fdb_database_object *object = FDB_DATABASE_THIS_OBJECT();
    object->database = database;
}
/* }}} */

/** {{{ \Foundation\Database::setOption()
 */
PHP_METHOD (foundation_db_database, setOption)
{
    zval *value;
    zend_long option;

    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_LONG(option)
            Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    RETURN_SELF;

    fdb_database_object *object = FDB_DATABASE_THIS_OBJECT();

    if (Z_TYPE_P(value) == IS_STRING) {
        fdb_check_error(fdb_database_set_option(object->database, option, (uint8_t const*)Z_STRVAL_P(value), Z_STRLEN_P(value)));

        return;
    }

    if (Z_TYPE_P(value) == IS_LONG) {
        fdb_check_error(fdb_database_set_option(object->database, option, (uint8_t const*)Z_LVAL_P(value), sizeof(uint8_t)));

        return;
    }
}
/* }}} */

/** {{{ \Foundation\Database::createTransaction()
 */
PHP_METHOD (foundation_db_database, createTransaction)
{
    FDBTransaction *transaction = NULL;
    fdb_database_object *object = FDB_DATABASE_THIS_OBJECT();

    fdb_check_error(fdb_database_create_transaction(object->database, &transaction));

    foundation_db_transaction_objects_create(return_value, transaction);
}
/* }}} */


/** {{{ \Foundation\Database::__destruct()
 */
PHP_METHOD (foundation_db_database, __destruct)
{
    fdb_database_object *object = FDB_DATABASE_THIS_OBJECT();

    if (object->database != NULL) {
        fdb_database_destroy(object->database);
    }

    object->database = NULL;
}
/* }}} */

/** {{{ future_methods
 */
zend_function_entry database_methods[] = {
        PHP_ME(foundation_db_database, __construct,       fdb_arg_database_consturct,          ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_database, __destruct,        fdb_arg_database_destruct,           ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_database, setOption,         fdb_arg_database_set_option,         ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_database, createTransaction, fdb_arg_database_create_transaction, ZEND_ACC_PUBLIC)
        PHP_FE_END
};
/* }}} */

FOUNDATION_DB_STARTUP_FUNCTION(foundation_database)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "Foundation", "Database", database_methods);
    ce.create_object = foundation_db_database_objects_new;
    foundation_db_database_ce = zend_register_internal_class(&ce);

    memcpy(&foundation_db_database_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    foundation_db_database_handlers.offset   = XtOffsetOf(fdb_database_object , zo);
    foundation_db_database_handlers.free_obj = foundation_db_database_objects_free;

    return SUCCESS;
}