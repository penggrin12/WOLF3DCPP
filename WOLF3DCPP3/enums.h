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