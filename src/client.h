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


#ifndef FOUNDATIONDB_CLIENT_H
#define FOUNDATIONDB_CLIENT_H

#define FDB_FUTURE_HANDLE "future_handle"

extern zend_class_entry *foundationdb_client_ce;

FOUNDATIONDB_STARTUP_FUNCTION(foundation_client);

#endif
