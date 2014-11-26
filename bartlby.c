/* $Id$ */
/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2005-2008 Helmut Januschka - All Rights Reserved
 *   Contact: <helmut@januschka.com>, <contact@bartlby.org>
 *
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge MA 02139,
 *   USA; either version 2 of the License, or (at your option) any later
 *   version; incorporated herein by reference.
 *
 *   visit: www.bartlby.org for support
 * ----------------------------------------------------------------------- */
/*
$Revision$
$HeadURL$
$Date$
$Author$ 
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_bartlby.h"

ZEND_DECLARE_MODULE_GLOBALS(bartlby)

typedef struct _bartlby_res {
    char *cfgfile;
    void * bartlby_address;
    void * SOHandle;
    
    
} bartlby_res;



#define BARTLBY_RES_NAME "Bartlby Resource"

int le_bartlby;

 
 
zend_function_entry bartlby_functions[] = {
	
	PHP_FE(bartlby_callback_test, NULL)
	PHP_FE(bartlby_new,NULL)
	PHP_FE(bartlby_close,NULL)
	PHP_FE(bartlby_get_info,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_version,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_config,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_lib_info, NULL)
	

	PHP_FE(bartlby_get_service,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_worker,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_server,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_downtime,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_downtime_by_id,	NULL)		/* For testing, remove later. */
	

	PHP_FE(bartlby_get_servicegroup,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_servicegroup_by_id,	NULL)		/* For testing, remove later. */


	PHP_FE(bartlby_get_servergroup,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_servergroup_by_id,	NULL)		/* For testing, remove later. */





	PHP_FE(bartlby_add_server,NULL)
	PHP_FE(bartlby_delete_server,NULL)
	PHP_FE(bartlby_modify_server,NULL)
	PHP_FE(bartlby_get_server_by_id,NULL)
	
	
	PHP_FE(bartlby_encode,NULL)
	PHP_FE(bartlby_decode,NULL)
	
	
	PHP_FE(bartlby_add_service,NULL)
	PHP_FE(bartlby_add_service_array, NULL)
	PHP_FE(bartlby_delete_service,NULL)
	PHP_FE(bartlby_modify_service,NULL)
	PHP_FE(bartlby_get_service_by_id,NULL)
	
	PHP_FE(bartlby_add_worker, NULL)
	PHP_FE(bartlby_delete_worker, NULL)
	PHP_FE(bartlby_modify_worker, NULL)
	PHP_FE(bartlby_get_worker_by_id, NULL)
	
	
	PHP_FE(bartlby_add_downtime, NULL)
	//PHP_FE(bartlby_downtime_map, NULL)
	PHP_FE(bartlby_modify_downtime, NULL)
	PHP_FE(bartlby_delete_downtime, NULL)
	
	
	PHP_FE(bartlby_add_servergroup, NULL)
	//PHP_FE(bartlby_servergroup_map, NULL)
	PHP_FE(bartlby_modify_servergroup, NULL)
	PHP_FE(bartlby_delete_servergroup, NULL)
	
	
	PHP_FE(bartlby_add_servicegroup, NULL)
	//PHP_FE(bartlby_servicegroup_map, NULL)
	PHP_FE(bartlby_modify_servicegroup, NULL)
	PHP_FE(bartlby_delete_servicegroup, NULL)
	
	
	PHP_FE(bartlby_toggle_servicegroup_notify, NULL)
	PHP_FE(bartlby_toggle_servergroup_notify, NULL)
	PHP_FE(bartlby_toggle_servicegroup_active, NULL)
	PHP_FE(bartlby_toggle_servergroup_active, NULL)
	
	
	
	
	PHP_FE(bartlby_toggle_service_notify, NULL)
	PHP_FE(bartlby_toggle_service_handled, NULL)
	PHP_FE(bartlby_toggle_server_notify, NULL)
	PHP_FE(bartlby_toggle_service_active, NULL)
	PHP_FE(bartlby_toggle_server_active, NULL)
	
	
	PHP_FE(bartlby_toggle_sirene, NULL)
	
	//PHP_FE(bartlby_svc_map,NULL)
	//PHP_FE(bartlby_svc_map_test,NULL)
	
	PHP_FE(bartlby_reload, NULL)
	PHP_FE(bartlby_shm_destroy, NULL)
	
	PHP_FE(bartlby_ack_problem, NULL)
	
	PHP_FE(bartlby_check_shm_size, NULL)
	
	PHP_FE(bartlby_check_force, NULL)
	
	
	PHP_FE(bartlby_event_tick, NULL)
	PHP_FE(bartlby_event_fetch, NULL)
	
	PHP_FE(bartlby_set_passive, NULL)
	
	PHP_FE(bartlby_set_server_id, NULL)
	PHP_FE(bartlby_set_service_id, NULL)
	PHP_FE(bartlby_set_worker_id, NULL)
	PHP_FE(bartlby_set_worker_state, NULL)
	PHP_FE(bartlby_set_downtime_id, NULL)
	
	PHP_FE(bartlby_set_servergroup_id, NULL)
	PHP_FE(bartlby_set_servicegroup_id, NULL)
	

	PHP_FE(bartlby_bulk_service_active,NULL)
	PHP_FE(bartlby_bulk_service_notify,NULL)
	PHP_FE(bartlby_bulk_force_services,NULL)
	
	PHP_FE(bartlby_service_set_interval,NULL)
	
	PHP_FE(bartlby_get_core_extension_info,NULL)
	
	
	PHP_FE(bartlby_notification_log_at_index, NULL)
	
	{NULL, NULL, NULL}	/* Must be the last line in bartlby_functions[] */
};
/* }}} */

/* {{{ bartlby_module_entry
 */
zend_module_entry bartlby_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"bartlby",
	bartlby_functions,
	PHP_MINIT(bartlby),
	PHP_MSHUTDOWN(bartlby),
	PHP_RINIT(bartlby),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(bartlby),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(bartlby),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_BARTLBY
ZEND_GET_MODULE(bartlby)
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("bartlby.force_audit",      "0", PHP_INI_ALL, OnUpdateLong, force_audit, zend_bartlby_globals, bartlby_globals)
    
PHP_INI_END()
/* }}} */

/* {{{ php_bartlby_init_globals
 */
static void php_bartlby_init_globals(zend_bartlby_globals *bartlby_globals)
{
	bartlby_globals->force_audit = 0;
	
}

/* }}} */

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



int bartlby_generic_audit(zval * bartlby_resource,  long object_id, long audit_type, char * logline) {
	
	zval **params[4];
	zval type;
	zval id;
	zval act;
	zval function_name;
	zval log_line;
	
	zval *return_user_call;

	zval *t;
	zval *t2;
	zval *t3;
	zval *t4;
	
	int res;
	INIT_ZVAL(type);
	INIT_ZVAL(id);
	INIT_ZVAL(function_name);
	INIT_ZVAL(act);
	INIT_ZVAL(log_line);


	ZVAL_LONG(&type, audit_type);
	ZVAL_LONG(&id, object_id);
	ZVAL_STRING(&log_line, logline, 0);
	ZVAL_STRING(&function_name, "bartlby_generic_audit", 0);

	
	
	params[0] = &bartlby_resource;
	
	t=&type;
	params[1] = &t;
	t2=&id;
	params[2] = &t2;
	t3=&log_line;
	params[3] = &t3;
	
	
	
	res=call_user_function_ex(EG(function_table), NULL, &function_name, &return_user_call, 4, params, 1, NULL TSRMLS_CC);
	if (res == SUCCESS && return_user_call != NULL && zval_is_true(return_user_call)) {
		return 0;
	} else {
		if((int)BARTLBY_G(force_audit) == 1) {
			return -1;
		}
	} 
    return -1;
    //	if((long)BARTLBY_G(force_audit) == 1)  {

}

int bartlby_object_audit(zval * bartlby_resource,  long audit_type, long object_id, long action) {
	
	zval **params[4];
	zval type;
	zval id;
	zval act;
	zval function_name;
	
	zval *return_user_call;

	zval *t;
	zval *t2;
	zval *t3;
	zval *t4;
	
	int res;
	INIT_ZVAL(type);
	INIT_ZVAL(id);
	INIT_ZVAL(function_name);
	INIT_ZVAL(act);


	ZVAL_LONG(&type, audit_type);
	ZVAL_LONG(&id, object_id);
	ZVAL_LONG(&act, action);
	ZVAL_STRING(&function_name, "bartlby_object_audit", 0);

	
	
	params[0] = &bartlby_resource;
	
	t=&type;
	params[1] = &t;
	t2=&id;
	params[2] = &t2;
	t3=&act;
	params[3] = &t3;
	
	
	
	res=call_user_function_ex(EG(function_table), NULL, &function_name, &return_user_call, 4, params, 1, NULL TSRMLS_CC);
	if (res == SUCCESS && return_user_call != NULL && zval_is_true(return_user_call)) {
		return 0;
	} else {
		if((int)BARTLBY_G(force_audit) == 1) {
			return -1;
		}
	} 
    return -1;
    //	if((long)BARTLBY_G(force_audit) == 1)  {

}

PHP_FUNCTION(bartlby_callback_test) {

	zval * zbartlby_resource;
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);


	BARTLBY_OBJECT_AUDIT(zbartlby_resource, 1, 2222, 1);
	BARTLBY_OBJECT_AUDIT(zbartlby_resource, 2, 3333, 2);
	RETURN_LONG(1);
}

void xbartlby_decode(char * msg, int length) {
	int x;
		
	for(x=0; x<length; x++) {
		//msg[x]=2^msg[x];	
		
	}
	
}
void xbartlby_encode(char * msg, int length) {
	int x;
	for(x=0; x<length; x++) {
		//msg[x]=msg[x]^2;	
	}
	
}

char * getConfigValue(char * key, char * fname) {
	FILE * fp;
	char  str[1024];

	char * tok;
	
	fp=fopen(fname, "r");
	if(!fp) {
		php_error(E_WARNING, "Config file '%s' failed", fname);
		return NULL;
	}
	
	while(fgets(str,1024,fp) != NULL) {
		str[strlen(str)-1]='\0';
		tok=strtok(str, "=");
		if(tok != NULL) {
				if(strcmp(tok, key) == 0) {
						tok=strtok(NULL, "=");
						if(tok == NULL) {
								return NULL;
						}
						if(tok[strlen(tok)-1] == '\r') {
							tok[strlen(tok)-1]='\0';
						}
						fclose(fp);
						return strdup(tok);
						
				} else {
					continue;
				}
					
		}
			
	}
	
	
	fclose(fp);
	
	
	return NULL;
}






void * bartlby_get_sohandle(char * cfgfile) {
	char * data_lib;
	char * dlmsg;
	void * SOHandle;
	
	data_lib=getConfigValue("data_library", cfgfile);
	if(data_lib == NULL) {
		php_error(E_WARNING, "cannot find data_lib key in %s", cfgfile);	
		return NULL;
	}
	SOHandle=dlopen(data_lib, RTLD_LAZY);
	if((dlmsg=dlerror()) != NULL) {
		php_error(E_WARNING, "DL Error: %s", dlmsg);
        	return NULL;
    	}	
    	free(data_lib);
    	return SOHandle;
} 

void * bartlby_get_shm(char * cfgfile) {
	char * shmtok;
	void * bartlby_address;
	int shm_id;
	
	shmtok = getConfigValue("shm_key", cfgfile);
	if(shmtok == NULL) {
		return NULL;	
	}		
	
	
	shm_id = shmget(ftok(shmtok, 32), 0,  0777);
	free(shmtok);
	if(shm_id != -1) {
		bartlby_address=shmat(shm_id,NULL,0);
		return bartlby_address;
	} else {
		php_error(E_WARNING, "cannot attach to SHM");	
		return NULL;
	}
}



