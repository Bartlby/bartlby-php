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



void xbartlby_decode(char * msg, int length) {
	int x;
		
	for(x=0; x<length; x++) {
		//msg[x]=2^msg[x];	
		
	}
	
}
void xbartlby_encode(char * msg, int length) {
	int x;
	for(x=0; x<length; x++) {
		//msg[x]=msg[x]^2;	
	}
	
}

char * getConfigValue(char * key, char * fname) {
	FILE * fp;
	char  str[1024];

	char * tok;
	
	fp=fopen(fname, "r");
	if(!fp) {
		php_error(E_WARNING, "Config file '%s' failed", fname);
		return NULL;
	}
	
	while(fgets(str,1024,fp) != NULL) {
		str[strlen(str)-1]='\0';
		tok=strtok(str, "=");
		if(tok != NULL) {
				if(strcmp(tok, key) == 0) {
						tok=strtok(NULL, "=");
						if(tok == NULL) {
								fclose(fp);
								return NULL;
						}
						if(tok[strlen(tok)-1] == '\r') {
							tok[strlen(tok)-1]='\0';
						}
						fclose(fp);
						return strdup(tok);
						
				} else {
					continue;
				}
					
		}
			
	}
	
	
	fclose(fp);
	
	
	return NULL;
}



int btl_is_array(zval * ar, long service_id) {
	HashTable *arr_hash;
	zval **data;
    HashPosition pointer;
    int array_count;
    char * string_key;
    long num_key;
    zval * val;

    zend_string *key;
    	
    if(Z_TYPE_P(ar) != IS_ARRAY) {
    	return 1;
    }
	
	arr_hash = Z_ARRVAL_P(ar);
	ZEND_HASH_FOREACH_KEY_VAL(arr_hash, num_key, key, val) {
		if(Z_TYPE_P(val) == IS_STRING) {
			
		} else if(Z_TYPE_P(val) == IS_LONG) {
                	if(Z_LVAL_P(val) == service_id) {
                        	return 1;
                	}
		}

	} ZEND_HASH_FOREACH_END();
/*
    for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(arr_hash, &pointer)) {
		//convert_to_long(*data); FIXME
        if(Z_TYPE_PP(data) == IS_STRING) {
        	//printf("String: %s\n", Z_STRVAL_PP(data));	
        } else if(Z_TYPE_PP(data) == IS_LONG) {
            //printf("Long: %d\n", Z_LVAL_PP(data));	
           	if(Z_LVAL_PP(data) == service_id) {
        		return 1;
        	}
        }

            	
    }
*/
    return -1;
}



void * bartlby_get_sohandle(char * cfgfile) {
	char * data_lib;
	char * dlmsg;
	void * SOHandle;
	
	data_lib=getConfigValue("data_library", cfgfile);
	if(data_lib == NULL) {
		php_error(E_WARNING, "cannot find data_lib key in %s", cfgfile);	
		
		return NULL;
	}
	SOHandle=dlopen(data_lib, RTLD_LAZY);
	free(data_lib);
	
	if((dlmsg=dlerror()) != NULL) {
		php_error(E_WARNING, "DL Error: %s", dlmsg);
			dlclose(SOHandle);
        	return NULL;
    	}	
    	
    	return SOHandle;
} 

void * bartlby_get_shm(char * cfgfile) {
	char * shmtok;
	void * bartlby_address;
	int shm_id;
	
	shmtok = getConfigValue("shm_key", cfgfile);
	if(shmtok == NULL) {
		return NULL;	
	}		
	
	
	shm_id = shmget(ftok(shmtok, 32), 0,  0777);
	free(shmtok);
	if(shm_id != -1) {
		bartlby_address=shmat(shm_id,NULL,0);
		return bartlby_address;
	} else {
		php_error(E_WARNING, "cannot attach to SHM");	
		return NULL;
	}
}


