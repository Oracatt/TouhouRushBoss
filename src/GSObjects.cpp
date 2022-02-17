#include "GSObjects.h"

const char charArray[7][13] =
{
	{'A','B','C','D','E','F','G','H','I','J','K','L','M'},
	{'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'},
	{'a','b','c','d','e','f','g','h','i','j','k','l','m'},
	{'n','o','p','q','r','s','t','u','v','w','x','y','z'},
	{'0','1','2','3','4','5','6','7','8','9','+','-','='},
	{'.',',','!','?','@',':',';','[',']','(',')','_','/'},
	{'{','}','|','~','^','#','$','%','&','*',129,127,128}
};

void GamePauseMenu::KeyboardMenu::LeaveForSaveMenu()
{
	for (int r = 0; r < 7; r++)
	{
		for (int c = 0; c < 13; c++)
		{
			if (keys[r][c])
				keys[r][c]->Destroy();
		}
	}
	textRef->Destroy();
	editor->Destroy();
	auto g = new GamePauseMenu::SaveMenu;
	g->menu = menu;
	scene->AttachActor(g);
	Destroy();
}

void GameOverMenu::KeyboardMenu::LeaveForSaveMenu()
{
	for (int r = 0; r < 7; r++)
	{
		for (int c = 0; c < 13; c++)
		{
			if (keys[r][c])
				keys[r][c]->Destroy();
		}
	}
	textRef->Destroy();
	editor->Destroy();
	auto g = new GameOverMenu::SaveMenu;
	g->menu = menu;
	scene->AttachActor(g);
	Destroy();
}

void Victory::KeyboardMenu::LeaveForSaveMenu()
{
	for (int r = 0; r < 7; r++)
	{
		for (int c = 0; c < 13; c++)
		{
			if (keys[r][c])
				keys[r][c]->Destroy();
		}
	}
	textRef->Destroy();
	editor->Destroy();
	auto g = new Victory::SaveMenu;
	scene->AttachActor(g);
	Destroy();
}