int bartlby_mark_object_gone(zval * zbartlby_resource, bartlby_res * bres, long id, int type, int msg) {
	
	
	int x;
	struct shm_header * shm_hdr;
	struct service * svcmap;
	struct server * srvmap;	
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	char * tmpstr;
	
	int audit_type=0;
	int audit_action=0;

	int rtc;
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void *)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
	svcgrpmap=(struct servicegroup *)(void *)&srvgrpmap[shm_hdr->srvgroupcount+1];
	


	//if(msg != BARTLBY_AUDIT_ACTION_ADD) {
		switch(type) {
			case BARTLBY_SERVICE_GONE:
				for(x=0; x<shm_hdr->svccount; x++) {
					if(svcmap[x].service_id == id) {
						svcmap[x].is_gone = msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_SERVICE;

			break;
			case BARTLBY_SERVER_GONE:
				for(x=0; x<shm_hdr->srvcount; x++) {
					if(srvmap[x].server_id == id) {
						srvmap[x].is_gone=msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_SERVER;
			break;
			case BARTLBY_WORKER_GONE:
				for(x=0; x<shm_hdr->wrkcount; x++) {
					if(wrkmap[x].worker_id == id) {
						wrkmap[x].is_gone = msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_WORKER;
			break;

			case BARTLBY_DOWNTIME_GONE:
				for(x=0; x<shm_hdr->dtcount; x++) {
					if(dtmap[x].downtime_id == id) {
						dtmap[x].is_gone = msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_DOWNTIME;
			break;
			case BARTLBY_SERVICEGROUP_GONE:
				for(x=0; x<shm_hdr->svcgroupcount; x++) {
					if(svcgrpmap[x].servicegroup_id == id) {
						//svcgrpmap[x].is_gone = msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_SERVICEGROUP;
			break;
			case BARTLBY_SERVERGROUP_GONE:
				for(x=0; x<shm_hdr->srvgroupcount; x++) {
					if(srvgrpmap[x].servergroup_id == id) {
						//srvgrpmap[x].is_gone = msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_SERVERGROUP;
			break;
			
				
//		}
	}

	switch(msg) {
		case BARTLBY_OBJECT_DELETED:
			audit_action=BARTLBY_AUDIT_ACTION_DELETE;
			bartlby_generic_audit(zbartlby_resource, id, audit_type, "Deleted");
		break;
		case BARTLBY_OBJECT_CHANGED:
			audit_action=BARTLBY_AUDIT_ACTION_MODIFY;
			bartlby_generic_audit(zbartlby_resource, id,audit_type, "Edited");
		break;
		case BARTLBY_OBJECT_ADDED:
			audit_action=BARTLBY_AUDIT_ACTION_ADD;
			bartlby_generic_audit(zbartlby_resource, id, audit_type, "Added");
		break;

	}

	rtc=bartlby_object_audit(zbartlby_resource, audit_type , id,  audit_action);

	
	return rtc;
	


}


PHP_FUNCTION(bartlby_close)
{
	
		//FIXME check resource type BARTLBY
    zval *zbartlby;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby) == FAILURE) {
        RETURN_FALSE;
    }

    zend_list_delete(Z_LVAL_P(zbartlby));
    RETURN_TRUE;
}



static void bartlby_res_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    bartlby_res *res = (bartlby_res*)rsrc->ptr;
    if (res) {
    	//FIXME
    	//shmdt
    	//dlclose
    	
    	if(res->bartlby_address != NULL) {
    		shmdt(res->bartlby_address);
    	}
    	if(res->SOHandle != NULL) {
    		dlclose(res->SOHandle);
    	}
    	efree(res->cfgfile);
    	
      efree(res);
    }
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(bartlby)
{
	REGISTER_INI_ENTRIES();
	/* If you have INI entries, uncomment these lines 
	ZEND_INIT_MODULE_GLOBALS(bartlby, php_bartlby_init_globals, NULL);
	
	*/
	
	/*
	zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "Bartlby", bartlby_class_functions);

  bartlby_ce = zend_register_internal_class(&ce TSRMLS_CC);
	
	zend_declare_property_string(bartlby_ce, "foo", sizeof("foo") - 1, "bar", ZEND_ACC_PUBLIC TSRMLS_CC);
	*/
	
	/*

	#define BARTLBY_AUDIT_TYPE_SERVICE 1
#define BARTLBY_AUDIT_TYPE_SERVER 2
#define BARTLBY_AUDIT_TYPE_WORKER 3
#define BARTLBY_AUDIT_TYPE_SERVERGROUP 4
#define BARTLBY_AUDIT_TYPE_SERVICEGROUP 5
#define BARTLBY_AUDIT_TYPE_DOWNTIME 6


#define BARTLBY_AUDIT_ACTION_ADD 1
#define BARTLBY_AUDIT_ACTION_MODIFY 2
#define BARTLBY_AUDIT_ACTION_DELETE 3

*/
	
	le_bartlby = zend_register_list_destructors_ex(bartlby_res_dtor, NULL, BARTLBY_RES_NAME, module_number);
	
	


	//SET CONSTANTS

	//REGISTER_LONG_CONSTANT(name, lval, flags);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_SERVICE", BARTLBY_AUDIT_TYPE_SERVICE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_SERVER", BARTLBY_AUDIT_TYPE_SERVER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_GENERIC", BARTLBY_AUDIT_TYPE_GENERIC, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_WORKER", BARTLBY_AUDIT_TYPE_WORKER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_SERVERGROUP", BARTLBY_AUDIT_TYPE_SERVERGROUP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_SERVICEGROUP", BARTLBY_AUDIT_TYPE_SERVICEGROUP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_DOWNTIME", BARTLBY_AUDIT_TYPE_DOWNTIME, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_ACTION_ADD", BARTLBY_AUDIT_ACTION_ADD, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_ACTION_MODIFY", BARTLBY_AUDIT_ACTION_MODIFY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_ACTION_DELETE", BARTLBY_AUDIT_ACTION_DELETE, CONST_CS | CONST_PERSISTENT);


	
	return SUCCESS;
	
}

PHP_METHOD(Bartlby, testFunc) /* {{{ */
{
   
    RETURN_STRING("Hello World\n", 1);
}

/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(bartlby)
{
	UNREGISTER_INI_ENTRIES();
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(bartlby)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(bartlby)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(bartlby)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "bartlby support", "enabled");
	php_info_print_table_row(2, "php-ext version", BARTLBY_VERSION);
	php_info_print_table_end();
	DISPLAY_INI_ENTRIES();

	/* Remove comments if you have entries in php.ini
	
	*/
}

PHP_FUNCTION(bartlby_version) {
	RETURN_STRING(BARTLBY_VERSION,1);	
	
}

PHP_FUNCTION(bartlby_new) {
	bartlby_res * res;
	
	zval * bartlby_config;
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &bartlby_config)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	//FIXME return NULL if SHM or DL error
	convert_to_string(bartlby_config);
  res = emalloc(sizeof(bartlby_res));
  res->cfgfile = estrndup(Z_STRVAL_P(bartlby_config), strlen(Z_STRVAL_P(bartlby_config)));
  res->SOHandle=bartlby_get_sohandle(Z_STRVAL_P(bartlby_config));
  res->bartlby_address=bartlby_get_shm(Z_STRVAL_P(bartlby_config));
  
  if(res->bartlby_address == NULL || res->SOHandle == NULL) {
  	RETURN_FALSE;
  }
  
  ZEND_REGISTER_RESOURCE(return_value, res, le_bartlby);
  	
}


PHP_FUNCTION(bartlby_set_downtime_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	char * dlmsg;
	int ret;
	int (*DowntimeChangeId)(int, int, char*);
	
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&from, &to)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	convert_to_long(from);
	convert_to_long(to);
	
	
	LOAD_SYMBOL(DowntimeChangeId,bres->SOHandle, "DowntimeChangeId");
	
	ret=DowntimeChangeId(Z_LVAL_P(from),Z_LVAL_P(to),bres->cfgfile);
	
	RETURN_LONG(ret);	
}
PHP_FUNCTION(bartlby_set_worker_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	char * dlmsg;
	
	int ret;
	
	int (*WorkerChangeId)(int, int,  char*);
	bartlby_res * bres;	
	
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&from, &to)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(from);
	convert_to_long(to);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	LOAD_SYMBOL(WorkerChangeId,bres->SOHandle, "WorkerChangeId");
	ret=WorkerChangeId(Z_LVAL_P(from),Z_LVAL_P(to),bres->cfgfile);
	RETURN_LONG(ret);	
}
PHP_FUNCTION(bartlby_set_service_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	char * dlmsg;
	
	int ret;
	
	int (*ServiceChangeId)(int, int,  char*);
	bartlby_res * bres;
	
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&from, &to)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(from);
	convert_to_long(to);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	LOAD_SYMBOL(ServiceChangeId,bres->SOHandle, "ServiceChangeId");
	ret=ServiceChangeId(Z_LVAL_P(from),Z_LVAL_P(to),bres->cfgfile);
	
	RETURN_LONG(ret);	
}
PHP_FUNCTION(bartlby_set_server_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	void * SOHandle;
	char * dlmsg;
	
	int ret;
	
	int (*ServerChangeId)(int, int, int, char*);
	bartlby_res * bres;

	
	
	if (ZEND_NUM_ARGS() != 4 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzzz", &zbartlby_resource,&from, &to, &mig)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(from);
	convert_to_long(to);
	convert_to_long(mig);
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	LOAD_SYMBOL(ServerChangeId,bres->SOHandle, "ServerChangeId");
	ret=ServerChangeId(Z_LVAL_P(from),Z_LVAL_P(to),Z_LVAL_P(mig),bres->cfgfile);
	RETURN_LONG(ret);	
}

PHP_FUNCTION(bartlby_delete_downtime) {
	zval * zbartlby_resource;
	zval * downtime_id;
	
	char * dlmsg;
	int ret;
	int (*DeleteDowntime)(int, char*);
	struct service svc;
	bartlby_res * bres;


	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&downtime_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(downtime_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	LOAD_SYMBOL(DeleteDowntime,bres->SOHandle, "DeleteDowntime");
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(downtime_id), BARTLBY_DOWNTIME_GONE, BARTLBY_OBJECT_DELETED);
	ret=DeleteDowntime(Z_LVAL_P(downtime_id),bres->cfgfile);

	
	RETURN_LONG(ret);	
}
PHP_FUNCTION(bartlby_modify_downtime) {
	zval * zbartlby_resource;
	zval * downtime_from;
	zval * downtime_to;
	zval * downtime_type;
	zval * downtime_notice;
	zval * downtime_service;
	zval * downtime_id;
	zval * orch_id;
	
	zval ** temp_pp;
	zval * options_array;
	
	
	char * dlmsg;
	int ret;
	int (*UpdateDowntime)(struct downtime *,char *);
	struct downtime svc;
	bartlby_res * bres;


	
	if(ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&downtime_id, &options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}


	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	
	GETARRAY_EL_FROM_HASH(downtime_from, "downtime_from", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(downtime_to, "downtime_to", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(downtime_type, "downtime_type", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(downtime_service, "downtime_service", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(downtime_notice, "downtime_notice", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"default notice");
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	
	
	convert_to_long(downtime_from);
	convert_to_long(downtime_to);
	convert_to_long(downtime_type);
	convert_to_long(downtime_service);
	convert_to_long(downtime_id);
	convert_to_string(downtime_notice);
	convert_to_long(orch_id);
	
	
	
	LOAD_SYMBOL(UpdateDowntime,bres->SOHandle, "UpdateDowntime");
	
	strcpy(svc.downtime_notice, Z_STRVAL_P(downtime_notice));
	svc.downtime_from=Z_LVAL_P(downtime_from);
	svc.downtime_to=Z_LVAL_P(downtime_to);
	svc.downtime_type=Z_LVAL_P(downtime_type);
	svc.service_id=Z_LVAL_P(downtime_service);
	svc.downtime_id=Z_LVAL_P(downtime_id);
	svc.orch_id=Z_LVAL_P(orch_id);
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,svc.downtime_id, BARTLBY_DOWNTIME_GONE, BARTLBY_OBJECT_CHANGED);
	
	

	ret=UpdateDowntime(&svc, bres->cfgfile);
	
	

	RETURN_LONG(ret);		
}




PHP_FUNCTION(bartlby_add_downtime) {
	zval * zbartlby_resource;
	zval * downtime_from;
	zval * downtime_to;
	zval * downtime_type;
	zval * downtime_notice;
	zval * downtime_service;
	zval * orch_id;
	
	zval ** temp_pp;
	zval * options_array;
	
	char * dlmsg;
	int ret;
	int (*AddDowntime)(struct downtime *,char *);
	struct downtime svc;
	bartlby_res * bres;	

	
	if(ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}

	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	
	GETARRAY_EL_FROM_HASH(downtime_from, "downtime_from", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(downtime_to, "downtime_to", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(downtime_type, "downtime_type", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(downtime_service, "downtime_service", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(downtime_notice, "downtime_notice", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"default notice");
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	
	
	
	
	
	convert_to_long(downtime_from);
	convert_to_long(downtime_to);
	convert_to_long(downtime_type);
	convert_to_long(downtime_service);
	convert_to_long(orch_id);
	convert_to_string(downtime_notice);
		
	
	
	
	LOAD_SYMBOL(AddDowntime,bres->SOHandle, "AddDowntime");
	
	strcpy(svc.downtime_notice, Z_STRVAL_P(downtime_notice));
	svc.downtime_from=Z_LVAL_P(downtime_from);
	svc.downtime_to=Z_LVAL_P(downtime_to);
	svc.downtime_type=Z_LVAL_P(downtime_type);
	svc.service_id=Z_LVAL_P(downtime_service);
	svc.orch_id=Z_LVAL_P(orch_id);
	ret=AddDowntime(&svc, bres->cfgfile);
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,ret, BARTLBY_DOWNTIME_GONE, BARTLBY_OBJECT_ADDED);

	

	RETURN_LONG(ret);	
}



PHP_FUNCTION(bartlby_event_tick) {
	zval * zbartlby_resource;

	struct shm_header * shm_hdr;
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	
	int idx;

	bartlby_res * bres;
		
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	idx=shm_hdr->cur_event_index;
	RETURN_LONG(idx);
		
	
}

PHP_FUNCTION(bartlby_event_fetch) {
	zval * zbartlby_resource;
	zval * bartlby_service_id;
	zval * event_index;
	
	struct shm_header * shm_hdr;
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct server * srvmap;	
	
	bartlby_res * bres;



	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &event_index)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(event_index);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	

	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
		
	
	
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	
	if(Z_LVAL_P(event_index) < EVENT_QUEUE_MAX) {
		add_assoc_string(return_value, "message", evntmap[Z_LVAL_P(event_index)].evnt_message, 1);
		add_assoc_long(return_value, "id", evntmap[Z_LVAL_P(event_index)].evnt_id);
		add_assoc_long(return_value, "time", evntmap[Z_LVAL_P(event_index)].evnt_time);
	} else {
		RETURN_FALSE;
	}
		
		
}


PHP_FUNCTION(bartlby_check_shm_size) {
	zval * zbartlby_resource;
	zval * bartlby_service_id;
	struct shm_header * shm_hdr;
	int my_size;
	
	struct service * svcmap;	
	bartlby_res * bres;

	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	my_size=sizeof(struct shm_header)+ sizeof(struct server) + sizeof(struct worker)+sizeof(struct service)+sizeof(struct downtime)+sizeof(struct servergroup)+sizeof(struct servicegroup);
	if(my_size != shm_hdr->size_of_structs) {	
		RETURN_FALSE;
	} else {
		RETURN_TRUE;
	}		
		
}

PHP_FUNCTION(bartlby_toggle_sirene) {
	zval * zbartlby_resource;
	struct shm_header * shm_hdr;
	int r;
	
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	if(shm_hdr->sirene_mode== 1) {
		shm_hdr->sirene_mode = 0;
		r=0;
	} else {
		shm_hdr->sirene_mode = 1;
		r=1;
	}
	RETURN_LONG(r);
		
}


PHP_FUNCTION(bartlby_set_passive) {
	zval * zbartlby_resource;
	zval * bartlby_new_state;
	zval * bartlby_new_output;
	zval * bartlby_service_id;
	struct shm_header * shm_hdr;
	int r;
	
	struct service * svcmap;	
	bartlby_res * bres;


	if (ZEND_NUM_ARGS() != 4 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzzz", &zbartlby_resource, &bartlby_service_id, &bartlby_new_state, &bartlby_new_output)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(bartlby_service_id);
	convert_to_long(bartlby_new_state);
	convert_to_string(bartlby_new_output);
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
		
	
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
		php_error(E_WARNING, "Service id out of bounds");	
		RETURN_FALSE;	
	}
		
	svcmap[Z_LVAL_P(bartlby_service_id)].last_state = svcmap[Z_LVAL_P(bartlby_service_id)].current_state;
	svcmap[Z_LVAL_P(bartlby_service_id)].current_state = Z_LVAL_P(bartlby_new_state);	
	snprintf(svcmap[Z_LVAL_P(bartlby_service_id)].new_server_text, 2040, "%s",Z_STRVAL_P(bartlby_new_output)); 
	svcmap[Z_LVAL_P(bartlby_service_id)].last_check = time(NULL);	
	svcmap[Z_LVAL_P(bartlby_service_id)].service_retain_current=0; //Reset Retain counter
	//svcmap[Z_LVAL_P(bartlby_service_id)].notify_last_state=svcmap[Z_LVAL_P(bartlby_service_id)].current_state; //Reset Retain counter
	
	r=1;

	BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcmap[Z_LVAL_P(bartlby_service_id)].service_id ,BARTLBY_AUDIT_TYPE_SERVICE, "Submitted Passive Result via UI");
	RETURN_LONG(r);
	
}


PHP_FUNCTION(bartlby_ack_problem) {
	zval * zbartlby_resource;
	zval * bartlby_service_id;
	struct shm_header * shm_hdr;
	int r;
	
	struct service * svcmap;	
	bartlby_res * bres;

	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &bartlby_service_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(bartlby_service_id);
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
		
		
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
		php_error(E_WARNING, "Service id out of bounds");	
		RETURN_FALSE;	
	}
		
	svcmap[Z_LVAL_P(bartlby_service_id)].service_ack_current = 1;	
	
	BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcmap[Z_LVAL_P(bartlby_service_id)].service_id ,BARTLBY_AUDIT_TYPE_SERVICE, "Acknowledged Problem");

	r=1;
	RETURN_LONG(r);
		
}
PHP_FUNCTION(bartlby_notification_log_at_index) {
		zval * zbartlby_resource;
		zval * idx;
		struct shm_header * shm_hdr;
		bartlby_res * bres;
		/*
int notification_valid; //-1 invalid == end of list
	long worker_id; //Worker id
	long service_id; //Service_id
	int state; //State
	int aggregated; //Default -1 > 0 - this notification has already been aggregated
	char trigger_name[512];
	int type; // 0 if it was a normal notification, 1 = it was a escalation notification to the standby's
	time_t time;
	int aggregation_interval;
		*/

		if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &idx)==FAILURE) {
			WRONG_PARAM_COUNT;
		}

		ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
		convert_to_long(idx);


		if(Z_LVAL_P(idx) < 0 || Z_LVAL_P(idx) >= NOTIFICATION_LOG_MAX) {
			RETURN_FALSE;
		}
		if (array_init(return_value) == FAILURE) {
			RETURN_FALSE;
		}
		shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;

		add_assoc_long(return_value, "notification_valid", shm_hdr->notification_log[Z_LVAL_P(idx)].notification_valid);
		add_assoc_long(return_value, "worker_id", shm_hdr->notification_log[Z_LVAL_P(idx)].worker_id);
		add_assoc_long(return_value, "service_id", shm_hdr->notification_log[Z_LVAL_P(idx)].service_id);
		add_assoc_long(return_value, "state", shm_hdr->notification_log[Z_LVAL_P(idx)].state);
		add_assoc_long(return_value, "aggregated", shm_hdr->notification_log[Z_LVAL_P(idx)].aggregated);
		add_assoc_long(return_value, "type", shm_hdr->notification_log[Z_LVAL_P(idx)].type);
		add_assoc_long(return_value, "time", shm_hdr->notification_log[Z_LVAL_P(idx)].time);
		add_assoc_long(return_value, "aggregation_interval", shm_hdr->notification_log[Z_LVAL_P(idx)].aggregation_interval);
		add_assoc_long(return_value, "received_via", shm_hdr->notification_log[Z_LVAL_P(idx)].received_via);
		add_assoc_string(return_value, "trigger_name", shm_hdr->notification_log[Z_LVAL_P(idx)].trigger_name, 1);
		

}
PHP_FUNCTION(bartlby_get_core_extension_info) {
	//FIXME LOOK AT THIS if it could fit with RESOURCE
	void * SOHandle;
	char * dlmsg;
	zval * extensions_path;
	
	char * (*GetName)();
	char * (*GetAutor)();
	char * (*GetVersion)();
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &extensions_path)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(extensions_path);
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	SOHandle=dlopen(Z_STRVAL_P(extensions_path), RTLD_LAZY);
	if((dlmsg=dlerror()) != NULL) {
				php_error(E_ERROR, "DL Error: %s", dlmsg);
        	return;
   }	
   
  
  
  LOAD_SYMBOL(GetName,SOHandle, "GetName");
  LOAD_SYMBOL(GetAutor,SOHandle, "GetAutor");
  LOAD_SYMBOL(GetVersion,SOHandle, "GetVersion");
  
  add_assoc_string(return_value, "name", GetName(), 1);
  add_assoc_string(return_value, "autor", GetAutor(), 1);
  add_assoc_string(return_value, "version", GetVersion(), 1); 
	
	dlclose(SOHandle);
	
	
}


PHP_FUNCTION(bartlby_service_set_interval) {
	zval * zbartlby_resource;
	zval * bartlby_service_id;
	zval * bartlby_interval;
	zval * do_writeback;
	
	struct shm_header * shm_hdr;
	int r;
	char * dlmsg;
	
	struct service * svcmap;	
	int (*UpdateServiceInterval)(struct service *, char *);
	bartlby_res * bres;


	if (ZEND_NUM_ARGS() != 4 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzzz", &zbartlby_resource, &bartlby_service_id, &bartlby_interval, &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(bartlby_service_id);
	convert_to_long(bartlby_interval);
	convert_to_long(do_writeback);
	

	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
		
		
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
		php_error(E_WARNING, "Service id out of bounds");	
		RETURN_FALSE;	
	}
		
	svcmap[Z_LVAL_P(bartlby_service_id)].check_interval = Z_LVAL_P(bartlby_interval);	
	if(Z_LVAL_P(do_writeback) == 1) {
		LOAD_SYMBOL(UpdateServiceInterval,bres->SOHandle, "UpdateServiceInterval");
		UpdateServiceInterval(&svcmap[Z_LVAL_P(bartlby_service_id)], bres->cfgfile);
	}
	r=1;

	BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcmap[Z_LVAL_P(bartlby_service_id)].service_id ,BARTLBY_AUDIT_TYPE_SERVICE, "Changed Interval");
	RETURN_LONG(r);
		
}
 



PHP_FUNCTION(bartlby_toggle_service_active) {
	zval * zbartlby_resource;
	zval * bartlby_service_id;
	zval * do_writeback;
	
	struct shm_header * shm_hdr;
	int r;
	char * dlmsg;
	
	struct service * svcmap;	
	bartlby_res * bres;

	int (*UpdateService)(struct service *, char *);
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &bartlby_service_id, &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(bartlby_service_id);
	convert_to_long(do_writeback);
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
		
		
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
		php_error(E_WARNING, "Service id out of bounds");	
		RETURN_FALSE;	
	}
	if(svcmap[Z_LVAL_P(bartlby_service_id)].service_active == 1) {
		svcmap[Z_LVAL_P(bartlby_service_id)].service_active = 0;
		r=0;
	} else {
		svcmap[Z_LVAL_P(bartlby_service_id)].service_active = 1;	
		r=1;
	}
	if(Z_LVAL_P(do_writeback) == 1) {
		LOAD_SYMBOL(UpdateService,bres->SOHandle, "UpdateService");
		UpdateService(&svcmap[Z_LVAL_P(bartlby_service_id)], bres->cfgfile);
	}
	BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcmap[Z_LVAL_P(bartlby_service_id)].service_id ,BARTLBY_AUDIT_TYPE_SERVICE, "Toggled Service active state");	
	RETURN_LONG(r);
		
	
		
}



PHP_FUNCTION(bartlby_toggle_server_notify) {
	zval * zbartlby_resource;
	zval * bartlby_service_id;
	zval * do_writeback;
	
	struct shm_header * shm_hdr;
	int r;
	char * dlmsg;
	
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct server * srvmap;
		
	int (*ModifyServer)(struct server *, char *);
	
	bartlby_res * bres;


	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &bartlby_service_id, &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(bartlby_service_id);
	convert_to_long(do_writeback);
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
		
		
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->srvcount-1) {
		php_error(E_WARNING, "Server id out of bounds");	
		RETURN_FALSE;	
	}
	if(srvmap[Z_LVAL_P(bartlby_service_id)].server_notify == 1) {
		srvmap[Z_LVAL_P(bartlby_service_id)].server_notify = 0;
		r=0;
	} else {
		srvmap[Z_LVAL_P(bartlby_service_id)].server_notify = 1;	
		r=1;
	}
	if(Z_LVAL_P(do_writeback) == 1) {
		LOAD_SYMBOL(ModifyServer,bres->SOHandle, "ModifyServer");
		ModifyServer(&srvmap[Z_LVAL_P(bartlby_service_id)], bres->cfgfile);
	}
	
	BARTLBY_GENERIC_AUDIT(zbartlby_resource, srvmap[Z_LVAL_P(bartlby_service_id)].server_id ,BARTLBY_AUDIT_TYPE_SERVER, "Toggled Server notify state");	

	RETURN_LONG(r);
	
}



PHP_FUNCTION(bartlby_toggle_server_active) {
	zval * zbartlby_resource;
	zval * bartlby_service_id;
	zval * do_writeback;
	
	struct shm_header * shm_hdr;
	int r;
	char * dlmsg;
	
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct server * srvmap;
		
	int (*ModifyServer)(struct server *, char *);

	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &bartlby_service_id, &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(bartlby_service_id);
	convert_to_long(do_writeback);
	
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	
	
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->srvcount-1) {
		php_error(E_WARNING, "Server id out of bounds");	
		RETURN_FALSE;	
	}
	if(srvmap[Z_LVAL_P(bartlby_service_id)].server_enabled == 1) {
		srvmap[Z_LVAL_P(bartlby_service_id)].server_enabled = 0;
		r=0;
	} else {
		srvmap[Z_LVAL_P(bartlby_service_id)].server_enabled = 1;	
		r=1;
	}
	if(Z_LVAL_P(do_writeback) == 1) {
		LOAD_SYMBOL(ModifyServer,bres->SOHandle, "ModifyServer");
		ModifyServer(&srvmap[Z_LVAL_P(bartlby_service_id)], bres->cfgfile);
	}

	BARTLBY_GENERIC_AUDIT(zbartlby_resource, srvmap[Z_LVAL_P(bartlby_service_id)].server_id ,BARTLBY_AUDIT_TYPE_SERVER, "Toggled Server active state");	
		
	RETURN_LONG(r);
	
}

PHP_FUNCTION(bartlby_set_worker_state) {
	zval * zbartlby_resource;
	zval * bartlby_worker_id;
	zval * new_state;
	
	struct shm_header * shm_hdr;
	int r;
	struct worker * wrkmap;
	struct service * svcmap;
	char * dlmsg;
	int (*UpdateWorker)(struct worker *, char *);
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &bartlby_worker_id, &new_state)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(bartlby_worker_id);
	convert_to_long(new_state);
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	

	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
		
	
		
	if(Z_LVAL_P(bartlby_worker_id) > shm_hdr->wrkcount-1) {
		php_error(E_WARNING, "Worker id out of bounds");	
		RETURN_FALSE;	
	}
		
		
	r=1;
	wrkmap[Z_LVAL_P(bartlby_worker_id)].active=Z_LVAL_P(new_state);
	
	LOAD_SYMBOL(UpdateWorker,bres->SOHandle, "UpdateWorker");
	UpdateWorker(&wrkmap[Z_LVAL_P(bartlby_worker_id)], bres->cfgfile);


	BARTLBY_GENERIC_AUDIT(zbartlby_resource, wrkmap[Z_LVAL_P(bartlby_worker_id)].worker_id ,BARTLBY_AUDIT_TYPE_WORKER, "Changed Worker State");	

	RETURN_LONG(r);
			
}
PHP_FUNCTION(bartlby_toggle_service_handled) {
	zval * zbartlby_resource;
	zval * bartlby_service_id;
	zval * do_writeback;
	struct shm_header * shm_hdr;
	int r;
	struct service * svcmap; 
	char * dlmsg;
	int (*UpdateService)(struct service *, char *);
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &bartlby_service_id, &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(bartlby_service_id);
	convert_to_long(do_writeback);
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
		
		
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
		php_error(E_WARNING, "Service id out of bounds");	
		RETURN_FALSE;	
	}
	if(svcmap[Z_LVAL_P(bartlby_service_id)].handled == 1) {
		svcmap[Z_LVAL_P(bartlby_service_id)].handled = 0;
		r=0;
	} else {
		svcmap[Z_LVAL_P(bartlby_service_id)].handled = 1;	
		r=1;
	}
	if(Z_LVAL_P(do_writeback) == 1) {
		LOAD_SYMBOL(UpdateService,bres->SOHandle, "UpdateService");
		UpdateService(&svcmap[Z_LVAL_P(bartlby_service_id)], bres->cfgfile);
		
	}

	BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcmap[Z_LVAL_P(bartlby_service_id)].service_id ,BARTLBY_AUDIT_TYPE_SERVICE, "toggled HANDLE State");	
		
	RETURN_LONG(r);
		
}

