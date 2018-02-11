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

PHP_FUNCTION(bartlby_get_downtime_by_id) {
	zval * zbartlby_resource;
	zval * downtime_id;
	
	char * dlmsg;
	int ret;
	int (*GetDowntimeById)(long,struct downtime *, char *);
	struct downtime  svc;
	
	bartlby_res * bres;

	
	ZEND_PARSE_PARAMETERS_START(2,2)
		Z_PARAM_RESOURCE(zbartlby_resource)
		Z_PARAM_ZVAL(downtime_id)		
	ZEND_PARSE_PARAMETERS_END(); 

	
	convert_to_long(downtime_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	


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
		add_assoc_string(return_value, "downtime_notice", svc.downtime_notice);

	}
		
}

PHP_FUNCTION(bartlby_get_downtime) {


	struct shm_header * shm_hdr;
	struct downtime * dtmap;
	int is_down;
	int current_time;
	int dtmapindex;


	int y, z, u;
	zval * zbartlby_resource;
	zval * bartlby_downtime_id;
	bartlby_res * bres;
	
	
	ZEND_PARSE_PARAMETERS_START(2,2)
		Z_PARAM_RESOURCE(zbartlby_resource)
		Z_PARAM_ZVAL(bartlby_downtime_id)		
	ZEND_PARSE_PARAMETERS_END(); 

	convert_to_long(bartlby_downtime_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	
	
	dtmap=bartlby_SHM_DowntimeMap(bres->bartlby_address);
	
	
	
	
	
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
	add_assoc_string(return_value, "downtime_notice", dtmap[Z_LVAL_P(bartlby_downtime_id)].downtime_notice);

}

PHP_FUNCTION(bartlby_delete_downtime) {
	zval * zbartlby_resource;
	zval * downtime_id;
	
	char * dlmsg;
	int ret;
	int (*DeleteDowntime)(long, char*);
	struct service svc;
	bartlby_res * bres;


	
	ZEND_PARSE_PARAMETERS_START(2,2)
		Z_PARAM_RESOURCE(zbartlby_resource)
		Z_PARAM_ZVAL(downtime_id)		
	ZEND_PARSE_PARAMETERS_END(); 


	convert_to_long(downtime_id);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
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


	
	
	ZEND_PARSE_PARAMETERS_START(3,3)
		Z_PARAM_RESOURCE(zbartlby_resource)
		Z_PARAM_ZVAL(downtime_id)
		Z_PARAM_ARRAY(options_array)		
	ZEND_PARSE_PARAMETERS_END(); 

	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}


	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	
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
	
	strncpy(svc.downtime_notice, Z_STRVAL_P(downtime_notice),2047);
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
	long ret;
	long (*AddDowntime)(struct downtime *,char *);
	struct downtime svc;
	bartlby_res * bres;	

	
	ZEND_PARSE_PARAMETERS_START(2,2)
		Z_PARAM_RESOURCE(zbartlby_resource)
		Z_PARAM_ARRAY(options_array)		
	ZEND_PARSE_PARAMETERS_END(); 

	
	if(Z_TYPE_P(options_array) != IS_ARRAY) {
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "SECOND parameter needs to be array object");
		RETURN_BOOL(0);
	}

	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	
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
	
	strncpy(svc.downtime_notice, Z_STRVAL_P(downtime_notice), 2047);
	svc.downtime_from=Z_LVAL_P(downtime_from);
	svc.downtime_to=Z_LVAL_P(downtime_to);
	svc.downtime_type=Z_LVAL_P(downtime_type);
	svc.service_id=Z_LVAL_P(downtime_service);
	svc.orch_id=Z_LVAL_P(orch_id);
	ret=AddDowntime(&svc, bres->cfgfile);
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,ret, BARTLBY_DOWNTIME_GONE, BARTLBY_OBJECT_ADDED);

	

	RETURN_LONG(ret);	
}


PHP_FUNCTION(bartlby_set_downtime_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	char * dlmsg;
	long ret;
	long (*DowntimeChangeId)(long, long, char*);
	
	bartlby_res * bres;

	

	ZEND_PARSE_PARAMETERS_START(3,3)
		Z_PARAM_RESOURCE(zbartlby_resource)
		Z_PARAM_ZVAL(from)		
		Z_PARAM_ZVAL(to)
	ZEND_PARSE_PARAMETERS_END(); 

	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	convert_to_long(from);
	convert_to_long(to);
	
	
	LOAD_SYMBOL(DowntimeChangeId,bres->SOHandle, "DowntimeChangeId");
	
	ret=DowntimeChangeId(Z_LVAL_P(from),Z_LVAL_P(to),bres->cfgfile);
	
	RETURN_LONG(ret);	
}
