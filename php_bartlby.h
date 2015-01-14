/*
//
  +----------------------------------------------------------------------+
  | PHP Version 4                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2003 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.02 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/2_02.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+

  $Id: php_bartlby.h,v 1.48 2008/03/14 01:11:19 hjanuschka Exp $ 
*/
#define BARTLBY_VERSION "1.5.0"



#define BARTLBY_AUDIT_TYPE_SERVICE 1
#define BARTLBY_AUDIT_TYPE_SERVER 2
#define BARTLBY_AUDIT_TYPE_WORKER 3
#define BARTLBY_AUDIT_TYPE_SERVERGROUP 4
#define BARTLBY_AUDIT_TYPE_SERVICEGROUP 5
#define BARTLBY_AUDIT_TYPE_DOWNTIME 6
#define BARTLBY_AUDIT_TYPE_GENERIC 7
#define BARTLBY_AUDIT_TYPE_TRAP 7


#define BARTLBY_AUDIT_ACTION_ADD 1
#define BARTLBY_AUDIT_ACTION_MODIFY 2
#define BARTLBY_AUDIT_ACTION_DELETE 3



#define BARTLBY_SERVICE_GONE 1
#define BARTLBY_SERVER_GONE 2
#define BARTLBY_WORKER_GONE 3
#define BARTLBY_DOWNTIME_GONE 4
#define BARTLBY_SERVICEGROUP_GONE 5
#define BARTLBY_SERVERGROUP_GONE 6
#define BARTLBY_TRAP_GONE 7


#define BARTLBY_OBJECT_CHANGED 1
#define BARTLBY_OBJECT_DELETED 2
#define BARTLBY_OBJECT_OUT_OF_SYNC 3
#define BARTLBY_OBJECT_ADDED  4



#define DT_SERVICE 1
#define DT_SERVER 2
#define DT_SERVERGROUP 3
#define DT_SERVICEGROUP 4

#define SERVICE_UNHANDLED 0;
#define SERVICE_HANDLED 1;

#ifndef PHP_BARTLBY_H
#define PHP_BARTLBY_H

extern zend_module_entry bartlby_module_entry;
#define phpext_bartlby_ptr &bartlby_module_entry

#ifdef PHP_WIN32
#define PHP_BARTLBY_API __declspec(dllexport)
#else
#define PHP_BARTLBY_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/times.h> 


typedef struct _bartlby_res {
    char *cfgfile;
    void * bartlby_address;
    void * SOHandle;
    
    
} bartlby_res;
#define BARTLBY_RES_NAME "Bartlby Resource"

#define LOAD_SYMBOL(x,y,z) 	x=dlsym(y, z); \
    	if((dlmsg=dlerror()) != NULL) { \
        	php_error(E_WARNING, "dl error: %s", dlmsg);	\
		RETURN_FALSE;	\
    	}
    	

#define BARTLBY_DEFAULT_STRING 1
#define BARTLBY_DEFAULT_LONG 2
#define BARTLBY_DEFAULT_ERROR 3
#define BARTLBY_FIELD_REQUIRED 1
#define BARTLBY_FIELD_OPTIONAL 0

#define GETARRAY_EL_FROM_HASH(target, element,temp,  array, required, def_type, def_value) \
if(SUCCESS == zend_hash_find(Z_ARRVAL_P(array), element, strlen(element) + 1, (void**) &temp)) { \
	target = *temp; \
}  else { \
	target = NULL; \
} \
if(required == 1) { \
	if(target == NULL) { \
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Element %s is required\n", element); \
		RETURN_BOOL(0); \
	} \
	if(Z_TYPE_P(target) == IS_NULL) {\
		MAKE_STD_ZVAL(target); \
		if(def_type == BARTLBY_DEFAULT_STRING) { \
			ZVAL_STRING(target, (char*)def_value, 1); \
		} \
		if(def_type == BARTLBY_DEFAULT_LONG) { \
			ZVAL_LONG(target, (long)def_value);\
		}\
		if(def_type == BARTLBY_DEFAULT_ERROR) {\
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Element %s is required and has no value\n", element); \
			RETURN_BOOL(0); \
		} \
	}\
}
	

