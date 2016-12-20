#include "GameProcess.h"
#include "Unit.h"
#include "UnitProperty.h"
#include "SpriteUnit.h"
#include "aStarLibrary.h"
#include "MapScrollView.h"


GameProcess*	GameProcess::_instance = nullptr;

GameProcess * GameProcess::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new GameProcess();
	}
	return _instance;
}

GameProcess::GameProcess()
: _DungeonMap(nullptr)
, _TickCount(0)
, _Player(nullptr)
{
	_UserList.clear();
	_EnemyList.clear();
}


GameProcess::~GameProcess()
{

}

void	GameProcess::onUdate(float dt)
{
	_TickCount++;
	findPath();
	updateUnit(dt);
}

void	GameProcess::updateUnit(float dt)
{
	SpriteUnit * sprite = (SpriteUnit*)_Player->getSpriteData();
	sprite->setPosition(cocos2d::Vec2(_Player->getPosX(), _Player->getPosY()));
	sprite->changeSpriteAnimation();
}

void	GameProcess::init()
{
	for (int y = 0; y < MAX_HEIGHT; y++)
	{
		for (int x = 0; x < MAX_WIDTH; x++)
		{
			TileState * tileState = _DungeonMap->getTileState(x, y);
			if (tileState->_tile != ' '
				&& tileState->_tile != '+'
				&& tileState->_tile != '-'
				&& tileState->_tile != '<'
				&& tileState->_tile != '>'
		//		&& (tileState->_isMonster|| tileState->_isTreasure)
 				)
			{
				walkability[x][y] = unwalkable;
			}
			else
			{
				walkability[x][y] = walkable;
			}
		}
	}
	InitializePathfinder();
}

void	GameProcess::createPlayer(cocos2d::Layer * layer)
{
	std::pair<int, int> pos;
	
	pos = _DungeonMap->findTilePos(Dungeon::Tile::UpStairs);
	UnitProperty * property = new UnitProperty();
	property->_name = "DarkKnight";
	property->_uid = 1;
	_Player = new Unit();
	_Player->setPosX(pos.first * TILE_SIZE + TILE_SIZE*0.5);
	_Player->setPosY(pos.second * TILE_SIZE + TILE_SIZE*0.5);
	_Player->setPrePosX(_Player->getPosX());
	_Player->setPrePosY(_Player->getPosY());
	_Player->setProperty(property);
	
	auto	spr = SpriteUnit::create(_Player);
	_Player->setSpriteData(spr);
	layer->addChild(spr);
		
	cocos2d::Vec2 vecPos;
		
	vecPos.x = (float)_Player->getPosX() * _mapScrollView->getScale();
	vecPos.y = (float)_Player->getPosY() * _mapScrollView->getScale();
	vecPos.x = (_mapScrollView->getContentSize().width - vecPos.x) - _mapScrollView->getContentSize().width / 2;
	vecPos.y = (_mapScrollView->getContentSize().height - vecPos.y) - _mapScrollView->getContentSize().height / 2;
	_mapScrollView->setCenterPos(vecPos);
}

