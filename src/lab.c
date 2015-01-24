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

PHP_FUNCTION(bartlby_cleanup_tests) {
	zval * zbartlby_resource;
	bartlby_res * bres;
	char * dlmsg;
	int ret;
	int (*CleanupTestData)(char *);
	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);

	LOAD_SYMBOL(CleanupTestData,bres->SOHandle, "CleanupTestData");
	ret=CleanupTestData(bres->cfgfile);

	RETURN_LONG(ret);

}
PHP_FUNCTION(bartlby_callback_test) {

	zval * zbartlby_resource;
	bartlby_res * bres;

	
	if (ZEND_NUM_ARGS() != 1 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby_resource)==FAILURE) {
		WRONG_PARAM_COUNT;
	}
	
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);


	BARTLBY_OBJECT_AUDIT(zbartlby_resource, 1, 2222, 1);
	BARTLBY_OBJECT_AUDIT(zbartlby_resource, 2, 3333, 2);
	RETURN_LONG(1);
}



PHP_METHOD(Bartlby, testFunc) /* {{{ */
{
   
    RETURN_STRING("Hello World\n", 1);
}





PHP_FUNCTION(bartlby_svc_map_test) {
	zval * subarray;
	zval * howmuch;
	void * bartlby_address;
	struct shm_header * shm_hdr;
	
	int x;
	int y;
	int is_down;
	int current_time;
	int current_state;
	
	struct service * svcmap;
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct server * srvmap;
	struct btl_event * evntmap;
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	
	
	zval * zbartlby_resource;
	zval * svc_right_array;
	zval * server_right_array;
	
	zval * groups;
	zval * groupinfo;
	
	char * group_has_server;
	int is_member;
	int u;
	int z;
	int hh;
	
	bartlby_res * bres;
	
	if (ZEND_NUM_ARGS() != 3 || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rzz", &zbartlby_resource, &svc_right_array, &server_right_array)==FAILURE) {
		WRONG_PARAM_COUNT;
	}	
	//convert_to_long(howmuch);
	ZEND_FETCH_RESOURCE(bres, bartlby_res*, &zbartlby_resource, -1, BARTLBY_RES_NAME, le_bartlby);
	
	if (array_init(return_value) == FAILURE) {
		RETURN_FALSE;
	}
	
	bartlby_address=bres->bartlby_address;
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
	wrkmap=bartlby_SHM_WorkerMap(bres->bartlby_address);
	dtmap=bartlby_SHM_DowntimeMap(bres->bartlby_address);
	srvmap=bartlby_SHM_ServerMap(bres->bartlby_address);
	evntmap=bartlby_SHM_EventMap(bres->bartlby_address);
	srvgrpmap=bartlby_SHM_ServerGroupMap(bres->bartlby_address);
	svcgrpmap=bartlby_SHM_ServiceGroupMap(bres->bartlby_address);
			
			
			
	current_time=time(NULL);
	for(hh=0; hh<300; hh++) {
	for(x=0; x<shm_hdr->svccount; x++) {
		if(btl_is_array(svc_right_array, svcmap[x].service_id) == -1 &&  btl_is_array(server_right_array, svcmap[x].server_id) == -1) {
			continue;	
		}
		ALLOC_INIT_ZVAL(subarray);
		array_init(subarray);
		add_assoc_long(subarray, "service_id", svcmap[x].service_id);
		add_assoc_long(subarray, "server_id", svcmap[x].server_id);
		add_assoc_long(subarray, "last_state", svcmap[x].last_state);
			
			
		if(svcmap[x].is_server_dead < 0) {
			add_assoc_long(subarray, "current_state", -3);
			add_assoc_long(subarray, "server_is_dead", 1);
		} else {
			add_assoc_long(subarray, "current_state", svcmap[x].current_state);
		}
			
			
		add_assoc_long(subarray, "client_port", srvmap[svcmap[x].srv_place].client_port);
				
		add_assoc_string(subarray, "current_output", svcmap[x].current_output, 1);
		add_assoc_string(subarray, "service_name", svcmap[x].service_name, 1);
		add_assoc_string(subarray, "server_name", srvmap[svcmap[x].srv_place].server_name, 1);
		add_assoc_string(subarray, "client_ip", srvmap[svcmap[x].srv_place].client_ip, 1);
		add_assoc_string(subarray, "server_ssh_keyfile", srvmap[svcmap[x].srv_place].server_ssh_keyfile, 1);
		add_assoc_string(subarray, "server_ssh_passphrase", srvmap[svcmap[x].srv_place].server_ssh_passphrase, 1);
		add_assoc_string(subarray, "server_ssh_username", srvmap[svcmap[x].srv_place].server_ssh_username, 1);
		add_assoc_string(subarray, "server_enabled_triggers", srvmap[svcmap[x].srv_place].enabled_triggers, 1);
		add_assoc_string(subarray, "plugin", svcmap[x].plugin, 1);
		add_assoc_string(subarray, "plugin_arguments", svcmap[x].plugin_arguments, 1);
	
		add_assoc_long(subarray, "check_interval", svcmap[x].check_interval);
		add_assoc_long(subarray, "check_interval_original", svcmap[x].check_interval_original);
		add_assoc_long(subarray, "last_check", svcmap[x].last_check);
		
		add_assoc_string(subarray, "exec_plan", svcmap[x].service_exec_plan, 1);
		
		add_assoc_long(subarray, "notify_enabled", svcmap[x].notify_enabled);
		add_assoc_long(subarray, "last_notify_send", svcmap[x].last_notify_send);
		add_assoc_long(subarray, "last_state_change", svcmap[x].last_state_change);
		add_assoc_long(subarray, "flap_count", svcmap[x].flap_count);
		
		
		add_assoc_long(subarray, "service_active", svcmap[x].service_active);
		add_assoc_long(subarray, "service_type", svcmap[x].service_type);
		add_assoc_long(subarray, "service_passive_timeout", svcmap[x].service_passive_timeout);
		
		
		add_assoc_string(subarray, "service_var", svcmap[x].service_var, 1);
		add_assoc_string(subarray, "server_icon", srvmap[svcmap[x].srv_place].server_icon, 1);
		add_assoc_long(subarray, "service_check_timeout", svcmap[x].service_check_timeout);
		add_assoc_long(subarray, "service_ack_enabled", svcmap[x].service_ack_enabled);
		add_assoc_long(subarray, "service_ack_current", svcmap[x].service_ack_current);
		add_assoc_long(subarray, "service_retain", svcmap[x].service_retain);
		add_assoc_long(subarray, "service_retain_current", svcmap[x].service_retain_current);
		add_assoc_long(subarray, "check_is_running", svcmap[x].process.pid);
		add_assoc_long(subarray, "check_starttime", svcmap[x].process.start_time);
		
		add_assoc_long(subarray, "shm_place", x);
		
		add_assoc_long(subarray, "service_time_sum", svcmap[x].pstat.sum);
		add_assoc_long(subarray, "service_time_count",svcmap[x].pstat.counter);
	
		add_assoc_long(subarray, "service_delay_sum", svcmap[x].delay_time.sum);
		add_assoc_long(subarray, "service_delay_count",svcmap[x].delay_time.counter);
		
		//bartlby_get_service_by_id
		
		add_assoc_string(subarray, "service_snmp_community", svcmap[x].snmp_info.community, 1);
		add_assoc_string(subarray, "service_snmp_textmatch", svcmap[x].snmp_info.textmatch, 1);
		add_assoc_string(subarray, "service_snmp_objid", svcmap[x].snmp_info.objid, 1);
		
		add_assoc_long(subarray, "service_snmp_warning",svcmap[x].snmp_info.warn);
		add_assoc_long(subarray, "service_snmp_critical",svcmap[x].snmp_info.crit);
		add_assoc_long(subarray, "service_snmp_version",svcmap[x].snmp_info.version);
		add_assoc_long(subarray, "service_snmp_type",svcmap[x].snmp_info.type);
		
		
		add_assoc_long(subarray, "flap_seconds",svcmap[x].flap_seconds);
		
		add_assoc_long(subarray, "server_flap_seconds",srvmap[svcmap[x].srv_place].server_flap_seconds);
		add_assoc_long(subarray, "server_last_notify_send",srvmap[svcmap[x].srv_place].last_notify_send);
		add_assoc_long(subarray, "server_notify",srvmap[svcmap[x].srv_place].server_notify);
		add_assoc_long(subarray, "server_enabled",srvmap[svcmap[x].srv_place].server_enabled);
		add_assoc_long(subarray, "server_dead",srvmap[svcmap[x].srv_place].server_dead);
		
		
		//svc.renotify_interval, svc.escalate_divisor
		add_assoc_long(subarray, "renotify_interval",svcmap[x].renotify_interval);
		add_assoc_long(subarray, "escalate_divisor",svcmap[x].escalate_divisor);
		add_assoc_long(subarray, "fires_events",svcmap[x].fires_events);
		
		add_assoc_string(subarray, "enabled_triggers", svcmap[x].enabled_triggers, 1);
		
		add_assoc_long(subarray, "server_gone",srvmap[svcmap[x].srv_place].is_gone);
		
		//svc.is_gone
		add_assoc_long(subarray, "is_gone",svcmap[x].is_gone);
		
		
		for(y=0; y<shm_hdr->dtcount; y++) {
			is_down=0;
			if(current_time >= dtmap[y].downtime_from && current_time <= dtmap[y].downtime_to) {
				switch(dtmap[y].downtime_type) {
					case DT_SERVICE:
						if(dtmap[y].service_id == svcmap[x].service_id) {
							is_down=1;	
							
						}
					break;
					case DT_SERVER:
						if(dtmap[y].service_id == svcmap[x].server_id) {
							is_down=2;	
						}
					break;	
					case DT_SERVERGROUP:
						
						if(srvmap[svcmap[x].srv_place].servergroup_counter > 0) {
							for(u=0; u<srvmap[svcmap[x].srv_place].servergroup_counter; u++){
								z=srvmap[svcmap[x].srv_place].servergroup_place[u];
								if(srvgrpmap[z].servergroup_id == dtmap[y].service_id) {
										is_down=3;
										break;
								}
									
							}
						}
						
					break;		
					case DT_SERVICEGROUP:
						if(svcmap[x].servicegroup_counter > 0) {
							for(u=0; u<svcmap[x].servicegroup_counter; u++){
									z=svcmap[x].servicegroup_place[u];
									if(svcgrpmap[z].servicegroup_id == dtmap[y].service_id) {
										is_down=4;
										break;
									}
							}
						}
				
					
					break;	
				}
				if(is_down > 0) {
					add_assoc_long(subarray, "is_downtime", 1);
					add_assoc_long(subarray, "downtime_from", dtmap[y].downtime_from);
					add_assoc_long(subarray, "downtime_to", dtmap[y].downtime_to);
					add_assoc_string(subarray, "downtime_notice", dtmap[y].downtime_notice, 1);
					add_assoc_long(subarray, "downtime_service", dtmap[y].service_id);
					add_assoc_long(subarray, "downtime_type", dtmap[y].downtime_type);
					break;
				}
			}	
		}
		if(is_down==0) {
			add_assoc_long(subarray, "is_downtime", 0);	
		}
		
				//Resolve ServerGroups
		
		
		

	
					if(srvmap[svcmap[x].srv_place].servergroup_counter > 0) {
						ALLOC_INIT_ZVAL(groups);
						array_init(groups);
						for(y=0; y<srvmap[svcmap[x].srv_place].servergroup_counter; y++){
					
					
								ALLOC_INIT_ZVAL(groupinfo);
								array_init(groupinfo);
								
								z=srvmap[svcmap[x].srv_place].servergroup_place[y];
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
				
				
				
					add_assoc_zval(subarray, "servergroups", groups);
				}
					
			
			
	
			
			
			//Resolve Service Groups
			
			
				//is member of following groups #SERVICEGROUP


		
	
		
		if(svcmap[x].servicegroup_counter > 0) {
			ALLOC_INIT_ZVAL(groups);
			array_init(groups);
			for(y=0; y<svcmap[x].servicegroup_counter; y++){
					
					
					ALLOC_INIT_ZVAL(groupinfo);
					array_init(groupinfo);
					
					z=svcmap[x].servicegroup_place[y];
					add_assoc_long(groupinfo,"servicegroup_place", y);
					add_assoc_string(groupinfo,"servicegroup_name", svcgrpmap[z].servicegroup_name,1);
					add_assoc_string(groupinfo,"servicegroup_member", svcgrpmap[z].servicegroup_members,1);
					
					add_assoc_long(groupinfo,"servicegroup_active", svcgrpmap[z].servicegroup_active);
					add_assoc_long(groupinfo,"servicegroup_notify", svcgrpmap[z].servicegroup_notify);
					add_assoc_long(groupinfo,"servicegroup_id", svcgrpmap[z].servicegroup_id);
					add_assoc_long(groupinfo,"servicegroup_dead", svcgrpmap[z].servicegroup_dead);
					add_assoc_string(groupinfo,"enabled_triggers", svcgrpmap[z].enabled_triggers,1);
					
					add_next_index_zval(groups, groupinfo);
					
			}		
			
			
			
			add_assoc_zval(subarray, "servicegroups", groups);
		}
			
			
			//Push SVC to map
			add_next_index_zval(return_value, subarray);
			
		}

	}
		
		
		
		
		

	
	
		
}