#define EVENT_QUEUE_MAX 128


PHP_MINIT_FUNCTION(bartlby);
PHP_MSHUTDOWN_FUNCTION(bartlby);
PHP_RINIT_FUNCTION(bartlby);
PHP_RSHUTDOWN_FUNCTION(bartlby);
PHP_MINFO_FUNCTION(bartlby);

PHP_FUNCTION(confirm_bartlby_compiled);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_info);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_version);
PHP_FUNCTION(bartlby_config);
PHP_FUNCTION(bartlby_lib_info);
PHP_FUNCTION(bartlby_add_server);
PHP_FUNCTION(bartlby_delete_server);
PHP_FUNCTION(bartlby_modify_server);
PHP_FUNCTION(bartlby_get_server_by_id);

PHP_FUNCTION(bartlby_get_worker);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_service);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_server);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_downtime);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_downtime_by_id);	/* For testing, remove later. */

PHP_FUNCTION(bartlby_get_servergroup);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_servergroup_by_id);	/* For testing, remove later. */

PHP_FUNCTION(bartlby_get_servicegroup);	/* For testing, remove later. */
PHP_FUNCTION(bartlby_get_servicegroup_by_id);	/* For testing, remove later. */


PHP_FUNCTION(bartlby_encode);
PHP_FUNCTION(bartlby_decode);

PHP_FUNCTION(bartlby_add_service);
PHP_FUNCTION(bartlby_add_service_array);
PHP_FUNCTION(bartlby_delete_service);
PHP_FUNCTION(bartlby_modify_service);
PHP_FUNCTION(bartlby_get_service_by_id);


PHP_FUNCTION(bartlby_callback_test);

PHP_FUNCTION(bartlby_add_worker);
PHP_FUNCTION(bartlby_delete_worker);
PHP_FUNCTION(bartlby_modify_worker);
PHP_FUNCTION(bartlby_get_worker_by_id);

PHP_FUNCTION(bartlby_add_downtime);
//PHP_FUNCTION(bartlby_downtime_map);
PHP_FUNCTION(bartlby_modify_downtime);
PHP_FUNCTION(bartlby_delete_downtime);

PHP_FUNCTION(bartlby_reload);
//PHP_FUNCTION(bartlby_svc_map);
//PHP_FUNCTION(bartlby_svc_map_test);
PHP_FUNCTION(bartlby_shm_destroy);


PHP_FUNCTION(bartlby_add_servergroup);
//PHP_FUNCTION(bartlby_servergroup_map);
PHP_FUNCTION(bartlby_modify_servergroup);
PHP_FUNCTION(bartlby_delete_servergroup);
PHP_FUNCTION(bartlby_add_servicegroup);
//PHP_FUNCTION(bartlby_servicegroup_map);
PHP_FUNCTION(bartlby_modify_servicegroup);
PHP_FUNCTION(bartlby_delete_servicegroup);
PHP_FUNCTION(bartlby_toggle_servicegroup_notify);
PHP_FUNCTION(bartlby_toggle_servergroup_notify);
PHP_FUNCTION(bartlby_toggle_servicegroup_active);
PHP_FUNCTION(bartlby_toggle_servergroup_active);
	
PHP_FUNCTION(bartlby_set_servergroup_id);
PHP_FUNCTION(bartlby_set_servicegroup_id);
	


PHP_FUNCTION(bartlby_toggle_service_notify);
PHP_FUNCTION(bartlby_toggle_service_handled);
PHP_FUNCTION(bartlby_toggle_server_notify);
PHP_FUNCTION(bartlby_toggle_service_active);
PHP_FUNCTION(bartlby_toggle_server_active);
PHP_FUNCTION(bartlby_toggle_sirene);

PHP_FUNCTION(bartlby_ack_problem);

PHP_FUNCTION(bartlby_check_shm_size);
PHP_FUNCTION(bartlby_check_force);


PHP_FUNCTION(bartlby_event_tick);
PHP_FUNCTION(bartlby_event_fetch);

