<?
$trigger_dummy = array(
		"trigger_name" => "trigger_name",
		"trigger_enabled" => 1,
		"trigger_type" => 1,
		"trigger_data" => "mail.sh",
		"orch_id" => 999,
		"trigger_execplan" => ""
	);
$worker_dummy = array(
				"name" =>"worker_name",
				"mobilenr" => "+4312344444999",
				"mail" => "unit@test.com",
				"services" => "",
				"notify_levels" => "",
				"active" => 1,
				"password" => "123",
				"enabled_triggers" => "",
				"escalation_limit" => 50,
				"escalation_minutes" => 2,
				"notify_plan" => "",				
				"selected_services" => "",
				"selected_servers" => "",
				"visible_servers" => "",
				"visible_services" => "",
				"is_super_user" => 1,
				"notification_aggregation_interval" => 60,
				"orch_id" => 999,
				"api_pubkey" => "unit-test",
				"api_privkey" => "unit-test",
				"api_enabled" => 0			
			);


				$service_dummy = array(
					
					"plugin"=>"bartlby_load",
					"service_name"=>"unit-test",
					"notify_enabled"=>0,					
					"plugin_arguments"=>"-p",
					"check_interval"=>120,
					"service_type"=>1,
					"service_passive_timeout" => 120,
					"server_id" => 1,
					"service_check_timeout" => 30,
					"service_var" => "",
					"exec_plan" => "",
					"service_ack_enabled" => 0,
					"service_retain" => 2,
					"snmp_community" => "public",
					"snmp_version" => 2,
					"snmp_objid" => "aaa",
					"snmp_warning" => 10,
					"snmp_critical" => 20,
					"snmp_type" => 1,
					"service_active" => 0,
					"snmp_textmatch" => "",
					"flap_seconds" => 122,
					"escalate_divisor" => 0,
					"fires_events" => 0,
					"renotify_interval" => 0,
					"enabled_triggers" => "",
					"handled" => 0,
					"prio" => 100,
					"notify_super_users" => 0,
					"usid" => "unitusid",
					"script" => "",
					"script_enabled" => 0,
					"baseline" => "{}",
					"baseline_enabled" => 0,
					"orch_id" => 999
				);
				$server_dummy = array(
					"server_name" => "unit-test",
					"server_ip" => "localhost",
					"server_port" => 9030,
					"server_icon" => "linux.gif",
					"server_enabled" => 0,
					"server_notify" => 0,
					"server_flap_seconds" => 122,
					"server_ssh_keyfile" => "",
					"server_ssh_passphrase" => "",
					"server_ssh_username" => "",
					"server_dead" => 0,
					"exec_plan" => "",
					"enabled_triggers" => "",
					"default_service_type" => 1,
					"orch_id" => 999,
					"web_hooks" => "",
					'json_endpoint' => "",
					'web_hooks_level' => 0
					
				);
			$servergroup_dummy = array(
				"servergroup_name" => "unit-test",
				"servergroup_active" => 0,
				"servergroup_notify" => 0,
				"enabled_triggers" => "",
				"servergroup_members" => "|1|",
				"servergroup_dead" => 0,
				"orch_id" => 999
						
			);
			$servicegroup_dummy = array(
				"servicegroup_name" => "unit-test",
				"servicegroup_active" => 0,
				"servicegroup_notify" => 0,
				"enabled_triggers" => "",
				"servicegroup_members" => "|1|",
				"servicegroup_dead" => 0,
				"orch_id" => 999
						
			);

			$trap_dummy = array(
				"trap_name" => "unit-test",
				"trap_catcher" => ".*",
				"trap_status_text" => ".*",
				"trap_status_ok" => ".*",
				"trap_status_warning" => ".*",
				"trap_status_critical" => ".*",
				"trap_service_id" => -1,
				"trap_fixed_status" => -2,
				"trap_is_final" => 1,
				"trap_prio" => 100,
				"orch_id" => 999,
				
			
			);					

			$downtime_dummy = array(
				"downtime_from" => 0,
				"downtime_to" => 0,
				"downtime_type" => 1,
				"downtime_notice" => "unit-test",
				"downtime_service" => 1,
				"orch_id" => 999

			);


function bartlby_generic_audit($res, $type,  $id, $line) {
		return true;
	}
	function bartlby_object_audit($res, $type, $id, $action) {
		
		return true;
	}
?>
