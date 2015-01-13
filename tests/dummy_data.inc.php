<?
$worker_dummy = array(
				"name" =>"worker_name",
				"icq" => "1123123",
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
					"orch_id" => 999
				);
				

function bartlby_generic_audit($res, $type,  $id, $line) {
		
		return true;
	}
	function bartlby_object_audit($res, $type, $id, $action) {
		return true;
	}
?>