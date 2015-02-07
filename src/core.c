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

extern int le_bartlby;


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
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	my_size=sizeof(struct shm_header)+ sizeof(struct server) + sizeof(struct worker)+sizeof(struct service)+sizeof(struct downtime)+sizeof(struct servergroup)+sizeof(struct servicegroup)+sizeof(struct trap)+sizeof(struct btl_event)+sizeof(struct trigger);
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
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
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
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
		
	
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
		php_error(E_WARNING, "Service id out of bounds");	
		RETURN_FALSE;	
	}
		
	svcmap[Z_LVAL_P(bartlby_service_id)].last_state = svcmap[Z_LVAL_P(bartlby_service_id)].current_state;
	svcmap[Z_LVAL_P(bartlby_service_id)].current_state = Z_LVAL_P(bartlby_new_state);	
	snprintf(svcmap[Z_LVAL_P(bartlby_service_id)].current_output, 2040, "%s",Z_STRVAL_P(bartlby_new_output)); 
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
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
		
		
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
		shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);

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
				dlclose(SOHandle);
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
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
		
		
	if(Z_LVAL_P(bartlby_service_id) > shm_hdr->svccount-1) {
		php_error(E_WARNING, "Service id out of bounds");	
		RETURN_FALSE;	
	}
	svcmap[Z_LVAL_P(bartlby_service_id)].do_force = 1;	
	r=1;
	BARTLBY_GENERIC_AUDIT(zbartlby_resource, svcmap[Z_LVAL_P(bartlby_service_id)].service_id, BARTLBY_AUDIT_TYPE_SERVICE, "Forced a Check");
	RETURN_LONG(r);
		
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
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	shm_hdr->do_reload=1;
		
					
		
		
	RETURN_TRUE;	
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
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	
	add_assoc_long(return_value, "services", shm_hdr->svccount);
	add_assoc_long(return_value, "workers", shm_hdr->wrkcount);
	add_assoc_long(return_value, "server", shm_hdr->srvcount);
	add_assoc_long(return_value, "downtimes", shm_hdr->dtcount);
	add_assoc_long(return_value, "servergroups", shm_hdr->srvgroupcount);
	add_assoc_long(return_value, "servicegroups", shm_hdr->svcgroupcount);
	add_assoc_long(return_value, "traps", shm_hdr->trapcount);

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


PHP_FUNCTION(bartlby_get_object_by_id) {

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
	zval * zbartlby_resource;
	zval * object_type;
	zval * object_id;

	bartlby_res * bres;
	

	struct shm_header * shm_hdr;
	struct service * svcmap;
	struct server * srvmap;	
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	struct trap * trapmap;
	struct trigger * triggermap;
	
	int x;
	int use_idx=-1;
	
	int res;
	
	

	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &object_type, &object_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	INIT_ZVAL(function_name);
	INIT_ZVAL(id);
	

	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
	wrkmap=bartlby_SHM_WorkerMap(bres->bartlby_address);
	dtmap=bartlby_SHM_DowntimeMap(bres->bartlby_address);
	srvmap=bartlby_SHM_ServerMap(bres->bartlby_address);
	evntmap=bartlby_SHM_EventMap(bres->bartlby_address);
	srvgrpmap=bartlby_SHM_ServerGroupMap(bres->bartlby_address);
	svcgrpmap=bartlby_SHM_ServiceGroupMap(bres->bartlby_address);
	trapmap=bartlby_SHM_TrapMap(bres->bartlby_address);
	triggermap=bartlby_SHM_TriggerMap(bres->bartlby_address);
	
	
	switch(Z_LVAL_P(object_type)) {
		case BARTLBY_OBJECT_SERVICE:
				for(x=0; x<shm_hdr->svccount; x++) {
					if(svcmap[x].service_id == Z_LVAL_P(object_id)) {
						use_idx=x;
						break;
					}	
				}	
		ZVAL_STRING(&function_name, "bartlby_get_service", 1);
		break;
		case BARTLBY_OBJECT_SERVER:
				for(x=0; x<shm_hdr->srvcount; x++) {
					if(srvmap[x].server_id == Z_LVAL_P(object_id)) {
						use_idx=x;
						break;
					}	
				}	
		ZVAL_STRING(&function_name, "bartlby_get_server", 1);				
		break;
		case BARTLBY_OBJECT_WORKER:
				for(x=0; x<shm_hdr->wrkcount; x++) {
					if(wrkmap[x].worker_id == Z_LVAL_P(object_id)) {
						use_idx=x;
						break;
					}	
				}	
		ZVAL_STRING(&function_name, "bartlby_get_worker", 1);				
		break;
		case BARTLBY_OBJECT_DOWNTIME:
				for(x=0; x<shm_hdr->dtcount; x++) {
					if(dtmap[x].downtime_id == Z_LVAL_P(object_id)) {
						use_idx=x;
						break;
					}	
				}	
		ZVAL_STRING(&function_name, "bartlby_get_downtime", 1);				
		break;		
		case BARTLBY_OBJECT_TRAP:
				for(x=0; x<shm_hdr->trapcount; x++) {
					if(trapmap[x].trap_id == Z_LVAL_P(object_id)) {
						use_idx=x;
						break;
					}	
				}	
		ZVAL_STRING(&function_name, "bartlby_get_trap", 1);				
		break;		
		case BARTLBY_OBJECT_TRIGGER:
				for(x=0; x<shm_hdr->triggercount; x++) {
					if(triggermap[x].trigger_id == Z_LVAL_P(object_id)) {
						use_idx=x;
						break;
					}	
				}	
		ZVAL_STRING(&function_name, "bartlby_get_trigger", 1);				
		break;		
		case BARTLBY_OBJECT_SERVERGROUP:
				for(x=0; x<shm_hdr->srvgroupcount; x++) {
					if(srvgrpmap[x].servergroup_id == Z_LVAL_P(object_id)) {
						use_idx=x;
						break;
					}	
				}	
		ZVAL_STRING(&function_name, "bartlby_get_servergroup", 1);				
		break;	
		case BARTLBY_OBJECT_SERVICEGROUP:
				for(x=0; x<shm_hdr->svcgroupcount; x++) {
					if(svcgrpmap[x].servicegroup_id == Z_LVAL_P(object_id)) {
						use_idx=x;
						break;
					}	
				}	
		ZVAL_STRING(&function_name, "bartlby_get_servicegroup", 1);
		break;				
	}


	if(use_idx < 0) RETURN_FALSE;


	
	ZVAL_LONG(&id, use_idx);
	
	
	params[0] = &zbartlby_resource;
	t=&id;
	params[1] = &t;
	
	
	res=call_user_function_ex(NULL, NULL, &function_name, &return_user_call, 2, params, 0, NULL TSRMLS_CC);
	
	

	RETVAL_ZVAL(return_user_call, 1,1);

	

	

	

}
