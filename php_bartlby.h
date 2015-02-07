/*
//
  +----------------------------------------------------------------------+
  | PHP Version 4                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2003 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.02 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/2_02.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+

  $Id: php_bartlby.h,v 1.48 2008/03/14 01:11:19 hjanuschka Exp $ 
*/
#define BARTLBY_VERSION "1.6.0"

#define BARTLBY_OBJECT_SERVICE 1
#define BARTLBY_OBJECT_SERVER 2
#define BARTLBY_OBJECT_DOWNTIME 3
#define BARTLBY_OBJECT_WORKER 4
#define BARTLBY_OBJECT_TRAP 5
#define BARTLBY_OBJECT_SERVICEGROUP 6
#define BARTLBY_OBJECT_SERVERGROUP 7
#define BARTLBY_OBJECT_TRIGGER 8


#define BARTLBY_AUDIT_TYPE_SERVICE 1
#define BARTLBY_AUDIT_TYPE_SERVER 2
#define BARTLBY_AUDIT_TYPE_WORKER 3
#define BARTLBY_AUDIT_TYPE_SERVERGROUP 4
#define BARTLBY_AUDIT_TYPE_SERVICEGROUP 5
#define BARTLBY_AUDIT_TYPE_DOWNTIME 6
#define BARTLBY_AUDIT_TYPE_GENERIC 7
#define BARTLBY_AUDIT_TYPE_TRAP 7
#define BARTLBY_AUDIT_TYPE_TRIGGER 8


#define BARTLBY_AUDIT_ACTION_ADD 1
#define BARTLBY_AUDIT_ACTION_MODIFY 2
#define BARTLBY_AUDIT_ACTION_DELETE 3



#define BARTLBY_SERVICE_GONE 1
#define BARTLBY_SERVER_GONE 2
#define BARTLBY_WORKER_GONE 3
#define BARTLBY_DOWNTIME_GONE 4
#define BARTLBY_SERVICEGROUP_GONE 5
#define BARTLBY_SERVERGROUP_GONE 6
#define BARTLBY_TRAP_GONE 7
#define BARTLBY_TRIGGER_GONE 8


#define BARTLBY_OBJECT_CHANGED 1
#define BARTLBY_OBJECT_DELETED 2
#define BARTLBY_OBJECT_OUT_OF_SYNC 3
#define BARTLBY_OBJECT_ADDED  4



#define DT_SERVICE 1
#define DT_SERVER 2
#define DT_SERVERGROUP 3
#define DT_SERVICEGROUP 4

#define SERVICE_UNHANDLED 0;
#define SERVICE_HANDLED 1;

#ifndef PHP_BARTLBY_H
#define PHP_BARTLBY_H

extern zend_module_entry bartlby_module_entry;
#define phpext_bartlby_ptr &bartlby_module_entry

#ifdef PHP_WIN32
#define PHP_BARTLBY_API __declspec(dllexport)
#else
#define PHP_BARTLBY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/times.h> 


typedef struct _bartlby_res {
    char *cfgfile;
    void * bartlby_address;
    void * SOHandle;
    
    
} bartlby_res;
#define BARTLBY_RES_NAME "Bartlby Resource"

#define LOAD_SYMBOL(x,y,z) 	x=dlsym(y, z); \
    	if((dlmsg=dlerror()) != NULL) { \
        	php_error(E_WARNING, "dl error: %s", dlmsg);	\
		RETURN_FALSE;	\
    	}
    	

#define BARTLBY_DEFAULT_STRING 1
#define BARTLBY_DEFAULT_LONG 2
#define BARTLBY_DEFAULT_ERROR 3
#define BARTLBY_FIELD_REQUIRED 1
#define BARTLBY_FIELD_OPTIONAL 0

#define GETARRAY_EL_FROM_HASH(target, element,temp,  array, required, def_type, def_value) \
if(SUCCESS == zend_hash_find(Z_ARRVAL_P(array), element, strlen(element) + 1, (void**) &temp)) { \
	target = *temp; \
}  else { \
	target = NULL; \
} \
if(required == 1) { \
	if(target == NULL) { \
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Element %s is required\n", element); \
		RETURN_BOOL(0); \
	} \
	if(Z_TYPE_P(target) == IS_NULL) {\
		MAKE_STD_ZVAL(target); \
		if(def_type == BARTLBY_DEFAULT_STRING) { \
			ZVAL_STRING(target, (char*)def_value, 1); \
		} \
		if(def_type == BARTLBY_DEFAULT_LONG) { \
			ZVAL_LONG(target, (long)def_value);\
		}\
		if(def_type == BARTLBY_DEFAULT_ERROR) {\
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Element %s is required and has no value\n", element); \
			RETURN_BOOL(0); \
		} \
	}\
}
	




