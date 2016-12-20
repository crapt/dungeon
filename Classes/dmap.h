#include <random>
#include <vector>
#include <iostream>

#include "cocos2d.h"
#include "GameDefine.h"

namespace
{
	std::random_device rd;
	std::mt19937 mt(rd());

	int randomInt(int exclusiveMax)
	{
		std::uniform_int_distribution<> dist(0, exclusiveMax - 1);
		return dist(mt);
	}

	int randomInt(int min, int max) // inclusive min/max
	{
		std::uniform_int_distribution<> dist(0, max - min);
		return dist(mt) + min;
	}

	bool randomBool(double probability = 0.5)
	{
		std::bernoulli_distribution dist(probability);
		return dist(mt);
	}
}

struct Rect
{
	int x, y;
	int width, height;
};

struct TileState
{
public:

	char	_tile;
	bool	_isMonster;
	bool	_isTreasure;
	TileState()
	{
		_isMonster = false;
		_isTreasure = false;
	}

};


class Dungeon
{
public:
	enum Tile
	{
		Unused = ' ',
		Floor = '.',
		Corridor = ',',
		Wall = '#',
		ClosedDoor = '+',
		OpenDoor = '-',
		UpStairs = '<',
		DownStairs = '>'

	};

	enum Direction
	{
		North,
		South,
		West,
		East,
		DirectionCount
	};

public:

	Dungeon()
		: _width(0)
		, _height(0)
		, _tiles()
		, _rooms()
		, _exits()
	{

	}
	Dungeon(int width, int height)
		: _width(width)
		, _height(height)
		, _rooms()
		, _exits()
	{
		for (int i = 0; i < width* height; i++)
		{
			TileState* tmpTile = new TileState();
			tmpTile->_tile = Unused;
			tmpTile->_isMonster = false;
			tmpTile->_isTreasure = false;
			_tiles.push_back(tmpTile);
		}
	}
	void init(int width, int height)
	{
		_width = width;
		_height = height;
		for (int i = 0; i < width*height; i++)
		{
			TileState* tmpTile = new TileState();
			tmpTile->_tile = Unused;
			tmpTile->_isMonster = false;
			tmpTile->_isTreasure = false;
			_tiles.push_back(tmpTile);
		}
	}

	void generate(int maxFeatures)
	{
		// place the first room in the center
		if (!makeRoom(_width / 2, _height / 2, static_cast<Direction>(randomInt(4), true)))
		{
			std::cout << "Unable to place the first room.\n";
			return;
		}

		// we already placed 1 feature (the first room)
		for (int i = 1; i < maxFeatures; ++i)
		{
			if (!createFeature())
			{
				std::cout << "Unable to place more features (placed " << i << ").\n";
				break;
			}
		}

		if (!placeObject(UpStairs))
		{
			std::cout << "Unable to place up stairs.\n";
			return;
		}

		if (!placeObject(DownStairs))
		{
			std::cout << "Unable to place down stairs.\n";
			return;
		}

		for (int i = 0; i < _tiles.size(); i++)
		{
			char& tile = _tiles[i]->_tile;
			if (tile == Unused)
				tile = '.';
			else if (tile == Floor || tile == Corridor)
				tile = ' ';
		}
	}

	void print()
	{
		for (int y = 0; y < _height; ++y)
		{
			for (int x = 0; x < _width; ++x) {
				CCLOG("%c", getTile(x, y));
				std::cout << getTile(x, y);
			}

			CCLOG("\n");
			std::cout << std::endl;
		}
		
	}

	std::pair<int, int> setMonsterPos(Tile tile)
	{
		std::pair<int, int> pos(-1, -1);
		for (int y = 0; y < _height; ++y)
		{
			for (int x = 0; x < _width; ++x)
			{
				TileState * state = getTileState(x,y);

				if (state->_tile == tile && !state->_isMonster && !state->_isTreasure)
				{
					pos.first = x;
					pos.second = y;
					return pos;
				}
			}
		}
		return pos;
	}


