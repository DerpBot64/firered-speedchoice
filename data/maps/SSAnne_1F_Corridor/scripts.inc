SSAnne_1F_Corridor_MapScripts:: @ 81608CB
	map_script MAP_SCRIPT_ON_TRANSITION, SSAnne_1F_Corridor_OnTransition
	.byte 0

SSAnne_1F_Corridor_OnTransition:: @ 816084F
	compare VAR_MAP_SCENE_VERMILION_CITY, 3
	goto_if_eq SSAnne_1F_Corridor_ResetBoat
	end

SSAnne_1F_Corridor_ResetBoat:: @ 81608CC
	setvar VAR_MAP_SCENE_VERMILION_CITY, 1
	end

SSAnne_1F_Corridor_EventScript_WorkerM:: @ 81608CC
	msgbox SSAnne_1F_Corridor_Text_LeStrongSilentType, MSGBOX_NPC
	end

SSAnne_1F_Corridor_EventScript_Sailor:: @ 81608D5
	msgbox SSAnne_1F_Corridor_Text_PassengersAreRestless, MSGBOX_NPC
	end