PHP_FUNCTION(bartlby_set_server_id);
PHP_FUNCTION(bartlby_set_service_id);
PHP_FUNCTION(bartlby_set_worker_id);
PHP_FUNCTION(bartlby_set_worker_state);
PHP_FUNCTION(bartlby_set_downtime_id);
PHP_FUNCTION(bartlby_set_passive);

	
PHP_FUNCTION(bartlby_bulk_service_active);
PHP_FUNCTION(bartlby_bulk_service_notify);
PHP_FUNCTION(bartlby_bulk_force_services);
PHP_FUNCTION(bartlby_service_set_interval);

PHP_FUNCTION(bartlby_get_core_extension_info);

PHP_FUNCTION(bartlby_new);
PHP_FUNCTION(bartlby_close);



PHP_FUNCTION(bartlby_get_trap);
PHP_FUNCTION(bartlby_add_trap);
PHP_FUNCTION(bartlby_modify_trap);
PHP_FUNCTION(bartlby_delete_trap);
PHP_FUNCTION(bartlby_set_trap_id);
PHP_FUNCTION(bartlby_get_trap_by_id);


PHP_FUNCTION(bartlby_cleanup_tests);
//Notitification Log

PHP_FUNCTION(bartlby_notification_log_at_index);

PHP_METHOD(Bartlby, testFunc);





#ifdef ZTS
#define BARTLBY_G(v) TSRMG(bartlby_globals_id, zend_bartlby_globals *, v)
#else
#define BARTLBY_G(v) (bartlby_globals.v)
#endif

ZEND_BEGIN_MODULE_GLOBALS(bartlby)
	long  force_audit;
ZEND_END_MODULE_GLOBALS(bartlby)


#endif	/* PHP_BARTLBY_H */





#define BARTLBY_OBJECT_GONE(zbartlby_resource, bres, id, type, msg) if(bartlby_mark_object_gone(zbartlby_resource, bres, id, type, msg) < 0) { \
																		php_error_docref(NULL TSRMLS_CC, E_ERROR, "bartlby_mark_object_gone() Callback failed - and force_audit is enabled via INI"); \
															   			RETURN_BOOL(0); \
																	}

#define BARTLBY_OBJECT_AUDIT(zbartlby_resource, id, type, action) if(bartlby_object_audit(zbartlby_resource, id, type, action) < 0) { \
																		php_error_docref(NULL TSRMLS_CC, E_ERROR, "bartlby_object_audit() Callback failed - and force_audit is enabled via INI"); \
															   			RETURN_BOOL(0); \
																	}

#define BARTLBY_GENERIC_AUDIT(zbartlby_resource, id, type, logl) if(bartlby_generic_audit(zbartlby_resource, id, type, logl) < 0) { \
																		php_error_docref(NULL TSRMLS_CC, E_ERROR, "bartlby_generic_audit() Callback failed - and force_audit is enabled via INI"); \
															   			RETURN_BOOL(0); \
																	}
#define BARTLBY_GENERIC_AUDIT_INT(zbartlby_resource, id, type, logl) if(bartlby_generic_audit(zbartlby_resource, id, type, logl) < 0) { \
																		php_error_docref(NULL TSRMLS_CC, E_ERROR, "bartlby_generic_audit() Callback failed - and force_audit is enabled via INI"); \
															   			return -1; \
																	}





//FUNCTIONS INSIDE EXTENSION
char * getConfigValue(char * key, char * fname);
void xbartlby_encode(char * msg, int length);
void xbartlby_decode(char * msg, int length);
void * bartlby_get_sohandle(char * cfgfile);
void * bartlby_get_shm(char * cfgfile);
int bartlby_mark_object_gone(zval * zbartlby_resource, bartlby_res * bres, long id, int type, int msg);
int bartlby_generic_audit(zval * bartlby_resource,  long object_id, long audit_type, char * logline);
int btl_is_array(zval * ar, long service_id);
static void php_bartlby_init_globals(zend_bartlby_globals *bartlby_globals);


