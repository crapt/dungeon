#include	"LayerUnit.h"
#include	"SpriteUnit.h"


USING_NS_CC;


LayerUnit::LayerUnit()
{
}

LayerUnit::~LayerUnit()
{
}

bool	LayerUnit::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}
	return true;
}

