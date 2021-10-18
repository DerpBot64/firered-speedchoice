const struct MonCoords gTrainerBackPicCoords[] = {
    {.size = 8, .y_offset = 5},
    {.size = 8, .y_offset = 5},
    {.size = 8, .y_offset = 4},
    {.size = 8, .y_offset = 4},
    {.size = 8, .y_offset = 4},
    {.size = 8, .y_offset = 4}
};

const struct CompressedSpriteSheet gTrainerBackPicTable[] = {
    { (const u32 *)gTrainerBackPic_Red, 0x2800, 0 },
    { (const u32 *)gTrainerBackPic_Leaf, 0x2800, 1 },
    { (const u32 *)gTrainerBackPic_RSBrendan, 0x2000, 2 },
    { (const u32 *)gTrainerBackPic_RSMay, 0x2000, 3 },
    { (const u32 *)gTrainerBackPic_Pokedude, 0x2000, 4 },
    { (const u32 *)gTrainerBackPic_OldMan, 0x2000, 5 }
};

const struct CompressedSpritePalette gTrainerBackPicPaletteTable[] = {
    { gTrainerPalette_RedBackPic, 0 },
    { gTrainerPalette_LeafBackPic, 1 },
    { gTrainerPalette_RSBrendan1, 2 },
    { gTrainerPalette_RSMay1, 3 },
    { gTrainerPalette_PokedudeBackPic, 4 },
    { gTrainerPalette_OldManBackPic, 5 }
};

#define AVATAR_RED   			0
#define AVATAR_GREEN 			1
#define AVATAR_BRENDAN   		2
#define AVATAR_MAY 				3
#define AVATAR_BLUE 			4
#define AVATAR_ETHAN 			5
#define AVATAR_RS_BRENDAN   	6
#define AVATAR_RS_MAY 			7
#define AVATAR_RS_MAY_BLUE		8
#define AVATAR_DAWN				9
#define AVATAR_LUCAS			10
#define AVATAR_LYRA				11
#define AVATAR_PEACH			12
#define AVATAR_YUGI				13
#define AVATAR_RETRORED			14
#define AVATAR_ARCHIE			15
#define AVATAR_DUSCLOPS			16
#define AVATAR_LORELEI			17
#define AVATAR_MAXIE			18
#define AVATAR_OAK				19
#define AVATAR_STEVEN			20
#define AVATAR_WALLACE			21
#define AVATAR_WALLY			22
#define AVATAR_ZIGZAG			23
#define AVATAR_KRIS				24
#define AVATAR_BARRY			25
#define AVATAR_CYNTHIA			26
#define AVATAR_LANCE			27
#define AVATAR_SILVER			28

#define AVATAR_COUNT 			29

const u16 gAvatarBackPicTable_Avatar5Frame[] =
{
    0,
    0x0800,
    0x1000,
    0x1800,
    0x2000
};

const union AnimCmd *const *const gAvatarBackAnimsPtrTable[AVATAR_COUNT] = {
    sBackAnims_Red,//5 frame red
	sBackAnims_Red,//5 frame green
    sBackAnims_RSBrendan,//4 frame brendan
	sBackAnims_RSBrendan,//4 frame may
	sBackAnims_RSBrendan,//4 frame blue
	sBackAnims_Red,//5 frame ethan
	sBackAnims_RSBrendan,//4 frame rs brendan
	sBackAnims_RSBrendan,//4 frame rs may
	sBackAnims_RSBrendan,//4 frame rs may blue
	sBackAnims_Red,//5 frame dawn
	sBackAnims_Red,//5 frame lucas
	sBackAnims_Red,//5 frame lyra
	sBackAnims_Red,//4 frame peach
	sBackAnims_Red,//4 frame yugi
	sBackAnims_RSBrendan,//4 frame retrored
	sBackAnims_Red,//4 frame archie
	sBackAnims_Red,//4 frame dusclops
	sBackAnims_Red,//4 frame lorelei
	sBackAnims_Red,//4 frame maxie
	sBackAnims_RSBrendan,//4 frame oak
	sBackAnims_RSBrendan,//4 frame steven
	sBackAnims_Red,//4 frame wallace
	sBackAnims_RSBrendan,//4 frame wally
	sBackAnims_Red,//4 frame zigzag
	sBackAnims_RSBrendan,//4 frame kris
	sBackAnims_Red,//5 frame barry
	sBackAnims_RSBrendan,//4 frame cynthia
	sBackAnims_Red,//5 frame lance
	sBackAnims_Red//5 frame silver
};

