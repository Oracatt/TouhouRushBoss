#pragma once
#include "VirtualLib.h"

/*Misc*/
extern ComPtr<ITexture> src_player_reimu;
extern ComPtr<ITexture> src_bullet_1;
extern ComPtr<ITexture> src_bullet_2;
extern ComPtr<ITexture> src_bullet_3;
extern ComPtr<ITexture> src_bullet_4;
extern ComPtr<ITexture> src_laser_1;
extern ComPtr<ITexture> src_slowEffect;
extern ComPtr<ITexture> src_etbreak;
extern ComPtr<ITexture> src_charge;
extern ComPtr<ITexture> src_deadcircle;
extern ComPtr<ITexture> src_dummy;
extern ComPtr<ITexture> src_base;
extern ComPtr<ITexture> src_item;
extern ComPtr<ITexture> src_magicsquare;
extern ComPtr<ITexture> src_aura;
extern ComPtr<ITexture> src_front;
extern ComPtr<ITexture> src_lifebar;
extern ComPtr<ITexture> src_reimu_bomb;
extern ComPtr<ITexture> src_artia;
extern ComPtr<ITexture> src_balloon;
extern ComPtr<ITexture> src_balloon_1;
extern ComPtr<ITexture> src_maple;
extern ComPtr<ITexture> src_ascii;
extern ComPtr<ITexture> src_artia_cdbg1;
extern ComPtr<ITexture> src_artia_cdbg2;
extern ComPtr<ITexture> src_line;
extern ComPtr<ITexture> src_switch;
extern ComPtr<ITexture> src_switchbg;
extern ComPtr<ITexture> src_loading;
extern ComPtr<ITexture> src_player_marisa;
extern ComPtr<ITexture> src_marisa_bomb;
extern ComPtr<ITexture> src_marisa_bomb_2;
extern ComPtr<ITexture> src_pause;
extern ComPtr<ITexture> src_fog;
extern ComPtr<ITexture> src_monstone;
extern ComPtr<ITexture> src_monstone_cdbg1;
extern ComPtr<ITexture> src_monstone_cdbg2;
extern ComPtr<ITexture> src_stageclear;
extern ComPtr<ITexture> src_sunnymilk;
extern ComPtr<ITexture> src_sunnymilk_cdbg1;
extern ComPtr<ITexture> src_sunnymilk_cdbg2;
extern ComPtr<ITexture> src_victorybg;
extern ComPtr<ITexture> src_victory;
extern ComPtr<ITexture> src_bossname;

/*Face*/
extern ComPtr<ITexture> src_reimuface_bs;
extern ComPtr<ITexture> src_reimuface[10];
extern ComPtr<ITexture> src_marisaface_bs;
extern ComPtr<ITexture> src_marisaface[10];
extern ComPtr<ITexture> src_artiaface_ct;
extern ComPtr<ITexture> src_artiaface[10];
extern ComPtr<ITexture> src_monstone_ct;
extern ComPtr<ITexture> src_sunny_ct;
extern ComPtr<ITexture> src_sunnyface_bs;
extern ComPtr<ITexture> src_sunnyface[10];

struct SourceUnit
{
	ITexture** addressOf;

	string path;
};

class SourceLoader
{
public:

	vector<SourceUnit> units;

	void AddSource(ComPtr<ITexture>& pTexture, const string& path);
	void Load(void(*progressReport)(float percent) = 0);
};

void LoadGameSource();