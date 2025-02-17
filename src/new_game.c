#include "global.h"
#include "gflib.h"
#include "random.h"
#include "overworld.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/moves.h"
#include "load_save.h"
#include "item_menu.h"
#include "tm_case.h"
#include "berry_pouch.h"
#include "quest_log.h"
#include "wild_encounter.h"
#include "event_data.h"
#include "mail_data.h"
#include "play_time.h"
#include "money.h"
#include "battle_records.h"
#include "pokemon_size_record.h"
#include "pokemon_storage_system.h"
#include "roamer.h"
#include "item.h"
#include "player_pc.h"
#include "speedchoice.h"
#include "berry.h"
#include "easy_chat.h"
#include "union_room_chat.h"
#include "mevent.h"
#include "renewable_hidden_items.h"
#include "trainer_tower.h"
#include "script.h"
#include "berry_powder.h"
#include "pokemon_jump.h"
#include "event_scripts.h"
#include "done_button.h"
#include "save_location.h"
#include "constants/heal_locations.h"
#include "pokedex_screen.h"
#include "constants/region_map_sections.h"
#include "pokemon.h"

// this file's functions
static void ResetMiniGamesResults(void);

// EWRAM vars
EWRAM_DATA bool8 gDifferentSaveFile = FALSE;

void SetTrainerId(u32 trainerId, u8 *dst)
{
    dst[0] = trainerId;
    dst[1] = trainerId >> 8;
    dst[2] = trainerId >> 16;
    dst[3] = trainerId >> 24;
}

void CopyTrainerId(u8 *dst, u8 *src)
{
    s32 i;
    for (i = 0; i < 4; i++)
        dst[i] = src[i];
}

static void InitPlayerTrainerId(void)
{
    u32 trainerId = (Random() << 0x10) | GetGeneratedTrainerIdLower();
    SetTrainerId(trainerId, gSaveBlock2Ptr->playerTrainerId);
}

static void SetDefaultOptions(void)
{
    gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_INST;
    gSaveBlock2Ptr->optionsWindowFrameType = 0;
    gSaveBlock2Ptr->optionsSound = OPTIONS_SOUND_MONO;
    gSaveBlock2Ptr->optionsBattleStyle = OPTIONS_BATTLE_STYLE_SET;
    gSaveBlock2Ptr->optionsBattleScene = OPTIONS_BATTLE_SCENE_OFF;
    gSaveBlock2Ptr->regionMapZoom = FALSE;
    gSaveBlock2Ptr->optionsButtonMode = OPTIONS_BUTTON_MODE_L_EQUALS_A;
    gSaveBlock2Ptr->optionsNickname = OPTIONS_NICKNAME_NO;
}

static void ClearPokedexFlags(void)
{
    memset(&gSaveBlock2Ptr->pokedex.owned, 0, sizeof(gSaveBlock2Ptr->pokedex.owned));
    memset(&gSaveBlock2Ptr->pokedex.seen, 0, sizeof(gSaveBlock2Ptr->pokedex.seen));
}

static void ClearBattleTower(void)
{
    CpuFill32(0, &gSaveBlock2Ptr->battleTower, sizeof(gSaveBlock2Ptr->battleTower));
}

static void WarpToPlayersRoom(void)
{
    SetWarpDestination(MAP_GROUP(PALLET_TOWN_PLAYERS_HOUSE_2F), MAP_NUM(PALLET_TOWN_PLAYERS_HOUSE_2F), -1, 6, 6);
    WarpIntoMap();
    sInIntro = FALSE;
    sInSubMenu = FALSE;
    sInBattle = FALSE;
    sInField = TRUE;
}

void Sav2_ClearSetDefault(void)
{
    ClearSav2();
    SetDefaultOptions();
}

void ResetMenuAndMonGlobals(void)
{
    gDifferentSaveFile = FALSE;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetBagCursorPositions();
    ResetTMCaseCursorPos();
    BerryPouch_CursorResetToTop();
    ResetQuestLog();
    SeedWildEncounterRng(Random());
    ResetSpecialVars();
}

