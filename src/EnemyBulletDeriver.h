#pragma once
#include "EnemyBullet.h"
#include "EffectColor.h"
#include "Mathf.h"

class DianDan :public EnemyBullet
{
public:

	DianDan() :EnemyBullet(AR_NONE, false)
	{
		breakSize = 0.4;
		breakColor = breakColor16;
		checkRadius = 2.4;
		transform->scale = { 8,8,1 };
		sr->SetTexture(src_bullet_1.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		if (index < 8)
			sr->SetSourceRect({ index * 8 / 256.0f,240 / 256.0f,8 / 256.0f,8 / 256.0f });
		else
			sr->SetSourceRect({ (index - 8) * 8 / 256.0f,248 / 256.0f,8 / 256.0f,8 / 256.0f });
	}
};

class JunDan :public EnemyBullet
{
public:

	JunDan() :EnemyBullet(AR_NONE, false)
	{
		breakSize = 0.4;
		breakColor = breakColor16;
		checkRadius = 2.4;
		transform->scale = { 8,8,1 };
		sr->SetTexture(src_bullet_1.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		if (index < 8)
			sr->SetSourceRect({ index * 8 / 256.0f,192 / 256.0f,8 / 256.0f,8 / 256.0f });
		else
			sr->SetSourceRect({ (index - 8) * 8 / 256.0f,200 / 256.0f,8 / 256.0f,8 / 256.0f });
	}
};

class XiaoYu :public EnemyBullet
{
public:

	XiaoYu() :EnemyBullet(AR_NONE, false)
	{
		breakSize = 0.6;
		breakColor = breakColor16;
		checkRadius = 4;
		transform->scale = { 16,16,1 };
		sr->SetTexture(src_bullet_1.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 16 / 256.0f,48 / 256.0f,16 / 256.0f,16 / 256.0f });
	}
};

class HuanYu :public EnemyBullet
{
public:

	HuanYu() :EnemyBullet(AR_NONE, false)
	{
		breakSize = 0.6;
		breakColor = breakColor16;
		checkRadius = 4;
		transform->scale = { 16,16,1 };
		sr->SetTexture(src_bullet_1.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 16 / 256.0f,32 / 256.0f,16 / 256.0f,16 / 256.0f });
	}
};

class MiDan :public EnemyBullet
{
public:

	MiDan() :EnemyBullet(AR_VELOCITY, false)
	{
		breakSize = 0.5;
		breakColor = breakColor16;
		checkRadius = 2.4;
		transform->scale = { 16,16,1 };
		sr->SetTexture(src_bullet_1.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 16 / 256.0f,64 / 256.0f,16 / 256.0f,16 / 256.0f });
	}
};

class LinDan :public EnemyBullet
{
public:

	LinDan() :EnemyBullet(AR_VELOCITY, false)
	{
		breakSize = 0.5;
		breakColor = breakColor16;
		checkRadius = 2.4;
		transform->scale = { 16,16,1 };
		sr->SetTexture(src_bullet_1.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 16 / 256.0f,16 / 256.0f,16 / 256.0f,16 / 256.0f });
	}
};

class LianDan :public EnemyBullet
{
public:

	LianDan() :EnemyBullet(AR_VELOCITY, false)
	{
		breakSize = 0.5;
		breakColor = breakColor16;
		checkRadius = 2.4;
		transform->scale = { 16,16,1 };
		sr->SetTexture(src_bullet_1.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 16 / 256.0f,80 / 256.0f,16 / 256.0f,16 / 256.0f });
	}
};

class ZhenDan :public EnemyBullet
{
public:

	ZhenDan() :EnemyBullet(AR_VELOCITY, false)
	{
		breakSize = 0.5;
		breakColor = breakColor16;
		checkRadius = 2.4;
		transform->scale = { 16,16,1 };
		sr->SetTexture(src_bullet_1.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 16 / 256.0f,96 / 256.0f + 0.001f,16 / 256.0f,16 / 256.0f - 0.002f });
	}
};

class ZhaDan :public EnemyBullet
{
public:

