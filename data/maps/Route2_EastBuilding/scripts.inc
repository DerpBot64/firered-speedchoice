.equ REQUIRED_SEEN_MONS, 10

Route2_EastBuilding_MapScripts:: @ 816F67E
	.byte 0

Route2_EastBuilding_EventScript_Aide:: @ 816F67F
	lock
	faceplayer
	call Route2_EastBuilding_EventScript_GetAideRequestInfo
	goto_if_set FLAG_GOT_HM05, Route2_EastBuilding_EventScript_AlreadyGotHM05
	checkspeedchoice EASY_DEX_REWARDS, EASY_DEX_REWARDS_ON
	goto_if_eq Route2_EastBuilding_EventScript_SkipAsk
	msgbox Route2_EastBuilding_Text_GiveHM05IfSeen10Mons, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq Aide_EventScript_DeclineCheckMons
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetPokedexCount
	getnumberstring 2, VAR_0x8006
	call Route2_EastBuilding_EventScript_GetAideRequestInfo
	compare VAR_0x8006, REQUIRED_SEEN_MONS
	goto_if_lt Aide_EventScript_HaventCaughtEnough
	msgbox Route2_EastBuilding_Text_GreatHereYouGo
	goto Route2_EastBuilding_EventScript_GiveHM

Route2_EastBuilding_EventScript_SkipAsk::
	msgbox Route2_EastBuilding_Text_GiveHMnoCheck
Route2_EastBuilding_EventScript_GiveHM::
	checkitemspace ITEM_HM05, 1
	compare VAR_RESULT, FALSE
	goto_if_eq Aide_EventScript_NoRoomForItem
	giveitem_msg Route2_EastBuilding_Text_ReceivedHM05FromAide, ITEM_HM05
	setflag FLAG_GOT_HM05
	checkspeedchoice EASY_FALSE_SWIPE, EASY_FALSE_SWIPE_HM05
	goto_if_eq Route2_EastBuilding_EventScript_ExplainHM05FALSESWIPE
	msgbox Route2_EastBuilding_Text_ExplainHM05
	release
	end

Route2_EastBuilding_EventScript_AlreadyGotHM05:: @ 816F701
	checkspeedchoice EASY_FALSE_SWIPE, EASY_FALSE_SWIPE_HM05
	goto_if_eq Route2_EastBuilding_EventScript_ExplainHM05FALSESWIPE
	msgbox Route2_EastBuilding_Text_ExplainHM05
	release
	end

Route2_EastBuilding_EventScript_ExplainHM05FALSESWIPE:: @ 816F701
	msgbox Route2_EastBuilding_Text_ExplainHM05FALSESWIPE
	release
	end

Route2_EastBuilding_EventScript_GetAideRequestInfo:: @ 816F70B
	getnumberstring 0, REQUIRED_SEEN_MONS
	getitemname 1, ITEM_HM05
	return

Route2_EastBuilding_EventScript_Rocker:: @ 816F714
	msgbox Route2_EastBuilding_Text_CanGetThroughRockTunnel, MSGBOX_NPC
	end