void NewGameResetDoneButtonStats(){
	//excludes timers, those are done on hitting the go button
    gSaveBlock2Ptr->doneButtonStats.saveCount = 0;
    gSaveBlock2Ptr->doneButtonStats.reloadCount = 0;
    gSaveBlock2Ptr->doneButtonStats.stepCount = 0;
    gSaveBlock2Ptr->doneButtonStats.stepCountWalk = 0;
    gSaveBlock2Ptr->doneButtonStats.stepCountSurf = 0;
    gSaveBlock2Ptr->doneButtonStats.stepCountBike = 0;
    gSaveBlock2Ptr->doneButtonStats.stepCountRun = 0;
    gSaveBlock2Ptr->doneButtonStats.bonks = 0;
    gSaveBlock2Ptr->doneButtonStats.totalDamageDealt = 0;
    gSaveBlock2Ptr->doneButtonStats.actualDamageDealt = 0;
    gSaveBlock2Ptr->doneButtonStats.totalDamageTaken = 0;
    gSaveBlock2Ptr->doneButtonStats.actualDamageTaken = 0;
    gSaveBlock2Ptr->doneButtonStats.ownMovesHit = 0;
    gSaveBlock2Ptr->doneButtonStats.ownMovesMissed = 0;
    gSaveBlock2Ptr->doneButtonStats.enemyMovesHit = 0;
    gSaveBlock2Ptr->doneButtonStats.enemyMovesMissed = 0;
    gSaveBlock2Ptr->doneButtonStats.ownMovesSE = 0;
    gSaveBlock2Ptr->doneButtonStats.ownMovesNVE = 0;
    gSaveBlock2Ptr->doneButtonStats.enemyMovesSE = 0;
    gSaveBlock2Ptr->doneButtonStats.enemyMovesNVE = 0;
    gSaveBlock2Ptr->doneButtonStats.critsDealt = 0;
    gSaveBlock2Ptr->doneButtonStats.OHKOsDealt = 0;
    gSaveBlock1Ptr->doneButtonStats.critsTaken = 0;
    gSaveBlock1Ptr->doneButtonStats.OHKOsTaken = 0;
    gSaveBlock1Ptr->doneButtonStats.playerHPHealed = 0;
    gSaveBlock1Ptr->doneButtonStats.enemyHPHealed = 0;
    gSaveBlock1Ptr->doneButtonStats.playerPokemonFainted = 0;
    gSaveBlock1Ptr->doneButtonStats.enemyPokemonFainted = 0;
    gSaveBlock1Ptr->doneButtonStats.expGained = 0;
    gSaveBlock1Ptr->doneButtonStats.switchouts = 0;
    gSaveBlock1Ptr->doneButtonStats.battles = 0;
    gSaveBlock1Ptr->doneButtonStats.trainerBattles = 0;
    gSaveBlock1Ptr->doneButtonStats.wildBattles = 0;
    gSaveBlock1Ptr->doneButtonStats.battlesFled = 0;
    gSaveBlock1Ptr->doneButtonStats.failedRuns = 0;
    gSaveBlock1Ptr->doneButtonStats.moneyMade = 0;
    gSaveBlock1Ptr->doneButtonStats.moneySpent = 0;
    gSaveBlock1Ptr->doneButtonStats.moneyLost = 0;
    gSaveBlock1Ptr->doneButtonStats.itemsPickedUp = 0;
    gSaveBlock1Ptr->doneButtonStats.itemsBought = 0;
    gSaveBlock1Ptr->doneButtonStats.itemsSold = 0;
    gSaveBlock1Ptr->doneButtonStats.movesLearnt = 0;
    gSaveBlock1Ptr->doneButtonStats.ballsThrown = 0;
    gSaveBlock1Ptr->doneButtonStats.pokemonCaughtInBalls = 0;
    gSaveBlock1Ptr->doneButtonStats.evosAttempted = 0;
    gSaveBlock1Ptr->doneButtonStats.evosCompleted = 0;
    gSaveBlock1Ptr->doneButtonStats.evosCancelled = 0;
}