	std::pair<int, int> findTilePos(Tile tile)
	{
		std::pair<int, int> pos(0, 0);
		for (int y = 0; y < _height; ++y)
		{
			for (int x = 0; x < _width; ++x)
			{
				if (getTile(x, y) == tile)
				{
					pos.first = x;
					pos.second = y;
					return pos;
				}
			}
		}
		return pos;
	}

	std::pair<int, int> findTilePos(cocos2d::Vec2 worldPos)
	{
		std::pair<int, int> pos(0, 0);
		int x = worldPos.x / TILE_SIZE;
		int y = worldPos.y / TILE_SIZE;
		pos.first = x;
		pos.second = y;

		return pos;
	}

	void drawTile(cocos2d::Node* layer)
	{

		cocos2d::Vec2 tilePos = cocos2d::Vec2(0.0f, 0.0f);
	
		for (int y = 0; y < _height; ++y)
		{
			for (int x = 0; x < _width; ++x) 
			{
				if (getTile(x, y) == ' ')
				{
					int tileNum = rand() % 6 + 1;
					std::string fileName = cocos2d::StringUtils::format("tile_%d.png", tileNum);
					auto sprite = cocos2d::Sprite::create(fileName);
					//sprite->setScale(0.1f);
					sprite->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
					sprite->setPosition(tilePos);
					sprite->setScale(MAP_SCALE);
					layer->addChild(sprite);
				}
				if (getTile(x, y) == '>')
				{
				
				
					auto sprite = cocos2d::Sprite::create("wall_left.png");
					//sprite->setScale(0.1f);
					sprite->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
					sprite->setPosition(tilePos);
					sprite->setScale(MAP_SCALE);
					layer->addChild(sprite);
				}

				cocos2d::Sprite* sprite = nullptr;

				
				if (getTile(x, y) == 'a') sprite = cocos2d::Sprite::create("wall_top.png");
				if (getTile(x, y) == 'b') sprite = cocos2d::Sprite::create("wall_right.png");
				if (getTile(x, y) == 'c') sprite = cocos2d::Sprite::create("wall_bottom.png");
				if (getTile(x, y) == 'd') sprite = cocos2d::Sprite::create("wall_left.png");
				
				if (getTile(x, y) == '1') sprite = cocos2d::Sprite::create("wall_edge_1.png");
				if (getTile(x, y) == '2') sprite = cocos2d::Sprite::create("wall_edge.png");
				if (getTile(x, y) == '3') sprite = cocos2d::Sprite::create("wall_edge_3.png");
				if (getTile(x, y) == '4') sprite = cocos2d::Sprite::create("wall_edge_2.png");
				
				if(sprite)
				{
					sprite->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
					sprite->setPosition(tilePos);
					sprite->setScale(MAP_SCALE);
					layer->addChild(sprite);
				}
				tilePos.x += TILE_SIZE;
			}
			tilePos.y += TILE_SIZE;
			tilePos.x = 0.0f;
		}
		//drawWall(layer);
	}

	// 		Unused = ' ',
	// 			Floor = '.',
	// 			Corridor = ',',
	// 			Wall = '#',
	// 			ClosedDoor = '+',
	// 			OpenDoor = '-',
	// 			UpStairs = '<',
	// 			DownStairs = '>'

