#include "global.h"
#include "gflib.h"
#include "task.h"
#include "text_window_graphics.h"
#include "scanline_effect.h"
#include "flash_missing_screen.h"
#include "text_window.h"
#include "new_menu_helpers.h"
#include "menu.h"
#include "constants/songs.h"

EWRAM_DATA u8 gWhichErrorMessage = 0;
EWRAM_DATA u8 gWhichTestFailed[100] = {};

#define TX_MARG_LEFT 2
#define TX_MARG_RIGHT 2
#define TX_MARG_TOP 3
#define TX_WINWID 26

extern const u16 sMainMenuTextPal[16];
extern void CB2_SetUpIntro(void);

static const u16 sBgPals[] = INCBIN_U16("graphics/interface/save_failed_screen.gbapal");

static const struct BgTemplate sBgTemplates[] = {
    {
        0,
        0,
        31,
        0,
        0,
        0,
        0x000
    }
};

static const struct WindowTemplate sWindowTemplates[] = {
    {
        0,
        2,
        2,
        TX_WINWID,
        16,
        15,
        10
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 sText_FlashMissing_1[] = _("No valid backup media was detected.");
static const u8 sText_FlashMissing_2[] = _("Pokémon FireRed{COLOR RED}{SHADOW LIGHT_RED} requires the 1M");
static const u8 sText_FlashMissing_3[] = _("sub-circuit board to be installed.");
static const u8 sText_FlashMissing_5[] = _("Please turn off the power.");
static const u8 sText_FlashMissing_7[] = _("{COLOR DARK_GREY}{SHADOW LIGHT_GREY}mGBA: Tools {RIGHT_ARROW} Game overrides…");
static const u8 sText_FlashMissing_8[] = _("NOGBA: Options {RIGHT_ARROW} Files Setup");
static const u8 sText_FlashMissing_9[] = _("VBA: Emulator not supported");

static const u8 sText_PipelineFail_1[] = _("Emulation accuracy test failed.");
static const u8 sText_PipelineFail_2[] = _("Inaccurate emulators are not");
static const u8 sText_PipelineFail_3[] = _("approved for {COLOR GREEN}{SHADOW LIGHT_GREEN}PSR {COLOR RED}{SHADOW LIGHT_RED}races.");
static const u8 sText_PipelineFail_4[] = _("FAILED TEST(S):");

static const u8 sText_PressAnyKeyToContinue[] = _("Press Button A or B to continue.");

struct FatalErrorText
{
    u8 alignment;
    u8 color;
    const u8 * str;
};

struct FatalErrorCnt
{
    bool8 isFatal;
    struct FatalErrorText texts[9];
};

enum {
    FMS_COLOR_GREY = 0,
    FMS_COLOR_RED,
    FMS_COLOR_BLUE,
    FMS_COLOR_GREEN,
};

enum {
    TEXT_LEFT,
    TEXT_CENTER,
    TEXT_RIGHT
};

static const u8 sTextColors[][3] = {
    [FMS_COLOR_GREY] = { TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GREY, TEXT_COLOR_LIGHT_GREY },
    [FMS_COLOR_RED] = { TEXT_COLOR_WHITE, TEXT_COLOR_RED, TEXT_COLOR_LIGHT_RED },
    [FMS_COLOR_BLUE] = { TEXT_COLOR_WHITE, TEXT_COLOR_BLUE, TEXT_COLOR_LIGHT_BLUE },
    [FMS_COLOR_GREEN] = { TEXT_COLOR_WHITE, TEXT_COLOR_GREEN, TEXT_COLOR_LIGHT_GREEN },
};

static const struct FatalErrorCnt sTexts_FatalError[] = {
    [FATAL_NO_FLASH] = {
        TRUE,
        {
            {TEXT_CENTER, FMS_COLOR_RED, sText_FlashMissing_1},
            {TEXT_CENTER, FMS_COLOR_BLUE, sText_FlashMissing_2},
            {TEXT_CENTER, FMS_COLOR_RED, sText_FlashMissing_3},
            {},
            {TEXT_CENTER, FMS_COLOR_RED, sText_FlashMissing_5},
            {},
            {TEXT_LEFT, FMS_COLOR_GREY, sText_FlashMissing_7},
            {TEXT_LEFT, FMS_COLOR_GREY, sText_FlashMissing_8},
            {TEXT_LEFT, FMS_COLOR_GREY, sText_FlashMissing_9}
        }
    },
    [FATAL_ACCU_FAIL] = {
        FALSE,
        {
            {TEXT_CENTER, FMS_COLOR_RED, sText_PipelineFail_1},
            {TEXT_CENTER, FMS_COLOR_RED, sText_PipelineFail_2},
            {TEXT_CENTER, FMS_COLOR_RED, sText_PipelineFail_3},
            {TEXT_LEFT, FMS_COLOR_GREY, sText_PipelineFail_4},
            {TEXT_CENTER, FMS_COLOR_BLUE, gWhichTestFailed}
        }
    }
};

static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void Task_FlashMissingScreen(u8 taskId);

void CB2_FlashMissingScreen(void)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankCallback(NULL);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
        DmaClear32(3, (void *)OAM, OAM_SIZE);
        DmaClear16(3, (void *)PLTT, PLTT_SIZE);
        gMain.state++;
        break;
    case 1:
        FillPalette(RGB_BLACK, 0, PLTT_SIZE);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sBgTemplates, NELEMS(sBgTemplates));
        ChangeBgX(0, 0, 0);
        ChangeBgY(0, 0, 0);
        gMain.state++;
        break;
    case 2:
        ResetPaletteFade();
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        gMain.state++;
        break;
    case 3:
        InitWindows(sWindowTemplates);
        DeactivateAllTextPrinters();
        gMain.state++;
        break;
    case 4:
        LoadBgTiles(0, GetUserFrameGraphicsInfo(gSaveBlock2Ptr->optionsWindowFrameType)->tiles, 9 * TILE_SIZE_4BPP, 1);
        gMain.state++;
        break;
    case 5:
        LoadPalette(sBgPals, 0, 32);
        LoadPalette(GetUserFrameGraphicsInfo(gSaveBlock2Ptr->optionsWindowFrameType)->palette, 0x70, 0x20);
        LoadPalette(GetUserFrameGraphicsInfo(gSaveBlock2Ptr->optionsWindowFrameType)->palette, 0x20, 0x20);
        LoadPalette(stdpal_get(2), 0x10, 0x20);
        LoadPalette(sMainMenuTextPal, 0xF0, sizeof(sMainMenuTextPal));
        gMain.state++;
        break;
    case 6:
        gMain.state++;
        break;
    case 7:
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, 0);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
        ShowBg(0);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        CreateTask(Task_FlashMissingScreen, 0);
        SetMainCallback2(MainCB2);
        SetVBlankCallback(VBlankCB);
        break;
    }
}

