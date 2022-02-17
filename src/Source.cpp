#include "Source.h"
#include "EngineGlobal.h"

using namespace Global;

ComPtr<ITexture> src_player_reimu;
ComPtr<ITexture> src_bullet_1;
ComPtr<ITexture> src_bullet_2;
ComPtr<ITexture> src_bullet_3;
ComPtr<ITexture> src_bullet_4;
ComPtr<ITexture> src_laser_1;
ComPtr<ITexture> src_slowEffect;
ComPtr<ITexture> src_etbreak;
ComPtr<ITexture> src_charge;
ComPtr<ITexture> src_deadcircle;
ComPtr<ITexture> src_dummy;
ComPtr<ITexture> src_base;
ComPtr<ITexture> src_item;
ComPtr<ITexture> src_magicsquare;
ComPtr<ITexture> src_aura;
ComPtr<ITexture> src_front;
ComPtr<ITexture> src_lifebar;
ComPtr<ITexture> src_reimu_bomb;
ComPtr<ITexture> src_artia;
ComPtr<ITexture> src_balloon;
ComPtr<ITexture> src_balloon_1;
ComPtr<ITexture> src_maple;
ComPtr<ITexture> src_ascii;
ComPtr<ITexture> src_artia_cdbg1;
ComPtr<ITexture> src_artia_cdbg2;
ComPtr<ITexture> src_line;
ComPtr<ITexture> src_switch;
ComPtr<ITexture> src_switchbg;
ComPtr<ITexture> src_loading;
ComPtr<ITexture> src_player_marisa;
ComPtr<ITexture> src_marisa_bomb;
ComPtr<ITexture> src_marisa_bomb_2;
ComPtr<ITexture> src_pause;
ComPtr<ITexture> src_fog;
ComPtr<ITexture> src_monstone;
ComPtr<ITexture> src_monstone_cdbg1;
ComPtr<ITexture> src_monstone_cdbg2;
ComPtr<ITexture> src_stageclear;
ComPtr<ITexture> src_sunnymilk;
ComPtr<ITexture> src_sunnymilk_cdbg1;
ComPtr<ITexture> src_sunnymilk_cdbg2;
ComPtr<ITexture> src_victorybg;
ComPtr<ITexture> src_victory;
ComPtr<ITexture> src_bossname;

ComPtr<ITexture> src_reimuface_bs;
ComPtr<ITexture> src_reimuface[10];
ComPtr<ITexture> src_marisaface_bs;
ComPtr<ITexture> src_marisaface[10];
ComPtr<ITexture> src_artiaface_ct;
ComPtr<ITexture> src_artiaface[10];
ComPtr<ITexture> src_monstone_ct;
ComPtr<ITexture> src_sunny_ct;
ComPtr<ITexture> src_sunnyface_bs;
ComPtr<ITexture> src_sunnyface[10];

void SourceLoader::AddSource(ComPtr<ITexture>& pTexture, const string& path)
{
	units.push_back({ pTexture.ReleaseAndGetAddressOf(),path });
}

void SourceLoader::Load(void(*progressReport)(float percent))
{
	for (int i = 0; i < units.size(); i++)
	{
		engine->CreateTextureFromFile(units[i].path.c_str(), units[i].addressOf);
		if (progressReport)
			progressReport((i + 1) / (float)units.size());
	}
}