//SHM HANDLE STUFF
struct service * bartlby_SHM_ServiceMap(void *);
struct btl_event * bartlby_SHM_EventMap(void * shm_addr);
struct downtime * bartlby_SHM_DowntimeMap(void * shm_addr);
struct shm_header * bartlby_SHM_GetHDR(void *);
struct worker * bartlby_SHM_WorkerMap(void * shm_addr);
struct server * bartlby_SHM_ServerMap(void * shm_addr);
struct trap * bartlby_SHM_TrapMap(void * shm_addr);
struct servicegroup * bartlby_SHM_ServiceGroupMap(void * shm_addr);
struct servergroup * bartlby_SHM_ServerGroupMap(void * shm_addr);




/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
 
#define MAX_GROUP_MEMBERS 512
#define GROUP_MEMBER_STR_LENGTH 2048

 
struct shm_counter {
	long worker;
	long services;
	long downtimes;
	long servers;	
	long servergroups;
	long servicegroups;
	long traps;
};

struct perf_statistic {
	long sum;
	long counter;	
};


struct snmpi {
	char community[512];
	int version;
	char objid[1024];
	long warn;
	long crit;
	long type;
	char textmatch[1024];
};
struct sprocess {
	int start_time;
	int pid;
		
};

struct sched_worker {
	int pid;
	struct service * svc;
	int start_time;
	int  idle;
	int shutdown;
	struct tms timing;
	int idx;


} astt;

struct notification_log_entry {
	int notification_valid; //-1 invalid == end of list
	long worker_id; //Worker id
	long service_id; //Service_id
	int state; //State
	int aggregated; //Default -1 > 0 - this notification has already been aggregated
	char trigger_name[512];
	int type; // 0 if it was a normal notification, 1 = it was a escalation notification to the standby's
	time_t time;
	int aggregation_interval;
	int received_via;
};
#define NOTIFICATION_LOG_MAX 512
#define NOTIFICATION_VIA_LOCAL 1
#define NOTIFICATION_VIA_UPSTREAM 2


struct shm_header {
	long size_of_structs;
	long thrdcount;
	long svccount;
	long wrkcount;
	long srvcount;
	long srvgroupcount;
	long svcgroupcount;
	long trapcount;
	long current_running;
	char  version[50];
	int do_reload;
	int last_replication;
	int startup_time;
	long dtcount;
	int sirene_mode;
	struct perf_statistic pstat;
	int cur_event_index;
	long checks_performed;
	int checks_performed_time;
	struct  sched_worker worker_threads[50];
	struct notification_log_entry notification_log[NOTIFICATION_LOG_MAX];
	long notification_log_current_top;	
	time_t notification_log_aggregate_last_run;
	
};

struct server {
	long server_id;
	char  client_ip[2048];
	char  server_name[2048];
	char server_icon[1024];
	int server_enabled;
	int client_port;
	int server_dead;
	int server_notify;
	long server_flap_seconds;
	int flap_count;
	int last_notify_send;
	
	struct service * dead_marker;
	int is_gone;
	
	struct servergroup * servergroups[MAX_GROUP_MEMBERS];
	long servergroup_counter;
	long servergroup_place[MAX_GROUP_MEMBERS];
	
	char server_ssh_keyfile[512];
	char server_ssh_passphrase[512];
	char server_ssh_username[512];
	
	char enabled_triggers[512];
	int default_service_type;
	int orch_id;
	char exec_plan[2048];
	char web_hooks[1024];
	char json_endpoint[256];
	int web_hooks_level;
} xxyz;


struct trap {
    long trap_id;
    char trap_name[512];
    char trap_catcher[512];
    char trap_status_text[512];
    char trap_status_ok[512];
    char trap_status_warning[512];
    char trap_status_critical[512];
    long trap_service_id;
    long service_shm_place;
    int trap_fixed_status;
    int trap_prio;
    int trap_is_final;
    int orch_id;
    int is_gone;
    int matched;
    int trap_last_match;
    char trap_last_data[2048];
} zzk;


struct service {
	long service_id;
	long server_id;
	int last_state;
	int current_state;
	
