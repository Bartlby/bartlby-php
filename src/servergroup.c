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

PHP_FUNCTION(bartlby_get_servergroup_by_id) {
	zval * zbartlby_resource;
	zval * servergroup_id;
	
	char * dlmsg;
	int ret;
	int (*GetServerGroupById)(long,struct servergroup *, char *);
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
	
	long ret;
	
	long (*AddServerGroup)(struct servergroup *,char *);
	
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
	
	int (*DeleteServerGroup)(long, char*);
	
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
	long ret;
	long (*ServerGroupChangeId)(long, long, char*);
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


