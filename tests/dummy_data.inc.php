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


function bartlby_generic_audit($res, $type,  $id, $line) {
		
		return true;
	}
	function bartlby_object_audit($res, $type, $id, $action) {
		return true;
	}
?>