PHP_FUNCTION(bartlby_toggle_service_notify) {
	zval * zbartlby_resource;
	zval * bartlby_service_id;
	zval * do_writeback;
	struct shm_header * shm_hdr;
	int r;
	struct service * svcmap; 
	char * dlmsg;
	int (*UpdateService)(struct service *, char *);
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &bartlby_service_id, &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(bartlby_service_id);
	convert_to_long(do_writeback);
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
		
		
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
		php_error(E_WARNING, "Service id out of bounds");	
		RETURN_FALSE;	
	}
	if(svcmap[Z_LVAL_P(bartlby_service_id)].notify_enabled == 1) {
		svcmap[Z_LVAL_P(bartlby_service_id)].notify_enabled = 0;
		r=0;
	} else {
		svcmap[Z_LVAL_P(bartlby_service_id)].notify_enabled = 1;	
		r=1;
	}
	if(Z_LVAL_P(do_writeback) == 1) {
		LOAD_SYMBOL(UpdateService,bres->SOHandle, "UpdateService");
		UpdateService(&svcmap[Z_LVAL_P(bartlby_service_id)], bres->cfgfile);
		
	}
	BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcmap[Z_LVAL_P(bartlby_service_id)].service_id ,BARTLBY_AUDIT_TYPE_SERVICE, "toggled Service Notification state");		
	RETURN_LONG(r);
		
}


//bartlby_check_force
PHP_FUNCTION(bartlby_check_force) {
	zval * zbartlby_resource;
	zval * bartlby_service_id;
	struct shm_header * shm_hdr;
	int r;
	struct service * svcmap; 
	bartlby_res * bres;

	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &bartlby_service_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(bartlby_service_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
		
		
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
		php_error(E_WARNING, "Service id out of bounds");	
		RETURN_FALSE;	
	}
	svcmap[Z_LVAL_P(bartlby_service_id)].do_force = 1;	
	r=1;
	BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcmap[Z_LVAL_P(bartlby_service_id)].service_id, BARTLBY_AUDIT_TYPE_SERVICE, "Forced a Check");
	RETURN_LONG(r);
		
}


int btl_is_array(zval * ar, int service_id) {
	HashTable *arr_hash;
	zval **data;
    HashPosition pointer;
    int array_count;
    char * string_key;
    int str_key_len;
    long num_key;
    	
    if(Z_TYPE_P(ar) != IS_ARRAY) {
    	return 1;
    }
	
	arr_hash = Z_ARRVAL_P(ar);
    array_count = zend_hash_num_elements(arr_hash);
    for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(arr_hash, &pointer)) {
		//convert_to_long(*data); FIXME
        if(Z_TYPE_PP(data) == IS_STRING) {
        	//printf("String: %s\n", Z_STRVAL_PP(data));	
        } else if(Z_TYPE_PP(data) == IS_LONG) {
            //printf("Long: %d\n", Z_LVAL_PP(data));	
           	if(Z_LVAL_PP(data) == service_id) {
        		return 1;
        	}
        }

            	
    }
    return -1;
}

PHP_FUNCTION(bartlby_bulk_force_services) {
	zval * zbartlby_resource;
	zval * bartlby_service_ids;
	zval * do_writeback;
	zval * state;
	struct shm_header * shm_hdr;
	int r;
	int x;
	struct service * svcmap; 
	
	bartlby_res * bres;

	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &bartlby_service_ids)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
		
	r=0;
	for(x=0; x<shm_hdr->svccount; x++) {
		if(btl_is_array(bartlby_service_ids, svcmap[x].service_id) == 1)	 {
			svcmap[x].do_force=1;
			BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcmap[x].service_id ,BARTLBY_AUDIT_TYPE_SERVICE, "Forced a check");		
			r++;
		}
	}
		
	RETURN_LONG(r);
	
	
}
PHP_FUNCTION(bartlby_bulk_service_notify) {


	zval * zbartlby_resource;
	zval * bartlby_service_ids;
	zval * do_writeback;
	zval * state;
	struct shm_header * shm_hdr;
	int r;
	int x;
	struct service * svcmap; 
	
	char * dlmsg;
	int (*UpdateService)(struct service *, char *);
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 4 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzzz", &zbartlby_resource, &bartlby_service_ids, &state,  &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(do_writeback);
	convert_to_long(state);
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
		
	r=0;
	for(x=0; x<shm_hdr->svccount; x++) {
		if(btl_is_array(bartlby_service_ids, svcmap[x].service_id) == 1)	 {
			svcmap[x].notify_enabled=Z_LVAL_P(state);
			if(Z_LVAL_P(do_writeback) == 1) {
				LOAD_SYMBOL(UpdateService,bres->SOHandle, "UpdateService");
				UpdateService(&svcmap[x], bres->cfgfile);
					
			}
			BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcmap[x].service_id ,BARTLBY_AUDIT_TYPE_SERVICE, "Toggled service notify state");		
			r++;
				
					
		}
	}
		
		
	RETURN_LONG(r);
	
}

PHP_FUNCTION(bartlby_bulk_service_active) {


	zval * zbartlby_resource;
	zval * bartlby_service_ids;
	zval * do_writeback;
	zval * state;
	struct shm_header * shm_hdr;
	int r;
	int x;
	struct service * svcmap; 
	
	char * dlmsg;
	int (*UpdateService)(struct service *, char *);
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 4 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzzz", &zbartlby_resource, &bartlby_service_ids, &state,  &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(do_writeback);
	convert_to_long(state);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	
		
	
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
		
	r=0;
	for(x=0; x<shm_hdr->svccount; x++) {
		if(btl_is_array(bartlby_service_ids, svcmap[x].service_id) == 1)	 {
			svcmap[x].service_active=Z_LVAL_P(state);
			if(Z_LVAL_P(do_writeback) == 1) {
				LOAD_SYMBOL(UpdateService,bres->SOHandle, "UpdateService");
				UpdateService(&svcmap[x], bres->cfgfile);
				
			}
			BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcmap[x].service_id ,BARTLBY_AUDIT_TYPE_SERVICE, "Toggled service active state");		
			r++;
			
				
		}
	}
		
		
		
		
	RETURN_LONG(r);
	
}
PHP_FUNCTION(bartlby_svc_map_test) {
	zval * subarray;
	zval * howmuch;
	void * bartlby_address;
	struct shm_header * shm_hdr;
	
	int x;
	int y;
	int is_down;
	int current_time;
	int current_state;
	
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct server * srvmap;
	struct btl_event * evntmap;
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	
	
	zval * zbartlby_resource;
	zval * svc_right_array;
	zval * server_right_array;
	
	zval * groups;
	zval * groupinfo;
	
	char * group_has_server;
	int is_member;
	int u;
	int z;
	int hh;
	
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &svc_right_array, &server_right_array)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	//convert_to_long(howmuch);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	bartlby_address=bres->bartlby_address;
	
	shm_hdr=(struct shm_header *)(void *)bartlby_address;
	svcmap=(struct service *)(void *)(bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
	svcgrpmap=(struct servicegroup *)(void *)&srvgrpmap[shm_hdr->srvgroupcount+1];
			
			
			
	current_time=time(NULL);
	for(hh=0; hh<300; hh++) {
	for(x=0; x<shm_hdr->svccount; x++) {
		if(btl_is_array(svc_right_array, svcmap[x].service_id) == -1 &&  btl_is_array(server_right_array, svcmap[x].server_id) == -1) {
			continue;	
		}
		ALLOC_INIT_ZVAL(subarray);
		array_init(subarray);
		add_assoc_long(subarray, "service_id", svcmap[x].service_id);
		add_assoc_long(subarray, "server_id", svcmap[x].server_id);
		add_assoc_long(subarray, "last_state", svcmap[x].last_state);
			
			
		if(svcmap[x].is_server_dead < 0) {
			add_assoc_long(subarray, "current_state", -3);
			add_assoc_long(subarray, "server_is_dead", 1);
		} else {
			add_assoc_long(subarray, "current_state", svcmap[x].current_state);
		}
			
			
		add_assoc_long(subarray, "client_port", srvmap[svcmap[x].srv_place].client_port);
				
		add_assoc_string(subarray, "new_server_text", svcmap[x].new_server_text, 1);
		add_assoc_string(subarray, "service_name", svcmap[x].service_name, 1);
		add_assoc_string(subarray, "server_name", srvmap[svcmap[x].srv_place].server_name, 1);
		add_assoc_string(subarray, "client_ip", srvmap[svcmap[x].srv_place].client_ip, 1);
		add_assoc_string(subarray, "server_ssh_keyfile", srvmap[svcmap[x].srv_place].server_ssh_keyfile, 1);
		add_assoc_string(subarray, "server_ssh_passphrase", srvmap[svcmap[x].srv_place].server_ssh_passphrase, 1);
		add_assoc_string(subarray, "server_ssh_username", srvmap[svcmap[x].srv_place].server_ssh_username, 1);
		add_assoc_string(subarray, "server_enabled_triggers", srvmap[svcmap[x].srv_place].enabled_triggers, 1);
		add_assoc_string(subarray, "plugin", svcmap[x].plugin, 1);
		add_assoc_string(subarray, "plugin_arguments", svcmap[x].plugin_arguments, 1);
	
		add_assoc_long(subarray, "check_interval", svcmap[x].check_interval);
		add_assoc_long(subarray, "check_interval_original", svcmap[x].check_interval_original);
		add_assoc_long(subarray, "last_check", svcmap[x].last_check);
		
		add_assoc_string(subarray, "exec_plan", svcmap[x].service_exec_plan, 1);
		
		add_assoc_long(subarray, "notify_enabled", svcmap[x].notify_enabled);
		add_assoc_long(subarray, "last_notify_send", svcmap[x].last_notify_send);
		add_assoc_long(subarray, "last_state_change", svcmap[x].last_state_change);
		add_assoc_long(subarray, "flap_count", svcmap[x].flap_count);
		
		
		add_assoc_long(subarray, "service_active", svcmap[x].service_active);
		add_assoc_long(subarray, "service_type", svcmap[x].service_type);
		add_assoc_long(subarray, "service_passive_timeout", svcmap[x].service_passive_timeout);
		
		
		add_assoc_string(subarray, "service_var", svcmap[x].service_var, 1);
		add_assoc_string(subarray, "server_icon", srvmap[svcmap[x].srv_place].server_icon, 1);
		add_assoc_long(subarray, "service_check_timeout", svcmap[x].service_check_timeout);
		add_assoc_long(subarray, "service_ack_enabled", svcmap[x].service_ack_enabled);
		add_assoc_long(subarray, "service_ack_current", svcmap[x].service_ack_current);
		add_assoc_long(subarray, "service_retain", svcmap[x].service_retain);
		add_assoc_long(subarray, "service_retain_current", svcmap[x].service_retain_current);
		add_assoc_long(subarray, "check_is_running", svcmap[x].process.pid);
		add_assoc_long(subarray, "check_starttime", svcmap[x].process.start_time);
		
		add_assoc_long(subarray, "shm_place", x);
		
		add_assoc_long(subarray, "service_time_sum", svcmap[x].pstat.sum);
		add_assoc_long(subarray, "service_time_count",svcmap[x].pstat.counter);
	
		add_assoc_long(subarray, "service_delay_sum", svcmap[x].delay_time.sum);
		add_assoc_long(subarray, "service_delay_count",svcmap[x].delay_time.counter);
		
		//bartlby_get_service_by_id
		
		add_assoc_string(subarray, "service_snmp_community", svcmap[x].snmp_info.community, 1);
		add_assoc_string(subarray, "service_snmp_textmatch", svcmap[x].snmp_info.textmatch, 1);
		add_assoc_string(subarray, "service_snmp_objid", svcmap[x].snmp_info.objid, 1);
		
		add_assoc_long(subarray, "service_snmp_warning",svcmap[x].snmp_info.warn);
		add_assoc_long(subarray, "service_snmp_critical",svcmap[x].snmp_info.crit);
		add_assoc_long(subarray, "service_snmp_version",svcmap[x].snmp_info.version);
		add_assoc_long(subarray, "service_snmp_type",svcmap[x].snmp_info.type);
		
		
		add_assoc_long(subarray, "flap_seconds",svcmap[x].flap_seconds);
		
		add_assoc_long(subarray, "server_flap_seconds",srvmap[svcmap[x].srv_place].server_flap_seconds);
		add_assoc_long(subarray, "server_last_notify_send",srvmap[svcmap[x].srv_place].last_notify_send);
		add_assoc_long(subarray, "server_notify",srvmap[svcmap[x].srv_place].server_notify);
		add_assoc_long(subarray, "server_enabled",srvmap[svcmap[x].srv_place].server_enabled);
		add_assoc_long(subarray, "server_dead",srvmap[svcmap[x].srv_place].server_dead);
		
		
		//svc.renotify_interval, svc.escalate_divisor
		add_assoc_long(subarray, "renotify_interval",svcmap[x].renotify_interval);
		add_assoc_long(subarray, "escalate_divisor",svcmap[x].escalate_divisor);
		add_assoc_long(subarray, "fires_events",svcmap[x].fires_events);
		
		add_assoc_string(subarray, "enabled_triggers", svcmap[x].enabled_triggers, 1);
		
		add_assoc_long(subarray, "server_gone",srvmap[svcmap[x].srv_place].is_gone);
		
		//svc.is_gone
		add_assoc_long(subarray, "is_gone",svcmap[x].is_gone);
		
		
		for(y=0; y<shm_hdr->dtcount; y++) {
			is_down=0;
			if(current_time >= dtmap[y].downtime_from && current_time <= dtmap[y].downtime_to) {
				switch(dtmap[y].downtime_type) {
					case DT_SERVICE:
						if(dtmap[y].service_id == svcmap[x].service_id) {
							is_down=1;	
							
						}
					break;
					case DT_SERVER:
						if(dtmap[y].service_id == svcmap[x].server_id) {
							is_down=2;	
						}
					break;	
					case DT_SERVERGROUP:
						
						if(srvmap[svcmap[x].srv_place].servergroup_counter > 0) {
							for(u=0; u<srvmap[svcmap[x].srv_place].servergroup_counter; u++){
								z=srvmap[svcmap[x].srv_place].servergroup_place[u];
								if(srvgrpmap[z].servergroup_id == dtmap[y].service_id) {
										is_down=3;
										break;
								}
									
							}
						}
						
					break;		
					case DT_SERVICEGROUP:
						if(svcmap[x].servicegroup_counter > 0) {
							for(u=0; u<svcmap[x].servicegroup_counter; u++){
									z=svcmap[x].servicegroup_place[u];
									if(svcgrpmap[z].servicegroup_id == dtmap[y].service_id) {
										is_down=4;
										break;
									}
							}
						}
				
					
					break;	
				}
				if(is_down > 0) {
					add_assoc_long(subarray, "is_downtime", 1);
					add_assoc_long(subarray, "downtime_from", dtmap[y].downtime_from);
					add_assoc_long(subarray, "downtime_to", dtmap[y].downtime_to);
					add_assoc_string(subarray, "downtime_notice", dtmap[y].downtime_notice, 1);
					add_assoc_long(subarray, "downtime_service", dtmap[y].service_id);
					add_assoc_long(subarray, "downtime_type", dtmap[y].downtime_type);
					break;
				}
			}	
		}
		if(is_down==0) {
			add_assoc_long(subarray, "is_downtime", 0);	
		}
		
				//Resolve ServerGroups
		
		
		

	
					if(srvmap[svcmap[x].srv_place].servergroup_counter > 0) {
						ALLOC_INIT_ZVAL(groups);
						array_init(groups);
						for(y=0; y<srvmap[svcmap[x].srv_place].servergroup_counter; y++){
					
					
								ALLOC_INIT_ZVAL(groupinfo);
								array_init(groupinfo);
								
								z=srvmap[svcmap[x].srv_place].servergroup_place[y];
								add_assoc_long(groupinfo,"servergroup_place", y);
								add_assoc_string(groupinfo,"servergroup_name", srvgrpmap[z].servergroup_name,1);
								add_assoc_string(groupinfo,"servergroup_members", srvgrpmap[z].servergroup_members,1);
								
								add_assoc_long(groupinfo,"servergroup_active", srvgrpmap[z].servergroup_active);
								add_assoc_long(groupinfo,"servergroup_notify", srvgrpmap[z].servergroup_notify);
								add_assoc_long(groupinfo,"servergroup_id", srvgrpmap[z].servergroup_id);
								add_assoc_long(groupinfo,"servergroup_dead", srvgrpmap[z].servergroup_dead);
								add_assoc_string(groupinfo,"enabled_triggers", srvgrpmap[z].enabled_triggers,1);
									
									
									
								add_next_index_zval(groups, groupinfo);
								
					
					}		
				
				
				
					add_assoc_zval(subarray, "servergroups", groups);
				}
					
			
			
	
			
			
			//Resolve Service Groups
			
			
				//is member of following groups #SERVICEGROUP


		
	
		
		if(svcmap[x].servicegroup_counter > 0) {
			ALLOC_INIT_ZVAL(groups);
			array_init(groups);
			for(y=0; y<svcmap[x].servicegroup_counter; y++){
					
					
					ALLOC_INIT_ZVAL(groupinfo);
					array_init(groupinfo);
					
					z=svcmap[x].servicegroup_place[y];
					add_assoc_long(groupinfo,"servicegroup_place", y);
					add_assoc_string(groupinfo,"servicegroup_name", svcgrpmap[z].servicegroup_name,1);
					add_assoc_string(groupinfo,"servicegroup_member", svcgrpmap[z].servicegroup_members,1);
					
					add_assoc_long(groupinfo,"servicegroup_active", svcgrpmap[z].servicegroup_active);
					add_assoc_long(groupinfo,"servicegroup_notify", svcgrpmap[z].servicegroup_notify);
					add_assoc_long(groupinfo,"servicegroup_id", svcgrpmap[z].servicegroup_id);
					add_assoc_long(groupinfo,"servicegroup_dead", svcgrpmap[z].servicegroup_dead);
					add_assoc_string(groupinfo,"enabled_triggers", svcgrpmap[z].enabled_triggers,1);
					
					add_next_index_zval(groups, groupinfo);
					
			}		
			
			
			
			add_assoc_zval(subarray, "servicegroups", groups);
		}
			
			
			//Push SVC to map
			add_next_index_zval(return_value, subarray);
			
		}

	}
		
		
		
		
		

	
	
		
}


PHP_FUNCTION(bartlby_encode) {
	zval * instr;
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &instr)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(instr);
	xbartlby_encode(Z_STRVAL_P(instr), strlen(Z_STRVAL_P(instr)));
	
	RETURN_STRING(Z_STRVAL_P(instr),1);
	
	
}