PHP_MINIT_FUNCTION(bartlby);
PHP_MSHUTDOWN_FUNCTION(bartlby);
PHP_RINIT_FUNCTION(bartlby);
PHP_RSHUTDOWN_FUNCTION(bartlby);
PHP_MINFO_FUNCTION(bartlby);

PHP_FUNCTION(confirm_bartlby_compiled);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_info);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_version);
PHP_FUNCTION(bartlby_config);
PHP_FUNCTION(bartlby_lib_info);
PHP_FUNCTION(bartlby_add_server);
PHP_FUNCTION(bartlby_delete_server);
PHP_FUNCTION(bartlby_modify_server);
PHP_FUNCTION(bartlby_get_server_by_id);

PHP_FUNCTION(bartlby_get_worker);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_service);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_server);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_downtime);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_downtime_by_id);	/* For testing, remove later. */

PHP_FUNCTION(bartlby_get_servergroup);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_servergroup_by_id);	/* For testing, remove later. */

PHP_FUNCTION(bartlby_get_servicegroup);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_servicegroup_by_id);	/* For testing, remove later. */


PHP_FUNCTION(bartlby_encode);
PHP_FUNCTION(bartlby_decode);

PHP_FUNCTION(bartlby_add_service);
PHP_FUNCTION(bartlby_add_service_array);
PHP_FUNCTION(bartlby_delete_service);
PHP_FUNCTION(bartlby_modify_service);
PHP_FUNCTION(bartlby_get_service_by_id);


PHP_FUNCTION(bartlby_callback_test);

PHP_FUNCTION(bartlby_add_worker);
PHP_FUNCTION(bartlby_delete_worker);
PHP_FUNCTION(bartlby_modify_worker);
PHP_FUNCTION(bartlby_get_worker_by_id);

PHP_FUNCTION(bartlby_add_downtime);
//PHP_FUNCTION(bartlby_downtime_map);
PHP_FUNCTION(bartlby_modify_downtime);
PHP_FUNCTION(bartlby_delete_downtime);

PHP_FUNCTION(bartlby_reload);
//PHP_FUNCTION(bartlby_svc_map);
//PHP_FUNCTION(bartlby_svc_map_test);
PHP_FUNCTION(bartlby_shm_destroy);


PHP_FUNCTION(bartlby_add_servergroup);
//PHP_FUNCTION(bartlby_servergroup_map);
PHP_FUNCTION(bartlby_modify_servergroup);
PHP_FUNCTION(bartlby_delete_servergroup);
PHP_FUNCTION(bartlby_add_servicegroup);
//PHP_FUNCTION(bartlby_servicegroup_map);
PHP_FUNCTION(bartlby_modify_servicegroup);
PHP_FUNCTION(bartlby_delete_servicegroup);
PHP_FUNCTION(bartlby_toggle_servicegroup_notify);
PHP_FUNCTION(bartlby_toggle_servergroup_notify);
PHP_FUNCTION(bartlby_toggle_servicegroup_active);
PHP_FUNCTION(bartlby_toggle_servergroup_active);
	
PHP_FUNCTION(bartlby_set_servergroup_id);
PHP_FUNCTION(bartlby_set_servicegroup_id);
	


PHP_FUNCTION(bartlby_toggle_service_notify);
PHP_FUNCTION(bartlby_toggle_service_handled);
PHP_FUNCTION(bartlby_toggle_server_notify);
PHP_FUNCTION(bartlby_toggle_service_active);
PHP_FUNCTION(bartlby_toggle_server_active);
PHP_FUNCTION(bartlby_toggle_sirene);

PHP_FUNCTION(bartlby_ack_problem);

PHP_FUNCTION(bartlby_check_shm_size);
PHP_FUNCTION(bartlby_check_force);


PHP_FUNCTION(bartlby_event_tick);
PHP_FUNCTION(bartlby_event_fetch);

PHP_FUNCTION(bartlby_set_server_id);
PHP_FUNCTION(bartlby_set_service_id);
PHP_FUNCTION(bartlby_set_worker_id);
PHP_FUNCTION(bartlby_set_worker_state);
PHP_FUNCTION(bartlby_set_downtime_id);
PHP_FUNCTION(bartlby_set_passive);

	
PHP_FUNCTION(bartlby_bulk_service_active);
PHP_FUNCTION(bartlby_bulk_service_notify);
PHP_FUNCTION(bartlby_bulk_force_services);
PHP_FUNCTION(bartlby_service_set_interval);

PHP_FUNCTION(bartlby_get_core_extension_info);