	std::string		drawWall(int x, int y)
	{
		bool east = getTile(x + 1, y) == Wall;
		bool west = getTile(x - 1, y) == Wall;
		bool north = getTile(x, y + 1) == Wall;
		bool south = getTile(x, y - 1) == Wall;

		if (east && west && north && south)
		{

		} 
		/*
		int startIdxX, startIdxY;

		for (int y = 0; y < _height; ++y)
		{
			for (int x = 0; x < _width; ++x)
			{
				if (getTile(x, y) == '#')
				{
					startIdxX = x;
					startIdxY = y;
					y = 100000;
					break;
				}
			}
		}
		bool loop = true;
		int currInxX = startIdxX;
		int currInxY = startIdxY;
		
		/*
		enum Direction
		{
			North,
			South,
			West,
			East,
			DirectionCount
		};
				

		std::vector<int> dirList;
		dirList.push_back(100);
		int count = 1;
		while (loop)
		{
			bool east = getTile(currInxX + 1, currInxY) == Wall;
			bool west = getTile(currInxX - 1, currInxY) == Wall;
			bool north = getTile(currInxX, currInxY + 1) == Wall;
			bool south = getTile(currInxX, currInxY - 1) == Wall;
					

			//오른쪽으로 따라가면서 벽을 검색한다
			if (east && dirList[count - 1] != West)
			{				
				dirList.push_back(East);
				count++;
				currInxX += 1;
			}
			else if (south && dirList[count - 1] != North)
			{
				if (west)
				{
					dirList.push_back(West);
					count++;
					currInxX -= 1;
				}
				else if (north)
				{
					dirList.push_back(North);
					count++;
					currInxY += 1;
				} 
				else
				{
					dirList.push_back(South);
					count++;
					currInxY -= 1;
				}
			}
			else if (north && dirList[count - 1] != South)
			{
				if (east)
				{
					dirList.push_back(East);
					count++;
					currInxX += 1;
				}
				else if (south)
				{
					dirList.push_back(South);
					count++;
					currInxY -= 1;
				} 
				else
				{
					dirList.push_back(North);
					count++;
					currInxY += 1;
				}
			}
			
			else if (west && dirList[count - 1] != East)
			{
				dirList.push_back(West);
				count++;
				currInxX -= 1;
			}
			else
			{
				for (int i = 0; i < dirList.size(); i++)
					CCLOG("%d", dirList[i]);

				int a = 1;
			}
			auto sprite = cocos2d::Sprite::create("wall_left.png");
			sprite->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
			sprite->setPosition(cocos2d::Vec2(currInxX * TILE_SIZE, currInxY * TILE_SIZE));
			sprite->setScale(MAP_SCALE);
			layer->addChild(sprite);

			if (currInxX == startIdxX && currInxY == startIdxY)
			{
				loop = false;
			}
			
		}

		/*
		char left = getTile(x - 1, y);
		char right = getTile(x + 1, y);
		char up = getTile(x, y + 1);
		char down = getTile(x, y - 1);

		//왼쪽
		if (right != Floor && (left == Floor || x<1 ) && up == Wall && down == Wall)
			return "wall_left.png";
		//오른쪽
		if (left != Floor && right == Floor && up == Wall && down == Wall)
			return "wall_right.png";
		//위
		if (down != Floor && (up ==Floor || y -1 == _height)&& left == Wall && right == Wall)
			return "wall_top.png";
		//아래
		if (up != Floor && (down == Floor || y <1)&&left == Wall && right == Wall)
			return "wall_bottom.png";

		//왼쪽 위 

		
		*/
		return "";
	}

public:
	
	char getTile(int x, int y) const
	{
		if (x < 0 || y < 0 || x >= _width || y >= _height)
			return Unused;
		return _tiles[x + y * _width]->_tile;
	}

	TileState * getTileState(int x, int y) const
	{
		return _tiles[x + y * _width];
	}


	void setTile(int x, int y, char tile)
	{
		 _tiles[x + y * _width]->_tile =  tile;
	}

	bool createFeature()
	{
		for (int i = 0; i < 1000; ++i)
		{
			if (_exits.empty())
				break;

			// choose a random side of a random room or corridor
			int r = randomInt(_exits.size());
			int x = randomInt(_exits[r].x, _exits[r].x + _exits[r].width - 1);
			int y = randomInt(_exits[r].y, _exits[r].y + _exits[r].height - 1);

			// north, south, west, east
			for (int j = 0; j < DirectionCount; ++j)
			{
				if (createFeature(x, y, static_cast<Direction>(j)))
				{
					_exits.erase(_exits.begin() + r);
					return true;
				}
			}
		}

		return false;
	}

