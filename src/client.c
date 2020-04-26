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
#include "client.h"

zend_class_entry *foundationdb_client_ce;

/* {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(fdb_connection, 0, 0, 1)
                ZEND_ARG_INFO(0, host)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_database, 0, 0, 1)
                ZEND_ARG_INFO(0, database_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_get, 0, 0, 1)
                ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_set, 0, 0, 2)
                ZEND_ARG_INFO(0, key)
                ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_clear, 0, 0, 1)
                ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_range, 0, 0, 2)
                ZEND_ARG_INFO(0, start)
                ZEND_ARG_INFO(0, end)
ZEND_END_ARG_INFO()
/* }}} */

PHP_METHOD (foundationdb_client, __construct)
{
    //
}

/** {{{ \Foundation\Client::connection()
 */
PHP_METHOD (foundationdb_client, connection)
{
    zend_string *host;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_STR(host)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_COPY(return_value, getThis());

    pthread_t *pthread = FOUNDATION_DB_G(network_thread_ptr);

    if (pthread != NULL) {
        return;
    }

    fdb_check_error(fdb_select_api_version(FDB_API_VERSION));
    fdb_check_error(fdb_setup_network());

    pthread_t netThread;
    pthread_create(&netThread, NULL, (void *)runNetwork, NULL);

    FDBFuture *clusterFuture = fdb_create_cluster(ZSTR_VAL(host));
    fdb_wait_check_error(clusterFuture);

    foundation_db_object *obj = Z_FOUNDATION_DB_P(getThis());

    FOUNDATION_DB_G(cluster_future_ptr) = clusterFuture;
    FOUNDATION_DB_G(network_thread_ptr) = &netThread;
}
/* }}} */

/** {{{ \Foundation\Client::database()
 */
PHP_METHOD (foundationdb_client, database)
{
    zend_string *database_name;

    FDBCluster *cluster;
    FDBFuture *dbFuture;
    FDBFuture *f;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_STR(database_name)
    ZEND_PARSE_PARAMETERS_END();

    fdb_check_error(fdb_future_get_cluster(FOUNDATION_DB_G(cluster_future_ptr), &cluster));

    dbFuture = fdb_cluster_create_database(cluster, (uint8_t *)ZSTR_VAL(database_name), (int)ZSTR_LEN(database_name));

    fdb_wait_check_error(dbFuture);

    FOUNDATION_DB_G(db_future_ptr) = dbFuture;

    ZVAL_COPY(return_value, getThis());
}
/* }}} */

/** {{{ \Foundation\Client::set()
 */
PHP_METHOD (foundationdb_client, set)
{
    zend_string *key, *value;

    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_STR(key)
            Z_PARAM_STR(value)
    ZEND_PARSE_PARAMETERS_END();

    foundation_db_object *obj = Z_FOUNDATION_DB_P(getThis());

    FOUNDATIONDB_CREATE_TRANSACTION(obj);

    fdb_transaction_set(obj->transaction, (uint8_t *)ZSTR_VAL(key), (int)ZSTR_LEN(key), (uint8_t *)ZSTR_VAL(value), (int)ZSTR_LEN(value));

    FOUNDATIONDB_COMMIT_TRANSACTION(obj);

    RETURN_TRUE;
}
/* }}} */

/** {{{ \Foundation\Client::range()
 */
PHP_METHOD (foundationdb_client, range)
{
    zend_long start_offset, end_offset;

    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_LONG(start_offset)
            Z_PARAM_LONG(end_offset)
    ZEND_PARSE_PARAMETERS_END();

    foundation_db_object *obj = Z_FOUNDATION_DB_P(getThis());

    FOUNDATIONDB_CREATE_TRANSACTION(obj);

    fdb_bool_t begin_or_equal = 0;
    fdb_bool_t end_or_equal = 0;

    int limit = 0;
    int target_bytes = 0;
    int iteration = 0;

    FDBFuture *getFuture = fdb_transaction_get_range(
            obj->transaction,
            (uint8_t *)"",
            (int) strlen(""),
            begin_or_equal,
            (int) start_offset,
            (uint8_t *) "",
            (int) strlen(""),
            end_or_equal,
            (int) end_offset + 1,
            limit,
            target_bytes,
            FDB_STREAMING_MODE_WANT_ALL,
            iteration,
            0, 0
    );

    fdb_check_error(fdb_future_block_until_ready(getFuture));
    fdb_check_error(fdb_future_get_error(getFuture));

    array_init(return_value);

    FOUNDATIONDB_GET_KEYVAL(getFuture, return_value);
}
/* }}} */

/** {{{ \Foundation\Client::get()
 */
PHP_METHOD (foundationdb_client, get)
{
    zend_string *key;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_STR(key)
    ZEND_PARSE_PARAMETERS_END();

    foundation_db_object *obj = Z_FOUNDATION_DB_P(getThis());

    FOUNDATIONDB_CREATE_TRANSACTION(obj);

    FDBFuture *getFuture = fdb_transaction_get(obj->transaction, (uint8_t *)ZSTR_VAL(key), (int)ZSTR_LEN(key), 0);
    fdb_wait_check_error(getFuture);

    fdb_bool_t valuePresent;
    const uint8_t *value = NULL;
    int valueLength;
    fdb_check_error(fdb_future_get_value(getFuture, &valuePresent, &value, &valueLength));

    if (value == NULL) {
        RETURN_NULL();
    }

    RETURN_STRINGL((char *)value, valueLength);
}
/* }}} */

/** {{{ \Foundation\Client::clear()
 */
PHP_METHOD (foundationdb_client, clear)
{
    zend_string *key;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_STR(key)
    ZEND_PARSE_PARAMETERS_END();

    foundation_db_object *obj = Z_FOUNDATION_DB_P(getThis());

    FOUNDATIONDB_CREATE_TRANSACTION(obj);

    fdb_transaction_clear(obj->transaction, (uint8_t *)ZSTR_VAL(key), (int)ZSTR_LEN(key));

    FOUNDATIONDB_COMMIT_TRANSACTION(obj);

    RETURN_TRUE;
}
/* }}} */

/** {{{ client_methods
*/
zend_function_entry client_methods[] = {
        PHP_ME(foundationdb_client, __construct, NULL            , ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
        PHP_ME(foundationdb_client, connection,  fdb_connection  , ZEND_ACC_PUBLIC)
        PHP_ME(foundationdb_client, database,    fdb_database    , ZEND_ACC_PUBLIC)
        PHP_ME(foundationdb_client, set,         fdb_set         , ZEND_ACC_PUBLIC)
        PHP_ME(foundationdb_client, range,       fdb_range       , ZEND_ACC_PUBLIC)
        PHP_ME(foundationdb_client, get,         fdb_get         , ZEND_ACC_PUBLIC)
        PHP_ME(foundationdb_client, clear,       fdb_clear       , ZEND_ACC_PUBLIC)
        PHP_FE_END
};
/* }}} */

FOUNDATIONDB_STARTUP_FUNCTION(foundation_client)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "Foundation", "Client", client_methods);

    ce.create_object       = foundation_db_objects_new;
    foundationdb_client_ce = zend_register_internal_class(&ce);

    memcpy(&foundationdb_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    foundationdb_handlers.offset   = XtOffsetOf(foundation_db_object, zo);
    foundationdb_handlers.free_obj = foundation_db_objects_free;

    return SUCCESS;
}