PHP_FUNCTION(bartlby_new);
PHP_FUNCTION(bartlby_close);



PHP_FUNCTION(bartlby_get_trap);
PHP_FUNCTION(bartlby_add_trap);
PHP_FUNCTION(bartlby_modify_trap);
PHP_FUNCTION(bartlby_delete_trap);
PHP_FUNCTION(bartlby_set_trap_id);
PHP_FUNCTION(bartlby_get_trap_by_id);


PHP_FUNCTION(bartlby_get_trigger);
PHP_FUNCTION(bartlby_add_trigger);
PHP_FUNCTION(bartlby_modify_trigger);
PHP_FUNCTION(bartlby_delete_trigger);
PHP_FUNCTION(bartlby_set_trigger_id);
PHP_FUNCTION(bartlby_get_trigger_by_id);




PHP_FUNCTION(bartlby_cleanup_tests);
PHP_FUNCTION(bartlby_get_object_by_id);

//Notitification Log

PHP_FUNCTION(bartlby_notification_log_at_index);

PHP_METHOD(Bartlby, testFunc);

PHP_FUNCTION(bartlby_get_thread_info);
PHP_FUNCTION(bartlby_get_thread_count);



#ifdef ZTS
#define BARTLBY_G(v) TSRMG(bartlby_globals_id, zend_bartlby_globals *, v)
#else
#define BARTLBY_G(v) (bartlby_globals.v)
#endif

ZEND_BEGIN_MODULE_GLOBALS(bartlby)
	long  force_audit;
ZEND_END_MODULE_GLOBALS(bartlby)


#endif	/* PHP_BARTLBY_H */





#define BARTLBY_OBJECT_GONE(zbartlby_resource, bres, id, type, msg) if(bartlby_mark_object_gone(zbartlby_resource, bres, id, type, msg) < 0) { \
																		php_error_docref(NULL TSRMLS_CC, E_ERROR, "bartlby_mark_object_gone() Callback failed - and force_audit is enabled via INI"); \
															   			RETURN_BOOL(0); \
																	}

#define BARTLBY_OBJECT_AUDIT(zbartlby_resource, id, type, action) if(bartlby_object_audit(zbartlby_resource, id, type, action) < 0) { \
																		php_error_docref(NULL TSRMLS_CC, E_ERROR, "bartlby_object_audit() Callback failed - and force_audit is enabled via INI"); \
															   			RETURN_BOOL(0); \
																	}

#define BARTLBY_GENERIC_AUDIT(zbartlby_resource, id, type, logl) if(bartlby_generic_audit(zbartlby_resource, id, type, logl) < 0) { \
																		php_error_docref(NULL TSRMLS_CC, E_ERROR, "bartlby_generic_audit() Callback failed - and force_audit is enabled via INI"); \
															   			RETURN_BOOL(0); \
																	}
#define BARTLBY_GENERIC_AUDIT_INT(zbartlby_resource, id, type, logl) if(bartlby_generic_audit(zbartlby_resource, id, type, logl) < 0) { \
																		php_error_docref(NULL TSRMLS_CC, E_ERROR, "bartlby_generic_audit() Callback failed - and force_audit is enabled via INI"); \
															   			return -1; \
																	}





//FUNCTIONS INSIDE EXTENSION
char * getConfigValue(char * key, char * fname);
void xbartlby_encode(char * msg, int length);
void xbartlby_decode(char * msg, int length);
void * bartlby_get_sohandle(char * cfgfile);
void * bartlby_get_shm(char * cfgfile);
int bartlby_mark_object_gone(zval * zbartlby_resource, bartlby_res * bres, long id, int type, int msg);
int bartlby_generic_audit(zval * bartlby_resource,  long object_id, long audit_type, char * logline);
int btl_is_array(zval * ar, long service_id);
static void php_bartlby_init_globals(zend_bartlby_globals *bartlby_globals);


//SHM HANDLE STUFF
struct service * bartlby_SHM_ServiceMap(void *);
struct btl_event * bartlby_SHM_EventMap(void * shm_addr);
struct downtime * bartlby_SHM_DowntimeMap(void * shm_addr);
struct shm_header * bartlby_SHM_GetHDR(void *);
struct worker * bartlby_SHM_WorkerMap(void * shm_addr);
struct server * bartlby_SHM_ServerMap(void * shm_addr);
struct trap * bartlby_SHM_TrapMap(void * shm_addr);
struct servicegroup * bartlby_SHM_ServiceGroupMap(void * shm_addr);
struct servergroup * bartlby_SHM_ServerGroupMap(void * shm_addr);
struct trigger * bartlby_SHM_TriggerMap(void * shm_addr);



#include "bartlby_shm.h"



