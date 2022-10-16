#pragma once

//#define DMG_MELEE 0x000001

enum DMG
{
	DMG_MELEE = 0,
	DMG_BULLET,
	DMG_EXPLOSION,
	DMG_LAVA,
};

enum TEAM
{
	TEAM_PLAYER = 0,
	TEAM_MONSTER,
};

enum MAP_FLOOR
{
	F_TI = 0,
	F_BR,
	F_LV,
	F_DW,
	F_DE,
};

enum MAP_WALL
{
	W_NO = 0,
	W_TI,
	W_BR,
	W_LV,
	W_DW,
	W_DE
};