.equ LOCALID_ROUTE5_DAYCARE_MAN, 1

Route5_MapScripts:: @ 8167FA7
	map_script MAP_SCRIPT_ON_TRANSITION, Route5_OnTransition
	.byte 0
	
Route5_OnTransition:: @ 8167CB9
	goto_if_unset FLAG_0x0AF, Route5_EventScript_SetLayoutSpeedchoice
	end

Route5_EventScript_SetLayoutSpeedchoice:: @ 8164D50
	setmaplayoutindex LAYOUT_ROUTE5_SPEEDCHOICE
	call Route5_EventScript_TrySetDayCareManPos
	return

Route5_EventScript_UndergroundPathSign:: @ 8167FA8
	msgbox Route5_Text_UndergroundPathSign, MSGBOX_SIGN
	end
	
Route5_EventScript_TrySetDayCareManPos:: @ 8167CD1
	goto_if_unset FLAG_PENDING_DAYCARE_EGG, Route5_EventScript_EndSetDayCareManPos
	setobjectxyperm LOCALID_ROUTE5_DAYCARE_MAN, 20, 26
Route5_EventScript_EndSetDayCareManPos:: @ 8167CE1
	return

Route5_EventScript_DaycareMan:: @ 8167D55
	lock
	faceplayer
	special GetDaycareMonNicknames
	specialvar VAR_RESULT, GetDaycareState
	compare VAR_RESULT, DAYCARE_EGG_WAITING
	goto_if_eq Route5_EventScript_DaycareEggWaiting
	compare VAR_RESULT, DAYCARE_ONE_MON
	goto_if_eq Route5_EventScript_CheckOnOneMon
	compare VAR_RESULT, DAYCARE_TWO_MONS
	goto_if_eq Route5_EventScript_CheckOnTwoMons
	msgbox DayCare_Text_ImDaycareManSpeakToMyWife
	release
	end
	
Route5_EventScript_DaycareEggWaiting:: @ 8167D9B
	msgbox DayCare_Text_DoYouWantEgg, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route5_EventScript_DaycareAcceptEgg
	msgbox DayCare_Text_IWillKeepDoYouWantIt, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route5_EventScript_DaycareAcceptEgg
	msgbox DayCare_Text_IllKeepIt
	clearflag FLAG_PENDING_DAYCARE_EGG
	special RejectEggFromDayCare
	release
	end

Route5_EventScript_DaycareAcceptEgg:: @ 8167DD1
	specialvar VAR_RESULT, CalculatePlayerPartyCount
	compare VAR_RESULT, PARTY_SIZE
	goto_if_ne Route5_EventScript_DaycareReceivedEgg
	msgbox DayCare_Text_YouHaveNoRoomForIt
	release
	end

Route5_EventScript_DaycareReceivedEgg:: @ 8167DEB
	textcolor 3
	message DayCare_Text_ReceivedEgg
	call EventScript_RestorePrevTextColor
	playfanfare MUS_LEVEL_UP
	waitfanfare
	waitbuttonpress
	msgbox DayCare_Text_TakeGoodCareOfIt
	special GiveEggFromDaycare
	clearflag FLAG_PENDING_DAYCARE_EGG
	release
	end

Route5_EventScript_CheckOnOneMon:: @ 8167E0C
	special GetDaycareMonNicknames
	msgbox DayCare_Text_YourMonIsDoingFine
	release
	end

Route5_EventScript_CheckOnTwoMons:: @ 8167E19
	special GetDaycareMonNicknames
	msgbox DayCare_Text_YourMonsAreDoingFine
	special SetDaycareCompatibilityString
	special ShowFieldMessageStringVar4
	waitmessage
	waitbuttonpress
	release
	end