	bool createFeature(int x, int y, Direction dir)
	{
		static const int roomChance = 50; // corridorChance = 100 - roomChance

		int dx = 0;
		int dy = 0;

		if (dir == North)
			dy = 1;
		else if (dir == South)
			dy = -1;
		else if (dir == West)
			dx = 1;
		else if (dir == East)
			dx = -1;

		if (getTile(x + dx, y + dy) != Floor && getTile(x + dx, y + dy) != Corridor)
			return false;

		if (randomInt(100) < roomChance)
		{
			if (makeRoom(x, y, dir))
			{
				setTile(x, y, ClosedDoor);

				return true;
			}
		}

		else
		{
			if (makeCorridor(x, y, dir))
			{
				if (getTile(x + dx, y + dy) == Floor)
					setTile(x, y, ClosedDoor);
				else // don't place a door between corridors
					setTile(x, y, Corridor);

				return true;
			}
		}

		return false;
	}

	bool makeRoom(int x, int y, Direction dir, bool firstRoom = false)
	{
		static const int minRoomSize = 3;
		static const int maxRoomSize = 6;

		Rect room;
		room.width = randomInt(minRoomSize, maxRoomSize);
		room.height = randomInt(minRoomSize, maxRoomSize);

		if (dir == North)
		{
			room.x = x - room.width / 2;
			room.y = y - room.height;
		}

		else if (dir == South)
		{
			room.x = x - room.width / 2;
			room.y = y + 1;
		}

		else if (dir == West)
		{
			room.x = x - room.width;
			room.y = y - room.height / 2;
		}

		else if (dir == East)
		{
			room.x = x + 1;
			room.y = y - room.height / 2;
		}

		if (placeRect(room, Floor))
		{
			_rooms.emplace_back(room);

			if (dir != South || firstRoom) // north side
				_exits.emplace_back(Rect{ room.x, room.y - 1, room.width, 1 });
			if (dir != North || firstRoom) // south side
				_exits.emplace_back(Rect{ room.x, room.y + room.height, room.width, 1 });
			if (dir != East || firstRoom) // west side
				_exits.emplace_back(Rect{ room.x - 1, room.y, 1, room.height });
			if (dir != West || firstRoom) // east side
				_exits.emplace_back(Rect{ room.x + room.width, room.y, 1, room.height });

			return true;
		}

		return false;
	}

	bool makeCorridor(int x, int y, Direction dir)
	{
		static const int minCorridorLength = 3;
		static const int maxCorridorLength = 6;

		Rect corridor;
		corridor.x = x;
		corridor.y = y;

		if (randomBool()) // horizontal corridor
		{
			corridor.width = randomInt(minCorridorLength, maxCorridorLength);
			corridor.height = 1;

			if (dir == North)
			{
				corridor.y = y - 1;

				if (randomBool()) // west
					corridor.x = x - corridor.width + 1;
			}

			else if (dir == South)
			{
				corridor.y = y + 1;

				if (randomBool()) // west
					corridor.x = x - corridor.width + 1;
			}

			else if (dir == West)
				corridor.x = x - corridor.width;

			else if (dir == East)
				corridor.x = x + 1;
		}

		else // vertical corridor
		{
			corridor.width = 1;
			corridor.height = randomInt(minCorridorLength, maxCorridorLength);

			if (dir == North)
				corridor.y = y - corridor.height;

			else if (dir == South)
				corridor.y = y + 1;

			else if (dir == West)
			{
				corridor.x = x - 1;

				if (randomBool()) // north
					corridor.y = y - corridor.height + 1;
			}

			else if (dir == East)
			{
				corridor.x = x + 1;

				if (randomBool()) // north
					corridor.y = y - corridor.height + 1;
			}
		}

		if (placeRect(corridor, Corridor))
		{
			if (dir != South && corridor.width != 1) // north side
				_exits.emplace_back(Rect{ corridor.x, corridor.y - 1, corridor.width, 1 });
			if (dir != North && corridor.width != 1) // south side
				_exits.emplace_back(Rect{ corridor.x, corridor.y + corridor.height, corridor.width, 1 });
			if (dir != East && corridor.height != 1) // west side
				_exits.emplace_back(Rect{ corridor.x - 1, corridor.y, 1, corridor.height });
			if (dir != West && corridor.height != 1) // east side
				_exits.emplace_back(Rect{ corridor.x + corridor.width, corridor.y, 1, corridor.height });

			return true;
		}

		return false;
	}