	char  new_server_text[2048];
	char  service_name[2048];
	
	char  plugin[2048];
	char  plugin_arguments[2048];
	long check_interval;
	long check_interval_original;
	int last_check;
	struct timeval lcheck;
	
	/*Time stuff*/
	
	char service_exec_plan[2048];
	
	/*Notify things*/
	int notify_enabled;
	int last_notify_send;
	int last_state_change;
	long flap_count;
	
	int service_active;
	
	char  service_var[2048];
	int service_type;
	long service_passive_timeout;
	
	int notify_last_state;
	
	long service_check_timeout;
	
	
	
	int service_ack_enabled;
	int service_ack_current;
	
	
	long service_retain;
	long service_retain_current;
	
	
	
	struct perf_statistic pstat;
	struct perf_statistic delay_time;
	
	int do_force;
	
	struct snmpi snmp_info;
	
	int recovery_outstanding; //Flag to see if recover is waiting
	
	struct sprocess process;
	
	long flap_seconds;
	
	struct server * srv;
	long srv_place;
	
	int is_server_dead;
	
	/**/
	long renotify_interval; // interval to renotify
	long escalate_divisor; //

	int is_gone;
	
	
	struct servicegroup * servicegroups[MAX_GROUP_MEMBERS];
	long servicegroup_counter;
	long servicegroup_place[MAX_GROUP_MEMBERS];
	
	long fires_events;
	
	char enabled_triggers[512];

	int handled;
	int orch_id;
	int last_orch_sync;

	char usid[50];
	int prio;
	int notify_super_users;
};

struct servicegroup {
	long servicegroup_id;
	char servicegroup_name[1024];
	int servicegroup_notify;
	int servicegroup_active;
	char servicegroup_members[GROUP_MEMBER_STR_LENGTH];
	
	int servicegroup_dead;
	struct service * dead_marker;
	
	char enabled_triggers[512];
	int orch_id;
};

struct servergroup {
	long servergroup_id;
	char servergroup_name[1024];
	int servergroup_notify;
	int servergroup_active;
	char servergroup_members[GROUP_MEMBER_STR_LENGTH];
	int servergroup_dead;
	struct service * dead_marker;
	char enabled_triggers[512];
	int orch_id;
};


struct service_sort {
	struct service * svc;	
};
struct sched_worker_sort {
	struct sched_worker * th;
};


struct worker {
	char name[2048];
	char  mail[2048];
	char  icq[2048];
	
	
	long worker_id;
	int active;
	char notify_plan[2048];
	char password[2048];
	
	int escalation_count;
	int escalation_time;
	char notify_levels[20];
	char enabled_triggers[2048];
	char t[500];
	long escalation_limit;
	long escalation_minutes;
	int is_gone;

	char  visible_services[2048];
	char  visible_servers[2048];
	char  selected_services[2048];
	char  selected_servers[2048];
	int  notification_aggregation_interval;
	int is_super_user;

	int orch_id;
	
	char api_pubkey[255];
	char api_privkey[255];

	int api_enabled;
}sa;


struct downtime {
	long downtime_id;
	int downtime_type;
	int downtime_from;
	int downtime_to;
	char downtime_notice[2048];
	int service_id;
	
	int is_gone;

	int orch_id;
	
}sb;

struct btl_event {
	long evnt_id;
	char evnt_message[1024];
	int evnt_time;
		
}eb;
struct ext_notify {
	struct service * svc;	
	struct worker * wrk;
	char * trigger;
} ty;



typedef struct v2_packet_struct{

	u_int32_t crc32_value;
	int16_t   exit_code;
	int16_t   packet_type;
	char      output[2048];
	char      cmdline[2048];
	char      plugin[2048];
	char 	   perf_handler[1024];
	
	 
} agent_v2_packet;

typedef struct port_packet_struct{

	u_int32_t crc32_value;
	int16_t   exit_code;
	int16_t   packet_type;
	char      output[2048];
	char      cmdline[2048];
	char      plugin[2048];
	char 	   perf_handler[1024];
	int32_t	   service_id;
	
	 
} portier_packet;


