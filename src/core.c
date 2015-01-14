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
	RETURN_STRING(BARTLBY_VERSION);	
	
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
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	my_size=sizeof(struct shm_header)+ sizeof(struct server) + sizeof(struct worker)+sizeof(struct service)+sizeof(struct downtime)+sizeof(struct servergroup)+sizeof(struct servicegroup)+sizeof(struct trap);
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
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
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
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
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
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
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

		ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
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
		add_assoc_string(return_value, "trigger_name", shm_hdr->notification_log[Z_LVAL_P(idx)].trigger_name);
		

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
  
  add_assoc_string(return_value, "name", GetName());
  add_assoc_string(return_value, "autor", GetAutor());
  add_assoc_string(return_value, "version", GetVersion()); 
	
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
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
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
	
	RETURN_STRING(Z_STRVAL_P(instr));
	
	
}

PHP_FUNCTION(bartlby_decode) {
	zval * instr;
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &instr)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_string(instr);
	xbartlby_decode(Z_STRVAL_P(instr), strlen(Z_STRVAL_P(instr)));
	
	RETURN_STRING(Z_STRVAL_P(instr));	
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
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
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
		RETURN_STRING(k);	
	}
	
	
}

PHP_FUNCTION(bartlby_get_info) {
	
	struct shm_header * shm_hdr;
	zval * zbartlby_resource;
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
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
	add_assoc_string(return_value, "version", shm_hdr->version);
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
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	LOAD_SYMBOL(GetAutor,bres->SOHandle, "GetAutor");
    LOAD_SYMBOL(GetVersion,bres->SOHandle, "GetVersion");
    LOAD_SYMBOL(GetName,bres->SOHandle, "GetName");
    	
	GetAutorStr=GetAutor();
    GetVersionStr=GetVersion();
    GetNameStr=GetName();
    	
    if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	add_assoc_string(return_value, "Autor", GetAutorStr);
	add_assoc_string(return_value, "Version", GetVersionStr);
	add_assoc_string(return_value, "Name", GetNameStr);
	
	free(GetAutorStr);
	free(GetVersionStr);
	free(GetNameStr);
		
}