	bool placeRect(const Rect& rect, char tile)
	{
		if (rect.x < 1 || rect.y < 1 || rect.x + rect.width > _width - 1 || rect.y + rect.height > _height - 1)
			return false;

		for (int y = rect.y; y < rect.y + rect.height; ++y)
		for (int x = rect.x; x < rect.x + rect.width; ++x)
		{
			if (getTile(x, y) != Unused)
				return false; // the area already used
		}

		for (int y = rect.y - 1; y < rect.y + rect.height + 1; ++y)
		for (int x = rect.x - 1; x < rect.x + rect.width + 1; ++x)
		{
			if (x == rect.x - 1 || y == rect.y - 1 || x == rect.x + rect.width || y == rect.y + rect.height)
			{
				
				//왼쪽 아래 모서리 
				if (x == rect.x - 1 && y == rect.y + rect.height)
				{
					setTile(x, y, '1');
				}
				//왼쪽 위 모서리 
				else if (x == rect.x - 1 && y == rect.y - 1)
				{
					setTile(x, y, '2');
				}
				//오른쪽 위 모서리 
				else if (x == rect.x + rect.width && y == rect.y - 1)
				{
					setTile(x, y, '3');

				}
				//오른쪽 아래 모서리 
				else if (x == rect.x + rect.width && y == rect.y + rect.height)
				{
					setTile(x, y, '4');
				}
				//아래
				else if (y == rect.y + rect.height)
				{
					setTile(x, y, 'a');
				}
				//오른쪽
				else if (x == rect.x + rect.width)
				{
					setTile(x, y, 'b');
				}
				//위에
				else if (y == rect.y - 1)
				{
					setTile(x, y, 'c');
				}
				//왼쪽
				else if (x == rect.x - 1)
				{
					setTile(x, y, 'd');
				} 
				else
				{
					int a = 1;
				}

			}
			else
				setTile(x, y, tile);
		}
		int a = 1;

		return true;
	}

	bool placeObject(char tile)
	{
		if (_rooms.empty())
			return false;

		int r = randomInt(_rooms.size()); // choose a random room
		int x = randomInt(_rooms[r].x + 1, _rooms[r].x + _rooms[r].width - 2);
		int y = randomInt(_rooms[r].y + 1, _rooms[r].y + _rooms[r].height - 2);

		if (getTile(x, y) == Floor)
		{
			setTile(x, y, tile);

			// place one object in one room (optional)
			_rooms.erase(_rooms.begin() + r);

			return true;
		}

		return false;
	}

private:
	
	int _width, _height;
	std::vector<TileState*> _tiles;
	std::vector<Rect> _rooms; // rooms for place stairs or monsters
	std::vector<Rect> _exits; // 4 sides of rooms or corridors

};

// int main()
// {
// 	Dungeon d(79, 24);
// 	d.generate(50);
// 	d.print();
// 
// 	std::cout << "Press Enter to quit... ";
// 	std::cin.get();
// 
// 	return 0;
// }