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

ZEND_DECLARE_MODULE_GLOBALS(bartlby)

extern int le_bartlby;





int bartlby_generic_audit(zval * bartlby_resource,  long object_id, long audit_type, char * logline) {
	
	zval **params[4];
	zval type;
	zval id;
	zval act;
	zval function_name;
	zval log_line;
	
	zval *return_user_call;

	zval *t;
	zval *t2;
	zval *t3;
	zval *t4;
	
	int res;
	INIT_ZVAL(type);
	INIT_ZVAL(id);
	INIT_ZVAL(function_name);
	INIT_ZVAL(act);
	INIT_ZVAL(log_line);


	ZVAL_LONG(&type, audit_type);
	ZVAL_LONG(&id, object_id);
	ZVAL_STRING(&log_line, logline);
	ZVAL_STRING(&function_name, "bartlby_generic_audit");

	
	
	params[0] = &bartlby_resource;
	
	t=&type;
	params[1] = &t;
	t2=&id;
	params[2] = &t2;
	t3=&log_line;
	params[3] = &t3;
	
	
	
	res=call_user_function_ex(EG(function_table), NULL, &function_name, &return_user_call, 4, params, 1, NULL TSRMLS_CC);
	if (res == SUCCESS && return_user_call != NULL && zval_is_true(return_user_call)) {
		return 0;
	} else {
		if((int)BARTLBY_G(force_audit) == 1) {
			return -1;
		}
	} 
    return -1;
    //	if((long)BARTLBY_G(force_audit) == 1)  {

}

int bartlby_object_audit(zval * bartlby_resource,  long audit_type, long object_id, long action) {
	
	zval **params[4];
	zval type;
	zval id;
	zval act;
	zval function_name;
	
	zval *return_user_call;

	zval *t;
	zval *t2;
	zval *t3;
	zval *t4;
	
	int res;
	INIT_ZVAL(type);
	INIT_ZVAL(id);
	INIT_ZVAL(function_name);
	INIT_ZVAL(act);


	ZVAL_LONG(&type, audit_type);
	ZVAL_LONG(&id, object_id);
	ZVAL_LONG(&act, action);
	ZVAL_STRING(&function_name, "bartlby_object_audit");

	
	
	params[0] = &bartlby_resource;
	
	t=&type;
	params[1] = &t;
	t2=&id;
	params[2] = &t2;
	t3=&act;
	params[3] = &t3;
	
	
	
	res=call_user_function_ex(EG(function_table), NULL, &function_name, &return_user_call, 4, params, 1, NULL TSRMLS_CC);
	if (res == SUCCESS && return_user_call != NULL && zval_is_true(return_user_call)) {
		return 0;
	} else {
		if((int)BARTLBY_G(force_audit) == 1) {
			return -1;
		}
	} 
    return -1;
    //	if((long)BARTLBY_G(force_audit) == 1)  {

}




int bartlby_mark_object_gone(zval * zbartlby_resource, bartlby_res * bres, long id, int type, int msg) {
	
	
	int x;
	struct shm_header * shm_hdr;
	struct service * svcmap;
	struct server * srvmap;	
	struct worker * wrkmap;
	struct downtime * dtmap;
	struct btl_event * evntmap;
	struct servergroup * srvgrpmap;
	struct servicegroup * svcgrpmap;
	struct trap * trapmap;
	char * tmpstr;
	
	int audit_type=0;
	int audit_action=0;

	int rtc;
	
	shm_hdr=bartlby_SHM_GetHDR(bres->bartlby_address);
	svcmap=bartlby_SHM_ServiceMap(bres->bartlby_address);
	wrkmap=bartlby_SHM_WorkerMap(bres->bartlby_address);
	dtmap=bartlby_SHM_DowntimeMap(bres->bartlby_address);
	srvmap=bartlby_SHM_ServerMap(bres->bartlby_address);
	evntmap=bartlby_SHM_EventMap(bres->bartlby_address);
	srvgrpmap=bartlby_SHM_ServerGroupMap(bres->bartlby_address);
	svcgrpmap=bartlby_SHM_ServiceGroupMap(bres->bartlby_address);
	trapmap=bartlby_SHM_TrapMap(bres->bartlby_address);
	


	//if(msg != BARTLBY_AUDIT_ACTION_ADD) {
		switch(type) {
			case BARTLBY_SERVICE_GONE:
				for(x=0; x<shm_hdr->svccount; x++) {
					if(svcmap[x].service_id == id) {
						svcmap[x].is_gone = msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_SERVICE;

			break;
			case BARTLBY_SERVER_GONE:
				for(x=0; x<shm_hdr->srvcount; x++) {
					if(srvmap[x].server_id == id) {
						srvmap[x].is_gone=msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_SERVER;
			break;
			case BARTLBY_WORKER_GONE:
				for(x=0; x<shm_hdr->wrkcount; x++) {
					if(wrkmap[x].worker_id == id) {
						wrkmap[x].is_gone = msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_WORKER;
			break;

			case BARTLBY_DOWNTIME_GONE:
				for(x=0; x<shm_hdr->dtcount; x++) {
					if(dtmap[x].downtime_id == id) {
						dtmap[x].is_gone = msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_DOWNTIME;
			break;
			case BARTLBY_SERVICEGROUP_GONE:
				for(x=0; x<shm_hdr->svcgroupcount; x++) {
					if(svcgrpmap[x].servicegroup_id == id) {
						//svcgrpmap[x].is_gone = msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_SERVICEGROUP;
			break;
			case BARTLBY_SERVERGROUP_GONE:
				for(x=0; x<shm_hdr->srvgroupcount; x++) {
					if(srvgrpmap[x].servergroup_id == id) {
						//srvgrpmap[x].is_gone = msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_SERVERGROUP;
			break;
			case BARTLBY_TRAP_GONE:
				for(x=0; x<shm_hdr->trapcount; x++) {
					if(trapmap[x].trap_id == id) {
						trapmap[x].is_gone = msg;
					}	
				}	
				audit_type=BARTLBY_AUDIT_TYPE_TRAP;
			break;
			
				
//		}
	}

	switch(msg) {
		case BARTLBY_OBJECT_DELETED:
			audit_action=BARTLBY_AUDIT_ACTION_DELETE;
			
			BARTLBY_GENERIC_AUDIT_INT(zbartlby_resource, id, audit_type, "Deleted");
		break;
		case BARTLBY_OBJECT_CHANGED:
			audit_action=BARTLBY_AUDIT_ACTION_MODIFY;
			BARTLBY_GENERIC_AUDIT_INT(zbartlby_resource, id, audit_type, "Edited");
			
		break;
		case BARTLBY_OBJECT_ADDED:
			audit_action=BARTLBY_AUDIT_ACTION_ADD;
			
			BARTLBY_GENERIC_AUDIT_INT(zbartlby_resource, id, audit_type, "Added");
		break;

	}

	rtc=bartlby_object_audit(zbartlby_resource, audit_type , id,  audit_action);

	
	return rtc;
	


}

