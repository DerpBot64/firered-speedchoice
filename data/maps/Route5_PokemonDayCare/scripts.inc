@ Handled by data/scripts/day_care.inc

.set LOCALID_ROUTE5_DAYCARE_WOMAN, 2

Route5_PokemonDayCare_MapScripts:: @ 816F8FF
	.byte 0

Route5_PokemonDayCare_EventScript_DaycareWoman:: @ 8171940
	lock
	faceplayer
	specialvar VAR_RESULT, GetDaycareState
	compare VAR_RESULT, DAYCARE_EGG_WAITING
	goto_if_eq Route5_PokemonDayCare_EggWaiting
	compare VAR_RESULT, DAYCARE_ONE_MON
	goto_if_eq Route5_PokemonDayCare_OneMonInDaycare
	compare VAR_RESULT, DAYCARE_TWO_MONS
	goto_if_eq Route5_PokemonDayCare_TwoMonsInDaycare
	msgbox DayCare_Text_WouldYouLikeUsToRaiseMon, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route5_PokemonDayCare_GiveMonToRaise
	msgbox DayCare_Text_FineThenComeAgain
	release
	end

Route5_PokemonDayCare_GiveMonToRaise:: @ 8171993
	specialvar VAR_RESULT, CountPartyNonEggMons
	compare VAR_RESULT, 1
	goto_if_eq Route5_PokemonDayCare_OnlyOneMonInParty
	msgbox DayCare_Text_WhichMonShouldWeRaise
	fadescreen FADE_TO_BLACK
	special ChooseSendDaycareMon
	waitstate
	compare VAR_0x8004, PARTY_SIZE
	goto_if_ge Route5_PokemonDayCare_ComeAgain
	specialvar VAR_RESULT, CountPartyAliveNonEggMons_IgnoreVar0x8004Slot
	compare VAR_RESULT, 0
	goto_if_eq Route5_PokemonDayCare_OnlyOneAliveMonInParty
	specialvar VAR_0x8005, GetSelectedMonNicknameAndSpecies
	waitse
	playmoncry VAR_0x8005, 0
	msgbox DayCare_Text_WellRaiseYourMon
	waitmoncry
	special StoreSelectedPokemonInDaycare
	incrementgamestat GAME_STAT_USED_DAYCARE
	specialvar VAR_RESULT, GetDaycareState
	compare VAR_RESULT, DAYCARE_ONE_MON
	goto_if_eq Route5_PokemonDayCare_CanRaiseOneMore
	release
	end

Route5_PokemonDayCare_ComeAgain:: @ 81719F7
	msgbox DayCare_Text_ComeAgain
	release
	end

Route5_PokemonDayCare_CanRaiseOneMore:: @ 8171A01
	msgbox DayCare_Text_WeCanRaiseOneMore, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route5_PokemonDayCare_GiveMonToRaise
	goto Route5_PokemonDayCare_ComeAgain
	end

Route5_PokemonDayCare_OnlyOneMonInParty:: @ 8171A1A
	msgbox DayCare_Text_YouHaveJustOneMon
	release
	end

Route5_PokemonDayCare_OnlyOneAliveMonInParty:: @ 8171A24
	msgbox DayCare_Text_WhatWillYouBattleWith
	release
	end

Route5_PokemonDayCare_EggWaiting:: @ 8171A2E
	msgbox DayCare_Text_HusbandWasLookingForYou
	release
	end

Route5_PokemonDayCare_MonHasGrownXLevels:: @ 8171A38
	msgbox DayCare_Text_YourMonHasGrownXLevels
	return

Route5_PokemonDayCare_DisplayLevelsGained:: @ 8171A41
	specialvar VAR_RESULT, GetNumLevelsGainedFromDaycare
	compare VAR_RESULT, 0
	call_if_ne Route5_PokemonDayCare_MonHasGrownXLevels
	return

Route5_PokemonDayCare_OneMonInDaycare:: @ 8171A52
	msgbox DayCare_Text_GoodToSeeYou
	setvar VAR_0x8004, 0
	call Route5_PokemonDayCare_DisplayLevelsGained
	msgbox DayCare_Text_WeCanRaiseOneMore, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route5_PokemonDayCare_GiveMonToRaise
	msgbox DayCare_Text_TakeYourMonBack, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route5_PokemonDayCare_TryRetrieveMon
	goto Route5_PokemonDayCare_ComeAgain
	end

Route5_PokemonDayCare_TryRetrieveMon:: @ 8171A90
	specialvar VAR_RESULT, CalculatePlayerPartyCount
	compare VAR_RESULT, PARTY_SIZE
	goto_if_eq Route5_PokemonDayCare_NoRoomInParty
	specialvar VAR_RESULT, GetDaycareState
	setvar VAR_0x8004, 0
	compare VAR_RESULT, DAYCARE_ONE_MON
	goto_if_eq Route5_PokemonDayCare_CostPrompt
	special ShowDaycareLevelMenu
	waitstate
	copyvar VAR_0x8004, VAR_RESULT
	compare VAR_RESULT, DAYCARE_EXITED_LEVEL_MENU
	goto_if_eq Route5_PokemonDayCare_ComeAgain
	goto Route5_PokemonDayCare_CostPrompt
	end

Route5_PokemonDayCare_CostPrompt:: @ 8171ACF
	special GetDaycareCost
	msgbox DayCare_Text_ItWillCostX, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route5_PokemonDayCare_CheckEnoughMoney
	goto Route5_PokemonDayCare_ComeAgain
	end

Route5_PokemonDayCare_CheckEnoughMoney:: @ 8171AEB
	specialvar VAR_RESULT, IsEnoughForCostInVar0x8005
	compare VAR_RESULT, TRUE
	goto_if_eq Route5_PokemonDayCare_RetrieveMon
	msgbox DayCare_Text_NotEnoughMoney
	release
	end

Route5_PokemonDayCare_RetrieveMon:: @ 8171B05
	specialvar VAR_RESULT, TakePokemonFromDaycare
	special SubtractMoneyFromVar0x8005
	playse SE_RS_SHOP
	msgbox DayCare_Text_HeresYourMon
	waitse
	playmoncry VAR_RESULT, 0
	textcolor 3
	msgbox DayCare_Text_TookBackMon
	call EventScript_RestorePrevTextColor
	waitmoncry
	specialvar VAR_RESULT, GetDaycareState
	compare VAR_RESULT, DAYCARE_ONE_MON
	goto_if_eq Route5_PokemonDayCare_AskRetrieveOtherMon
	goto Route5_PokemonDayCare_ComeAgain
	end

Route5_PokemonDayCare_AskRetrieveOtherMon:: @ 8171B4E
	msgbox DayCare_Text_TakeOtherOneBackToo, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route5_PokemonDayCare_TryRetrieveMon
	goto Route5_PokemonDayCare_ComeAgain
	end

Route5_PokemonDayCare_NoRoomInParty:: @ 8171B67
	msgbox DayCare_Text_YourPartyIsFull
	release
	end

Route5_PokemonDayCare_TwoMonsInDaycare:: @ 8171B86
	msgbox DayCare_Text_GoodToSeeYou
	setvar VAR_0x8004, 0
	call Route5_PokemonDayCare_DisplayLevelsGained
	setvar VAR_0x8004, 1
	call Route5_PokemonDayCare_DisplayLevelsGained
	msgbox DayCare_Text_TakeYourMonBack, MSGBOX_YESNO
	compare VAR_RESULT, YES
	goto_if_eq Route5_PokemonDayCare_TryRetrieveMon
	msgbox DayCare_Text_ComeAgain
	release
	end