	ZhaDan() :EnemyBullet(AR_VELOCITY, false)
	{
		breakSize = 0.55;
		breakColor = breakColor16;
		checkRadius = 2.8;
		transform->scale = { 16,16,1 };
		sr->SetTexture(src_bullet_1.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 16 / 256.0f,112 / 256.0f,16 / 256.0f,16 / 256.0f });
	}
};

class ChongDan :public EnemyBullet
{
public:

	ChongDan() :EnemyBullet(AR_VELOCITY, false)
	{
		breakSize = 0.5;
		breakColor = breakColor16;
		checkRadius = 2.4;
		transform->scale = { 16,16,1 };
		sr->SetTexture(src_bullet_1.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 16 / 256.0f,128 / 256.0f,16 / 256.0f,16 / 256.0f });
	}
};

class XingDanS :public EnemyBullet
{
public:

	XingDanS() :EnemyBullet(AR_ROTATECCW, false)
	{
		breakSize = 0.6;
		breakColor = breakColor16;
		checkRadius = 4;
		transform->scale = { 16,16,1 };
		sr->SetTexture(src_bullet_1.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 16 / 256.0f,160 / 256.0f,16 / 256.0f,16 / 256.0f });
	}
};

class ZhongYu :public EnemyBullet
{
public:

	ZhongYu() :EnemyBullet(AR_NONE, false)
	{
		breakSize = 0.8;
		breakColor = breakColor8;
		checkRadius = 8.5;
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_bullet_2.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 32 / 256.0f,32 / 256.0f,32 / 256.0f,32 / 256.0f });
	}
};

class TuoDan :public EnemyBullet
{
public:

	TuoDan() :EnemyBullet(AR_VELOCITY, false)
	{
		breakSize = 0.7;
		breakColor = breakColor8;
		checkRadius = 7;
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_bullet_2.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 32 / 256.0f,128 / 256.0f,32 / 256.0f,32 / 256.0f });
	}
};

class DaoDan :public EnemyBullet
{
public:

	DaoDan() :EnemyBullet(AR_VELOCITY, false)
	{
		breakSize = 0.6;
		breakColor = breakColor8;
		checkRadius = 6;
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_bullet_2.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 32 / 256.0f,96 / 256.0f,32 / 256.0f,32 / 256.0f });
	}
};

class DieDan:public EnemyBullet
{
public:

	DieDan() :EnemyBullet(AR_VELOCITY, false)
	{
		breakSize = 0.7;
		breakColor = breakColor8;
		checkRadius = 7;
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_bullet_2.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 32 / 256.0f,64 / 256.0f,32 / 256.0f,32 / 256.0f });
	}
};

class XingDanL :public EnemyBullet
{
public:

	XingDanL() :EnemyBullet(AR_ROTATECCW, false)
	{
		breakSize = 0.7;
		breakColor = breakColor8;
		checkRadius = 7;
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_bullet_2.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 32 / 256.0f,0 / 256.0f,32 / 256.0f,32 / 256.0f });
	}
};

class XinDan :public EnemyBullet
{
public:

	XinDan() :EnemyBullet(AR_VELOCITY, false)
	{
		breakSize = 0.8;
		breakColor = breakColor8;
		checkRadius = 10;
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_bullet_3.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 32 / 256.0f,0 / 256.0f,32 / 256.0f,32 / 256.0f });
	}
};

class GuangYuS :public EnemyBullet
{
public:

	GuangYuS() :EnemyBullet(AR_NONE, true)
	{
		breakSize = 0.6;
		breakColor = breakColor8;
		checkRadius = 4;
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_bullet_3.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 32 / 256.0f,64 / 256.0f,32 / 256.0f,32 / 256.0f });
	}
};

class DaYu :public EnemyBullet
{
public:

	DaYu() :EnemyBullet(AR_NONE, true)
	{
		outOfRangeTolerance = 32;
		breakSize = 1.0;
		breakColor = breakColor4;
		checkRadius = 14;
		transform->scale = { 64,64,1 };
		sr->SetTexture(src_bullet_2.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 64 / 256.0f,192 / 256.0f,64 / 256.0f,64 / 256.0f });
	}
};

