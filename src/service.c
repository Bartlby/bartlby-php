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

PHP_FUNCTION(bartlby_get_service_by_id) {
	zval * zbartlby_resource;
	zval * service_id;
	
	char * dlmsg;
	
	int ret;
	bartlby_res * bres;
	
	int (*GetServiceById)(long,struct service *, char *);
	
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
	
		
	
	
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
	wrkmap=bartlby_SHM_WorkerMap(bres->bartlby_address);
	dtmap=bartlby_SHM_DowntimeMap(bres->bartlby_address);
	srvmap=bartlby_SHM_ServerMap(bres->bartlby_address);
	evntmap=bartlby_SHM_EventMap(bres->bartlby_address);
	srvgrpmap=bartlby_SHM_ServerGroupMap(bres->bartlby_address);
	svcgrpmap=bartlby_SHM_ServiceGroupMap(bres->bartlby_address);
		
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

PHP_FUNCTION(bartlby_add_service) {
	struct service svc;
	char * dlmsg;
	int ret;
	int rtc;
	
	long (*AddService)(struct service *, char *);
	
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

PHP_FUNCTION(bartlby_delete_service) {
	zval * zbartlby_resource;
	zval * service_id;
	
	char * dlmsg;
	int ret;
	int (*DeleteService)(long, char*);
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
	

	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
		
		
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
	
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
		
		
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
	
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
		
		
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
	
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
		
		
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

PHP_FUNCTION(bartlby_set_service_id) {
	zval * zbartlby_resource;
	zval * from;
	zval * to;
	zval * mig;
	
	char * dlmsg;
	
	long ret;
	
	long (*ServiceChangeId)(long, long,  char*);
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
	
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
		
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
	
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
		
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
	
	
	
		
	
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
		
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



