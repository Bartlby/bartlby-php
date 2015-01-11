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
	
	int (*GetServerById)(long,struct server *, char *);
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
		add_assoc_string(return_value, "web_hooks", svc.web_hooks, 1);
		add_assoc_string(return_value, "json_endpoint", svc.json_endpoint, 1);
		
		add_assoc_string(return_value, "server_ssh_keyfile", svc.server_ssh_keyfile, 1);
		add_assoc_string(return_value, "server_ssh_passphrase", svc.server_ssh_passphrase, 1);
		add_assoc_string(return_value, "server_ssh_username", svc.server_ssh_username, 1);
		add_assoc_string(return_value, "enabled_triggers", svc.enabled_triggers, 1);
		
		add_assoc_string(return_value, "server_icon", svc.server_icon, 1);
		add_assoc_long(return_value, "server_port",svc.client_port);
		add_assoc_long(return_value, "orch_id",svc.orch_id);
		add_assoc_long(return_value, "server_id",Z_LVAL_P(server_id));

		add_assoc_long(return_value, "web_hooks_level",svc.web_hooks_level);

		
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
	zval * web_hooks;
	zval * json_endpoint;
	zval * web_hooks_level;

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

	GETARRAY_EL_FROM_HASH(web_hooks_level, "web_hooks_level", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG,0);

	GETARRAY_EL_FROM_HASH(exec_plan, "exec_plan", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(web_hooks, "web_hooks", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(json_endpoint, "json_endpoint", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");

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
	convert_to_long(web_hooks_level);
	
	convert_to_string(server_ssh_keyfile);
	convert_to_string(server_ssh_passphrase);
	convert_to_string(server_ssh_username);
	convert_to_string(web_hooks);
	convert_to_string(json_endpoint);
	
	
	
	
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
	srv.web_hooks_level=Z_LVAL_P(web_hooks_level);
	strcpy(srv.server_ssh_keyfile, Z_STRVAL_P(server_ssh_keyfile));
	strcpy(srv.server_ssh_passphrase, Z_STRVAL_P(server_ssh_passphrase));
	strcpy(srv.server_ssh_username, Z_STRVAL_P(server_ssh_username));
	strcpy(srv.exec_plan, Z_STRVAL_P(exec_plan));
	strcpy(srv.web_hooks, Z_STRVAL_P(web_hooks));
	strcpy(srv.json_endpoint, Z_STRVAL_P(json_endpoint));

	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,Z_LVAL_P(server_id), BARTLBY_SERVER_GONE, BARTLBY_OBJECT_CHANGED);
	
	ret=ModifyServer(&srv, bres->cfgfile);
	
	
	RETURN_LONG(ret);
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
	zval * web_hooks;
	zval * json_endpoint;
	zval * web_hooks_level;

	zval ** temp_pp;
	zval * options_array;
	
	
	char * dlmsg;
	
	long ret;
	
	long (*AddServer)(struct server *,char *);
	
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
	GETARRAY_EL_FROM_HASH(web_hooks, "web_hooks", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(json_endpoint, "json_endpoint", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_STRING, "");
	GETARRAY_EL_FROM_HASH(web_hooks_level, "web_hooks_level", temp_pp, options_array,BARTLBY_FIELD_REQUIRED,BARTLBY_DEFAULT_LONG, 0);

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
	convert_to_long(web_hooks_level);
	convert_to_string(enabled_triggers);
	convert_to_string(exec_plan);
	convert_to_string(web_hooks);
	convert_to_string(json_endpoint);
	
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
	srv.web_hooks_level=Z_LVAL_P(web_hooks_level);

	strcpy(srv.server_ssh_keyfile, Z_STRVAL_P(server_ssh_keyfile));
	strcpy(srv.server_ssh_passphrase, Z_STRVAL_P(server_ssh_passphrase));
	strcpy(srv.server_ssh_username, Z_STRVAL_P(server_ssh_username));
	strcpy(srv.exec_plan, Z_STRVAL_P(exec_plan));
	strcpy(srv.web_hooks, Z_STRVAL_P(web_hooks));
	strcpy(srv.json_endpoint, Z_STRVAL_P(json_endpoint));
	
	ret=AddServer(&srv, bres->cfgfile);
	
	BARTLBY_OBJECT_GONE(zbartlby_resource, bres,ret, BARTLBY_SERVER_GONE, BARTLBY_OBJECT_ADDED);
	
	RETURN_LONG(ret);
}

PHP_FUNCTION(bartlby_delete_server) {
	
	zval * zbartlby_resource;
	zval * server_id;
	
	char * dlmsg;
	int ret;
	int (*DeleteServer)(long, char*);
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
	add_assoc_string(return_value, "web_hooks", srvmap[Z_LVAL_P(bartlby_server_id)].web_hooks, 1);
	add_assoc_string(return_value, "json_endpoint", srvmap[Z_LVAL_P(bartlby_server_id)].json_endpoint, 1);

	add_assoc_long(return_value, "web_hooks_level",srvmap[Z_LVAL_P(bartlby_server_id)].web_hooks_level);
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



PHP_FUNCTION(bartlby_set_server_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	void * SOHandle;
	char * dlmsg;
	
	long ret;
	
	long (*ServerChangeId)(long, long, int, char*);
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


