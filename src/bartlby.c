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







int le_bartlby;

 
 
zend_function_entry bartlby_functions[] = {
	
	PHP_FE(bartlby_callback_test, NULL)
	PHP_FE(bartlby_new,NULL)
	PHP_FE(bartlby_close,NULL)
	PHP_FE(bartlby_get_info,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_version,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_config,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_lib_info, NULL)
	

	PHP_FE(bartlby_get_service,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_worker,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_server,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_downtime,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_downtime_by_id,	NULL)		/* For testing, remove later. */
	

	PHP_FE(bartlby_get_servicegroup,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_servicegroup_by_id,	NULL)		/* For testing, remove later. */


	PHP_FE(bartlby_get_servergroup,	NULL)		/* For testing, remove later. */
	PHP_FE(bartlby_get_servergroup_by_id,	NULL)		/* For testing, remove later. */





	PHP_FE(bartlby_add_server,NULL)
	PHP_FE(bartlby_delete_server,NULL)
	PHP_FE(bartlby_modify_server,NULL)
	PHP_FE(bartlby_get_server_by_id,NULL)
	
	
	PHP_FE(bartlby_encode,NULL)
	PHP_FE(bartlby_decode,NULL)
	
	
	PHP_FE(bartlby_add_service,NULL)
	PHP_FE(bartlby_add_service_array, NULL)
	PHP_FE(bartlby_delete_service,NULL)
	PHP_FE(bartlby_modify_service,NULL)
	PHP_FE(bartlby_get_service_by_id,NULL)
	
	PHP_FE(bartlby_add_worker, NULL)
	PHP_FE(bartlby_delete_worker, NULL)
	PHP_FE(bartlby_modify_worker, NULL)
	PHP_FE(bartlby_get_worker_by_id, NULL)
	
	
	PHP_FE(bartlby_add_downtime, NULL)
	//PHP_FE(bartlby_downtime_map, NULL)
	PHP_FE(bartlby_modify_downtime, NULL)
	PHP_FE(bartlby_delete_downtime, NULL)
	
	
	PHP_FE(bartlby_add_servergroup, NULL)
	//PHP_FE(bartlby_servergroup_map, NULL)
	PHP_FE(bartlby_modify_servergroup, NULL)
	PHP_FE(bartlby_delete_servergroup, NULL)
	
	
	PHP_FE(bartlby_add_servicegroup, NULL)
	//PHP_FE(bartlby_servicegroup_map, NULL)
	PHP_FE(bartlby_modify_servicegroup, NULL)
	PHP_FE(bartlby_delete_servicegroup, NULL)
	
	
	PHP_FE(bartlby_toggle_servicegroup_notify, NULL)
	PHP_FE(bartlby_toggle_servergroup_notify, NULL)
	PHP_FE(bartlby_toggle_servicegroup_active, NULL)
	PHP_FE(bartlby_toggle_servergroup_active, NULL)
	
	
	
	
	PHP_FE(bartlby_toggle_service_notify, NULL)
	PHP_FE(bartlby_toggle_service_handled, NULL)
	PHP_FE(bartlby_toggle_server_notify, NULL)
	PHP_FE(bartlby_toggle_service_active, NULL)
	PHP_FE(bartlby_toggle_server_active, NULL)
	
	
	PHP_FE(bartlby_toggle_sirene, NULL)
	
	//PHP_FE(bartlby_svc_map,NULL)
	//PHP_FE(bartlby_svc_map_test,NULL)
	
	PHP_FE(bartlby_reload, NULL)
	PHP_FE(bartlby_shm_destroy, NULL)
	
	PHP_FE(bartlby_ack_problem, NULL)
	
	PHP_FE(bartlby_check_shm_size, NULL)
	
	PHP_FE(bartlby_check_force, NULL)
	
	
	PHP_FE(bartlby_event_tick, NULL)
	PHP_FE(bartlby_event_fetch, NULL)
	
	PHP_FE(bartlby_set_passive, NULL)
	
	PHP_FE(bartlby_set_server_id, NULL)
	PHP_FE(bartlby_set_service_id, NULL)
	PHP_FE(bartlby_set_worker_id, NULL)
	PHP_FE(bartlby_set_worker_state, NULL)
	PHP_FE(bartlby_set_downtime_id, NULL)
	
	PHP_FE(bartlby_set_servergroup_id, NULL)
	PHP_FE(bartlby_set_servicegroup_id, NULL)
	

	PHP_FE(bartlby_bulk_service_active,NULL)
	PHP_FE(bartlby_bulk_service_notify,NULL)
	PHP_FE(bartlby_bulk_force_services,NULL)
	
	PHP_FE(bartlby_service_set_interval,NULL)
	
	PHP_FE(bartlby_get_core_extension_info,NULL)
	
	
	PHP_FE(bartlby_notification_log_at_index, NULL)

	PHP_FE(bartlby_get_trap, NULL)
	PHP_FE(bartlby_add_trap, NULL)
	PHP_FE(bartlby_modify_trap, NULL)
	PHP_FE(bartlby_delete_trap, NULL)
	PHP_FE(bartlby_set_trap_id, NULL)
	PHP_FE(bartlby_get_trap_by_id, NULL)
	PHP_FE(bartlby_cleanup_tests, NULL)

	
	{NULL, NULL, NULL}	/* Must be the last line in bartlby_functions[] */
};
/* }}} */

