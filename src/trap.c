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

PHP_FUNCTION(bartlby_get_trap) {

	struct shm_header * shm_hdr;
	struct trap * trapmap;
	int is_down;
	int current_time;
	int dtmapindex;


	int y, z, u;
	zval * zbartlby_resource;
	zval * bartlby_trap_id;
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource, &bartlby_trap_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	convert_to_long(bartlby_trap_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	trapmap=bartlby_SHM_TrapMap(bres->bartlby_address);
	
	if(Z_LVAL_P(bartlby_trap_id) > shm_hdr->trapcount-1) {
		php_error(E_WARNING, "Trap id out of bounds");	
		RETURN_FALSE;	
	}

	add_assoc_long(return_value, "trap_id", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_id);
	add_assoc_string(return_value, "trap_name", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_name,1);
	add_assoc_string(return_value, "trap_catcher", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_catcher,1);
	add_assoc_string(return_value, "trap_status_text", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_status_text,1);
	add_assoc_string(return_value, "trap_status_ok", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_status_ok,1);
	add_assoc_string(return_value, "trap_status_warning", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_status_warning,1);
	add_assoc_string(return_value, "trap_status_critical", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_status_critical,1);
	add_assoc_string(return_value, "trap_last_data", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_last_data,1);
	add_assoc_long(return_value, "trap_service_id", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_service_id);
	add_assoc_long(return_value, "trap_fixed_status", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_fixed_status);
	add_assoc_long(return_value, "trap_prio", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_prio);
	add_assoc_long(return_value, "trap_is_final", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_is_final);
	add_assoc_long(return_value, "orch_id", trapmap[Z_LVAL_P(bartlby_trap_id)].orch_id);
	add_assoc_long(return_value, "service_shm_place", trapmap[Z_LVAL_P(bartlby_trap_id)].service_shm_place);
	add_assoc_long(return_value, "matched", trapmap[Z_LVAL_P(bartlby_trap_id)].matched);
	add_assoc_long(return_value, "is_gone", trapmap[Z_LVAL_P(bartlby_trap_id)].is_gone);
	add_assoc_long(return_value, "trap_last_match", trapmap[Z_LVAL_P(bartlby_trap_id)].trap_last_match);

}



PHP_FUNCTION(bartlby_get_trap_by_id) {
	zval * zbartlby_resource;
	zval * trap_id;
	
	char * dlmsg;
	int ret;
	int (*GetTrapById)(long,struct trap *, char *);
	struct trap  svc;
	
	bartlby_res * bres;

	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&trap_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	convert_to_long(trap_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	


	LOAD_SYMBOL(GetTrapById,bres->SOHandle, "GetTrapById");
	ret=GetTrapById(Z_LVAL_P(trap_id),&svc, bres->cfgfile);
	
	if(ret < 0) {
		RETURN_FALSE;	
	} else {
		if (array_init(return_value) == FAILURE) {
			RETURN_FALSE;
		}
			add_assoc_long(return_value, "trap_id", svc.trap_id);
			add_assoc_string(return_value, "trap_name", svc.trap_name,1);
			add_assoc_string(return_value, "trap_catcher", svc.trap_catcher,1);
			add_assoc_string(return_value, "trap_status_text", svc.trap_status_text,1);
			add_assoc_string(return_value, "trap_status_ok", svc.trap_status_ok,1);
			add_assoc_string(return_value, "trap_status_warning", svc.trap_status_warning,1);
			add_assoc_string(return_value, "trap_status_critical", svc.trap_status_critical,1);
			add_assoc_string(return_value, "trap_last_data", svc.trap_last_data,1);
			add_assoc_long(return_value, "trap_service_id", svc.trap_service_id);
			add_assoc_long(return_value, "trap_fixed_status", svc.trap_fixed_status);
			add_assoc_long(return_value, "trap_prio", svc.trap_prio);
			add_assoc_long(return_value, "trap_is_final", svc.trap_is_final);
			add_assoc_long(return_value, "orch_id", svc.orch_id);
			add_assoc_long(return_value, "trap_last_match", svc.trap_last_match);
	}
		
}


PHP_FUNCTION(bartlby_add_trap) {
	zval * zbartlby_resource;
	zval * trap_name;
	zval * trap_catcher;
	zval * trap_status_text;
	zval * trap_status_ok;
	zval * trap_status_warning;
	zval * trap_status_critical;
	zval * trap_service_id;
	zval * trap_fixed_status;
	zval * trap_prio;
	zval * trap_is_final;
	zval * orch_id;
	
	
	
	zval ** temp_pp;
	zval * options_array;
	
	char * dlmsg;
	
	int ret;
	
	long (*AddTrap)(struct trap *,char *);
	
	struct trap svc;
	bartlby_res * bres;
	
	if(ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}
	
	GETARRAY_EL_FROM_HASH(trap_name, "trap_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_catcher, "trap_catcher", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_status_text, "trap_status_text", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_status_ok, "trap_status_ok", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_status_warning, "trap_status_warning", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_status_critical, "trap_status_critical", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_service_id, "trap_service_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(trap_fixed_status, "trap_fixed_status", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(trap_prio, "trap_prio", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(trap_is_final, "trap_is_final", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);

	

	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	convert_to_string(trap_name);
	convert_to_string(trap_catcher);
	convert_to_string(trap_status_text);
	convert_to_string(trap_status_ok);
	convert_to_string(trap_status_warning);
	convert_to_string(trap_status_critical);
	


	convert_to_long(trap_service_id);
	convert_to_long(trap_fixed_status);
	convert_to_long(trap_prio);
	convert_to_long(trap_is_final);
	convert_to_long(orch_id);
	
	
	
	
	LOAD_SYMBOL(AddTrap,bres->SOHandle, "AddTrap");
	
	strncpy(svc.trap_name, Z_STRVAL_P(trap_name), 512);
	strncpy(svc.trap_catcher, Z_STRVAL_P(trap_catcher), 512);
	strncpy(svc.trap_status_text, Z_STRVAL_P(trap_status_text), 512);
	strncpy(svc.trap_status_ok, Z_STRVAL_P(trap_status_ok), 512);
	strncpy(svc.trap_status_warning, Z_STRVAL_P(trap_status_warning),512);
	strncpy(svc.trap_status_critical, Z_STRVAL_P(trap_status_critical),512);
	

	svc.trap_service_id=Z_LVAL_P(trap_service_id);
	svc.trap_fixed_status=Z_LVAL_P(trap_fixed_status);
	svc.trap_prio=Z_LVAL_P(trap_prio);
	svc.trap_is_final=Z_LVAL_P(trap_is_final);
	svc.orch_id=Z_LVAL_P(orch_id);
	

	
	ret=AddTrap(&svc, bres->cfgfile);
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,ret, BARTLBY_TRAP_GONE, BARTLBY_OBJECT_ADDED);
	RETURN_LONG(ret);	
}



PHP_FUNCTION(bartlby_modify_trap) {
	zval * zbartlby_resource;
	zval * trap_name;
	zval * trap_catcher;
	zval * trap_status_text;
	zval * trap_status_ok;
	zval * trap_status_warning;
	zval * trap_status_critical;
	zval * trap_service_id;
	zval * trap_fixed_status;
	zval * trap_prio;
	zval * trap_is_final;
	zval * orch_id;	
	zval * trap_id;
	


	zval ** temp_pp;
	zval * options_array;
	char * dlmsg;
	
	int ret;
	
	int (*UpdateTrap)(struct trap *,char *);
	
	struct trap svc;
	bartlby_res * bres;
	
	
	
	if(ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&trap_id, &options_array) == FAILURE) {
		WRONG_PARAM_COUNT;	
	}
	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}

	GETARRAY_EL_FROM_HASH(trap_name, "trap_name", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_catcher, "trap_catcher", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_status_text, "trap_status_text", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_status_ok, "trap_status_ok", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_status_warning, "trap_status_warning", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_status_critical, "trap_status_critical", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "default_name");
	GETARRAY_EL_FROM_HASH(trap_service_id, "trap_service_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(trap_fixed_status, "trap_fixed_status", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(trap_prio, "trap_prio", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(trap_is_final, "trap_is_final", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	GETARRAY_EL_FROM_HASH(orch_id, "orch_id", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 1);
	
	
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	convert_to_string(trap_name);
	convert_to_string(trap_catcher);
	convert_to_string(trap_status_text);
	convert_to_string(trap_status_ok);
	convert_to_string(trap_status_warning);
	convert_to_string(trap_status_critical);
	

	convert_to_long(trap_service_id);
	convert_to_long(trap_fixed_status);
	convert_to_long(trap_prio);
	convert_to_long(trap_is_final);
	convert_to_long(orch_id);
	
		
	
	convert_to_long(trap_id);
	
	
	
	
	LOAD_SYMBOL(UpdateTrap,bres->SOHandle, "UpdateTrap");

	strncpy(svc.trap_name, Z_STRVAL_P(trap_name),512);
	strncpy(svc.trap_catcher, Z_STRVAL_P(trap_catcher),512);
	strncpy(svc.trap_status_text, Z_STRVAL_P(trap_status_text),512);
	strncpy(svc.trap_status_ok, Z_STRVAL_P(trap_status_ok), 512);
	strncpy(svc.trap_status_warning, Z_STRVAL_P(trap_status_warning),512);
	strncpy(svc.trap_status_critical, Z_STRVAL_P(trap_status_critical),512);
	

	svc.trap_service_id=Z_LVAL_P(trap_service_id);
	svc.trap_fixed_status=Z_LVAL_P(trap_fixed_status);
	svc.trap_prio=Z_LVAL_P(trap_prio);
	svc.trap_is_final=Z_LVAL_P(trap_is_final);
	svc.orch_id=Z_LVAL_P(orch_id);	
	svc.trap_id=Z_LVAL_P(trap_id);	
	
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,svc.trap_id, BARTLBY_TRAP_GONE, BARTLBY_OBJECT_CHANGED);
	ret=UpdateTrap(&svc, bres->cfgfile);
	
	

	RETURN_LONG(ret);		
}


PHP_FUNCTION(bartlby_delete_trap) {
	zval * zbartlby_resource;
	zval * trap_id;
	
	
	char * dlmsg;
	
	int ret;
	
	int (*DeleteTrap)(long, char*);
	
	struct trap svc;
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 2 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &zbartlby_resource,&trap_id)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	
	convert_to_long(trap_id);
	
	
	
	LOAD_SYMBOL(DeleteTrap,bres->SOHandle, "DeleteTrap");
	
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(trap_id), BARTLBY_TRAP_GONE, BARTLBY_OBJECT_DELETED);
	
	ret=DeleteTrap(Z_LVAL_P(trap_id),bres->cfgfile);

	

	RETURN_LONG(ret);	
}
PHP_FUNCTION(bartlby_set_trap_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	
	char * dlmsg;
	long ret;
	long (*TrapChangeId)(long, long, char*);
	bartlby_res * bres;
	
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource,&from, &to)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	convert_to_long(from);
	convert_to_long(to);
	
	

	LOAD_SYMBOL(TrapChangeId,bres->SOHandle, "TrapChangeId");
	
	
	
	ret=TrapChangeId(Z_LVAL_P(from),Z_LVAL_P(to),bres->cfgfile);
	RETURN_LONG(ret);	
}

