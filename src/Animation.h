#pragma once
#include "VirtualLib.h"

typedef struct {
	vector<RectangleF> rects;
	int interval;
}Animation;

class AnimationGroup
{
public:

	AnimationGroup(ITexture* src) :source(src), animIndex(0), frameIndex(0) {}

	void AddAnimation(const Animation& anim)
	{
		anims.emplace_back(anim);
	}

	void SetAnimIndex(int index)
	{
		if (animIndex != index)
			frameIndex = 0;
		animIndex = index;
	}

	void SetFrame(int frame)
	{
		frameIndex = frame;
	}

	void NextFrame()
	{
		frameIndex++;
		if (frameIndex >= anims[animIndex].rects.size())
			frameIndex = 0;
	}

	int GetAnimIndex() const
	{
		return animIndex;
	}

	int GetFrameIndex() const
	{
		return frameIndex;
	}

	ITexture* GetTexture() const
	{
		return source.Get();
	}

	RectangleF GetRect() const
	{
		return anims[animIndex].rects[frameIndex];
	}

	int GetInterval() const
	{
		return anims[animIndex].interval;
	}

	int GetFrameCount(int animIndex) const
	{
		return anims[animIndex].rects.size();
	}

private:

	ComPtr<ITexture> source;

	vector<Animation> anims;

	int animIndex;

	int frameIndex;
};