void	GameProcess::findPath()
{
	if (_Player->getIsFinding() == true)
	{
		int localPosX, localPosY, goalX, goalY = 0;
		localPosX = _Player->getPosX();
		localPosY = _Player->getPosY();
		goalX = _Player->getGoalX();
		goalY = _Player->getGoalY();

		if (goalX < 0)
			return;
		
		pathStatus[1] = FindPath(1, localPosX, localPosY, goalX, goalY);
		if (pathStatus[1] == found)
		{

			bool isFinding = false;
			_Player->setPreState(_Player->getCurrState());
			_Player->chageState(Unit::Move);
			//1.Read path information
			ReadPath(1, localPosX, localPosY, 1);
			//2.Move sprite. xLoc/yLoc = current location of sprite. xPath and
			//	yPath = coordinates of next step on the path that were/are
			//	read using the readPath function.
			if (localPosX > xPath[1])
			{
				isFinding = true;
				localPosX = localPosX - 1; //이동속도
				_Player->setPreDirecetion(_Player->getDirecetion());
				_Player->setDirecetion(Unit::West);
			}
			
			if (localPosX < xPath[1])
			{
				isFinding = true;
				localPosX = localPosX + 1;
				_Player->setPreDirecetion(_Player->getDirecetion());
				_Player->setDirecetion(Unit::East);
			}

			if (localPosY > yPath[1])
			{
				isFinding = true;
				localPosY = localPosY - 1;
				_Player->setPreDirecetion(_Player->getDirecetion());
				_Player->setDirecetion(Unit::South);
			}
			if (localPosY < yPath[1])
			{
				isFinding = true;
				localPosY = localPosY + 1;
				_Player->setPreDirecetion(_Player->getDirecetion());
				_Player->setDirecetion(Unit::North);
			}
			//3.When sprite reaches the end location square	(end of its current
			//	path) ...		

			if (isFinding == false)
			{
				_Player->setIsFinding(false);
				_Player->chageState(Unit::None);
				return;
			}
			if (pathLocation[1] == pathLength[1])
			{
				// 	Center the chaser in the square (not really necessary, but 
				//	it looks a little better for the chaser, which moves in 3 pixel
				//	increments and thus isn't always centered when it reaches its
				//	target).
				if (abs(localPosX - xPath[1]) < 1) localPosX = xPath[1];
				if (abs(localPosY - yPath[1]) < 1) localPosY = yPath[1];
			}
			_Player->setPrePosX(_Player->getPosX());
			_Player->setPrePosY(_Player->getPosY());
			_Player->setPosX(localPosX);
			_Player->setPosY(localPosY);

			float mapScale = _mapScrollView->getScale();
			cocos2d::Vec2 pos, prePos;
			cocos2d::Vec2 mapPosition = _mapScrollView->getPosition();
			cocos2d::Size mapContentSize = _mapScrollView->getContentSize();
			cocos2d::Size mapSize = _mapScrollView->getMapSize();

			pos.x = (float)_Player->getPosX() * mapScale * 2;  
			pos.y = (float)_Player->getPosY() * mapScale * 2;
			pos.x = (mapContentSize.width + pos.x) - mapContentSize.width * 0.5f;
			pos.y = (mapContentSize.height + pos.y) - mapContentSize.height * 0.5f;
			
			prePos.x = (float)_Player->getPrePosX()* mapScale * 2;
			prePos.y = (float)_Player->getPrePosY()* mapScale * 2;
			prePos.x = (mapContentSize.width + prePos.x) - mapContentSize.width * 0.5f;
			prePos.y = (mapContentSize.height + prePos.y) - mapContentSize.height * 0.5f;
			
			//캐릭터 중앙 맞추기 
			if (mapPosition.x >= 0)
			{
				if (_Player->getPosX()*mapScale < mapContentSize.width *0.5f) pos.x = prePos.x;
			}
			if (mapPosition.y >= 0)
			{
				if (_Player->getPosY()*mapScale < mapContentSize.height *0.5f) pos.y = prePos.y;
			}
			if (mapPosition.x <= _mapScrollView->getMaxDotWidth())
			{
				if (_Player->getPosX()*mapScale > mapSize.width*mapScale - (mapContentSize.width *0.5f)) pos.x = prePos.x;
			}
			if (mapPosition.y <= _mapScrollView->getMaxDotHeight())
			{
				if (_Player->getPosY()*mapScale > mapSize.height*mapScale - (mapContentSize.height *0.5f)) pos.y = prePos.y;
			}
			CCLOG("max X : %f", mapPosition.x);
			moveScrollView(prePos, pos);
		}
		else
		{
			_Player->setIsFinding(false);
			_Player->chageState(Unit::None);
			//EndPathfinder();
		}
	}
}

void	GameProcess::createEnemy(cocos2d::Layer * layer)
{
	std::string name[10] = {"DemonEvil","Goblin", "Ogre", "Skeleton", "Troll", "Witch", "LavaElemental", "Cerberus", "Necromancer", "Orc"};
	for (int i = 0; i < 10; i++)
	{
		std::pair<int, int> pos;
		pos = _DungeonMap->setMonsterPos(Dungeon::Tile::ClosedDoor);
		if (pos.first != -1)
		{
			_DungeonMap->getTileState(pos.first, pos.second)->_isMonster = true;
			UnitProperty * property = new UnitProperty();
			property->_name = name[i];
			property->_uid = i;
			Unit * enemy = new Unit();
			enemy->setProperty(property);
			enemy->setPosX(pos.first * TILE_SIZE + TILE_SIZE*0.5);
			enemy->setPosY(pos.second * TILE_SIZE + TILE_SIZE*0.5);
			enemy->setPrePosX(_Player->getPosX());
			enemy->setPrePosY(_Player->getPosY());
			auto	spr = SpriteUnit::create(enemy);
			enemy->setSpriteData(spr);
			layer->addChild(spr);
			_EnemyList.push_back(enemy);
			walkability[pos.first][pos.second] = unwalkable;
		}
		else
		{
			break;
		}
	}
}

void	GameProcess::createDungeon(cocos2d::Node* layer)
{
	_DungeonMap = new Dungeon(MAX_WIDTH, MAX_HEIGHT);
	_DungeonMap->generate(20);
	_DungeonMap->print();
	_DungeonMap->drawTile(layer);
	init();
}

void	GameProcess::moveScrollView(cocos2d::Vec2 pos, cocos2d::Vec2 prePos)
{
	_mapScrollView->handleMoveLogic(pos, prePos);
}