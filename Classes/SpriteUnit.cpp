#include	"SpriteUnit.h"
#include	"Unit.h"
#include	"UnitProperty.h"

USING_NS_CC;


SpriteUnit*	SpriteUnit::create(Unit* unit)
{
	SpriteUnit	*spr = new (std::nothrow) SpriteUnit();
	if (spr && spr->init(unit))
	{
		spr->autorelease();
		return spr;
	}
	CC_SAFE_DELETE(spr);
	return nullptr;
}


bool	SpriteUnit::init(Unit* unit)
{
	setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_unit = unit;
	std::string fileName = unit->getProperty()->_name + ".csb";
	std::string unitName = unit->getProperty()->_name;
	
	_unit->setSpriteData((void*)this);
	_actionNode = CSLoader::getInstance()->createNode("res/Animation/Unit/" + unitName + "/" + fileName);

	const auto& children = _actionNode->getChildren();
	
	float _scale = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		
		_moveNode[i] = children.at(i);
		_scale = TILE_SIZE / _moveNode[i]->getContentSize().width* 2.0f;
		_moveNode[i]->setScale(_scale);
		_moveNode[i]->setVisible(false);

	}
	//_moveNode[0]->setVisible(true);  //north
	//_moveNode[1]->setVisible(true); //east
	_moveNode[2]->setVisible(true);   //south
	//_moveNode[3]->setVisible(true); //west
	
	for (int i = 4, j = 0; i < 6; i++, j++)
	{
		_attackNode[j] = children.at(i);
		_attackNode[j]->setScale(_scale);
		_attackNode[j]->setVisible(false);
	}

	for (int i = 0; i < 2; i++)
		_dieNode[i] = nullptr;

	_dieNode[0] = children.at(6);
	_dieNode[0]->setVisible(false);
	_dieNode[0]->setScale(_scale);


	if (children.size()>7) {
		_dieNode[1] = children.at(7);
		_dieNode[1]->setVisible(false);
		_dieNode[1]->setScale(_scale);
	}
		
	_action = CSLoader::createTimeline("res/Animation/Unit/"+unitName+"/" + fileName);
	_action->setAnimationEndCallFunc("Die", CC_CALLBACK_0(SpriteUnit::releaseSpriteUnit, this));
	_actionNode->runAction(_action);
	_action->gotoFrameAndPlay(0, 0, false);
	
	
	addChild(_actionNode);
	
	//hp
// 	_hpLength = iRect.right - iRect.left + 20;
// 	_originX = _hpLength *0.5f;
// 	_originY = (iRect.bottom - iRect.top) *0.5f + 20;
// 	_hpLength = ((float)unit->_hp * _hpLength) / (float)unit->_hpMax;
// 	_hpDrawNode = cocos2d::DrawNode::create();
// 	_hpDrawNode->setAnchorPoint(Vec2(0, 0));
// 	_hpDrawNode->drawSolidRect(Vec2(-_originX, -_originY), Vec2(-_originX + _hpLength, -_originY + 5), Color4F(0, 1, 0, 1));
// 	addChild(_hpDrawNode);

	setPosition(cocos2d::Vec2(_unit->getPosX(), _unit->getPosY()));
	changeSpriteAnimation();
	return true;

}

void	SpriteUnit::releaseSpriteUnit()
{
	//CCLOG("Die!");
	_sprShadow->removeFromParentAndCleanup(true);
	this->removeFromParentAndCleanup(true);
}

void	SpriteUnit::insterUnit(cocos2d::Layer * layer)
{
	layer->addChild(this);
}

void	SpriteUnit::setPosition(const Vec2 &position)
{
	//auto	pos = positionMapToScreen(position);

	//spriteShadow()->setPosition(pos);
	Node::setPosition(position);
}

void	SpriteUnit::playDieAction()
{
	// 	for (int i = 0; i < 16; i++)
	// 	{
	// 		_moveNode[i]->setVisible(false);
	// 		_attackNode[i]->setVisible(false);
	// 	}
	// 	_dieNode->setVisible(true);
	// 	_action->play("Die", false);

	for (int i = 0; i < 4; i++)
	{
		_moveNode[i]->setVisible(false);
	}
	for (int i = 0; i < 2; i++)
	{
		_attackNode[i]->setVisible(false);

	}
// 	ANGLE  tmpDieAngle;
// 
// 	if (_curAngle == 0 || _curAngle == 1)
// 		tmpDieAngle = 0;
// 	else
// 		tmpDieAngle = 1;
// 
// 	if (_dieNode[tmpDieAngle])
// 		_dieNode[tmpDieAngle]->setVisible(true);
// 	else
// 		_dieNode[0]->setVisible(true);

	_action->setTimeSpeed(2.0f);
	_action->play("Die", false);

}

void	SpriteUnit::setHp()
{
// 	float temp = (float)_unit->_hp / _unit->_hpMax;
// 	_hpDrawNode->setScaleX(temp);
}

void	SpriteUnit::callbackAttackAnimation()
{
	

}

void	SpriteUnit::changeSpriteAnimation()
{

	if (_unit->getCurrState() != _unit->getPreState())
	{
		switch (_unit->getCurrState())
		{
		case	Unit::None:
		{
			
							  for (int i = 0; i < 4; i++)
							  {
								  _moveNode[i]->setVisible(false);

							  }
							  for (int i = 0; i < 2; i++)
								  _attackNode[i]->setVisible(false);

							  _moveNode[_unit->getDirecetion()]->setVisible(true);
							  _action->gotoFrameAndPlay(0, 0, false);
							  
															
		}
			break;
		case	Unit::Move:
		{
			
							  for (int i = 0; i < 4; i++)
							  {
								  _moveNode[i]->setVisible(false);

							  }
							  for (int i = 0; i < 2; i++)
								  _attackNode[i]->setVisible(false);

							  _action->setTimeSpeed(2.0f);
							  _action->play("Move", true);
							  int temp = _unit->getDirecetion();
							  _moveNode[_unit->getDirecetion()]->setVisible(true);
							  
												
														

		}
			break;
	
		case	Unit::Attack:
		{
								for (int i = 0; i < 4; i++)
								{
									_moveNode[i]->setVisible(false);
								}
								for (int i = 0; i < 2; i++)
									_attackNode[i]->setVisible(false);
								

// 								ANGLE  tmpAttackAngle;
// 								if (curAngle == 0 || curAngle == 1)
// 									tmpAttackAngle = 0;
// 								else
// 									tmpAttackAngle = 1;

								_attackNode[0]->setVisible(true);
								_action->setTimeSpeed(1.0f);
								_action->play("Attack", false);
								//_isAttackAnimationPlay = true;
								_action->setAnimationEndCallFunc("Attack", CC_CALLBACK_0(SpriteUnit::callbackAttackAnimation, this));
												
		}
			break;
		default:
			break;
		}
		//_state = state;
	}

	if (_unit->getPreDirecetion() != _unit->getDirecetion())
	{
		
		for (int i = 0; i < 4; i++)
		{
			_moveNode[i]->setVisible(false);
		}
		for (int i = 0; i < 2; i++)
			_attackNode[i]->setVisible(false);

		int tmpAttackAngle;
		if (_unit->getDirecetion() == 0 || _unit->getDirecetion() == 1)
			tmpAttackAngle = 0;
		else
			tmpAttackAngle = 1;

		if (_unit->getCurrState() == Unit::Attack)
			_attackNode[tmpAttackAngle]->setVisible(true);
		else {
			_moveNode[_unit->getDirecetion()]->setVisible(true);
		}
	}
}


