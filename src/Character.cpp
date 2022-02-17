#include "Character.h"
#include "EngineGlobal.h"

class Character::AnimationPerformer :public AComponent
{
public:

	Character* character;

	AnimationGroup& animGroup;

	SpriteRender* sr;

	int count;

	int lastAnimIndex;

	AnimationPerformer(Character* _character) :
		character(_character), animGroup(_character->animGroup), sr(_character->sr), count(0), lastAnimIndex(0) {}

	void OnUpdate() override
	{
		if (lastAnimIndex != animGroup.GetAnimIndex())
		{
			lastAnimIndex = animGroup.GetAnimIndex();
			count = 0;
		}
		count++;
		if (count >= animGroup.GetInterval())
		{
			count = 0;
			animGroup.NextFrame();
		}
		sr->SetSourceRect(animGroup.GetRect());
		sr->SetTexture(animGroup.GetTexture());
	}
};

Character::Character(const AnimationGroup& _animGroup) :
	characterFlags(0), animGroup(_animGroup), sr(AddComponent(new SpriteRender))
{
	Material mat;
	mat.ambient = { 0,0,0,0 };
	mat.diffuse = { 1,1,1,1 };
	mat.power = 0;
	mat.specular = { 0,0,0,0 };
	sr->SetMaterial(mat);
	AddComponent(new AnimationPerformer(this));
}

void Character::OnUpdate()
{
	if (characterFlags & CharacterFlag::CF_NoDraw)
		sr->SetActive(false);
	else
		sr->SetActive(true);
	if (characterFlags & CF_Floating)
		sr->SetLocalTransform(XMMatrixTranslation(0, 0.05 * sin(Global::timer->GetGameTime() * 4), 0));
}

AnimationGroup& Character::GetAnimationGroup()
{
	return animGroup;
}

void Character::SetCharacterFlags(DWORD flags)
{
	characterFlags = flags;
}

DWORD Character::GetCharacterFlags() const
{
	return characterFlags;
}