static void Task_FlashMissingScreen_Step(u8 taskId);

static void DrawFrame(void)
{
    FillBgTilemapBufferRect(0, 1, 1, 1, 1, 1, 2);
    FillBgTilemapBufferRect(0, 2, 2, 1, 26, 1, 2);
    FillBgTilemapBufferRect(0, 3, 28, 1, 1, 1, 2);
    FillBgTilemapBufferRect(0, 4, 1, 2, 1, 16, 2);
    FillBgTilemapBufferRect(0, 6, 28, 2, 1, 16, 2);
    FillBgTilemapBufferRect(0, 7, 1, 18, 1, 1, 2);
    FillBgTilemapBufferRect(0, 8, 2, 18, 26, 1, 2);
    FillBgTilemapBufferRect(0, 9, 28, 18, 1, 1, 2);
}

static s32 GetStringXpos(const u8 * str, u8 mode)
{
    switch (mode)
    {
    case TEXT_LEFT:
    default:
        return TX_MARG_RIGHT;
    case TEXT_CENTER:
        return (((TX_WINWID) * 8 - (TX_MARG_LEFT) - (TX_MARG_RIGHT)) - GetStringWidth(2, str, 1)) / 2 + TX_MARG_LEFT;
    case TEXT_RIGHT:
        return ((TX_WINWID) * 8 - (TX_MARG_RIGHT)) - GetStringWidth(2, str, 1);
    }
}

static void Task_FlashMissingScreen(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        s32 i;
        FillWindowPixelBuffer(0, PIXEL_FILL(1));
        DrawFrame();
        for (i = 0; i < 9; i++)
        {
            if (sTexts_FatalError[gWhichErrorMessage].texts[i].str != NULL)
                AddTextPrinterParameterized3(
                    0,
                    2,
                    GetStringXpos(sTexts_FatalError[gWhichErrorMessage].texts[i].str, sTexts_FatalError[gWhichErrorMessage].texts[i].alignment),
                    14 * i + TX_MARG_TOP,
                    sTextColors[sTexts_FatalError[gWhichErrorMessage].texts[i].color],
                    TEXT_SPEED_FF,
                    sTexts_FatalError[gWhichErrorMessage].texts[i].str
                );
        }
        PutWindowTilemap(0);
        CopyWindowToVram(0, COPYWIN_BOTH);
        gTasks[taskId].func = Task_FlashMissingScreen_Step;
        gTasks[taskId].data[0] = 0;
    }
}

static void Task_WaitButton(u8 taskId);
static void Task_BeginFadeOut(u8 taskId);
static void Task_WaitFadeOut(u8 taskId);

static void Task_FlashMissingScreen_Step(u8 taskId)
{
    switch (gTasks[taskId].data[0]++)
    {
    case 0:
    case 30:
        PlaySE(SE_BOO);
        break;
    case 60:
        PlaySE(SE_BOO);
        if (sTexts_FatalError[gWhichErrorMessage].isFatal)
            DestroyTask(taskId);
        break;
    case 180:
        AddTextPrinterParameterized3(
            0,
            2,
            GetStringXpos(sText_PressAnyKeyToContinue, TEXT_CENTER),
            14 * 8 + TX_MARG_TOP,
            sTextColors[FMS_COLOR_GREY],
            TEXT_SPEED_FF,
            sText_PressAnyKeyToContinue
        );
        PutWindowTilemap(0);
        CopyWindowToVram(0, COPYWIN_BOTH);
        gTasks[taskId].func = Task_WaitButton;
        break;
    }
}

static void Task_WaitButton(u8 taskId)
{
    if (JOY_NEW(A_BUTTON | B_BUTTON))
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[0] = 60;
        gTasks[taskId].func = Task_BeginFadeOut;
    }
}

static void Task_BeginFadeOut(u8 taskId)
{
    if (--gTasks[taskId].data[0] == 0)
    {
        BeginNormalPaletteFade(0xFFFFFFFF, 8, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_WaitFadeOut;
    }
}

static void Task_WaitFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(CB2_SetUpIntro);
        DestroyTask(taskId);
    }
}