/* {{{ bartlby_module_entry
 */
zend_module_entry bartlby_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"bartlby",
	bartlby_functions,
	PHP_MINIT(bartlby),
	PHP_MSHUTDOWN(bartlby),
	PHP_RINIT(bartlby),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(bartlby),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(bartlby),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_BARTLBY
ZEND_GET_MODULE(bartlby)
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("bartlby.force_audit",      "0", PHP_INI_ALL, OnUpdateLong, force_audit, zend_bartlby_globals, bartlby_globals)
    
PHP_INI_END()
/* }}} */

/* {{{ php_bartlby_init_globals
 */
static void php_bartlby_init_globals(zend_bartlby_globals *bartlby_globals)
{
	bartlby_globals->force_audit = 0;
	
}

/* }}} */











PHP_FUNCTION(bartlby_close)
{
	
		//FIXME check resource type BARTLBY
    zval *zbartlby;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zbartlby) == FAILURE) {
        RETURN_FALSE;
    }

    zend_list_delete(Z_LVAL_P(zbartlby));
    RETURN_TRUE;
}



static void bartlby_res_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    bartlby_res *res = (bartlby_res*)rsrc->ptr;
    if (res) {
    	//FIXME
    	//shmdt
    	//dlclose
    	
    	if(res->bartlby_address != NULL) {
    		shmdt(res->bartlby_address);
    	}
    	if(res->SOHandle != NULL) {
    		dlclose(res->SOHandle);
    	}
    	efree(res->cfgfile);
    	
      efree(res);
    }
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(bartlby)
{
	REGISTER_INI_ENTRIES();
	
	le_bartlby = zend_register_list_destructors_ex(bartlby_res_dtor, NULL, BARTLBY_RES_NAME, module_number);
	
	


	//SET CONSTANTS

	//REGISTER_LONG_CONSTANT(name, lval, flags);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_SERVICE", BARTLBY_AUDIT_TYPE_SERVICE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_SERVER", BARTLBY_AUDIT_TYPE_SERVER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_GENERIC", BARTLBY_AUDIT_TYPE_GENERIC, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_WORKER", BARTLBY_AUDIT_TYPE_WORKER, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_SERVERGROUP", BARTLBY_AUDIT_TYPE_SERVERGROUP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_SERVICEGROUP", BARTLBY_AUDIT_TYPE_SERVICEGROUP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_TRAP", BARTLBY_AUDIT_TYPE_TRAP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_TYPE_DOWNTIME", BARTLBY_AUDIT_TYPE_DOWNTIME, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_ACTION_ADD", BARTLBY_AUDIT_ACTION_ADD, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_ACTION_MODIFY", BARTLBY_AUDIT_ACTION_MODIFY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BARTLBY_AUDIT_ACTION_DELETE", BARTLBY_AUDIT_ACTION_DELETE, CONST_CS | CONST_PERSISTENT);


	
	return SUCCESS;
	
}



/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(bartlby)
{
	UNREGISTER_INI_ENTRIES();
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(bartlby)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(bartlby)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(bartlby)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "bartlby support", "enabled");
	php_info_print_table_row(2, "php-ext version", BARTLBY_VERSION);
	php_info_print_table_end();
	DISPLAY_INI_ENTRIES();

	/* Remove comments if you have entries in php.ini
	
	*/
}













