void NewGameInitData(void)
{
    u8 rivalName[PLAYER_NAME_LENGTH + 1];
    u16 * natDexScenePointer;

    StringCopy(rivalName, gSaveBlock1Ptr->rivalName);
    gDifferentSaveFile = TRUE;
    gSaveBlock2Ptr->encryptionKey = 0;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ClearBattleTower();
    ClearSav1();
    ClearMailData();
    gSaveBlock2Ptr->specialSaveWarpFlags = 0;
    gSaveBlock2Ptr->gcnLinkFlags = 0;
    gSaveBlock2Ptr->field_AC = 1;
    gSaveBlock2Ptr->field_AD = 0;
    InitPlayerTrainerId();
    PlayTimeCounter_Reset();
    ClearPokedexFlags();
    InitEventData();
    ResetFameChecker();
    SetMoney(&gSaveBlock1Ptr->money, 3000);
    ResetGameStats();
    ClearPlayerLinkBattleRecords();
    InitHeracrossSizeRecord();
    InitMagikarpSizeRecord();
    RS_EnableNationalPokedex();
    gPlayerPartyCount = 0;
    ZeroPlayerPartyMons();
    ResetPokemonStorageSystem();
    ClearRoamerData();
    gSaveBlock1Ptr->registeredItem = 0;
    ClearBag();
    NewGameInitPCItems();
    ClearEnigmaBerries();
    InitEasyChatPhrases();
    ResetTrainerFanClub();
    UnionRoomChat_InitializeRegisteredTexts();
    ResetMiniGamesResults();
    InitMEventData();
    SetAllRenewableItemFlags();
    WarpToPlayersRoom();
    ScriptContext2_RunNewScript(EventScript_ResetAllMapFlags);
    StringCopy(gSaveBlock1Ptr->rivalName, rivalName);
    ResetTrainerTowerResults();

    NewGameResetDoneButtonStats();

    //saved to flag so it can hide NPCs
    if(gSaveBlock2Ptr->speedchoiceConfig.earlyDaycare == DAYCARE_NO){
    	//hide NPCs and use normal map layout
    	FlagSet(FLAG_0x0AF);
    }
    else{
    	//show NPCs and show speedchoice map layout
    	FlagClear(FLAG_0x0AF);
    }

    if (gSaveBlock2Ptr->speedchoiceConfig.earlyBike == EARLY_BIKE_YES)
        AddBagItem(ITEM_BICYCLE, 1);
    if (gSaveBlock2Ptr->speedchoiceConfig.raceGoal == GOAL_MANUAL)
        AddBagItem(ITEM_DONE_BUTTON, 1);
    FlagSet(FLAG_SYS_B_DASH);

	natDexScenePointer = GetVarPointer(VAR_MAP_SCENE_PALLET_TOWN_OAK_NATIONAL_DEX_SCENE);
	*natDexScenePointer = 0;

#if DEVMODE
    {
		u16 i;
		u8 ev = 252;
		u8 ev2 = 6;
    	struct Pokemon * mon = AllocZeroed(sizeof(struct Pokemon));
        if (mon != NULL)
        {
            u32 pid;
            u32 otid = T2_READ_32(gSaveBlock2Ptr->playerTrainerId);
            do
			{
				pid = Random32();
			} while (NATURE_RASH != GetNatureFromPersonality(pid));
            CreateMon(mon, SPECIES_MEWTWO, 100, 31, TRUE, pid, OT_ID_PLAYER_ID, 0);
            SetMonMoveSlot(mon, MOVE_PSYCHIC, 0);
            SetMonMoveSlot(mon, MOVE_FLY, 1);
            SetMonMoveSlot(mon, MOVE_SURF, 2);
            SetMonMoveSlot(mon, MOVE_THUNDERBOLT, 3);
			SetMonData(mon, MON_DATA_ATK_EV, &ev);
			SetMonData(mon, MON_DATA_SPATK_EV, &ev);
			SetMonData(mon, MON_DATA_SPEED_EV, &ev2);
			CalculateMonStats(mon);
            GiveMonToPlayer(mon);
            Free(mon);
        }
        FlagSet(FLAG_WORLD_MAP_PALLET_TOWN);
        FlagSet(FLAG_WORLD_MAP_VIRIDIAN_CITY);
        FlagSet(FLAG_WORLD_MAP_PEWTER_CITY);
        FlagSet(FLAG_WORLD_MAP_CERULEAN_CITY);
        FlagSet(FLAG_WORLD_MAP_LAVENDER_TOWN);
        FlagSet(FLAG_WORLD_MAP_VERMILION_CITY);
        FlagSet(FLAG_WORLD_MAP_CELADON_CITY);
        FlagSet(FLAG_WORLD_MAP_FUCHSIA_CITY);
        FlagSet(FLAG_WORLD_MAP_CINNABAR_ISLAND);
        FlagSet(FLAG_WORLD_MAP_INDIGO_PLATEAU_EXTERIOR);
        FlagSet(FLAG_WORLD_MAP_SAFFRON_CITY);
        FlagSet(FLAG_WORLD_MAP_ONE_ISLAND);
        FlagSet(FLAG_WORLD_MAP_TWO_ISLAND);
        FlagSet(FLAG_WORLD_MAP_THREE_ISLAND);
        FlagSet(FLAG_WORLD_MAP_FOUR_ISLAND);
        FlagSet(FLAG_WORLD_MAP_FIVE_ISLAND);
        FlagSet(FLAG_WORLD_MAP_SEVEN_ISLAND);
        FlagSet(FLAG_WORLD_MAP_SIX_ISLAND);
        FlagSet(FLAG_WORLD_MAP_ROUTE4_POKEMON_CENTER_1F);
        FlagSet(FLAG_WORLD_MAP_ROUTE10_POKEMON_CENTER_1F);

        FlagSet( FLAG_SYS_SEVII_MAP_123);
		FlagSet( FLAG_SYS_SEVII_MAP_4567);
		FlagSet( FLAG_ENABLE_SHIP_NAVEL_ROCK);
		FlagSet( FLAG_ENABLE_SHIP_BIRTH_ISLAND);

		FlagSet(FLAG_HIDE_POKEMON_MANSION_B1F_SECRET_KEY);

		AddBagItem(ITEM_RAINBOW_PASS,1);
		AddBagItem(ITEM_MYSTIC_TICKET,1);
		AddBagItem(ITEM_AURORA_TICKET,1);
		AddBagItem(ITEM_SECRET_KEY,1);
		SetMoney(&gSaveBlock1Ptr->money, 999999);

		AddBagItem(ITEM_PP_MAX,99);
		AddBagItem(ITEM_MAX_ELIXIR,99);

		for(i = 0; i <50; i++){
			AddBagItem(ITEM_TM01+i,99);
		}

		for(i = 0; i <8; i++){
			AddBagItem(ITEM_HM01+i,1);
		}

		for(i = 1; i <387; i++){
			DexScreen_GetSetPokedexFlag(i, FLAG_SET_SEEN, 0);
		}



    }
#endif //DEVMODE

    if (gSaveBlock2Ptr->speedchoiceConfig.startLoc != START_PALLET){

    	struct Pokemon * magikarp;
    	u8 metLocTemp;

    	FlagSet(FLAG_SYS_POKEMON_GET);
    	FlagSet(FLAG_SYS_POKEDEX_GET);
    	SetUnlockedPokedexFlags();

    	magikarp = AllocZeroed(sizeof(struct Pokemon));
		if (magikarp != NULL)
		{
			u32 pid;
			u32 otid = T2_READ_32(gSaveBlock2Ptr->playerTrainerId);
			pid = Random32();
			CreateMon(magikarp, SPECIES_MAGIKARP, 1, 0, TRUE, pid, OT_ID_PLAYER_ID, 0);

			metLocTemp = METLOC_FATEFUL_ENCOUNTER;

			SetBoxMonData(&magikarp->box, MON_DATA_MET_LOCATION, &metLocTemp);
			SetMonMoveSlot(magikarp, MOVE_SPLASH, 0);
			SetMonMoveSlot(magikarp, MOVE_NONE, 1);
			SetMonMoveSlot(magikarp, MOVE_NONE, 2);
			SetMonMoveSlot(magikarp, MOVE_NONE, 3);
			GiveMonToPlayer(magikarp);
			Free(magikarp);
		}
		if(gSaveBlock2Ptr->speedchoiceConfig.startLoc == START_SAFARI){
			//open saffron if option is chosen
			if (gSaveBlock2Ptr->speedchoiceConfig.earlySaffron == SAFFRON_YES){
				u16 * varPtr = GetVarPointer(VAR_MAP_SCENE_ROUTE5_ROUTE6_ROUTE7_ROUTE8_GATES);
				*varPtr = 1;
			}

			SetWarpDestination(MAP_GROUP(FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE), MAP_NUM(FUCHSIA_CITY_SAFARI_ZONE_ENTRANCE), -1, 4, 6);
			WarpIntoMap();
			sInIntro = FALSE;
			sInSubMenu = FALSE;
			sInBattle = FALSE;
			sInField = TRUE;
			SetLastHealLocationWarp(SPAWN_FUCHSIA_CITY);
		}
		if(gSaveBlock2Ptr->speedchoiceConfig.startLoc == START_HITMON){

			//force early saffron
			u16 * varPtr = GetVarPointer(VAR_MAP_SCENE_ROUTE5_ROUTE6_ROUTE7_ROUTE8_GATES);
			*varPtr = 1;

			//Set trainers in dojo to fought already.
			FlagSet(1597);
			FlagSet(1598);
			FlagSet(1599);
			FlagSet(1600);
			FlagSet(1601);

			SetWarpDestination(MAP_GROUP(SAFFRON_CITY_DOJO), MAP_NUM(SAFFRON_CITY_DOJO), -1, 6, 4);
			WarpIntoMap();
			sInIntro = FALSE;
			sInSubMenu = FALSE;
			sInBattle = FALSE;
			sInField = TRUE;
			SetLastHealLocationWarp(SPAWN_SAFFRON_CITY);
		}
		if(gSaveBlock2Ptr->speedchoiceConfig.startLoc == START_CELADON){
			//open saffron if option is chosen
			if (gSaveBlock2Ptr->speedchoiceConfig.earlySaffron == SAFFRON_YES){
				u16 * varPtr = GetVarPointer(VAR_MAP_SCENE_ROUTE5_ROUTE6_ROUTE7_ROUTE8_GATES);
				*varPtr = 1;
			}
			SetWarpDestination(MAP_GROUP(CELADON_CITY_CONDOMINIUMS_ROOF_ROOM), MAP_NUM(CELADON_CITY_CONDOMINIUMS_ROOF_ROOM), -1, 6, 3);
			WarpIntoMap();
			sInIntro = FALSE;
			sInSubMenu = FALSE;
			sInBattle = FALSE;
			sInField = TRUE;
			SetLastHealLocationWarp(SPAWN_CELADON_CITY);
		}

    }
}

static void ResetMiniGamesResults(void)
{
    CpuFill16(0, &gSaveBlock2Ptr->berryCrush, sizeof(struct BerryCrush));
    SetBerryPowder(&gSaveBlock2Ptr->berryCrush.berryPowderAmount, 0);
    ResetPokeJumpResults();
    CpuFill16(0, &gSaveBlock2Ptr->berryPick, sizeof(struct BerryPickingResults));
}
