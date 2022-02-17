#pragma once
#include "VirtualLib.h"
#include "Animation.h"

enum CharacterFlag
{
	CF_ImmuneDamage = 1,
	CF_NoDraw = 2,
	CF_Floating = 4
};

class Character :public GameObject
{
public:

	Character(const AnimationGroup& _animGroup);

	AnimationGroup& GetAnimationGroup();

	void SetCharacterFlags(DWORD flags);

	DWORD GetCharacterFlags() const;

protected:

	DWORD characterFlags;

	AnimationGroup animGroup;

	SpriteRender* sr;

	class AnimationPerformer;

	void OnUpdate() override;
};