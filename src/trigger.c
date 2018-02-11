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

PHP_FUNCTION(bartlby_get_trigger) {

	struct shm_header * shm_hdr;
	struct trigger * triggermap;
	int is_down;
	int current_time;
	int dtmapindex;


	int y, z, u;
	zval * zbartlby_resource;
	zval * bartlby_trigger_id;
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &bartlby_trigger_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_long(bartlby_trigger_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	triggermap=bartlby_SHM_TriggerMap(bres->bartlby_address);
	
	if(Z_LVAL_P(bartlby_trigger_id) > shm_hdr->triggercount-1) {
		php_error(E_WARNING, "Trigger id out of bounds");	
		RETURN_FALSE;	
	}


	add_assoc_long(return_value, "trigger_id", triggermap[Z_LVAL_P(bartlby_trigger_id)].trigger_id);
	add_assoc_long(return_value, "trigger_count", triggermap[Z_LVAL_P(bartlby_trigger_id)].trigger_count);
	add_assoc_string(return_value, "trigger_name", triggermap[Z_LVAL_P(bartlby_trigger_id)].trigger_name);
	add_assoc_long(return_value, "trigger_enabled", triggermap[Z_LVAL_P(bartlby_trigger_id)].trigger_enabled);
	add_assoc_long(return_value, "trigger_type", triggermap[Z_LVAL_P(bartlby_trigger_id)].trigger_type);

	add_assoc_string(return_value, "trigger_data", triggermap[Z_LVAL_P(bartlby_trigger_id)].trigger_data);
	add_assoc_string(return_value, "trigger_execplan", triggermap[Z_LVAL_P(bartlby_trigger_id)].trigger_execplan);
	add_assoc_string(return_value, "trigger_full_path", triggermap[Z_LVAL_P(bartlby_trigger_id)].trigger_full_path);

	add_assoc_long(return_value, "orch_id", triggermap[Z_LVAL_P(bartlby_trigger_id)].orch_id);
	add_assoc_long(return_value, "is_gone", triggermap[Z_LVAL_P(bartlby_trigger_id)].is_gone);


}



PHP_FUNCTION(bartlby_get_trigger_by_id) {
	zval * zbartlby_resource;
	zval * trigger_id;
	
	char * dlmsg;
	int ret;
	int (*GetTriggerById)(long,struct trigger *, char *);
	struct trigger  svc;
	
	bartlby_res * bres;

	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&trigger_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(trigger_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	


	LOAD_SYMBOL(GetTriggerById,bres->SOHandle, "GetTriggerById");
	ret=GetTriggerById(Z_LVAL_P(trigger_id),&svc, bres->cfgfile);
	
	if(ret < 0) {
		RETURN_FALSE;	
	} else {
		if (array_init(return_value) == FAILURE) {
			RETURN_FALSE;
		}
				add_assoc_long(return_value, "trigger_id", svc.trigger_id);
				add_assoc_long(return_value, "trigger_count", svc.trigger_count);
				add_assoc_string(return_value, "trigger_name", svc.trigger_name);
				add_assoc_long(return_value, "trigger_enabled", svc.trigger_enabled);
				add_assoc_long(return_value, "trigger_type", svc.trigger_type);

				add_assoc_string(return_value, "trigger_data", svc.trigger_data);
				add_assoc_string(return_value, "trigger_execplan", svc.trigger_execplan);
				add_assoc_string(return_value, "trigger_full_path", svc.trigger_full_path);

				add_assoc_long(return_value, "orch_id", svc.orch_id);
				add_assoc_long(return_value, "is_gone", svc.is_gone);

	}
		
}


PHP_FUNCTION(bartlby_add_trigger) {
	zval * zbartlby_resource;
	zval * trigger_name;
	zval * trigger_enabled;
	zval * trigger_type;
	zval * trigger_data;
	zval * trigger_execplan;
	zval * orch_id;
	
	
	
	zval ** temp_pp;
	zval * options_array;
	
	char * dlmsg;
	
	int ret;
	
	long (*AddTrigger)(struct trigger *,char *);
	
	struct trigger svc;
	bartlby_res * bres;
	
	if(ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}
	
	GETARRAY_EL_FROM_HASH(trigger_name, "trigger_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trigger_data, "trigger_data", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_data");
	GETARRAY_EL_FROM_HASH(trigger_execplan, "trigger_execplan", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, " ");
	GETARRAY_EL_FROM_HASH(trigger_type, "trigger_type", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(trigger_enabled, "trigger_enabled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);

	

	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	convert_to_string(trigger_name);
	convert_to_string(trigger_data);
	convert_to_string(trigger_execplan);
	


	convert_to_long(trigger_enabled);
	convert_to_long(trigger_type);
	convert_to_long(orch_id);
	
	
	
	
	LOAD_SYMBOL(AddTrigger,bres->SOHandle, "AddTrigger");
	
	strncpy(svc.trigger_name, Z_STRVAL_P(trigger_name), 1023);
	strncpy(svc.trigger_data, Z_STRVAL_P(trigger_data), 1023);
	strncpy(svc.trigger_execplan, Z_STRVAL_P(trigger_data), 1023);

	svc.trigger_enabled=Z_LVAL_P(trigger_enabled);
	svc.trigger_type=Z_LVAL_P(trigger_type);
	svc.orch_id=Z_LVAL_P(orch_id);
	

	
	ret=AddTrigger(&svc, bres->cfgfile);
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,ret, BARTLBY_TRIGGER_GONE, BARTLBY_OBJECT_ADDED);
	RETURN_LONG(ret);	
}



PHP_FUNCTION(bartlby_modify_trigger) {
	zval * zbartlby_resource;
	zval * trigger_name;
	zval * trigger_enabled;
	zval * trigger_type;
	zval * trigger_data;
	zval * trigger_execplan;
	zval * trigger_id;	
	zval * orch_id;	


	zval ** temp_pp;
	zval * options_array;
	char * dlmsg;
	
	int ret;
	
	int (*UpdateTrigger)(struct trigger *,char *);
	
	struct trigger svc;
	bartlby_res * bres;
	
	
	
	if(ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&trigger_id, &options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}

	GETARRAY_EL_FROM_HASH(trigger_name, "trigger_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trigger_data, "trigger_data", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_data");
	GETARRAY_EL_FROM_HASH(trigger_execplan, "trigger_execplan", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, " ");
	GETARRAY_EL_FROM_HASH(trigger_type, "trigger_type", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(trigger_enabled, "trigger_enabled", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);

	
	
	
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	convert_to_string(trigger_name);
	convert_to_string(trigger_data);
	convert_to_string(trigger_execplan);
	


	convert_to_long(trigger_enabled);
	convert_to_long(trigger_type);
	convert_to_long(orch_id);
		
	
	
	
	LOAD_SYMBOL(UpdateTrigger,bres->SOHandle, "UpdateTrigger");

	strncpy(svc.trigger_name, Z_STRVAL_P(trigger_name), 1023);
	strncpy(svc.trigger_data, Z_STRVAL_P(trigger_data), 1023);
	strncpy(svc.trigger_execplan, Z_STRVAL_P(trigger_execplan), 1023);

	svc.trigger_id=Z_LVAL_P(trigger_id);
	svc.trigger_enabled=Z_LVAL_P(trigger_enabled);
	svc.trigger_type=Z_LVAL_P(trigger_type);
	svc.orch_id=Z_LVAL_P(orch_id);
	
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,svc.trigger_id, BARTLBY_TRIGGER_GONE, BARTLBY_OBJECT_CHANGED);
	ret=UpdateTrigger(&svc, bres->cfgfile);
	
	

	RETURN_LONG(ret);		
}


PHP_FUNCTION(bartlby_delete_trigger) {
	zval * zbartlby_resource;
	zval * trigger_id;
	
	
	char * dlmsg;
	
	int ret;
	
	int (*DeleteTrigger)(long, char*);
	
	struct trigger svc;
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&trigger_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	convert_to_long(trigger_id);
	
	
	
	LOAD_SYMBOL(DeleteTrigger,bres->SOHandle, "DeleteTrigger");
	
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(trigger_id), BARTLBY_TRIGGER_GONE, BARTLBY_OBJECT_DELETED);
	
	ret=DeleteTrigger(Z_LVAL_P(trigger_id),bres->cfgfile);

	

	RETURN_LONG(ret);	
}
PHP_FUNCTION(bartlby_set_trigger_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	
	char * dlmsg;
	long ret;
	long (*TriggerChangeId)(long, long, char*);
	bartlby_res * bres;
	
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&from, &to)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(from);
	convert_to_long(to);
	
	

	LOAD_SYMBOL(TriggerChangeId,bres->SOHandle, "TriggerChangeId");
	
	
	
	ret=TriggerChangeId(Z_LVAL_P(from),Z_LVAL_P(to),bres->cfgfile);
	RETURN_LONG(ret);	
}