PHP_FUNCTION(bartlby_decode) {
	zval * instr;
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &instr)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(instr);
	xbartlby_decode(Z_STRVAL_P(instr), strlen(Z_STRVAL_P(instr)));
	
	RETURN_STRING(Z_STRVAL_P(instr),1);	
}
PHP_FUNCTION(bartlby_shm_destroy) {
	//FIXME RESOURCE?
	char * shmtok;
	int shm_id;
	void * bartlby_address;
	struct shm_header * shm_hdr;
	struct shmid_ds shm_desc;
	
	
	zval * bartlby_config;
	
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &bartlby_config)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	
	convert_to_string(bartlby_config);
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shmtok = getConfigValue("shm_key", Z_STRVAL_P(bartlby_config));
	if(shmtok == NULL) {
		RETURN_FALSE;
	}		
	
	
	shm_id = shmget(ftok(shmtok, 32), 0,  0777);
	
	shmctl(shm_id, IPC_RMID, &shm_desc);
	
	
	free(shmtok);
	RETURN_TRUE;	
}

PHP_FUNCTION(bartlby_reload) {
	void * bartlby_address;
	struct shm_header * shm_hdr;
	
	
	
	zval * zbartlby_resource;
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	shm_hdr->do_reload=1;
		
					
		
		
	RETURN_TRUE;	
}
PHP_FUNCTION(bartlby_add_worker) {
	zval * zbartlby_resource;
	zval * mail;
	zval * icq;
	zval * selected_services;
	zval * visible_services;

	zval * selected_servers;
	zval * visible_servers;
	
	zval * notify_levels;
	zval * active;
	zval * name;
	zval * password;
	zval * enabled_triggers;
	zval * escalation_limit;
	zval * escalation_minutes;
	zval * notify_plan;
	zval * orch_id;

	zval * api_pubkey;
	zval * api_privkey;
	zval * api_enabled;

	zval * is_super_user;
	zval * notification_aggregation_interval;
	
	zval ** temp_pp;
	zval * options_array;
	
	char * dlmsg;
	int ret;
	int (*AddWorker)(struct worker *,char *);
	struct worker svc;
	bartlby_res * bres;

	
	if(ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}

	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	GETARRAY_EL_FROM_HASH(name, "name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"default");
	GETARRAY_EL_FROM_HASH(enabled_triggers, "enabled_triggers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(mail, "mail", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(icq, "icq", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(password, "password", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"default");
	GETARRAY_EL_FROM_HASH(selected_services, "selected_services", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(selected_servers, "selected_servers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");

	GETARRAY_EL_FROM_HASH(visible_services, "visible_services", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(visible_servers, "visible_servers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");


	GETARRAY_EL_FROM_HASH(notify_levels, "notify_levels", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	
	GETARRAY_EL_FROM_HASH(notify_plan, "notify_plan", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	
	GETARRAY_EL_FROM_HASH(active, "active", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,1);
	GETARRAY_EL_FROM_HASH(escalation_limit, "escalation_limit", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(escalation_minutes, "escalation_minutes", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	
	GETARRAY_EL_FROM_HASH(is_super_user, "is_super_user", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(notification_aggregation_interval, "notification_aggregation_interval", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	

	GETARRAY_EL_FROM_HASH(api_pubkey, "api_pubkey", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(api_privkey, "api_privkey", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(api_enabled, "api_enabled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);



	convert_to_string(enabled_triggers);
	convert_to_string(mail);
	convert_to_string(icq);
	convert_to_string(password);
	convert_to_string(selected_services);
	convert_to_string(selected_servers);
	convert_to_string(visible_services);
	convert_to_string(visible_servers);
	convert_to_string(notify_levels);
	convert_to_string(name);
	convert_to_string(notify_plan);
	convert_to_long(active);
	convert_to_long(escalation_limit);
	convert_to_long(escalation_minutes);
	convert_to_long(notification_aggregation_interval);
	convert_to_long(is_super_user);
	convert_to_long(orch_id);
	convert_to_long(api_enabled);

	convert_to_string(api_pubkey);
	convert_to_string(api_privkey);
	
	LOAD_SYMBOL(AddWorker,bres->SOHandle, "AddWorker");
	
	strcpy(svc.password, Z_STRVAL_P(password));
	strcpy(svc.name, Z_STRVAL_P(name));
	strcpy(svc.notify_plan, Z_STRVAL_P(notify_plan));
	strcpy(svc.mail, Z_STRVAL_P(mail));
	strcpy(svc.icq, Z_STRVAL_P(icq));
	strcpy(svc.selected_services, Z_STRVAL_P(selected_services));
	strcpy(svc.selected_servers, Z_STRVAL_P(selected_servers));
	strcpy(svc.visible_servers, Z_STRVAL_P(visible_servers));
	strcpy(svc.visible_services, Z_STRVAL_P(visible_services));
	strcpy(svc.notify_levels, Z_STRVAL_P(notify_levels));
	strcpy(svc.enabled_triggers, Z_STRVAL_P(enabled_triggers));
	strcpy(svc.api_pubkey, Z_STRVAL_P(api_pubkey));
	strcpy(svc.api_privkey, Z_STRVAL_P(api_privkey));
	svc.active=Z_LVAL_P(active);
	svc.escalation_limit=Z_LVAL_P(escalation_limit);
	svc.escalation_minutes=Z_LVAL_P(escalation_minutes);
	
	svc.is_super_user=Z_LVAL_P(is_super_user);
	svc.notification_aggregation_interval=Z_LVAL_P(notification_aggregation_interval);
	svc.orch_id=Z_LVAL_P(orch_id);
	svc.api_enabled=Z_LVAL_P(api_enabled);
	ret=AddWorker(&svc, bres->cfgfile);

	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,ret, BARTLBY_WORKER_GONE, BARTLBY_OBJECT_ADDED);

	RETURN_LONG(ret);	
}

PHP_FUNCTION(bartlby_delete_worker) {
	zval * zbartlby_resource;
	zval * worker_id;
	
	
	char * dlmsg;
	int ret;
	int (*DeleteWorker)(int, char*);
	bartlby_res * bres;

	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&worker_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(worker_id);
	
	
	
	LOAD_SYMBOL(DeleteWorker,bres->SOHandle, "DeleteWorker");
	

	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(worker_id), BARTLBY_WORKER_GONE, BARTLBY_OBJECT_DELETED);

	ret=DeleteWorker(Z_LVAL_P(worker_id),bres->cfgfile);
	

	RETURN_LONG(ret);	
}

PHP_FUNCTION(bartlby_modify_worker) {
	zval * zbartlby_resource;
	zval * mail;
	zval * icq;
	zval * visible_services;
	zval * visible_servers;
	zval * selected_services;
	zval * selected_servers;
	zval * notify_levels;
	zval * active;
	zval * name;
	zval * worker_id;
	zval * password;
	zval * enabled_triggers;
	zval * escalation_limit;
	zval * escalation_minutes;
	zval * notify_plan;
	zval * orch_id;
	zval * is_super_user;
	zval * notification_aggregation_interval;

	zval * api_pubkey;
	zval * api_privkey;

	zval * api_enabled;

	zval ** temp_pp;
	zval * options_array;
	
	
	char * dlmsg;
	int ret;
	int (*UpdateWorker)(struct worker *,char *);
	struct worker svc;
	
	bartlby_res * bres;

	if(ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&worker_id, &options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}
	
	GETARRAY_EL_FROM_HASH(name, "name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"default");
	GETARRAY_EL_FROM_HASH(enabled_triggers, "enabled_triggers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(mail, "mail", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(icq, "icq", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(password, "password", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"default");
	GETARRAY_EL_FROM_HASH(selected_services, "selected_services", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(selected_servers, "selected_servers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(visible_services, "visible_services", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(visible_servers, "visible_servers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(notify_levels, "notify_levels", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(notify_plan, "notify_plan", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(active, "active", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,1);
	GETARRAY_EL_FROM_HASH(escalation_limit, "escalation_limit", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(escalation_minutes, "escalation_minutes", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(is_super_user, "is_super_user", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(notification_aggregation_interval, "notification_aggregation_interval", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
		
	GETARRAY_EL_FROM_HASH(api_pubkey, "api_pubkey", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(api_privkey, "api_privkey", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING,"");
	GETARRAY_EL_FROM_HASH(api_enabled, "api_enabled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);

	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
		
	
	convert_to_string(enabled_triggers);
	convert_to_string(mail);
	convert_to_string(password);
	convert_to_string(icq);
	convert_to_string(visible_servers);
	convert_to_string(visible_services);
	convert_to_string(selected_servers);
	convert_to_string(selected_services);

	convert_to_string(notify_levels);
	convert_to_string(name);
	convert_to_long(active);
	convert_to_string(notify_plan);
	
	convert_to_long(worker_id);
	convert_to_long(escalation_limit);
	convert_to_long(escalation_minutes);
	convert_to_long(orch_id);
	convert_to_long(notification_aggregation_interval);
	convert_to_long(is_super_user);
	convert_to_long(api_enabled);

	convert_to_string(api_pubkey);
	convert_to_string(api_privkey);
		
	LOAD_SYMBOL(UpdateWorker,bres->SOHandle, "UpdateWorker");
	
	strcpy(svc.notify_plan, Z_STRVAL_P(notify_plan));
	strcpy(svc.password, Z_STRVAL_P(password));
	strcpy(svc.name, Z_STRVAL_P(name));
	strcpy(svc.mail, Z_STRVAL_P(mail));
	strcpy(svc.icq, Z_STRVAL_P(icq));
	strcpy(svc.selected_services, Z_STRVAL_P(selected_services));
	strcpy(svc.selected_servers, Z_STRVAL_P(selected_servers));
	strcpy(svc.visible_servers, Z_STRVAL_P(visible_servers));
	strcpy(svc.visible_services, Z_STRVAL_P(visible_services));

	strcpy(svc.notify_levels, Z_STRVAL_P(notify_levels));
	strcpy(svc.enabled_triggers, Z_STRVAL_P(enabled_triggers));

	strcpy(svc.api_pubkey, Z_STRVAL_P(api_pubkey));
	strcpy(svc.api_privkey, Z_STRVAL_P(api_privkey));

	svc.active=Z_LVAL_P(active);
	svc.worker_id=Z_LVAL_P(worker_id);
	svc.api_enabled=Z_LVAL_P(api_enabled);
	
	svc.escalation_limit=Z_LVAL_P(escalation_limit);
	svc.escalation_minutes=Z_LVAL_P(escalation_minutes);
	
	svc.is_super_user=Z_LVAL_P(is_super_user);
	svc.orch_id=Z_LVAL_P(orch_id);
	svc.notification_aggregation_interval=Z_LVAL_P(notification_aggregation_interval);

	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(worker_id), BARTLBY_WORKER_GONE, BARTLBY_OBJECT_CHANGED);


	ret=UpdateWorker(&svc, bres->cfgfile);
	
	
	
	RETURN_LONG(ret);		
}

PHP_FUNCTION(bartlby_get_worker_by_id) {
	zval * zbartlby_resource;
	zval * worker_id;
	
	char * dlmsg;
	int ret;
	int (*GetWorkerById)(int,struct worker *, char *);
	struct worker  svc;
	
	bartlby_res * bres;

	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&worker_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(worker_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	


	LOAD_SYMBOL(GetWorkerById,bres->SOHandle, "GetWorkerById");
	ret=GetWorkerById(Z_LVAL_P(worker_id),&svc, bres->cfgfile);
	
	if(ret < 0) {
		RETURN_FALSE;	
	} else {
		if (array_init(return_value) == FAILURE) {
			RETURN_FALSE;
		}
		add_assoc_string(return_value, "mail", svc.mail, 1);
		add_assoc_string(return_value, "icq", svc.icq, 1);
		add_assoc_string(return_value, "notify_plan", svc.notify_plan, 1);
		add_assoc_string(return_value, "selected_services", svc.selected_services, 1);
		add_assoc_string(return_value, "selected_servers", svc.selected_servers, 1);
		add_assoc_string(return_value, "visible_servers", svc.visible_servers, 1);
		add_assoc_string(return_value, "visible_services", svc.visible_services, 1);


		add_assoc_string(return_value, "notify_levels", svc.notify_levels,1);
		add_assoc_string(return_value, "name", svc.name,1);
		add_assoc_string(return_value, "password", svc.password,1);
		add_assoc_string(return_value, "enabled_triggers", svc.enabled_triggers,1);

		add_assoc_string(return_value, "api_pubkey", svc.api_pubkey,1);
		add_assoc_string(return_value, "api_privkey", svc.api_privkey,1);
		add_assoc_long(return_value, "api_enabled", svc.api_enabled);

		add_assoc_long(return_value, "worker_id", svc.worker_id);
		add_assoc_long(return_value, "active", svc.active);
		add_assoc_long(return_value, "escalation_limit", svc.escalation_limit);
		add_assoc_long(return_value, "escalation_minutes", svc.escalation_minutes);
		add_assoc_long(return_value, "is_super_user", svc.is_super_user);
		add_assoc_long(return_value, "notification_aggregation_interval", svc.notification_aggregation_interval);
		add_assoc_long(return_value, "orch_id", svc.orch_id);
	}
		
}

PHP_FUNCTION(bartlby_config) {
	zval * bartlby_config;
	zval * bartlby_key;
	char * k;
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &bartlby_config, &bartlby_key)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	
	convert_to_string(bartlby_config);	
	convert_to_string(bartlby_key);	
	
	k=getConfigValue(Z_STRVAL_P(bartlby_key), Z_STRVAL_P(bartlby_config));
	if(k == NULL) {
		
		RETURN_FALSE;
	} else {
		RETURN_STRING(k,1);	
	}
	
	
}

PHP_FUNCTION(bartlby_get_info) {
	
	struct shm_header * shm_hdr;
	zval * zbartlby_resource;
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	
	add_assoc_long(return_value, "services", shm_hdr->svccount);
	add_assoc_long(return_value, "workers", shm_hdr->wrkcount);
	add_assoc_long(return_value, "server", shm_hdr->srvcount);
	add_assoc_long(return_value, "downtimes", shm_hdr->dtcount);
	add_assoc_long(return_value, "servergroups", shm_hdr->srvgroupcount);
	add_assoc_long(return_value, "servicegroups", shm_hdr->svcgroupcount);

	add_assoc_long(return_value, "current_running", shm_hdr->current_running);
	add_assoc_long(return_value, "do_reload", shm_hdr->do_reload);
	add_assoc_string(return_value, "version", shm_hdr->version, 1);
	add_assoc_long(return_value, "last_replication", shm_hdr->last_replication);
	add_assoc_long(return_value, "startup_time", shm_hdr->startup_time);
	add_assoc_long(return_value, "downtimes", shm_hdr->dtcount);
	add_assoc_long(return_value, "sirene_mode", shm_hdr->sirene_mode);
	add_assoc_long(return_value, "round_time_sum", shm_hdr->pstat.sum);
	add_assoc_long(return_value, "round_time_count", shm_hdr->pstat.counter);
	add_assoc_long(return_value, "checks_performed", shm_hdr->checks_performed);
	add_assoc_long(return_value, "checks_performed_time", shm_hdr->checks_performed_time);
	

	
}

PHP_FUNCTION(bartlby_delete_server) {
	
	zval * zbartlby_resource;
	zval * server_id;
	
	char * dlmsg;
	int ret;
	int (*DeleteServer)(int, char*);
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&server_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(server_id);
	
	
	
	LOAD_SYMBOL(DeleteServer,bres->SOHandle, "DeleteServer");
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(server_id), BARTLBY_SERVER_GONE, BARTLBY_OBJECT_DELETED);

	ret=DeleteServer(Z_LVAL_P(server_id),bres->cfgfile);
	

	RETURN_LONG(ret);
}

PHP_FUNCTION(bartlby_get_service_by_id) {
	zval * zbartlby_resource;
	zval * service_id;
	
	char * dlmsg;
	
	int ret;
	bartlby_res * bres;
	
	int (*GetServiceById)(int,struct service *, char *);
	
	struct service  svc;
	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&service_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(service_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	

	LOAD_SYMBOL(GetServiceById,bres->SOHandle, "GetServiceById");
	
	
	
	
	ret=GetServiceById(Z_LVAL_P(service_id),&svc, bres->cfgfile );
	if(ret < 0) {
		RETURN_FALSE;	
	} else {
		
		if (array_init(return_value) == FAILURE) {
			RETURN_FALSE;
		}
		add_assoc_long(return_value, "service_id", svc.service_id);
		add_assoc_long(return_value, "server_id", svc.server_id);
		add_assoc_long(return_value, "last_state", svc.last_state);
		add_assoc_long(return_value, "current_state", svc.current_state);

				
		add_assoc_string(return_value, "new_server_text", svc.new_server_text, 1);
		add_assoc_string(return_value, "service_name", svc.service_name, 1);

		add_assoc_string(return_value, "plugin", svc.plugin, 1);
		add_assoc_string(return_value, "plugin_arguments", svc.plugin_arguments, 1);
		
		add_assoc_long(return_value, "check_interval", svc.check_interval);
		add_assoc_long(return_value, "check_interval_original", svc.check_interval_original);
		add_assoc_long(return_value, "last_check", svc.last_check);
		
		add_assoc_string(return_value, "server_name", svc.srv->server_name, 1);
		add_assoc_string(return_value, "client_ip", svc.srv->client_ip, 1);
		add_assoc_string(return_value, "server_ssh_keyfile", svc.srv->server_ssh_keyfile, 1);
		add_assoc_string(return_value, "server_ssh_passphrase", svc.srv->server_ssh_passphrase, 1);
		add_assoc_string(return_value, "server_ssh_username", svc.srv->server_ssh_username, 1);
		add_assoc_string(return_value, "server_enabled_triggers", svc.srv->enabled_triggers, 1);
		add_assoc_string(return_value, "server_icon", svc.srv->server_icon, 1);
		add_assoc_long(return_value, "client_port", svc.srv->client_port);
		
		add_assoc_string(return_value, "exec_plan", svc.service_exec_plan, 1);
		
		add_assoc_long(return_value, "notify_enabled", svc.notify_enabled);
		add_assoc_long(return_value, "last_notify_send", svc.last_notify_send);
		add_assoc_long(return_value, "last_state_change", svc.last_state_change);
		add_assoc_long(return_value, "flap_count", svc.flap_count);
		
		add_assoc_long(return_value, "service_active", svc.service_active);
		add_assoc_long(return_value, "service_type", svc.service_type);
		add_assoc_long(return_value, "service_passive_timeout", svc.service_passive_timeout);
		add_assoc_long(return_value, "service_check_timeout", svc.service_check_timeout);
		add_assoc_long(return_value, "service_ack_enabled", svc.service_ack_enabled);
		add_assoc_long(return_value, "service_ack_current", svc.service_ack_current);
		add_assoc_long(return_value, "service_retain", svc.service_retain);
		
		
		add_assoc_string(return_value, "service_var", svc.service_var, 1);

		
		add_assoc_string(return_value, "snmp_community", svc.snmp_info.community, 1);
		add_assoc_string(return_value, "snmp_textmatch", svc.snmp_info.textmatch, 1);
		add_assoc_string(return_value, "snmp_objid", svc.snmp_info.objid, 1);
		
		add_assoc_long(return_value, "snmp_warning",svc.snmp_info.warn);
		add_assoc_long(return_value, "snmp_critical",svc.snmp_info.crit);
		add_assoc_long(return_value, "snmp_version",svc.snmp_info.version);
		add_assoc_long(return_value, "snmp_type",svc.snmp_info.type);
		
		add_assoc_long(return_value, "flap_seconds",svc.flap_seconds);
		add_assoc_long(return_value, "handled",svc.handled);
		
		
		//svc.renotify_interval, svc.escalate_divisor
		add_assoc_long(return_value, "renotify_interval",svc.renotify_interval);
		add_assoc_long(return_value, "escalate_divisor",svc.escalate_divisor);
		add_assoc_long(return_value, "fires_events",svc.fires_events);
		add_assoc_long(return_value, "orch_id",svc.orch_id);
		add_assoc_long(return_value, "notify_last_state",svc.notify_last_state);
		
		add_assoc_string(return_value, "enabled_triggers", svc.enabled_triggers, 1);
		

		add_assoc_long(return_value, "prio",svc.prio);
		add_assoc_long(return_value, "notify_super_users",svc.notify_super_users);
		add_assoc_string(return_value, "usid",svc.usid, 1);
			
	}
	free(svc.srv); //gets allocated in the lib
	
}

PHP_FUNCTION(bartlby_delete_service) {
	zval * zbartlby_resource;
	zval * service_id;
	
	char * dlmsg;
	int ret;
	int (*DeleteService)(int, char*);
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &service_id) ==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(service_id);
	
	LOAD_SYMBOL(DeleteService,bres->SOHandle, "DeleteService");
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(service_id), BARTLBY_SERVICE_GONE, BARTLBY_OBJECT_DELETED);
	ret=DeleteService(Z_LVAL_P(service_id),bres->cfgfile);
	RETURN_LONG(ret);
	
}

PHP_FUNCTION(bartlby_modify_service) {
	struct service svc;
	char * dlmsg;
	
	int ret;
	int rtc;
	
	int (*UpdateService)(struct service *, char *);
	
	zval *service_id, * server_id , * zbartlby_resource, * plugin, * service_name , * plugin_arguments, * notify_enabled, *exec_plan, *check_interval, *service_type, *service_passive_timeout, *service_var, *service_check_timeout, * service_ack_enabled, * service_retain;
	zval * snmp_community, * snmp_objid, *snmp_version, *snmp_warning, *snmp_critical, *snmp_type;
	zval * snmp_textmatch;
	zval * service_active;
	zval * flap_seconds;
	zval * renotify_interval;
	zval * escalate_divisor;
	zval * fires_events;
	zval * handled;
	zval * enabled_triggers;
	zval * orch_id;
	zval * prio;
	zval * notify_super_users;
	zval * usid;
	zval ** temp_pp;
	zval * options_array;	

	bartlby_res * bres;
	if(ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&service_id,  &options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}
	
	GETARRAY_EL_FROM_HASH(plugin, "plugin", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "init");
	GETARRAY_EL_FROM_HASH(service_name, "service_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "DEFAULT_NAME");
	GETARRAY_EL_FROM_HASH(plugin_arguments, "plugin_arguments", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(notify_enabled, "notify_enabled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(check_interval, "check_interval", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 120);
	GETARRAY_EL_FROM_HASH(service_type, "service_type", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);	
	GETARRAY_EL_FROM_HASH(service_passive_timeout, "service_passive_timeout", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 240);
	GETARRAY_EL_FROM_HASH(server_id, "server_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_ERROR, 0);
	GETARRAY_EL_FROM_HASH(service_check_timeout, "service_check_timeout", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 240);
	GETARRAY_EL_FROM_HASH(service_var, "service_var", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(exec_plan, "exec_plan", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(service_ack_enabled, "service_ack_enabled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(service_retain, "service_retain", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 5);
	GETARRAY_EL_FROM_HASH(service_active, "service_active", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(flap_seconds, "flap_seconds", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 120);
	GETARRAY_EL_FROM_HASH(escalate_divisor, "escalate_divisor", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(renotify_interval, "renotify_interval", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(fires_events, "fires_events", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(enabled_triggers, "enabled_triggers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(snmp_community, "snmp_community", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(snmp_textmatch, "snmp_textmatch", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(snmp_objid, "snmp_objid", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(snmp_version, "snmp_version", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(snmp_warning, "snmp_warning", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(snmp_critical, "snmp_critical", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(snmp_type, "snmp_type", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	
	GETARRAY_EL_FROM_HASH(handled, "handled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);


	GETARRAY_EL_FROM_HASH(prio, "prio", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 50);
	GETARRAY_EL_FROM_HASH(notify_super_users, "notify_super_users", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(usid, "usid", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	


	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	

	convert_to_long(prio);
	convert_to_long(notify_super_users);
	convert_to_string(usid);
	
	convert_to_long(service_id);
	convert_to_string(plugin);
	convert_to_string(service_name);
	convert_to_string(plugin_arguments);
	convert_to_long(notify_enabled);
	convert_to_long(server_id);
	convert_to_long(orch_id);
	convert_to_long(service_check_timeout);
	convert_to_string(enabled_triggers);
		
	convert_to_long(check_interval);
	convert_to_long(service_type);
	convert_to_long(service_passive_timeout);
	convert_to_string(service_var);
	convert_to_long(service_ack_enabled);
	convert_to_long(service_retain);
	
	
	convert_to_string(snmp_community);
	convert_to_string(snmp_textmatch);
	convert_to_string(snmp_objid);
	convert_to_long(snmp_version);
	convert_to_long(snmp_warning);
	convert_to_long(snmp_critical);
	convert_to_long(snmp_type);
	convert_to_long(service_active);
	
	convert_to_long(flap_seconds);
	
	convert_to_long(renotify_interval);
	convert_to_long(escalate_divisor);
	convert_to_long(fires_events);
	
	convert_to_string(exec_plan);
	convert_to_long(handled);
	


	svc.prio=Z_LVAL_P(prio);
	svc.notify_super_users=Z_LVAL_P(notify_super_users);
	sprintf(svc.usid, "%s", Z_STRVAL_P(usid));

	svc.service_id=Z_LVAL_P(service_id);
	sprintf(svc.plugin, "%s", Z_STRVAL_P(plugin));
	sprintf(svc.service_name, "%s", Z_STRVAL_P(service_name));
	sprintf(svc.plugin_arguments, "%s", Z_STRVAL_P(plugin_arguments));
	
	sprintf(svc.service_exec_plan, "%s", Z_STRVAL_P(exec_plan));
	
	svc.notify_enabled=Z_LVAL_P(notify_enabled);
	
	
	svc.server_id=Z_LVAL_P(server_id);
	svc.service_ack_enabled=Z_LVAL_P(service_ack_enabled);
	svc.service_retain=Z_LVAL_P(service_retain);
	
	svc.service_check_timeout=Z_LVAL_P(service_check_timeout);
	
	svc.check_interval=Z_LVAL_P(check_interval);
	svc.service_type=Z_LVAL_P(service_type);
	svc.service_passive_timeout=Z_LVAL_P(service_passive_timeout);
	sprintf(svc.service_var, "%s", Z_STRVAL_P(service_var));
	
	sprintf(svc.snmp_info.community, "%s", Z_STRVAL_P(snmp_community));
	sprintf(svc.snmp_info.textmatch, "%s", Z_STRVAL_P(snmp_textmatch));
	sprintf(svc.snmp_info.objid, "%s", Z_STRVAL_P(snmp_objid));
	svc.snmp_info.version=Z_LVAL_P(snmp_version);
	svc.snmp_info.warn=Z_LVAL_P(snmp_warning);
	svc.snmp_info.crit=Z_LVAL_P(snmp_critical);
	svc.snmp_info.type=Z_LVAL_P(snmp_type);
	svc.orch_id=Z_LVAL_P(orch_id);
	svc.service_active=Z_LVAL_P(service_active);
	svc.flap_seconds=Z_LVAL_P(flap_seconds);
	
	svc.renotify_interval=Z_LVAL_P(renotify_interval);
	svc.escalate_divisor=Z_LVAL_P(escalate_divisor);
	svc.fires_events=Z_LVAL_P(fires_events);
	svc.handled=Z_LVAL_P(handled);
	sprintf(svc.enabled_triggers, "%s", Z_STRVAL_P(enabled_triggers));
	
	
	
	LOAD_SYMBOL(UpdateService,bres->SOHandle, "UpdateService");
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(service_id), BARTLBY_SERVICE_GONE, BARTLBY_OBJECT_CHANGED);
	rtc=UpdateService(&svc, bres->cfgfile);
	

	RETURN_LONG(rtc);
	
	
	
	
		
	
}


PHP_FUNCTION(bartlby_add_service_array) {
	//THIS IS A DEBUG ONLY FUNCTION
	zval * zbartlby_resource;
	zval * options_array;
	
	zval ** temp_pp;
	zval * service_name;
	zval * service_port;
	zval * op1;
	
	bartlby_res * bres;
	
	if(ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	GETARRAY_EL_FROM_HASH(service_name, "service_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_string");
	GETARRAY_EL_FROM_HASH(service_port, "service_port", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1234);
	GETARRAY_EL_FROM_HASH(op1, "op1", temp_pp, options_array,BARTLBY_FIELD_REQUIRED, BARTLBY_DEFAULT_LONG, 123);
	
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	convert_to_string(service_name);
	convert_to_long(service_port);
	
 	php_printf("service_name: %s\n", Z_STRVAL_P(service_name));
 	php_printf("service_port: %ld\n", Z_LVAL_P(service_port));
 
 
  object_init(return_value);

    // add a couple of properties
  zend_update_property_string(NULL, return_value, "name", strlen("name"), "yig" TSRMLS_CC);
  zend_update_property_long(NULL, return_value, "worshippers", strlen("worshippers"), 4 TSRMLS_CC);

	
	
	
	
}
PHP_FUNCTION(bartlby_add_service) {
	struct service svc;
	char * dlmsg;
	int ret;
	int rtc;
	
	int (*AddService)(struct service *, char *);
	
	zval * server_id , * zbartlby_resource, * plugin, * service_name , * plugin_arguments, * notify_enabled, *exec_plan, *check_interval, *service_type, *service_passive_timeout, *service_var, * service_check_timeout, * service_ack_enabled, * service_retain;
	zval * snmp_community, * snmp_objid, *snmp_version, *snmp_warning, *snmp_critical, *snmp_type;
	zval * snmp_textmatch;
	zval * service_active;
	zval * flap_seconds;
	zval * renotify_interval;
	zval * escalate_divisor;
	zval * fires_events;
	zval * enabled_triggers;
	zval * handled;
	zval * orch_id;
	zval * usid;
	zval * prio;
	zval * notify_super_users;

	zval ** temp_pp;
	zval * options_array;
	
	bartlby_res * bres;

	
	if(ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	//zend_hash_find(HASH_P(options), "fsync", strlen("fsync") + 1, (void**) &fsync_pp)
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}
	
	GETARRAY_EL_FROM_HASH(plugin, "plugin", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "init");
	GETARRAY_EL_FROM_HASH(service_name, "service_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "DEFAULT_NAME");
	GETARRAY_EL_FROM_HASH(plugin_arguments, "plugin_arguments", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(notify_enabled, "notify_enabled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(check_interval, "check_interval", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 120);
	GETARRAY_EL_FROM_HASH(service_type, "service_type", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);	
	GETARRAY_EL_FROM_HASH(service_passive_timeout, "service_passive_timeout", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 240);
	GETARRAY_EL_FROM_HASH(server_id, "server_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_ERROR, 0);
	GETARRAY_EL_FROM_HASH(service_check_timeout, "service_check_timeout", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 240);
	GETARRAY_EL_FROM_HASH(service_var, "service_var", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(exec_plan, "exec_plan", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(service_ack_enabled, "service_ack_enabled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(service_retain, "service_retain", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 5);
	GETARRAY_EL_FROM_HASH(service_active, "service_active", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(flap_seconds, "flap_seconds", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 120);
	GETARRAY_EL_FROM_HASH(escalate_divisor, "escalate_divisor", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(renotify_interval, "renotify_interval", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(fires_events, "fires_events", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(enabled_triggers, "enabled_triggers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(snmp_community, "snmp_community", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(snmp_textmatch, "snmp_textmatch", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(snmp_objid, "snmp_objid", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(snmp_version, "snmp_version", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(snmp_warning, "snmp_warning", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(snmp_critical, "snmp_critical", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(snmp_type, "snmp_type", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(handled, "handled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);

	GETARRAY_EL_FROM_HASH(prio, "prio", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 50);
	GETARRAY_EL_FROM_HASH(notify_super_users, "notify_super_users", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(usid, "usid", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	convert_to_string(plugin);
	convert_to_string(service_name);
	convert_to_string(plugin_arguments);
	convert_to_long(notify_enabled);
	convert_to_long(orch_id);
	
	convert_to_long(check_interval);
	convert_to_long(service_type);
	convert_to_long(service_passive_timeout);
	convert_to_long(server_id);
	convert_to_long(service_check_timeout);
	convert_to_string(service_var);
	convert_to_string(exec_plan);
	convert_to_long(service_ack_enabled);
	convert_to_long(service_retain);
	convert_to_long(service_active);
	convert_to_long(flap_seconds);
	convert_to_long(handled);
	
	convert_to_long(escalate_divisor);
	convert_to_long(renotify_interval);
	convert_to_long(fires_events);
	convert_to_string(enabled_triggers);
	
	convert_to_string(snmp_community);
	convert_to_string(snmp_textmatch);
	convert_to_string(snmp_objid);
	convert_to_long(snmp_version);
	convert_to_long(snmp_warning);
	convert_to_long(snmp_critical);
	convert_to_long(snmp_type);

	convert_to_long(prio);
	convert_to_long(notify_super_users);
	convert_to_string(usid);
	
	

	sprintf(svc.usid, "%s", Z_STRVAL_P(usid));
	svc.prio=Z_LVAL_P(prio);
	svc.notify_super_users=Z_LVAL_P(notify_super_users);

	sprintf(svc.plugin, "%s", Z_STRVAL_P(plugin));
	sprintf(svc.service_exec_plan, "%s", Z_STRVAL_P(exec_plan));
	
	sprintf(svc.service_name, "%s", Z_STRVAL_P(service_name));
	sprintf(svc.plugin_arguments, "%s", Z_STRVAL_P(plugin_arguments));
	svc.notify_enabled=Z_LVAL_P(notify_enabled);
	
	svc.service_ack_enabled = Z_LVAL_P(service_ack_enabled);
	svc.service_retain=Z_LVAL_P(service_retain);
	
	svc.server_id=Z_LVAL_P(server_id);
	svc.flap_seconds=Z_LVAL_P(flap_seconds);
	
	svc.escalate_divisor=Z_LVAL_P(escalate_divisor);
	svc.renotify_interval=Z_LVAL_P(renotify_interval);
	svc.fires_events=Z_LVAL_P(fires_events);
	
	svc.service_check_timeout=Z_LVAL_P(service_check_timeout);
	svc.orch_id=Z_LVAL_P(orch_id);
	svc.check_interval=Z_LVAL_P(check_interval);
	svc.service_type=Z_LVAL_P(service_type);
	svc.service_passive_timeout=Z_LVAL_P(service_passive_timeout);
	sprintf(svc.service_var, "%s", Z_STRVAL_P(service_var));
	
	sprintf(svc.snmp_info.community, "%s", Z_STRVAL_P(snmp_community));
	sprintf(svc.snmp_info.textmatch, "%s", Z_STRVAL_P(snmp_textmatch));
	sprintf(svc.snmp_info.objid, "%s", Z_STRVAL_P(snmp_objid));
	svc.snmp_info.version=Z_LVAL_P(snmp_version);
	svc.snmp_info.warn=Z_LVAL_P(snmp_warning);
	svc.snmp_info.crit=Z_LVAL_P(snmp_critical);
	svc.snmp_info.type=Z_LVAL_P(snmp_type);
	svc.service_active=Z_LVAL_P(service_active);
	svc.handled=Z_LVAL_P(handled);
	sprintf(svc.enabled_triggers, "%s", Z_STRVAL_P(enabled_triggers));
	
	
	
	LOAD_SYMBOL(AddService,bres->SOHandle, "AddService");
	
	rtc=AddService(&svc, bres->cfgfile);
	
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,rtc, BARTLBY_SERVICE_GONE, BARTLBY_OBJECT_ADDED);
	
	RETURN_LONG(rtc);
	
	
	
		
	
}
PHP_FUNCTION(bartlby_add_server) {
	
	zval * zbartlby_resource;
	zval * server_name;
	zval * server_ip;
	zval * server_port;
	zval * server_icon;
	zval * server_enabled;
	zval * server_flap_seconds;
	zval * server_notify;
	zval * server_dead;
	
	zval * server_ssh_keyfile;
	zval * server_ssh_passphrase;
	zval * server_ssh_username;
	zval * enabled_triggers;
	zval * default_service_type;
	zval * orch_id;
	zval * exec_plan;

	zval ** temp_pp;
	zval * options_array;
	
	
	char * dlmsg;
	
	int ret;
	
	int (*AddServer)(struct server *,char *);
	
	struct server srv;
	bartlby_res * bres;


	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&options_array)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}
	
	GETARRAY_EL_FROM_HASH(server_name, "server_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default name");
	GETARRAY_EL_FROM_HASH(server_ip, "server_ip", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "127.0.0.1");
	GETARRAY_EL_FROM_HASH(server_port, "server_port", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 9030);
	GETARRAY_EL_FROM_HASH(server_icon, "server_icon", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default.gif");
	GETARRAY_EL_FROM_HASH(server_enabled, "server_enabled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,1);
	GETARRAY_EL_FROM_HASH(server_flap_seconds, "server_flap_seconds", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 120);
	GETARRAY_EL_FROM_HASH(server_notify, "server_notify", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(server_dead, "server_dead", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(enabled_triggers, "enabled_triggers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");

	GETARRAY_EL_FROM_HASH(server_ssh_keyfile, "server_ssh_keyfile", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(server_ssh_passphrase, "server_ssh_passphrase", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(server_ssh_username, "server_ssh_username", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	
	GETARRAY_EL_FROM_HASH(default_service_type, "default_service_type", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(exec_plan, "exec_plan", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");


	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	convert_to_long(default_service_type);
	convert_to_string(server_name);
	convert_to_string(server_ip);
	convert_to_long(server_port);
	convert_to_string(server_icon);
	convert_to_long(server_enabled);
	convert_to_long(server_flap_seconds);
	convert_to_long(server_notify);
	convert_to_long(server_dead);
	convert_to_long(orch_id);
	convert_to_string(enabled_triggers);
	convert_to_string(exec_plan);
	
	convert_to_string(server_ssh_keyfile);
	convert_to_string(server_ssh_passphrase);
	convert_to_string(server_ssh_username);
	
	
	
	
	LOAD_SYMBOL(AddServer,bres->SOHandle, "AddServer");
	
	strcpy(srv.enabled_triggers, Z_STRVAL_P(enabled_triggers));
	strcpy(srv.server_name, Z_STRVAL_P(server_name));
	srv.client_port=Z_LVAL_P(server_port);
	strcpy(srv.client_ip, Z_STRVAL_P(server_ip));
	strcpy(srv.server_icon, Z_STRVAL_P(server_icon));
	srv.server_enabled=Z_LVAL_P(server_enabled);
	srv.server_flap_seconds=Z_LVAL_P(server_flap_seconds);
	srv.server_notify=Z_LVAL_P(server_notify);
	srv.server_dead=Z_LVAL_P(server_dead);
	srv.default_service_type=Z_LVAL_P(default_service_type);
	srv.orch_id=Z_LVAL_P(orch_id);
	
	strcpy(srv.server_ssh_keyfile, Z_STRVAL_P(server_ssh_keyfile));
	strcpy(srv.server_ssh_passphrase, Z_STRVAL_P(server_ssh_passphrase));
	strcpy(srv.server_ssh_username, Z_STRVAL_P(server_ssh_username));
	strcpy(srv.exec_plan, Z_STRVAL_P(exec_plan));
	
	ret=AddServer(&srv, bres->cfgfile);
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,ret, BARTLBY_SERVER_GONE, BARTLBY_OBJECT_ADDED);
	
	RETURN_LONG(ret);
}

PHP_FUNCTION(bartlby_modify_server) {
	
	zval * zbartlby_resource;
	zval * server_name;
	zval * server_ip;
	zval * server_port;
	zval * server_id;
	zval * server_icon;
	zval * server_enabled;
	zval * server_flap_seconds;
	zval * server_notify;
	zval * server_dead;
	zval * server_ssh_keyfile;
	zval * server_ssh_passphrase;
	zval * server_ssh_username;
	zval * enabled_triggers;
	zval * default_service_type;
	zval * orch_id;
	zval * exec_plan;
	zval ** temp_pp;
	zval * options_array;
	
	char * dlmsg;
	int ret;
	int (*ModifyServer)(struct server *,char *);
	bartlby_res * bres;
	struct server srv;

	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&server_id, &options_array)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}
	
	GETARRAY_EL_FROM_HASH(server_name, "server_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default name");
	GETARRAY_EL_FROM_HASH(server_ip, "server_ip", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "127.0.0.1");
	GETARRAY_EL_FROM_HASH(server_port, "server_port", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 9030);
	GETARRAY_EL_FROM_HASH(server_icon, "server_icon", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default.gif");
	GETARRAY_EL_FROM_HASH(server_enabled, "server_enabled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,1);
	GETARRAY_EL_FROM_HASH(server_flap_seconds, "server_flap_seconds", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 120);
	GETARRAY_EL_FROM_HASH(server_notify, "server_notify", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(server_dead, "server_dead", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(enabled_triggers, "enabled_triggers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");

	GETARRAY_EL_FROM_HASH(server_ssh_keyfile, "server_ssh_keyfile", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(server_ssh_passphrase, "server_ssh_passphrase", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(server_ssh_username, "server_ssh_username", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	
	GETARRAY_EL_FROM_HASH(default_service_type, "default_service_type", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,1);

	GETARRAY_EL_FROM_HASH(exec_plan, "exec_plan", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");

	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	convert_to_long(default_service_type);
	convert_to_long(server_id);
	convert_to_string(enabled_triggers);
	convert_to_string(server_name);
	convert_to_string(server_ip);
	convert_to_long(server_port);
	convert_to_string(server_icon);
	convert_to_long(server_enabled);
	convert_to_long(server_flap_seconds);
	convert_to_long(server_notify);
	convert_to_long(orch_id);
	convert_to_long(server_dead);
	
	convert_to_string(server_ssh_keyfile);
	convert_to_string(server_ssh_passphrase);
	convert_to_string(server_ssh_username);
	
	
	
	
	LOAD_SYMBOL(ModifyServer,bres->SOHandle, "ModifyServer");
	
	strcpy(srv.enabled_triggers, Z_STRVAL_P(enabled_triggers));
	strcpy(srv.server_name, Z_STRVAL_P(server_name));
	srv.client_port=Z_LVAL_P(server_port);
	srv.server_id=Z_LVAL_P(server_id);
	
	strcpy(srv.client_ip, Z_STRVAL_P(server_ip));
	strcpy(srv.server_icon, Z_STRVAL_P(server_icon));
	srv.server_enabled=Z_LVAL_P(server_enabled);
	srv.server_flap_seconds=Z_LVAL_P(server_flap_seconds);
	srv.server_notify=Z_LVAL_P(server_notify);
	srv.server_dead=Z_LVAL_P(server_dead);
	srv.default_service_type=Z_LVAL_P(default_service_type);
	srv.orch_id=Z_LVAL_P(orch_id);
	strcpy(srv.server_ssh_keyfile, Z_STRVAL_P(server_ssh_keyfile));
	strcpy(srv.server_ssh_passphrase, Z_STRVAL_P(server_ssh_passphrase));
	strcpy(srv.server_ssh_username, Z_STRVAL_P(server_ssh_username));
	strcpy(srv.exec_plan, Z_STRVAL_P(exec_plan));

	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(server_id), BARTLBY_SERVER_GONE, BARTLBY_OBJECT_CHANGED);
	
	ret=ModifyServer(&srv, bres->cfgfile);
	
	
	RETURN_LONG(ret);
}

PHP_FUNCTION(bartlby_get_server_by_id) {
	zval * zbartlby_resource;
	
	zval * server_id;
	
	char * dlmsg;
	struct shm_header * shm_hdr;
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct server * srvmap;
	struct servicegroup * grp;
	struct service * sv;
	struct btl_event * evntmap;
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	int x;
	int z;
	int y;
	int current_time;
	int is_down;
	int ret;
	zval * groups;
	zval * groupinfo;
	int u;
	int dtmapindex;
	
	int (*GetServerById)(int,struct server *, char *);
	struct server  svc;
	bartlby_res * bres;

	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&server_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	convert_to_long(server_id);
	
	
	
	LOAD_SYMBOL(GetServerById,bres->SOHandle, "GetServerById");
	ret=GetServerById(Z_LVAL_P(server_id),&svc, bres->cfgfile);
	
	if(ret < 0) {
		RETURN_FALSE;	
	} else {
		if (array_init(return_value) == FAILURE) {
			RETURN_FALSE;
		}
		add_assoc_string(return_value, "server_name", svc.server_name, 1);
		add_assoc_string(return_value, "server_ip", svc.client_ip, 1);
		add_assoc_string(return_value, "exec_plan", svc.exec_plan, 1);
		
		add_assoc_string(return_value, "server_ssh_keyfile", svc.server_ssh_keyfile, 1);
		add_assoc_string(return_value, "server_ssh_passphrase", svc.server_ssh_passphrase, 1);
		add_assoc_string(return_value, "server_ssh_username", svc.server_ssh_username, 1);
		add_assoc_string(return_value, "enabled_triggers", svc.enabled_triggers, 1);
		
		add_assoc_string(return_value, "server_icon", svc.server_icon, 1);
		add_assoc_long(return_value, "server_port",svc.client_port);
		add_assoc_long(return_value, "orch_id",svc.orch_id);
		add_assoc_long(return_value, "server_id",Z_LVAL_P(server_id));

		
		add_assoc_long(return_value, "default_service_type",svc.default_service_type);
		
		shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
		svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
		wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
		dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
		srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
		evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
		srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
		svcgrpmap=(struct servicegroup *)(void *)&srvgrpmap[shm_hdr->srvgroupcount+1];
			
		current_time=time(NULL);
		is_down=0;
			
		for(x=0; x<shm_hdr->srvcount; x++) {
			if(srvmap[x].server_id == Z_LVAL_P(server_id)) {
				add_assoc_long(return_value, "server_enabled",srvmap[x].server_enabled);
				add_assoc_long(return_value, "server_notify",srvmap[x].server_notify);
				add_assoc_long(return_value, "server_flap_seconds",srvmap[x].server_flap_seconds);
				add_assoc_long(return_value, "last_notify_send",srvmap[x].last_notify_send);
				add_assoc_long(return_value, "server_dead",srvmap[x].server_dead);
				add_assoc_long(return_value, "server_shm_place",x);
			
					if(srvmap[x].servergroup_counter > 0) {
						ALLOC_INIT_ZVAL(groups);
						array_init(groups);
						for(y=0; y<srvmap[x].servergroup_counter; y++){
						
						
								ALLOC_INIT_ZVAL(groupinfo);
								array_init(groupinfo);
									
								z=srvmap[x].servergroup_place[y];
								add_assoc_long(groupinfo,"servergroup_place", y);
								add_assoc_string(groupinfo,"servergroup_name", srvgrpmap[z].servergroup_name,1);
								add_assoc_string(groupinfo,"servergroup_members", srvgrpmap[z].servergroup_members,1);
								
								add_assoc_long(groupinfo,"servergroup_active", srvgrpmap[z].servergroup_active);
								add_assoc_long(groupinfo,"servergroup_notify", srvgrpmap[z].servergroup_notify);
								add_assoc_long(groupinfo,"servergroup_id", srvgrpmap[z].servergroup_id);
								
								
								for(u=0; u<shm_hdr->dtcount; u++) {
					
									if(current_time >= dtmap[u].downtime_from && current_time <= dtmap[u].downtime_to) {
										
											if(dtmap[u].downtime_type ==  DT_SERVERGROUP) {
												if(srvgrpmap[z].servergroup_id == dtmap[u].service_id) {
																is_down=3;
																dtmapindex=u;
																break;
												}
											}
										}
								}	
									
								add_next_index_zval(groups, groupinfo);
					
					}		
				
				
				
					add_assoc_zval(return_value, "groups", groups);
				}
					
					
					
					
					
					
				for(y=0; y<shm_hdr->dtcount; y++) {
			
					if(current_time >= dtmap[y].downtime_from && current_time <= dtmap[y].downtime_to) {
							
							if(dtmap[y].downtime_type ==  DT_SERVER) {
								if(Z_LVAL_P(server_id) == dtmap[y].service_id) {
												is_down=2;
												dtmapindex=y;
												break;
								}
							}
						}
					}	
				
				
					if(is_down > 0) {
							add_assoc_long(return_value, "is_downtime", 1);
							add_assoc_long(return_value, "downtime_from", dtmap[dtmapindex].downtime_from);
							add_assoc_long(return_value, "downtime_to", dtmap[dtmapindex].downtime_to);
							add_assoc_string(return_value, "downtime_notice", dtmap[dtmapindex].downtime_notice, 1);
							add_assoc_long(return_value, "downtime_service", dtmap[dtmapindex].service_id);
							add_assoc_long(return_value, "downtime_type", dtmap[dtmapindex].downtime_type);
							
					} else {
						add_assoc_long(return_value, "is_downtime", 0);
					}		
				
				
					
					
					
					
				break;
			}			
		}
			
	}
		
}

PHP_FUNCTION(bartlby_lib_info) {
	char * dlmsg;
	zval * zbartlby_resource;
	
	char * (*GetAutor)();
	char * (*GetVersion)();
	char * (*GetName)();
	char * GetAutorStr;
	char * GetVersionStr;
	char * GetNameStr;
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	LOAD_SYMBOL(GetAutor,bres->SOHandle, "GetAutor");
    LOAD_SYMBOL(GetVersion,bres->SOHandle, "GetVersion");
    LOAD_SYMBOL(GetName,bres->SOHandle, "GetName");
    	
	GetAutorStr=GetAutor();
    GetVersionStr=GetVersion();
    GetNameStr=GetName();
    	
    if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	add_assoc_string(return_value, "Autor", GetAutorStr, 1);
	add_assoc_string(return_value, "Version", GetVersionStr, 1);
	add_assoc_string(return_value, "Name", GetNameStr, 1);
	
	free(GetAutorStr);
	free(GetVersionStr);
	free(GetNameStr);
		
}

PHP_FUNCTION(bartlby_get_service) {
	
	struct shm_header * shm_hdr;
	int current_time;
	int is_down;
	int y;
	int x;
	
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct server * srvmap;
	struct servicegroup * grp;
	struct service * sv;
	struct btl_event * evntmap;
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	
	zval * groups;
	zval * groupinfo;
	
	int u;
	char * group_has_server;
	int is_member;
	int z;
	
	zval * zbartlby_resource;
	zval * bartlby_service_id; 
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &bartlby_service_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_long(bartlby_service_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
		
	
	
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
	svcgrpmap=(struct servicegroup *)(void *)&srvgrpmap[shm_hdr->srvgroupcount+1];
		
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
		php_error(E_WARNING, "Service id out of bounds");	
		RETURN_FALSE;	
	}
	if(Z_LVAL_P(bartlby_service_id) < 0) {
		php_error(E_WARNING, "Service id below zero");	
		RETURN_FALSE;	
	}
	add_assoc_long(return_value, "service_id", svcmap[Z_LVAL_P(bartlby_service_id)].service_id);
	add_assoc_long(return_value, "server_id", svcmap[Z_LVAL_P(bartlby_service_id)].server_id);
	add_assoc_long(return_value, "last_state", svcmap[Z_LVAL_P(bartlby_service_id)].last_state);
		
		
	if(svcmap[Z_LVAL_P(bartlby_service_id)].is_server_dead < 0) {
		add_assoc_long(return_value, "current_state", -3);
		add_assoc_long(return_value, "server_is_dead", 1);
	} else {
		add_assoc_long(return_value, "current_state", svcmap[Z_LVAL_P(bartlby_service_id)].current_state);
	}
		
		
		
	add_assoc_long(return_value, "client_port", srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].client_port);
				
	add_assoc_string(return_value, "new_server_text", svcmap[Z_LVAL_P(bartlby_service_id)].new_server_text, 1);
	add_assoc_string(return_value, "service_name", svcmap[Z_LVAL_P(bartlby_service_id)].service_name, 1);
	add_assoc_string(return_value, "server_name", srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].server_name, 1);
	add_assoc_string(return_value, "client_ip", srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].client_ip, 1);
	
	add_assoc_string(return_value, "server_ssh_keyfile", srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].server_ssh_keyfile, 1);
	add_assoc_string(return_value, "server_ssh_passphrase", srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].server_ssh_passphrase, 1);
	add_assoc_string(return_value, "server_ssh_username", srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].server_ssh_username, 1);
	
	add_assoc_string(return_value, "server_enabled_triggers", srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].enabled_triggers, 1);
	
	add_assoc_string(return_value, "plugin", svcmap[Z_LVAL_P(bartlby_service_id)].plugin, 1);
	add_assoc_string(return_value, "plugin_arguments", svcmap[Z_LVAL_P(bartlby_service_id)].plugin_arguments, 1);
		
	add_assoc_long(return_value, "check_interval", svcmap[Z_LVAL_P(bartlby_service_id)].check_interval);
	add_assoc_long(return_value, "check_interval_original", svcmap[Z_LVAL_P(bartlby_service_id)].check_interval_original);
	add_assoc_long(return_value, "last_check", svcmap[Z_LVAL_P(bartlby_service_id)].last_check);
	
		
	add_assoc_string(return_value, "exec_plan", svcmap[Z_LVAL_P(bartlby_service_id)].service_exec_plan, 1);
		
	add_assoc_long(return_value, "notify_enabled", svcmap[Z_LVAL_P(bartlby_service_id)].notify_enabled);
	add_assoc_long(return_value, "last_notify_send", svcmap[Z_LVAL_P(bartlby_service_id)].last_notify_send);
	add_assoc_long(return_value, "last_state_change", svcmap[Z_LVAL_P(bartlby_service_id)].last_state_change);
	add_assoc_long(return_value, "flap_count", svcmap[Z_LVAL_P(bartlby_service_id)].flap_count);
		
		
	add_assoc_long(return_value, "service_active", svcmap[Z_LVAL_P(bartlby_service_id)].service_active);
	add_assoc_long(return_value, "service_type", svcmap[Z_LVAL_P(bartlby_service_id)].service_type);
	add_assoc_long(return_value, "service_passive_timeout", svcmap[Z_LVAL_P(bartlby_service_id)].service_passive_timeout);
		
		
	add_assoc_string(return_value, "service_var", svcmap[Z_LVAL_P(bartlby_service_id)].service_var, 1);
	
	add_assoc_long(return_value, "service_check_timeout", svcmap[Z_LVAL_P(bartlby_service_id)].service_check_timeout);
	add_assoc_long(return_value, "service_ack_enabled", svcmap[Z_LVAL_P(bartlby_service_id)].service_ack_enabled);
	add_assoc_long(return_value, "service_ack_current", svcmap[Z_LVAL_P(bartlby_service_id)].service_ack_current);
	
	add_assoc_long(return_value, "service_retain", svcmap[Z_LVAL_P(bartlby_service_id)].service_retain);
	add_assoc_long(return_value, "service_retain_current", svcmap[Z_LVAL_P(bartlby_service_id)].service_retain_current);
	add_assoc_long(return_value, "check_is_running", svcmap[Z_LVAL_P(bartlby_service_id)].process.pid);
	add_assoc_long(return_value, "check_starttime", svcmap[Z_LVAL_P(bartlby_service_id)].process.start_time);
	
	
	add_assoc_long(return_value, "service_time_sum", svcmap[Z_LVAL_P(bartlby_service_id)].pstat.sum);
	add_assoc_long(return_value, "service_time_count",svcmap[Z_LVAL_P(bartlby_service_id)].pstat.counter);
	
	add_assoc_long(return_value, "service_delay_sum", svcmap[Z_LVAL_P(bartlby_service_id)].delay_time.sum);
	add_assoc_long(return_value, "service_delay_count",svcmap[Z_LVAL_P(bartlby_service_id)].delay_time.counter);
	
	
	add_assoc_string(return_value, "service_var",  svcmap[Z_LVAL_P(bartlby_service_id)].service_var, 1);
	add_assoc_string(return_value, "server_icon",  srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].server_icon, 1);
	
	add_assoc_string(return_value, "snmp_community",  svcmap[Z_LVAL_P(bartlby_service_id)].snmp_info.community, 1);
	add_assoc_string(return_value, "snmp_textmatch",  svcmap[Z_LVAL_P(bartlby_service_id)].snmp_info.textmatch, 1);
	add_assoc_string(return_value, "snmp_objid",  svcmap[Z_LVAL_P(bartlby_service_id)].snmp_info.objid, 1);
	
	add_assoc_long(return_value, "snmp_warning", svcmap[Z_LVAL_P(bartlby_service_id)].snmp_info.warn);
	add_assoc_long(return_value, "snmp_critical", svcmap[Z_LVAL_P(bartlby_service_id)].snmp_info.crit);
	add_assoc_long(return_value, "snmp_version", svcmap[Z_LVAL_P(bartlby_service_id)].snmp_info.version);
	add_assoc_long(return_value, "snmp_type", svcmap[Z_LVAL_P(bartlby_service_id)].snmp_info.type);
	
	add_assoc_long(return_value, "flap_seconds", svcmap[Z_LVAL_P(bartlby_service_id)].flap_seconds);
	
	add_assoc_long(return_value, "server_flap_seconds",srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].server_flap_seconds);
	add_assoc_long(return_value, "server_last_notify_send",srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].last_notify_send);
	add_assoc_long(return_value, "server_notify",srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].server_notify);
	add_assoc_long(return_value, "server_enabled",srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].server_enabled);
	add_assoc_long(return_value, "server_dead",srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].server_dead);
		
	add_assoc_long(return_value, "renotify_interval",svcmap[Z_LVAL_P(bartlby_service_id)].renotify_interval);
	add_assoc_long(return_value, "escalate_divisor",svcmap[Z_LVAL_P(bartlby_service_id)].escalate_divisor);
	add_assoc_long(return_value, "fires_events",svcmap[Z_LVAL_P(bartlby_service_id)].fires_events);
	add_assoc_long(return_value, "is_gone",svcmap[Z_LVAL_P(bartlby_service_id)].is_gone);
	add_assoc_long(return_value, "handled",svcmap[Z_LVAL_P(bartlby_service_id)].handled);
	add_assoc_long(return_value, "orch_id",svcmap[Z_LVAL_P(bartlby_service_id)].orch_id);

	add_assoc_long(return_value, "prio",svcmap[Z_LVAL_P(bartlby_service_id)].prio);
	add_assoc_long(return_value, "notify_super_users",svcmap[Z_LVAL_P(bartlby_service_id)].notify_super_users);
	add_assoc_string(return_value, "usid", svcmap[Z_LVAL_P(bartlby_service_id)].usid, 1);


	add_assoc_string(return_value, "enabled_triggers", svcmap[Z_LVAL_P(bartlby_service_id)].enabled_triggers, 1);
	
	add_assoc_long(return_value, "notify_last_state",svcmap[Z_LVAL_P(bartlby_service_id)].notify_last_state);
	
		
	//Downtime 060120
	is_down=0;
	current_time=time(NULL);
	for(y=0; y<shm_hdr->dtcount; y++) {
			is_down=0;
			if(current_time >= dtmap[y].downtime_from && current_time <= dtmap[y].downtime_to) {
				switch(dtmap[y].downtime_type) {
					case DT_SERVICE:
						if(dtmap[y].service_id == svcmap[Z_LVAL_P(bartlby_service_id)].service_id) {
							is_down=1;	
							
						}
					break;
					case DT_SERVER:
						if(dtmap[y].service_id == svcmap[Z_LVAL_P(bartlby_service_id)].server_id) {
							is_down=2;	
						}
					break;	
					case DT_SERVERGROUP:
						
						if(srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].servergroup_counter > 0) {
							for(u=0; u<srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].servergroup_counter; u++){
								z=srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].servergroup_place[u];
								if(srvgrpmap[z].servergroup_id == dtmap[y].service_id) {
										is_down=3;
										break;
								}
									
							}
						}
						
					break;		
					case DT_SERVICEGROUP:
						if(svcmap[Z_LVAL_P(bartlby_service_id)].servicegroup_counter > 0) {
							for(u=0; u<svcmap[Z_LVAL_P(bartlby_service_id)].servicegroup_counter; u++){
									z=svcmap[Z_LVAL_P(bartlby_service_id)].servicegroup_place[u];
									if(svcgrpmap[z].servicegroup_id == dtmap[y].service_id) {
										is_down=4;
										break;
									}
							}
						}
				
					
					break;
					
								
				}
				if(is_down > 0) {
					add_assoc_long(return_value, "is_downtime", 1);
					add_assoc_long(return_value, "downtime_from", dtmap[y].downtime_from);
					add_assoc_long(return_value, "downtime_to", dtmap[y].downtime_to);
					add_assoc_string(return_value, "downtime_notice", dtmap[y].downtime_notice, 1);
					add_assoc_long(return_value, "downtime_service", dtmap[y].service_id);
					add_assoc_long(return_value, "downtime_type", dtmap[y].downtime_type);
					break;
				}
			}	
	}
	if(is_down==0) {
		add_assoc_long(return_value, "is_downtime", 0);	
	}

	//is member of following servergroups

	if(srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].servergroup_counter > 0) {
		ALLOC_INIT_ZVAL(groups);
		array_init(groups);
		for(y=0; y<srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].servergroup_counter; y++){
			ALLOC_INIT_ZVAL(groupinfo);
			array_init(groupinfo);
								
			z=srvmap[svcmap[Z_LVAL_P(bartlby_service_id)].srv_place].servergroup_place[y];
			add_assoc_long(groupinfo,"servergroup_place", y);
			add_assoc_string(groupinfo,"servergroup_name", srvgrpmap[z].servergroup_name,1);
			add_assoc_string(groupinfo,"servergroup_members", srvgrpmap[z].servergroup_members,1);
								
			add_assoc_long(groupinfo,"servergroup_active", srvgrpmap[z].servergroup_active);
			add_assoc_long(groupinfo,"servergroup_notify", srvgrpmap[z].servergroup_notify);
			add_assoc_long(groupinfo,"servergroup_id", srvgrpmap[z].servergroup_id);
			add_assoc_long(groupinfo,"servergroup_dead", srvgrpmap[z].servergroup_dead);
			add_assoc_string(groupinfo,"enabled_triggers", srvgrpmap[z].enabled_triggers,1);
									
									
									
			add_next_index_zval(groups, groupinfo);
		
					
		}
		add_assoc_zval(return_value, "servergroups", groups);
	}		


	//is member of following groups #SERVICEGROUP
	if(svcmap[Z_LVAL_P(bartlby_service_id)].servicegroup_counter > 0) {
		ALLOC_INIT_ZVAL(groups);
		array_init(groups);
		for(y=0; y<svcmap[Z_LVAL_P(bartlby_service_id)].servicegroup_counter; y++){
				
				
				ALLOC_INIT_ZVAL(groupinfo);
				array_init(groupinfo);
				
				x=svcmap[Z_LVAL_P(bartlby_service_id)].servicegroup_place[y];
				add_assoc_long(groupinfo,"servicegroup_place", y);
				add_assoc_string(groupinfo,"servicegroup_name", svcgrpmap[x].servicegroup_name,1);
				add_assoc_string(groupinfo,"servicegroup_member", svcgrpmap[x].servicegroup_members,1);
				
				add_assoc_long(groupinfo,"servicegroup_active", svcgrpmap[x].servicegroup_active);
				add_assoc_long(groupinfo,"servicegroup_notify", svcgrpmap[x].servicegroup_notify);
				add_assoc_long(groupinfo,"servicegroup_id", svcgrpmap[x].servicegroup_id);
				
				
				add_next_index_zval(groups, groupinfo);
				
		}		
		
		
		
		add_assoc_zval(return_value, "servicegroups", groups);
	}

	
	
	
}

PHP_FUNCTION(bartlby_get_downtime_by_id) {
	zval * zbartlby_resource;
	zval * downtime_id;
	
	char * dlmsg;
	int ret;
	int (*GetDowntimeById)(int,struct downtime *, char *);
	struct downtime  svc;
	
	bartlby_res * bres;

	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&downtime_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(downtime_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	


	LOAD_SYMBOL(GetDowntimeById,bres->SOHandle, "GetDowntimeById");
	ret=GetDowntimeById(Z_LVAL_P(downtime_id),&svc, bres->cfgfile);
	
	if(ret < 0) {
		RETURN_FALSE;	
	} else {
		if (array_init(return_value) == FAILURE) {
			RETURN_FALSE;
		}
		add_assoc_long(return_value, "downtime_id", svc.downtime_id);
		add_assoc_long(return_value, "downtime_from", svc.downtime_from);
		add_assoc_long(return_value, "downtime_to", svc.downtime_to);
		add_assoc_long(return_value, "downtime_type", svc.downtime_type);
		add_assoc_long(return_value, "service_id", svc.service_id);
		add_assoc_long(return_value, "is_gone", svc.is_gone);
		add_assoc_long(return_value, "orch_id", svc.orch_id);
		add_assoc_string(return_value, "downtime_notice", svc.downtime_notice, 1);

	}
		
}

PHP_FUNCTION(bartlby_get_downtime) {


	struct shm_header * shm_hdr;
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct server * srvmap;	
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	int is_down;
	int current_time;
	int dtmapindex;


	int y, z, u;
	zval * zbartlby_resource;
	zval * bartlby_downtime_id;
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &bartlby_downtime_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_long(bartlby_downtime_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
	svcgrpmap=(struct servicegroup *)(void *)&srvgrpmap[shm_hdr->srvgroupcount+1];
	
	if(Z_LVAL_P(bartlby_downtime_id) > shm_hdr->dtcount-1) {
		php_error(E_WARNING, "Server id out of bounds");	
		RETURN_FALSE;	
	}



	add_assoc_long(return_value, "downtime_id", dtmap[Z_LVAL_P(bartlby_downtime_id)].downtime_id);
	add_assoc_long(return_value, "downtime_from", dtmap[Z_LVAL_P(bartlby_downtime_id)].downtime_from);
	add_assoc_long(return_value, "downtime_to", dtmap[Z_LVAL_P(bartlby_downtime_id)].downtime_to);
	add_assoc_long(return_value, "downtime_type", dtmap[Z_LVAL_P(bartlby_downtime_id)].downtime_type);
	add_assoc_long(return_value, "service_id", dtmap[Z_LVAL_P(bartlby_downtime_id)].service_id);
	add_assoc_long(return_value, "is_gone", dtmap[Z_LVAL_P(bartlby_downtime_id)].is_gone);
	add_assoc_long(return_value, "orch_id", dtmap[Z_LVAL_P(bartlby_downtime_id)].orch_id);
	add_assoc_string(return_value, "downtime_notice", dtmap[Z_LVAL_P(bartlby_downtime_id)].downtime_notice, 1);

}

PHP_FUNCTION(bartlby_get_servergroup_by_id) {
	zval * zbartlby_resource;
	zval * servergroup_id;
	
	char * dlmsg;
	int ret;
	int (*GetServerGroupById)(int,struct servergroup *, char *);
	struct servergroup  svc;
	
	bartlby_res * bres;

	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&servergroup_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(servergroup_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	


	LOAD_SYMBOL(GetServerGroupById,bres->SOHandle, "GetServergroupById");
	ret=GetServerGroupById(Z_LVAL_P(servergroup_id),&svc, bres->cfgfile);
	
	if(ret < 0) {
		RETURN_FALSE;	
	} else {
		if (array_init(return_value) == FAILURE) {
			RETURN_FALSE;
		}
		add_assoc_long(return_value, "servergroup_id", svc.servergroup_id);
		add_assoc_string(return_value, "servergroup_name", svc.servergroup_name,1);
		add_assoc_long(return_value, "servergroup_active", svc.servergroup_active);
		add_assoc_long(return_value, "servergroup_notify", svc.servergroup_notify);
		add_assoc_string(return_value, "servergroup_members", svc.servergroup_members, 1);
		add_assoc_long(return_value, "servergroup_dead", svc.servergroup_dead);
		add_assoc_long(return_value, "orch_id", svc.orch_id);
		add_assoc_string(return_value, "enabled_triggers", svc.enabled_triggers,1);
	}
		
}

PHP_FUNCTION(bartlby_get_servergroup) {

	struct shm_header * shm_hdr;
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct server * srvmap;	
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	int is_down;
	int current_time;
	int dtmapindex;


	int y, z, u;
	zval * zbartlby_resource;
	zval * bartlby_servergroup_id;
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &bartlby_servergroup_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_long(bartlby_servergroup_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
	svcgrpmap=(struct servicegroup *)(void *)&srvgrpmap[shm_hdr->srvgroupcount+1];
	
	if(Z_LVAL_P(bartlby_servergroup_id) > shm_hdr->srvgroupcount-1) {
		php_error(E_WARNING, "Server id out of bounds");	
		RETURN_FALSE;	
	}

	add_assoc_long(return_value, "servergroup_id", srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_id);
	add_assoc_string(return_value, "servergroup_name", srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_name,1);
	add_assoc_long(return_value, "servergroup_active", srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_active);
	add_assoc_long(return_value, "servergroup_notify", srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_notify);
	add_assoc_string(return_value, "servergroup_members", srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_members, 1);
	add_assoc_long(return_value, "servergroup_dead", srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_dead);
	add_assoc_long(return_value, "orch_id", srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].orch_id);
	add_assoc_string(return_value, "enabled_triggers", srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].enabled_triggers,1);
	add_assoc_long(return_value, "shm_place", Z_LVAL_P(bartlby_servergroup_id));
	current_time=time(NULL);
	is_down=0;
	for(y=0; y<shm_hdr->dtcount; y++) {
		if(current_time >= dtmap[y].downtime_from && current_time <= dtmap[y].downtime_to) {
			if(dtmap[y].downtime_type ==  DT_SERVERGROUP) {
				if(srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_id == dtmap[y].service_id) {
					is_down=3;
					break;
				}
			}
		}
	}	
	if(is_down > 0) {
		add_assoc_long(return_value, "is_downtime", 1);
		add_assoc_long(return_value, "downtime_from", dtmap[y].downtime_from);
		add_assoc_long(return_value, "downtime_to", dtmap[y].downtime_to);
		add_assoc_string(return_value, "downtime_notice", dtmap[y].downtime_notice, 1);
		add_assoc_long(return_value, "downtime_service", dtmap[y].service_id);
		add_assoc_long(return_value, "downtime_type", dtmap[y].downtime_type);
	} else {
		add_assoc_long(return_value, "is_downtime", 0);
	}		


}
PHP_FUNCTION(bartlby_get_servicegroup_by_id) {
  zval * zbartlby_resource;
  zval * servicegroup_id;
  
  char * dlmsg;
  int ret;
  int (*GetServicegroupById)(int,struct servicegroup *, char *);
  struct servicegroup  svc;
  
  bartlby_res * bres;

  if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&servicegroup_id)==FAILURE) {
    WRONG_PARAM_COUNT;
  }
  
  convert_to_long(servicegroup_id);
  ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
  


  LOAD_SYMBOL(GetServicegroupById,bres->SOHandle, "GetsServicegroupById");
  ret=GetServicegroupById(Z_LVAL_P(servicegroup_id),&svc, bres->cfgfile);
  
  if(ret < 0) {
    RETURN_FALSE; 
  } else {
    if (array_init(return_value) == FAILURE) {
      RETURN_FALSE;
    }
    add_assoc_long(return_value, "servicegroup_id", svc.servicegroup_id);
    add_assoc_string(return_value, "servicegroup_name", svc.servicegroup_name,1);
    add_assoc_long(return_value, "servicegroup_active", svc.servicegroup_active);
    add_assoc_long(return_value, "servicegroup_notify", svc.servicegroup_notify);
    add_assoc_string(return_value, "servicegroup_members", svc.servicegroup_members, 1);
    add_assoc_long(return_value, "servicegroup_dead", svc.servicegroup_dead);
    add_assoc_long(return_value, "orch_id", svc.orch_id);
    add_assoc_string(return_value, "enabled_triggers", svc.enabled_triggers,1);
  }
    
}

PHP_FUNCTION(bartlby_get_servicegroup) {

	struct shm_header * shm_hdr;
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct server * srvmap;	
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	int is_down;
	int current_time;
	int dtmapindex;


	int y, z, u;
	zval * zbartlby_resource;
	zval * bartlby_servicegroup_id;
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &bartlby_servicegroup_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_long(bartlby_servicegroup_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
	svcgrpmap=(struct servicegroup *)(void *)&srvgrpmap[shm_hdr->srvgroupcount+1];
	
	if(Z_LVAL_P(bartlby_servicegroup_id) > shm_hdr->svcgroupcount-1) {
		php_error(E_WARNING, "Server id out of bounds");	
		RETURN_FALSE;	
	}

	add_assoc_long(return_value, "servicegroup_id", svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_id);
	add_assoc_string(return_value, "servicegroup_name", svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_name,1);
	add_assoc_long(return_value, "servicegroup_active", svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_active);
	add_assoc_long(return_value, "servicegroup_notify", svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_notify);
	add_assoc_string(return_value, "servicegroup_members", svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_members, 1);
	add_assoc_long(return_value, "servicegroup_dead", svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_dead);
	add_assoc_long(return_value, "orch_id", svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].orch_id);
	add_assoc_string(return_value, "enabled_triggers", svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].enabled_triggers,1);
	add_assoc_long(return_value, "shm_place", Z_LVAL_P(bartlby_servicegroup_id));
	current_time=time(NULL);
	is_down=0;
	for(y=0; y<shm_hdr->dtcount; y++) {
		if(current_time >= dtmap[y].downtime_from && current_time <= dtmap[y].downtime_to) {
			if(dtmap[y].downtime_type ==  DT_SERVICEGROUP) {
				if(svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_id == dtmap[y].service_id) {
					is_down=3;
					break;
				}
			}
		}
	}	
	if(is_down > 0) {
		add_assoc_long(return_value, "is_downtime", 1);
		add_assoc_long(return_value, "downtime_from", dtmap[y].downtime_from);
		add_assoc_long(return_value, "downtime_to", dtmap[y].downtime_to);
		add_assoc_string(return_value, "downtime_notice", dtmap[y].downtime_notice, 1);
		add_assoc_long(return_value, "downtime_service", dtmap[y].service_id);
		add_assoc_long(return_value, "downtime_type", dtmap[y].downtime_type);
	} else {
		add_assoc_long(return_value, "is_downtime", 0);
	}		




}

PHP_FUNCTION(bartlby_get_server) {
	struct shm_header * shm_hdr;
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct server * srvmap;	
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	int is_down;
	int current_time;
	int dtmapindex;


	int y, z, u;
	zval * zbartlby_resource;
	zval * bartlby_server_id;
	zval * groups;
	zval * groupinfo;
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &bartlby_server_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_long(bartlby_server_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
	svcgrpmap=(struct servicegroup *)(void *)&srvgrpmap[shm_hdr->srvgroupcount+1];
	
	if(Z_LVAL_P(bartlby_server_id) > shm_hdr->srvcount-1) {
		php_error(E_WARNING, "Server id out of bounds");	
		RETURN_FALSE;	
	}
	
			
	current_time=time(NULL);
	is_down=0;
			
	add_assoc_string(return_value, "server_name", srvmap[Z_LVAL_P(bartlby_server_id)].server_name, 1);
	add_assoc_string(return_value, "server_ip", srvmap[Z_LVAL_P(bartlby_server_id)].client_ip, 1);
		
	add_assoc_string(return_value, "server_ssh_keyfile", srvmap[Z_LVAL_P(bartlby_server_id)].server_ssh_keyfile, 1);
	add_assoc_string(return_value, "server_ssh_passphrase", srvmap[Z_LVAL_P(bartlby_server_id)].server_ssh_passphrase, 1);
	add_assoc_string(return_value, "server_ssh_username", srvmap[Z_LVAL_P(bartlby_server_id)].server_ssh_username, 1);
	add_assoc_string(return_value, "enabled_triggers", srvmap[Z_LVAL_P(bartlby_server_id)].enabled_triggers, 1);

	add_assoc_long(return_value, "default_service_type",srvmap[Z_LVAL_P(bartlby_server_id)].default_service_type);
	
		
	add_assoc_string(return_value, "server_icon", srvmap[Z_LVAL_P(bartlby_server_id)].server_icon, 1);
	add_assoc_string(return_value, "exec_plan", srvmap[Z_LVAL_P(bartlby_server_id)].exec_plan, 1);
	add_assoc_long(return_value, "server_port",srvmap[Z_LVAL_P(bartlby_server_id)].client_port);
	add_assoc_long(return_value, "server_id",srvmap[Z_LVAL_P(bartlby_server_id)].server_id);
		
	add_assoc_long(return_value, "server_enabled",srvmap[Z_LVAL_P(bartlby_server_id)].server_enabled);
	add_assoc_long(return_value, "server_notify",srvmap[Z_LVAL_P(bartlby_server_id)].server_notify);
	add_assoc_long(return_value, "server_flap_seconds",srvmap[Z_LVAL_P(bartlby_server_id)].server_flap_seconds);
	add_assoc_long(return_value, "last_notify_send",srvmap[Z_LVAL_P(bartlby_server_id)].last_notify_send);
	add_assoc_long(return_value, "server_dead",srvmap[Z_LVAL_P(bartlby_server_id)].server_dead);
	add_assoc_long(return_value, "orch_id",srvmap[Z_LVAL_P(bartlby_server_id)].orch_id);
	add_assoc_long(return_value, "server_shm_place",Z_LVAL_P(bartlby_server_id));
		
	if(srvmap[Z_LVAL_P(bartlby_server_id)].servergroup_counter > 0) {
		ALLOC_INIT_ZVAL(groups);
		array_init(groups);
		for(y=0; y<srvmap[Z_LVAL_P(bartlby_server_id)].servergroup_counter; y++){
			ALLOC_INIT_ZVAL(groupinfo);
			array_init(groupinfo);
								
			z=srvmap[Z_LVAL_P(bartlby_server_id)].servergroup_place[y];
			add_assoc_long(groupinfo,"servergroup_place", y);
			add_assoc_string(groupinfo,"servergroup_name", srvgrpmap[z].servergroup_name,1);
			add_assoc_string(groupinfo,"servergroup_members", srvgrpmap[z].servergroup_members,1);
							
			add_assoc_long(groupinfo,"servergroup_active", srvgrpmap[z].servergroup_active);
			add_assoc_long(groupinfo,"servergroup_notify", srvgrpmap[z].servergroup_notify);
			add_assoc_long(groupinfo,"servergroup_id", srvgrpmap[z].servergroup_id);
			for(u=0; u<shm_hdr->dtcount; u++) {
				if(current_time >= dtmap[u].downtime_from && current_time <= dtmap[u].downtime_to) {
					if(dtmap[u].downtime_type ==  DT_SERVERGROUP) {
						if(srvgrpmap[z].servergroup_id == dtmap[u].service_id) {
							is_down=3;
							dtmapindex=u;
							break;
						}
					}
				}
			}	
								
			add_next_index_zval(groups, groupinfo);
				
		}
		
		add_assoc_zval(return_value, "groups", groups);
	}		
		

	for(y=0; y<shm_hdr->dtcount; y++) {
		if(current_time >= dtmap[y].downtime_from && current_time <= dtmap[y].downtime_to) {
			if(dtmap[y].downtime_type ==  DT_SERVER) {
				if(srvmap[Z_LVAL_P(bartlby_server_id)].server_id == dtmap[y].service_id) {
					is_down=2;
					dtmapindex=y;
					break;
				}
			}
		}
	}	
	if(is_down > 0) {
		add_assoc_long(return_value, "is_downtime", 1);
		add_assoc_long(return_value, "downtime_from", dtmap[dtmapindex].downtime_from);
		add_assoc_long(return_value, "downtime_to", dtmap[dtmapindex].downtime_to);
		add_assoc_string(return_value, "downtime_notice", dtmap[dtmapindex].downtime_notice, 1);
		add_assoc_long(return_value, "downtime_service", dtmap[dtmapindex].service_id);
		add_assoc_long(return_value, "downtime_type", dtmap[dtmapindex].downtime_type);
	} else {
		add_assoc_long(return_value, "is_downtime", 0);
	}
	
}



PHP_FUNCTION(bartlby_get_worker) {
	struct shm_header * shm_hdr;
	
	struct worker * wrkmap;
	struct service * svcmap;
	
	zval * zbartlby_resource;
	zval * bartlby_worker_id;
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &bartlby_worker_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_long(bartlby_worker_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
		
	if(Z_LVAL_P(bartlby_worker_id) > shm_hdr->wrkcount-1) {
		php_error(E_WARNING, "Worker id out of bounds");	
		RETURN_FALSE;	
	}
		
		
	add_assoc_string(return_value, "mail", wrkmap[Z_LVAL_P(bartlby_worker_id)].mail, 1);
	add_assoc_string(return_value, "icq", wrkmap[Z_LVAL_P(bartlby_worker_id)].icq, 1);
	add_assoc_string(return_value, "notify_plan", wrkmap[Z_LVAL_P(bartlby_worker_id)].notify_plan, 1);
	add_assoc_string(return_value, "visible_services", wrkmap[Z_LVAL_P(bartlby_worker_id)].visible_services, 1);
	add_assoc_string(return_value, "selected_services", wrkmap[Z_LVAL_P(bartlby_worker_id)].selected_services, 1);
	

	add_assoc_string(return_value, "visible_servers", wrkmap[Z_LVAL_P(bartlby_worker_id)].visible_servers, 1);
	add_assoc_string(return_value, "selected_servers", wrkmap[Z_LVAL_P(bartlby_worker_id)].selected_servers, 1);
	

	add_assoc_string(return_value, "api_pubkey", wrkmap[Z_LVAL_P(bartlby_worker_id)].api_pubkey, 1);
	add_assoc_string(return_value, "api_privkey", wrkmap[Z_LVAL_P(bartlby_worker_id)].api_privkey, 1);
	add_assoc_long(return_value, "api_enabled", wrkmap[Z_LVAL_P(bartlby_worker_id)].api_enabled);

	add_assoc_string(return_value, "notify_levels", wrkmap[Z_LVAL_P(bartlby_worker_id)].notify_levels,1);
	add_assoc_string(return_value, "name", wrkmap[Z_LVAL_P(bartlby_worker_id)].name,1);
	add_assoc_string(return_value, "password", wrkmap[Z_LVAL_P(bartlby_worker_id)].password,1);
	add_assoc_long(return_value, "worker_id", wrkmap[Z_LVAL_P(bartlby_worker_id)].worker_id);
	
	add_assoc_long(return_value, "escalation_limit", wrkmap[Z_LVAL_P(bartlby_worker_id)].escalation_limit);
	add_assoc_long(return_value, "escalation_minutes", wrkmap[Z_LVAL_P(bartlby_worker_id)].escalation_minutes);
	
	add_assoc_string(return_value, "enabled_triggers", wrkmap[Z_LVAL_P(bartlby_worker_id)].enabled_triggers,1);
	add_assoc_long(return_value, "active", wrkmap[Z_LVAL_P(bartlby_worker_id)].active);
	add_assoc_long(return_value, "is_gone", wrkmap[Z_LVAL_P(bartlby_worker_id)].is_gone);		

	add_assoc_long(return_value, "is_super_user", wrkmap[Z_LVAL_P(bartlby_worker_id)].is_super_user);		
	add_assoc_long(return_value, "notification_aggregation_interval", wrkmap[Z_LVAL_P(bartlby_worker_id)].notification_aggregation_interval);		
	add_assoc_long(return_value, "orch_id", wrkmap[Z_LVAL_P(bartlby_worker_id)].orch_id);		
	
	
}



PHP_FUNCTION(bartlby_add_servergroup) {
	zval * zbartlby_resource;
	zval * servergroup_name;
	zval * servergroup_active;
	zval * servergroup_notify;
	zval * servergroup_members;
	zval * servergroup_dead;
	zval * orch_id;
	zval * enabled_triggers;
	
	
	zval ** temp_pp;
	zval * options_array;
	
	char * dlmsg;
	
	int ret;
	
	int (*AddServerGroup)(struct servergroup *,char *);
	
	struct servergroup svc;
	bartlby_res * bres;
	
	if(ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}
	
	GETARRAY_EL_FROM_HASH(servergroup_name, "servergroup_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(enabled_triggers, "enabled_triggers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(servergroup_members, "servergroup_members", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(servergroup_active, "servergroup_active", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(servergroup_notify, "servergroup_notify", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(servergroup_dead, "servergroup_dead", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_string(servergroup_name);
	convert_to_string(enabled_triggers);
	convert_to_string(servergroup_members);
	convert_to_long(servergroup_active);
	convert_to_long(servergroup_notify);
	convert_to_long(servergroup_dead);
	convert_to_long(orch_id);
		
	
	
	
	LOAD_SYMBOL(AddServerGroup,bres->SOHandle, "AddServerGroup");
	
	strcpy(svc.servergroup_name, Z_STRVAL_P(servergroup_name));
	svc.servergroup_notify=Z_LVAL_P(servergroup_notify);
	svc.servergroup_active=Z_LVAL_P(servergroup_active);
	strcpy(svc.servergroup_members, Z_STRVAL_P(servergroup_members));
	strcpy(svc.enabled_triggers, Z_STRVAL_P(enabled_triggers));
	
	svc.servergroup_dead=Z_LVAL_P(servergroup_dead);
	svc.orch_id=Z_LVAL_P(orch_id);
	
	ret=AddServerGroup(&svc, bres->cfgfile);
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,ret, BARTLBY_SERVERGROUP_GONE, BARTLBY_OBJECT_ADDED);
	RETURN_LONG(ret);	
}



PHP_FUNCTION(bartlby_modify_servergroup) {
	zval * zbartlby_resource;
	zval * servergroup_name;
	zval * servergroup_active;
	zval * servergroup_notify;
	zval * servergroup_members;
	zval * servergroup_id;
	zval * servergroup_dead;
	zval * enabled_triggers;
	zval * orch_id;
	
	zval ** temp_pp;
	zval * options_array;
	char * dlmsg;
	
	int ret;
	
	int (*UpdateServerGroup)(struct servergroup *,char *);
	
	struct servergroup svc;
	bartlby_res * bres;
	
	
	
	if(ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&servergroup_id, &options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}
	
	GETARRAY_EL_FROM_HASH(servergroup_name, "servergroup_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(enabled_triggers, "enabled_triggers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(servergroup_members, "servergroup_members", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(servergroup_active, "servergroup_active", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(servergroup_notify, "servergroup_notify", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(servergroup_dead, "servergroup_dead", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	convert_to_string(servergroup_members);
	convert_to_string(enabled_triggers);
	convert_to_string(servergroup_name);
	convert_to_long(servergroup_active);
	convert_to_long(servergroup_notify);
	convert_to_long(servergroup_dead);
	convert_to_long(orch_id);

	
	convert_to_long(servergroup_id);
	
	
	
	
	LOAD_SYMBOL(UpdateServerGroup,bres->SOHandle, "UpdateServerGroup");
	
	strcpy(svc.enabled_triggers, Z_STRVAL_P(enabled_triggers));
	
	strcpy(svc.servergroup_name, Z_STRVAL_P(servergroup_name));
	strcpy(svc.servergroup_members, Z_STRVAL_P(servergroup_members));
	svc.servergroup_active=Z_LVAL_P(servergroup_active);
	svc.servergroup_notify=Z_LVAL_P(servergroup_notify);
	svc.servergroup_id=Z_LVAL_P(servergroup_id);
	svc.servergroup_dead=Z_LVAL_P(servergroup_dead);
	svc.orch_id=Z_LVAL_P(orch_id);
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,svc.servergroup_id, BARTLBY_SERVERGROUP_GONE, BARTLBY_OBJECT_CHANGED);
	ret=UpdateServerGroup(&svc, bres->cfgfile);
	
	

	RETURN_LONG(ret);		
}


PHP_FUNCTION(bartlby_delete_servergroup) {
	zval * zbartlby_resource;
	zval * servergroup_id;
	
	
	char * dlmsg;
	
	int ret;
	
	int (*DeleteServerGroup)(int, char*);
	
	struct service svc;
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&servergroup_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	convert_to_long(servergroup_id);
	
	
	
	LOAD_SYMBOL(DeleteServerGroup,bres->SOHandle, "DeleteServerGroup");
	
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(servergroup_id), BARTLBY_SERVERGROUP_GONE, BARTLBY_OBJECT_DELETED);
	
	ret=DeleteServerGroup(Z_LVAL_P(servergroup_id),bres->cfgfile);

	

	RETURN_LONG(ret);	
}
PHP_FUNCTION(bartlby_set_servergroup_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	
	char * dlmsg;
	int ret;
	int (*ServerGroupChangeId)(int, int, char*);
	bartlby_res * bres;
	
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&from, &to)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(from);
	convert_to_long(to);
	
	
	
	LOAD_SYMBOL(ServerGroupChangeId,bres->SOHandle, "ServerGroupChangeId");
	
	
	
	ret=ServerGroupChangeId(Z_LVAL_P(from),Z_LVAL_P(to),bres->cfgfile);
	RETURN_LONG(ret);	
}


PHP_FUNCTION(bartlby_toggle_servergroup_notify) {
	zval * zbartlby_resource;
	zval * bartlby_servergroup_id;
	zval * do_writeback;
	struct shm_header * shm_hdr;
	int r;
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct server * srvmap;	
	struct servergroup * srvgrpmap;
	char * dlmsg;
	int (*UpdateServerGroup)(struct servergroup *, char *);
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &bartlby_servergroup_id, &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(bartlby_servergroup_id);
	convert_to_long(do_writeback);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
		
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
		
	if(Z_LVAL_P(bartlby_servergroup_id) > shm_hdr->srvgroupcount-1) {
		php_error(E_WARNING, "service group id out of bounds");	
		RETURN_FALSE;	
	}
	if(srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_notify == 1) {
		srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_notify = 0;
		r=0;
	} else {
		srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_notify = 1;
		r=1;
	}
	if(Z_LVAL_P(do_writeback) == 1) {
		LOAD_SYMBOL(UpdateServerGroup,bres->SOHandle, "UpdateServerGroup");
		UpdateServerGroup(&srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)], bres->cfgfile);
		
	}
	BARTLBY_GENERIC_AUDIT(zbartlby_resource, srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_id ,BARTLBY_AUDIT_TYPE_SERVERGROUP, "Toggled servergroup notify state");		
	RETURN_LONG(r);
	
}

PHP_FUNCTION(bartlby_toggle_servergroup_active) {
	zval * zbartlby_resource;
	zval * bartlby_servergroup_id;
	zval * do_writeback;
	struct shm_header * shm_hdr;
	int r;
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct server * srvmap;	
	struct servergroup * srvgrpmap;
	
	char * dlmsg;
	int (*UpdateServerGroup)(struct servergroup *, char *);
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &bartlby_servergroup_id, &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(bartlby_servergroup_id);
	convert_to_long(do_writeback);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
		
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
		
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
		
	if(Z_LVAL_P(bartlby_servergroup_id) > shm_hdr->srvgroupcount-1) {
		php_error(E_WARNING, "service group id out of bounds");	
		RETURN_FALSE;	
	}
	if(srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_active == 1) {
		srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_active = 0;
		r=0;
	} else {
		srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_active = 1;
		r=1;
	}
	if(Z_LVAL_P(do_writeback) == 1) {
		LOAD_SYMBOL(UpdateServerGroup,bres->SOHandle, "UpdateServerGroup");
		UpdateServerGroup(&srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)], bres->cfgfile);
		
	}

	BARTLBY_GENERIC_AUDIT(zbartlby_resource, srvgrpmap[Z_LVAL_P(bartlby_servergroup_id)].servergroup_id ,BARTLBY_AUDIT_TYPE_SERVERGROUP, "Toggled servergroup active state");		
	
	RETURN_LONG(r);
	
}

PHP_FUNCTION(bartlby_add_servicegroup) {
	zval * zbartlby_resource;
	zval * servicegroup_name;
	zval * servicegroup_active;
	zval * servicegroup_notify;
	zval * servicegroup_members;
	zval * servicegroup_dead;
	zval * enabled_triggers;
	zval * orch_id;
	
	zval ** temp_pp;
	zval * options_array;
	
	char * dlmsg;
	int ret;
	int (*AddServiceGroup)(struct servicegroup *,char *);
	struct servicegroup svc;
	bartlby_res * bres;

	
	
	if(ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}
	
	GETARRAY_EL_FROM_HASH(servicegroup_name, "servicegroup_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(enabled_triggers, "enabled_triggers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(servicegroup_members, "servicegroup_members", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(servicegroup_active, "servicegroup_active", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(servicegroup_notify, "servicegroup_notify", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(servicegroup_dead, "servicegroup_dead", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	
	
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
		
	convert_to_string(servicegroup_name);
	convert_to_string(servicegroup_members);
	convert_to_long(servicegroup_active);
	convert_to_long(servicegroup_notify);
	convert_to_long(servicegroup_dead);
	convert_to_string(enabled_triggers);
	convert_to_long(orch_id);
	
	
	
	LOAD_SYMBOL(AddServiceGroup,bres->SOHandle, "AddServiceGroup");
	
	strcpy(svc.servicegroup_name, Z_STRVAL_P(servicegroup_name));
	svc.servicegroup_notify=Z_LVAL_P(servicegroup_notify);
	svc.servicegroup_active=Z_LVAL_P(servicegroup_active);
	strcpy(svc.servicegroup_members, Z_STRVAL_P(servicegroup_members));
	strcpy(svc.enabled_triggers, Z_STRVAL_P(enabled_triggers));
	svc.servicegroup_dead=Z_LVAL_P(servicegroup_dead);
	svc.orch_id=Z_LVAL_P(orch_id);
	
	ret=AddServiceGroup(&svc, bres->cfgfile);
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,ret, BARTLBY_SERVICEGROUP_GONE, BARTLBY_OBJECT_ADDED);

	RETURN_LONG(ret);	
}




PHP_FUNCTION(bartlby_modify_servicegroup) {
	zval * zbartlby_resource;
	zval * servicegroup_name;
	zval * servicegroup_active;
	zval * servicegroup_notify;
	zval * servicegroup_members;
	zval * servicegroup_id;
	zval * servicegroup_dead;
	zval * enabled_triggers;
	zval * orch_id;
	
	zval ** temp_pp;
	zval * options_array;
	
	
	char * dlmsg;
	int ret;
	int (*UpdateServiceGroup)(struct servicegroup *,char *);
	struct servicegroup svc;
	bartlby_res * bres;
	
	
	if(ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&servicegroup_id ,&options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}
	
	GETARRAY_EL_FROM_HASH(servicegroup_name, "servicegroup_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(enabled_triggers, "enabled_triggers", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(servicegroup_members, "servicegroup_members", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(servicegroup_active, "servicegroup_active", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(servicegroup_notify, "servicegroup_notify", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(servicegroup_dead, "servicegroup_dead", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	convert_to_string(enabled_triggers);
	convert_to_string(servicegroup_members);
	convert_to_string(servicegroup_name);
	convert_to_long(servicegroup_active);
	convert_to_long(servicegroup_notify);
	convert_to_long(servicegroup_dead);
	convert_to_long(orch_id);
	convert_to_long(servicegroup_id);
	
	
	
	
	LOAD_SYMBOL(UpdateServiceGroup,bres->SOHandle, "UpdateServiceGroup");
	
	strcpy(svc.enabled_triggers, Z_STRVAL_P(enabled_triggers));
	strcpy(svc.servicegroup_name, Z_STRVAL_P(servicegroup_name));
	strcpy(svc.servicegroup_members, Z_STRVAL_P(servicegroup_members));
	svc.servicegroup_active=Z_LVAL_P(servicegroup_active);
	svc.servicegroup_notify=Z_LVAL_P(servicegroup_notify);
	svc.servicegroup_id=Z_LVAL_P(servicegroup_id);
	svc.servicegroup_dead=Z_LVAL_P(servicegroup_dead);
	svc.orch_id=Z_LVAL_P(orch_id);
	

	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,svc.servicegroup_id, BARTLBY_SERVICEGROUP_GONE, BARTLBY_OBJECT_CHANGED);

	ret=UpdateServiceGroup(&svc, bres->cfgfile);
	
	
	
	RETURN_LONG(ret);		
}


PHP_FUNCTION(bartlby_delete_servicegroup) {
	zval * zbartlby_resource;
	zval * servicegroup_id;
	
	
	char * dlmsg;
	int ret;
	int (*DeleteServiceGroup)(int, char*);
	struct service svc;
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&servicegroup_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(servicegroup_id);
	
	
	
	LOAD_SYMBOL(DeleteServiceGroup,bres->SOHandle, "DeleteServiceGroup");
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(servicegroup_id), BARTLBY_SERVICEGROUP_GONE, BARTLBY_OBJECT_DELETED);
	ret=DeleteServiceGroup(Z_LVAL_P(servicegroup_id),bres->cfgfile);
	
	RETURN_LONG(ret);	
}
PHP_FUNCTION(bartlby_set_servicegroup_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	
	char * dlmsg;
	int ret;
	int (*ServiceGroupChangeId)(int, int, char*);
	bartlby_res * bres;
	
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&from, &to)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(from);
	convert_to_long(to);
	
	
	LOAD_SYMBOL(ServiceGroupChangeId,bres->SOHandle, "ServiceGroupChangeId");
	
	
	
	ret=ServiceGroupChangeId(Z_LVAL_P(from),Z_LVAL_P(to),bres->cfgfile);
	
	RETURN_LONG(ret);	
}


PHP_FUNCTION(bartlby_toggle_servicegroup_notify) {
	zval * zbartlby_resource;
	zval * bartlby_servicegroup_id;
	zval * do_writeback;
	struct shm_header * shm_hdr;
	int r;
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct server * srvmap;	
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	char * dlmsg;
	int (*UpdateServiceGroup)(struct servicegroup *, char *);
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &bartlby_servicegroup_id, &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(bartlby_servicegroup_id);
	convert_to_long(do_writeback);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
		
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
	svcgrpmap=(struct servicegroup *)(void *)&srvgrpmap[shm_hdr->srvgroupcount+1];
	if(Z_LVAL_P(bartlby_servicegroup_id) > shm_hdr->svcgroupcount-1) {
		php_error(E_WARNING, "service group id out of bounds");	
		RETURN_FALSE;	
	}
	if(svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_notify == 1) {
		svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_notify = 0;
		r=0;
	} else {
		svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_notify = 1;
		r=1;
	}
	if(Z_LVAL_P(do_writeback) == 1) {
		LOAD_SYMBOL(UpdateServiceGroup,bres->SOHandle, "UpdateServiceGroup");
		UpdateServiceGroup(&svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)],bres->cfgfile);
		
	}
	BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_id ,BARTLBY_AUDIT_TYPE_SERVICEGROUP, "Toggled servicegroup notification state");		
	RETURN_LONG(r);
	
}

PHP_FUNCTION(bartlby_toggle_servicegroup_active) {
	zval * zbartlby_resource;
	zval * bartlby_servicegroup_id;
	zval * do_writeback;
	struct shm_header * shm_hdr;
	int r;
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct server * srvmap;	
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	char * dlmsg;
	int (*UpdateServiceGroup)(struct servicegroup *, char *);
	
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &bartlby_servicegroup_id, &do_writeback)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(bartlby_servicegroup_id);
	convert_to_long(do_writeback);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	shm_hdr=(struct shm_header *)(void *)bres->bartlby_address;
		
	svcmap=(struct service *)(void *)(bres->bartlby_address+sizeof(struct shm_header));
	wrkmap=(struct worker *)(void*)&svcmap[shm_hdr->svccount+1];
	dtmap=(struct downtime *)(void*)&wrkmap[shm_hdr->wrkcount+1];
	srvmap=(struct server *)(void*)&dtmap[shm_hdr->dtcount+1];
	evntmap=(struct btl_event *)(void *)&srvmap[shm_hdr->srvcount+1];
	srvgrpmap=(struct servergroup *)(void *)&evntmap[EVENT_QUEUE_MAX+1];
	svcgrpmap=(struct servicegroup *)(void *)&srvgrpmap[shm_hdr->srvgroupcount+1];
	
	if(Z_LVAL_P(bartlby_servicegroup_id) > shm_hdr->svcgroupcount-1) {
		php_error(E_WARNING, "service group id out of bounds");	
		RETURN_FALSE;	
	}
	if(svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_active == 1) {
		svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_active = 0;
		r=0;
	} else {
		svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_active = 1;
		r=1;
	}
	if(Z_LVAL_P(do_writeback) == 1) {
		LOAD_SYMBOL(UpdateServiceGroup,bres->SOHandle, "UpdateServiceGroup");
		UpdateServiceGroup(&svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)], bres->cfgfile);
		
	}
	BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcgrpmap[Z_LVAL_P(bartlby_servicegroup_id)].servicegroup_id ,BARTLBY_AUDIT_TYPE_SERVICEGROUP, "Toggled servicegroup active state");		
	
	RETURN_LONG(r);
		
}



