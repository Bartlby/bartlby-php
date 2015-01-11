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
	long ret;
	long (*AddWorker)(struct worker *,char *);
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
	
	strncpy(svc.password, Z_STRVAL_P(password), 2047);
	strncpy(svc.name, Z_STRVAL_P(name), 2047);
	strncpy(svc.notify_plan, Z_STRVAL_P(notify_plan), 2047);
	strncpy(svc.mail, Z_STRVAL_P(mail), 2047);
	strncpy(svc.icq, Z_STRVAL_P(icq), 2047);
	strncpy(svc.selected_services, Z_STRVAL_P(selected_services), 2047);
	strncpy(svc.selected_servers, Z_STRVAL_P(selected_servers), 2047);
	strncpy(svc.visible_servers, Z_STRVAL_P(visible_servers), 2047);
	strncpy(svc.visible_services, Z_STRVAL_P(visible_services), 2047);
	strncpy(svc.notify_levels, Z_STRVAL_P(notify_levels), 19);
	strncpy(svc.enabled_triggers, Z_STRVAL_P(enabled_triggers), 2047);
	strncpy(svc.api_pubkey, Z_STRVAL_P(api_pubkey), 254);
	strncpy(svc.api_privkey, Z_STRVAL_P(api_privkey), 254);
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
	int (*DeleteWorker)(long, char*);
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
	
	strncpy(svc.notify_plan, Z_STRVAL_P(notify_plan), 2047);
	strncpy(svc.password, Z_STRVAL_P(password), 2047);
	strncpy(svc.name, Z_STRVAL_P(name), 2047);
	strncpy(svc.mail, Z_STRVAL_P(mail), 2047);
	strncpy(svc.icq, Z_STRVAL_P(icq), 2047);
	strncpy(svc.selected_services, Z_STRVAL_P(selected_services), 2047);
	strncpy(svc.selected_servers, Z_STRVAL_P(selected_servers), 2047);
	strncpy(svc.visible_servers, Z_STRVAL_P(visible_servers), 2047);
	strncpy(svc.visible_services, Z_STRVAL_P(visible_services), 2047);

	strncpy(svc.notify_levels, Z_STRVAL_P(notify_levels), 2047);
	strncpy(svc.enabled_triggers, Z_STRVAL_P(enabled_triggers), 2047);

	strncpy(svc.api_pubkey, Z_STRVAL_P(api_pubkey), 254);
	strncpy(svc.api_privkey, Z_STRVAL_P(api_privkey), 254);

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
	int (*GetWorkerById)(long,struct worker *, char *);
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


PHP_FUNCTION(bartlby_get_worker) {
	struct shm_header * shm_hdr;
	
	
	struct worker * wrkmap;
	
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
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	wrkmap=bartlby_SHM_WorkerMap(bres->bartlby_address);
		
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
PHP_FUNCTION(bartlby_set_worker_state) {
	zval * zbartlby_resource;
	zval * bartlby_worker_id;
	zval * new_state;
	
	struct shm_header * shm_hdr;
	int r;
	struct worker * wrkmap;
	
	char * dlmsg;
	int (*UpdateWorker)(struct worker *, char *);
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &bartlby_worker_id, &new_state)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	convert_to_long(bartlby_worker_id);
	convert_to_long(new_state);
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	

	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	wrkmap=bartlby_SHM_WorkerMap(bres->bartlby_address);
		
	
		
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

PHP_FUNCTION(bartlby_set_worker_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	char * dlmsg;
	
	long ret;
	
	long (*WorkerChangeId)(long, long,  char*);
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

