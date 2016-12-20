#include "Unit.h"


Unit::Unit()
:_Uid(0)
, _PosX(0)
, _PosY(0)
, _PrePosX(0)
, _PrePosY(0)
, _GoalX(-1)
, _GoalY(-1)
, _CurrState(UnitState::None)
, _PreState(UnitState::None)
, _SpriteData(nullptr)
, _Property(nullptr)
, _IsFinding(false)
, _Direcetion(Direction::South)
, _PreDirecetion(Direction::South)

{
}

Unit::~Unit()
{

}

void	Unit::onUdate(float dt)
{


}

void	Unit::chageState(UnitState state)
{
	_PreState = _CurrState;
	_CurrState = state;
}