class GuangYuL :public EnemyBullet
{
public:

	GuangYuL() :EnemyBullet(AR_NONE, true)
	{
		breakSize = 0.8;
		breakColor = breakColor8;
		checkRadius = 12;
		transform->scale = { 64,64,1 };
		sr->SetTexture(src_bullet_4.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		if (index < 4)
			sr->SetSourceRect({ index * 64 / 256.0f,0 / 256.0f,64 / 256.0f,64 / 256.0f });
		else
			sr->SetSourceRect({ (index - 4) * 64 / 256.0f,64 / 256.0f,64 / 256.0f,64 / 256.0f });
	}
};

class YanDan :public EnemyBullet
{
public:

	YanDan() :EnemyBullet(AR_VELOCITY, true), frame(0)
	{
		breakSize = 0.6;
		breakColor = breakColorYanDan;
		checkRadius = 4;
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_bullet_3.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		if (index < 2)
			sr->SetSourceRect({ index * 128 / 256.0f,128 / 256.0f,32 / 256.0f,32 / 256.0f });
		else
			sr->SetSourceRect({ (index - 2) * 128 / 256.0f,160 / 256.0f,32 / 256.0f,32 / 256.0f });
	}

	void OnUpdate() override
	{
		__super::OnUpdate();
		frame++;
		if (colorIndex < 2)
			sr->SetSourceRect({ (colorIndex * 128 + 32 * (frame / 2 % 4)) / 256.0f,128 / 256.0f,32 / 256.0f,32 / 256.0f });
		else
			sr->SetSourceRect({ ((colorIndex - 2) * 128 + 32 * (frame / 2 % 4)) / 256.0f,160 / 256.0f,32 / 256.0f,32 / 256.0f });
	}

private:

	int frame;
};

class FangDan :public EnemyBullet
{
public:

	FangDan() :EnemyBullet(AR_ROTATECCW, false)
	{
		breakSize = 0.7;
		breakColor = breakColor8;
		checkRadius = 7;
		transform->scale = { 32,32,1 };
		sr->SetTexture(src_bullet_3.Get());
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ index * 32 / 256.0f,224 / 256.0f,32 / 256.0f,32 / 256.0f });
	}
};

class LaserCurve :public EnemyBullet
{
public:

	class Part :public EnemyBullet
	{
	public:

		float width;

		Part(bool _cleanOnBomb, bool _cleanOnOutOfRange, bool _cleanOnHit, int _durFrame, int _animFrame, float _angle, float _width) :
			EnemyBullet(AR_NONE, true), durFrame(_durFrame), animFrame(_animFrame), frame(0), width(_width)
		{
			cleanOnBomb = _cleanOnBomb;
			cleanOnOutOfRange = _cleanOnOutOfRange;
			cleanOnHit = _cleanOnHit;
			breakSize = 0.06f * width;
			breakColor = breakColor16;
			checkRadius = 0.2f * width;
			transform->scale = { 0,12,1 };
			transform->SetEulerAngles({ 0,0,_angle });
			sr->SetTexture(src_bullet_1.Get());
			mat.diffuse.a = 0.35f;
			sr->SetMaterial(mat);
		}

		void SetColor(int index) override
		{
			colorIndex = index;
			sr->SetSourceRect({ index * 16 / 256.0f,0 / 256.0f,16 / 256.0f,16 / 256.0f });
		}

		void OnUpdate() override
		{
			__super::OnUpdate();
			if (IsValid())
			{
				frame++;
				if (frame <= animFrame)
				{
					transform->scale.x = width * pow(frame / (float)animFrame, 0.3);
				}
				if (frame >= durFrame - animFrame)
				{
					transform->scale.x = width * pow(1 - (frame - durFrame + animFrame) / (float)animFrame, 0.3);
				}
				if (frame >= durFrame)
					Destroy();
			}
		}

	private:

		int frame;

		int durFrame;

		int animFrame;
	};

	float width;

