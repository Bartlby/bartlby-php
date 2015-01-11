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

PHP_FUNCTION(bartlby_get_servicegroup_by_id) {
  zval * zbartlby_resource;
  zval * servicegroup_id;
  
  char * dlmsg;
  int ret;
  int (*GetServicegroupById)(long,struct servicegroup *, char *);
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
	struct downtime * dtmap;
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
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	dtmap=bartlby_SHM_DowntimeMap(bres->bartlby_address);
	svcgrpmap=bartlby_SHM_ServiceGroupMap(bres->bartlby_address);
	
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
	long ret;
	long (*AddServiceGroup)(struct servicegroup *,char *);
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
	int (*DeleteServiceGroup)(long, char*);
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
	long ret;
	long (*ServiceGroupChangeId)(long, long, char*);
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
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
		
	svcgrpmap=bartlby_SHM_ServiceGroupMap(bres->bartlby_address);
	
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
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
		
	svcgrpmap=bartlby_SHM_ServiceGroupMap(bres->bartlby_address);
	
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

