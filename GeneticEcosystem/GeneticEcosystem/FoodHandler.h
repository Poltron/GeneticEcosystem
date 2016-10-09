#pragma once
#include "Settings.h"
#include <vector>
#include <Eigen/Dense>

class FoodHandler
{
public:
	FoodHandler();
	~FoodHandler();

	void initialize();
	void update();
	void draw();
	void close();

	std::vector<float>* getFoods();
	float getFoodAt(int x, int y);
	Eigen::Vector2f getCenterPositionFromIndex(int index);
	Eigen::Vector2f getPositionFromIndex(int index);
	int getFoodIndexFromPosition(int x, int y);

	float consumeFoodAt(int x, int y, float quantity);
private:

	int m_numberOfTiles;
	int m_numberOfTilesX;
	int m_numberOfTilesY;

	int m_tilesWithFood;
	std::vector<float> m_foods;

};