	LaserCurve(int _durFrame, int _animFrame = 10) :
		EnemyBullet(AR_NONE, true), durFrame(_durFrame), animFrame(_animFrame), width(10)
	{
		cleanOnHit = false;
		cleanOnBomb = false;
		breakSize = 0.6;
		breakColor = breakColor16;
		checkRadius = 1.5;
	}

	void OnUpdate() override
	{
		__super::OnUpdate();
		if (IsValid())
		{
			auto d = scene->AttachActor(new Part(cleanOnBomb, cleanOnOutOfRange, cleanOnHit, durFrame, animFrame, GetAngle(mb->velocity) - (float)M_PI_2, width));
			d->transform->location = transform->location;
			d->transform->scale.y = 12.0f * mb->velocity.GetLength() / 150.0f;
			d->SetColor(colorIndex);
		}
	}

	void SetColor(int index) override
	{
		colorIndex = index;
	}

private:

	int durFrame;

	int animFrame;
};

class Laser :public EnemyBullet
{
public:

	float angle;

	float width;

	float length;

	bool checking;

	Laser() :EnemyBullet(AR_NONE, true), width(2), angle(0), length(300), checking(true)
	{
		cleanOnHit = false;
		cleanOnBomb = false;
		cleanOnOutOfRange = false;
		breakSize = 0.6;
		breakColor = breakColor16;
		checkRadius = 1.5;
		sr->SetTexture(src_laser_1.Get());
		sr->SetLocalTransform(XMMatrixTranslation(0.5, 0, 0));
	}

	void SetColor(int index) override
	{
		colorIndex = index;
		sr->SetSourceRect({ 0,index * 16 / 256.0f,1,16 / 256.0f });
	}

	void OnCheck() override
	{
		if (player && (!(player->GetCharacterFlags() & CF_ImmuneDamage)) && checking)
		{
			float checkWidth = 0.35f * width - 2;
			float k1 = tan(angle + 0.00001);
			float b1 = transform->location.y - k1 * transform->location.x;
			float d = abs(player->transform->location.x * k1 - player->transform->location.y + b1) / sqrt(k1 * k1 + 1);
			if (d < player->GetAttribution().checkRadius + checkWidth)
			{
				float k2 = -1 / k1;
				float b2 = player->transform->location.y - k2 * player->transform->location.x;
				float x = (b2 - b1) / (k1 - k2);
				float lx1 = transform->location.x + 0.01f * length * (float)cos(angle + 0.00001);
				float lx2 = transform->location.x + 0.99f * length * (float)cos(angle + 0.00001);
				if ((x<lx1 && x>lx2) || (x > lx1&& x < lx2))
				{
					OnHit(player.Get());
				}
			}
			else if ((!grazed) && d < player->GetAttribution().grazeRadius + checkWidth)
			{
				float k2 = -1 / k1;
				float b2 = player->transform->location.y - k2 * player->transform->location.x;
				float x = (b2 - b1) / (k1 - k2);
				float lx1 = transform->location.x + 0.01f * length * (float)cos(angle + 0.00001);
				float lx2 = transform->location.x + 0.99f * length * (float)cos(angle + 0.00001);
				if ((x<lx1 && x>lx2) || (x > lx1&& x < lx2))
				{
					OnGrazed();
				}
			}
		}
	}

	void OnCreated() override
	{
		__super::OnCreated();
		transform->SetEulerAngles({ 0,0,angle});
		transform->scale = { length,2 * width,1 };
	}

	void OnUpdate() override
	{
		__super::OnUpdate();
		transform->SetEulerAngles({ 0,0,angle});
		transform->scale = { length,2 * width,1 };
	}

	void Clean(BYTE cleanType) override
	{
		if (!IsValid())
			return;
		if (cleanOnHit && cleanType == CLEAN_HIT)
		{
			Destroy();
		}
		else if (cleanOnOutOfRange && cleanType == CLEAN_OUTOFRANGE)
		{
			Destroy();
		}
		else if (cleanOnBomb && cleanType == CLEAN_BOMB)
		{
			Destroy();
		}
		else if (cleanType == CLEAN_BONUS)
		{
			Destroy();
		}
	}
};