const struct CompressedSpritePalette gAvatarBackPicPaletteTable[AVATAR_COUNT] = {
    { gTrainerPalette_RedBackPic, AVATAR_RED },
    { gTrainerPalette_LeafBackPic, AVATAR_GREEN },
    { gAvatarPalette_BRENDAN, AVATAR_BRENDAN },
    { gAvatarPalette_MAY, AVATAR_MAY },
	{ gAvatarPalette_BLUE, AVATAR_BLUE },
	{ gAvatarPalette_ETHAN, AVATAR_ETHAN },
	{ gTrainerPalette_RSBrendan1, AVATAR_RS_BRENDAN },
	{ gTrainerPalette_RSMay1, AVATAR_RS_MAY },
	{ gAvatarPalette_RS_MAY_BLUE, AVATAR_RS_MAY_BLUE },
	{ gAvatarPalette_DAWN, AVATAR_DAWN },
	{ gAvatarPalette_LUCAS, AVATAR_LUCAS },
	{ gAvatarPalette_LYRA, AVATAR_LYRA },
	{ gTrainerPalette_RedBackPic, AVATAR_PEACH },
	{ gTrainerPalette_RedBackPic, AVATAR_YUGI },
	{ gAvatarPalette_RETRORED, AVATAR_RETRORED },
	{ gTrainerPalette_RedBackPic, AVATAR_ARCHIE },
	{ gTrainerPalette_RedBackPic, AVATAR_DUSCLOPS },
	{ gTrainerPalette_RedBackPic, AVATAR_LORELEI },
	{ gTrainerPalette_RedBackPic, AVATAR_MAXIE },
	{ gAvatarPalette_OAK, AVATAR_OAK },
	{ gAvatarPalette_STEVEN, AVATAR_STEVEN },
	{ gTrainerPalette_RedBackPic, AVATAR_WALLACE },
	{ gAvatarPalette_WALLY, AVATAR_WALLY },
	{ gTrainerPalette_RedBackPic, AVATAR_ZIGZAG },
	{ gAvatarPalette_KRIS, AVATAR_KRIS },
	{ gAvatarPalette_BARRY, AVATAR_BARRY },
	{ gAvatarPalette_CYNTHIA, AVATAR_CYNTHIA },
	{ gAvatarPalette_LANCE, AVATAR_LANCE },
	{ gAvatarPalette_SILVER, AVATAR_SILVER }
};

const struct CompressedSpriteSheet gAvatarBackPicTable[AVATAR_COUNT] = {
    { (const u32 *)gTrainerBackPic_Red, 0x2800, AVATAR_RED },
    { (const u32 *)gTrainerBackPic_Leaf, 0x2800, AVATAR_GREEN },
    { (const u32 *)gAvatarBackPic_BRENDAN, 0x2000, AVATAR_BRENDAN },
    { (const u32 *)gAvatarBackPic_MAY, 0x2000, AVATAR_MAY },
	{ (const u32 *)gAvatarBackPic_BLUE, 0x2000, AVATAR_BLUE },
	{ (const u32 *)gAvatarBackPic_ETHAN, 0x2000, AVATAR_ETHAN },
	{ (const u32 *)gTrainerBackPic_RSBrendan, 0x2000, AVATAR_RS_BRENDAN },
	{ (const u32 *)gTrainerBackPic_RSMay, 0x2000, AVATAR_RS_MAY },
	{ (const u32 *)gTrainerBackPic_RSMay, 0x2000, AVATAR_RS_MAY_BLUE },
	{ (const u32 *)gAvatarBackPic_DAWN, 0x2000, AVATAR_DAWN },
	{ (const u32 *)gAvatarBackPic_LUCAS, 0x2000, AVATAR_LUCAS },
	{ (const u32 *)gAvatarBackPic_LYRA, 0x2000, AVATAR_LYRA },
	{ (const u32 *)gAvatarBackPic_PEACH, 0x2000, AVATAR_PEACH },
	{ (const u32 *)gAvatarBackPic_YUGI, 0x2000, AVATAR_YUGI },
	{ (const u32 *)gAvatarBackPic_RETRORED, 0x2000, AVATAR_RETRORED },
	{ (const u32 *)gAvatarBackPic_ARCHIE, 0x2000, AVATAR_ARCHIE },
	{ (const u32 *)gAvatarBackPic_DUSCLOPS, 0x2000, AVATAR_DUSCLOPS },
	{ (const u32 *)gAvatarBackPic_LORELEI, 0x2000, AVATAR_LORELEI },
	{ (const u32 *)gAvatarBackPic_MAXIE, 0x2000, AVATAR_MAXIE },
	{ (const u32 *)gAvatarBackPic_OAK, 0x2000, AVATAR_OAK },
	{ (const u32 *)gAvatarBackPic_STEVEN, 0x2000, AVATAR_STEVEN },
	{ (const u32 *)gAvatarBackPic_WALLACE, 0x2000, AVATAR_WALLACE },
	{ (const u32 *)gAvatarBackPic_WALLY, 0x2000, AVATAR_WALLY },
	{ (const u32 *)gAvatarBackPic_ZIGZAG, 0x2000, AVATAR_ZIGZAG },
	{ (const u32 *)gAvatarBackPic_KRIS, 0x2000, AVATAR_KRIS },
	{ (const u32 *)gAvatarBackPic_BARRY, 0x2000, AVATAR_BARRY },
	{ (const u32 *)gAvatarBackPic_CYNTHIA, 0x2000, AVATAR_CYNTHIA },
	{ (const u32 *)gAvatarBackPic_LANCE, 0x2000, AVATAR_LANCE },
	{ (const u32 *)gAvatarBackPic_SILVER, 0x2000, AVATAR_SILVER }
};
