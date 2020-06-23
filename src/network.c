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

zend_class_entry *foundation_db_network_ce;

/* {{{ ARG_INFO
 */
ZEND_BEGIN_ARG_INFO_EX(fdb_arg_network_set_option, 0, 0, 2)
                ZEND_ARG_INFO(0, option)
                ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_network_setup, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_network_run, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(fdb_arg_network_stop, 0, 0, 0)
ZEND_END_ARG_INFO()
/* }}} */

/** {{{ \Foundation\Network::setOption(): self
 */
PHP_METHOD (foundation_db_network, setOption)
{
    zval *value;
    zend_long option;

    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_LONG(option)
            Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    ZVAL_COPY(return_value, getThis());

    if (Z_TYPE_P(value) == IS_STRING) {
        fdb_check_error(fdb_network_set_option(option, (uint8_t const*)Z_STRVAL_P(value), Z_STRLEN_P(value)));

        return;
    }

    if (Z_TYPE_P(value) == IS_LONG) {
        fdb_check_error(fdb_network_set_option(option, (uint8_t const*)Z_LVAL_P(value), sizeof(uint8_t)));

        return;
    }
}
/* }}} */

/** {{{ \Foundation\Network::setup(): self
 */
PHP_METHOD (foundation_db_network, setup)
{
    ZVAL_COPY(return_value, getThis());

    fdb_check_error(fdb_setup_network());
}
/* }}} */

/** {{{ \Foundation\Network::run(): bool
 */
PHP_METHOD (foundation_db_network, run)
{
    pthread_t netThread;
    pthread_create(&netThread, NULL, (void *)runNetwork, NULL);

    FOUNDATION_DB_G(network_thread_ptr) = &netThread;

    RETURN_TRUE;
}
/* }}} */

/** {{{ \Foundation\Network::stop(): bool
 */
PHP_METHOD (foundation_db_network, stop)
{
    fdb_check_error(fdb_stop_network());

    RETURN_TRUE;
}
/* }}} */

/** {{{ network_methods
 */
zend_function_entry network_methods[] = {
        PHP_ME(foundation_db_network, setOption, fdb_arg_network_set_option,  ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_network, setup,     fdb_arg_network_setup,       ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_network, run,       fdb_arg_network_run,         ZEND_ACC_PUBLIC)
        PHP_ME(foundation_db_network, stop,      fdb_arg_network_stop,        ZEND_ACC_PUBLIC)
        PHP_FE_END
};
/* }}} */

/** {{{ FOUNDATION_DB_STARTUP_FUNCTION
*/
FOUNDATION_DB_STARTUP_FUNCTION(foundation_network)
{
    zend_class_entry ce;

    INIT_NS_CLASS_ENTRY(ce, "Foundation", "Network", network_methods);
    foundation_db_network_ce = zend_register_internal_class(&ce);

    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_LOCAL_ADDRESS",                                FDB_NET_OPTION_LOCAL_ADDRESS)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_CLUSTER_FILE",                                 FDB_NET_OPTION_CLUSTER_FILE)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TRACE_ENABLE",                                 FDB_NET_OPTION_TRACE_ENABLE)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TRACE_ROLL_SIZE",                              FDB_NET_OPTION_TRACE_ROLL_SIZE)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TRACE_MAX_LOGS_SIZE",                          FDB_NET_OPTION_TRACE_MAX_LOGS_SIZE)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TRACE_LOG_GROUP",                              FDB_NET_OPTION_TRACE_LOG_GROUP)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TRACE_FORMAT",                                 FDB_NET_OPTION_TRACE_FORMAT)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_KNOB",                                         FDB_NET_OPTION_KNOB)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TLS_PLUGIN",                                   FDB_NET_OPTION_TLS_PLUGIN)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TLS_CERT_BYTES",                               FDB_NET_OPTION_TLS_CERT_BYTES)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TLS_CERT_PATH",                                FDB_NET_OPTION_TLS_CERT_PATH)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TLS_KEY_BYTES",                                FDB_NET_OPTION_TLS_KEY_BYTES)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TLS_KEY_PATH",                                 FDB_NET_OPTION_TLS_KEY_PATH)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TLS_VERIFY_PEERS",                             FDB_NET_OPTION_TLS_VERIFY_PEERS)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_BUGGIFY_ENABLE",                               FDB_NET_OPTION_BUGGIFY_ENABLE)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_BUGGIFY_DISABLE",                              FDB_NET_OPTION_BUGGIFY_DISABLE)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_BUGGIFY_SECTION_ACTIVATED_PROBABILITY",        FDB_NET_OPTION_BUGGIFY_SECTION_ACTIVATED_PROBABILITY)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_BUGGIFY_SECTION_FIRED_PROBABILITY",            FDB_NET_OPTION_BUGGIFY_SECTION_FIRED_PROBABILITY)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TLS_CA_BYTES",                                 FDB_NET_OPTION_TLS_CA_BYTES)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TLS_CA_PATH",                                  FDB_NET_OPTION_TLS_CA_PATH)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_TLS_PASSWORD",                                 FDB_NET_OPTION_TLS_PASSWORD)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_DISABLE_MULTI_VERSION_CLIENT_API",             FDB_NET_OPTION_DISABLE_MULTI_VERSION_CLIENT_API)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_CALLBACKS_ON_EXTERNAL_THREADS",                FDB_NET_OPTION_CALLBACKS_ON_EXTERNAL_THREADS)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_EXTERNAL_CLIENT_LIBRARY",                      FDB_NET_OPTION_EXTERNAL_CLIENT_LIBRARY)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_EXTERNAL_CLIENT_DIRECTORY",                    FDB_NET_OPTION_EXTERNAL_CLIENT_DIRECTORY)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_DISABLE_LOCAL_CLIENT",                         FDB_NET_OPTION_DISABLE_LOCAL_CLIENT)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_DISABLE_CLIENT_STATISTICS_LOGGING",            FDB_NET_OPTION_DISABLE_CLIENT_STATISTICS_LOGGING)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_ENABLE_SLOW_TASK_PROFILING",                   FDB_NET_OPTION_ENABLE_SLOW_TASK_PROFILING)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_CLIENT_BUGGIFY_ENABLE",                        FDB_NET_OPTION_CLIENT_BUGGIFY_ENABLE)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_CLIENT_BUGGIFY_DISABLE",                       FDB_NET_OPTION_CLIENT_BUGGIFY_DISABLE)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_CLIENT_BUGGIFY_SECTION_ACTIVATED_PROBABILITY", FDB_NET_OPTION_CLIENT_BUGGIFY_SECTION_ACTIVATED_PROBABILITY)
    REGISTER_CLASS_CONST_LONG(foundation_db_network_ce, "OPTION_CLIENT_BUGGIFY_SECTION_FIRED_PROBABILITY",     FDB_NET_OPTION_CLIENT_BUGGIFY_SECTION_FIRED_PROBABILITY)

    return SUCCESS;
}
/* }}} */