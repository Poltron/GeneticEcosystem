#include "stdafx.h"
#include "SDL2_gfxPrimitives.h"
#include "FoodHandler.h"
#include "Engine.h"

FoodHandler::FoodHandler()
{
}


FoodHandler::~FoodHandler()
{
}

std::vector<float>* FoodHandler::getFoods()
{
	return &m_foods;
}

Eigen::Vector2f FoodHandler::getCenterPositionFromIndex(int index)
{
	Eigen::Vector2f res = getPositionFromIndex(index);
	return res + Eigen::Vector2f(res.x() + GRID_TILE_SIZE / 2, res.y() + GRID_TILE_SIZE / 2);
}

Eigen::Vector2f FoodHandler::getPositionFromIndex(int index)
{
	int x1 = ((index % m_numberOfTilesX)) * GRID_TILE_SIZE;
	int y1 = (index / m_numberOfTilesX) * GRID_TILE_SIZE;
	return Eigen::Vector2f(x1, y1);
}

void FoodHandler::initialize()
{
	m_numberOfTilesX = WIDTH / GRID_TILE_SIZE;
	m_numberOfTilesY = HEIGHT / GRID_TILE_SIZE;
	m_numberOfTiles = m_numberOfTilesX * m_numberOfTilesY;
	
	m_tilesWithFood = 0;

	for (int i = 0; i < m_numberOfTiles; ++i)
	{
		m_foods.push_back(0);

		if (rand() % 100 < 7 && m_tilesWithFood < NB_OF_FOOD)
		{
			m_foods[i] = 1.0f;
			m_tilesWithFood++;
		}
	}
}

void FoodHandler::update()
{
	while (m_tilesWithFood < NB_OF_FOOD)
	{
		bool stop = false;
		
		while (!stop)
		{
			int random = rand() % m_numberOfTiles;
			
			if (m_foods[random] <= 0.f)
			{
				stop = true;
				m_foods[random] = 1.0f;
				m_tilesWithFood++;
			}
		}
	}
}

void FoodHandler::draw()
{
	for (int i = 0; i < m_numberOfTiles; ++i)
	{
		if (m_foods[i] > 0.f)
		{

			int x1 = ((i % m_numberOfTilesX)) * GRID_TILE_SIZE;
			int y1 = (i / m_numberOfTilesX) * GRID_TILE_SIZE;
			int x2 = x1 + GRID_TILE_SIZE;
			int y2 = y1 + GRID_TILE_SIZE;

			boxRGBA(Engine::Instance().getSdlRenderer(), x1, y1, x2, y2, 0x00, 0xFF, 0x00, (Uint8)(m_foods[i] * 255));
		}
	}
}

void FoodHandler::close()
{
	m_foods.clear();
}

float FoodHandler::getFoodAt(int x, int y)
{
	return m_foods[getFoodIndexFromPosition(x, y)];
}

float FoodHandler::consumeFoodAt(int x, int y, float quantity)
{
	float foodQuantityOnTile = getFoodAt(x, y);
	int foodIndex = getFoodIndexFromPosition(x, y);

	if (foodQuantityOnTile == 0)
	{
		return 0;
	}
	else if (foodQuantityOnTile > 0 && foodQuantityOnTile <= quantity)
	{
		m_tilesWithFood--;
		m_foods[foodIndex] = 0.0f;
		return foodQuantityOnTile;
	}
	else
	{
		m_foods[foodIndex] -= quantity;
		return quantity;
	}
}

int FoodHandler::getFoodIndexFromPosition(int x, int y)
{
	return (y / GRID_TILE_SIZE) * m_numberOfTilesX + (x / GRID_TILE_SIZE);
}