void LoadGameSource()
{
	SourceLoader loader;
	loader.AddSource(src_player_reimu, "image\\player\\pl00.png");
	loader.AddSource(src_bullet_1, "image\\bullet\\bullet1.png");
	loader.AddSource(src_bullet_2, "image\\bullet\\bullet2.png");
	loader.AddSource(src_bullet_3, "image\\bullet\\bullet3.png");
	loader.AddSource(src_bullet_4, "image\\bullet\\bullet4.png");
	loader.AddSource(src_laser_1, "image\\bullet\\laser1.png");
	loader.AddSource(src_slowEffect, "image\\effect\\eff_sloweffect.png");
	loader.AddSource(src_etbreak, "image\\effect\\etbreak.png");
	loader.AddSource(src_charge, "image\\effect\\eff_charge.png");
	loader.AddSource(src_deadcircle, "image\\effect\\eff_deadcircle.png");
	loader.AddSource(src_dummy, "image\\effect\\dummy.png");
	loader.AddSource(src_base, "image\\effect\\eff_base.png");
	loader.AddSource(src_item, "image\\bullet\\item.png");
	loader.AddSource(src_magicsquare, "image\\effect\\eff_magicsquare.png");
	loader.AddSource(src_aura, "image\\effect\\eff_aura.png");
	loader.AddSource(src_front, "image\\front\\front00.png");
	loader.AddSource(src_lifebar, "image\\front\\lifebar.png");
	loader.AddSource(src_reimu_bomb, "image\\player\\pl00b.png");
	loader.AddSource(src_artia, "image\\boss\\artia.png");
	loader.AddSource(src_balloon, "image\\face\\balloon_1024.png");
	loader.AddSource(src_balloon_1, "image\\face\\balloon_1024(1).png");
	loader.AddSource(src_maple, "image\\effect\\eff_maple.png");
	loader.AddSource(src_ascii, "image\\ascii\\ascii_1280.png");
	loader.AddSource(src_artia_cdbg1, "image\\boss\\artia_cdbg1.png");
	loader.AddSource(src_artia_cdbg2, "image\\boss\\artia_cdbg2.png");
	loader.AddSource(src_line, "image\\effect\\eff_line.png");
	loader.AddSource(src_switch, "image\\effect\\eff_switch.png");
	loader.AddSource(src_switchbg, "image\\effect\\eff_switchbg.png");
	loader.AddSource(src_loading, "image\\loading\\loading.png");
	loader.AddSource(src_player_marisa, "image\\player\\pl01.png");
	loader.AddSource(src_marisa_bomb, "image\\player\\pl01b.png");
	loader.AddSource(src_marisa_bomb_2, "image\\player\\pl01b2.png");
	loader.AddSource(src_pause, "image\\ascii\\pause.png");
	loader.AddSource(src_fog, "image\\effect\\fog.png");
	loader.AddSource(src_monstone, "image\\boss\\monstone.png");
	loader.AddSource(src_monstone_cdbg1, "image\\boss\\monstone_cdbg1.png");
	loader.AddSource(src_monstone_cdbg2, "image\\boss\\monstone_cdbg2.png");
	loader.AddSource(src_stageclear, "image\\front\\front01.png");
	loader.AddSource(src_sunnymilk, "image\\boss\\sunnymilk.png");
	loader.AddSource(src_sunnymilk_cdbg1, "image\\boss\\sunnymilk_cdbg1.png");
	loader.AddSource(src_sunnymilk_cdbg2, "image\\boss\\sunnymilk_cdbg2.png");
	loader.AddSource(src_victorybg, "image\\title\\titlebg_2.png");
	loader.AddSource(src_victory, "image\\ascii\\victory.png");
	loader.AddSource(src_bossname, "image\\front\\bossname.png");

	loader.AddSource(src_reimuface_bs, "image\\face\\reimu\\face_pl00bs.png");
	loader.AddSource(src_reimuface[0], "image\\face\\reimu\\face_pl00hp.png");
	loader.AddSource(src_reimuface[1], "image\\face\\reimu\\face_pl00dp.png");
	loader.AddSource(src_reimuface[2], "image\\face\\reimu\\face_pl00sw.png");
	loader.AddSource(src_reimuface[3], "image\\face\\reimu\\face_pl00an.png");
	loader.AddSource(src_reimuface[5], "image\\face\\reimu\\face_pl00sp.png");
	loader.AddSource(src_reimuface[6], "image\\face\\reimu\\face_pl00pr.png");
	loader.AddSource(src_reimuface[7], "image\\face\\reimu\\face_pl00no.png");
	loader.AddSource(src_reimuface[8], "image\\face\\reimu\\face_pl00n2.png");

	loader.AddSource(src_marisaface_bs, "image\\face\\marisa\\face_pl01bs.png");
	loader.AddSource(src_marisaface[0], "image\\face\\marisa\\face_pl01hp.png");
	loader.AddSource(src_marisaface[1], "image\\face\\marisa\\face_pl01dp.png");
	loader.AddSource(src_marisaface[2], "image\\face\\marisa\\face_pl01sw.png");
	loader.AddSource(src_marisaface[3], "image\\face\\marisa\\face_pl01an.png");
	loader.AddSource(src_marisaface[5], "image\\face\\marisa\\face_pl01sp.png");
	loader.AddSource(src_marisaface[6], "image\\face\\marisa\\face_pl01pr.png");
	loader.AddSource(src_marisaface[7], "image\\face\\marisa\\face_pl01no.png");
	loader.AddSource(src_marisaface[8], "image\\face\\marisa\\face_pl01n2.png");

	loader.AddSource(src_artiaface_ct, "image\\face\\artia\\artia_ct.png");
	loader.AddSource(src_artiaface[0], "image\\face\\artia\\artia_hp.png");
	loader.AddSource(src_artiaface[1], "image\\face\\artia\\artia_dp.png");
	loader.AddSource(src_artiaface[2], "image\\face\\artia\\artia_sw.png");
	loader.AddSource(src_artiaface[3], "image\\face\\artia\\artia_an.png");
	loader.AddSource(src_artiaface[4], "image\\face\\artia\\artia_an2.png");
	loader.AddSource(src_artiaface[5], "image\\face\\artia\\artia_sp.png");
	loader.AddSource(src_artiaface[7], "image\\face\\artia\\artia_no.png");

	loader.AddSource(src_monstone_ct, "image\\face\\monstone\\monstone_ct.png");

	loader.AddSource(src_sunny_ct, "image\\face\\sunnymilk\\face01ct.png");
	loader.AddSource(src_sunnyface_bs, "image\\face\\sunnymilk\\face01no.png");
	loader.AddSource(src_sunnyface[0], "image\\face\\sunnymilk\\face01hp.png");
	loader.AddSource(src_sunnyface[1], "image\\face\\sunnymilk\\face01dp.png");
	loader.AddSource(src_sunnyface[2], "image\\face\\sunnymilk\\face01sw.png");
	loader.AddSource(src_sunnyface[3], "image\\face\\sunnymilk\\face01an.png");
	loader.AddSource(src_sunnyface[5], "image\\face\\sunnymilk\\face01sp.png");
	loader.AddSource(src_sunnyface[6], "image\\face\\sunnymilk\\face01pr.png");
	loader.AddSource(src_sunnyface[8], "image\\face\\sunnymilk\\face01n2.png");
	loader.AddSource(src_sunnyface[9], "image\\face\\sunnymilk\\face01lo.png");

	loader.Load();
}