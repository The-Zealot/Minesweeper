#include "GameEngine.h"
#include <ctime>
#include <iostream>

GameEngine::GameEngine()
{
	field = NULL;
	height = 0;
	width = 0;
	isAlive = true;
}

GameEngine::GameEngine(size_t x, size_t y)
{
	field = NULL;
	height = x + 2;
	width = y + 2;
	isAlive = true;

	createField();
}

GameEngine::~GameEngine()
{
	deleteField();
}

void GameEngine::createField()
{
	if (field != NULL)
		return;

	field = new Field * [height];

	for (int i = 0; i < height; i++)
		field[i] = new Field[width];

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			field[i][j].count = 0;
			field[i][j].state = CLOSED;
			if (i == 0 or i == height - 1 or j == 0 or j == width - 1)
			{
				field[i][j].count = 10;
				field[i][j].state = FLAG;
			}
		}

	std::cout << "[GameEngine]: Поле успешно создано!" << std::endl;
}

void GameEngine::deleteField()
{
	if (field != NULL)
	{
		for (int i = 0; i < height; i++)
			delete field[i];
		delete field;
	}

	height = 0;
	width = 0;
	field = NULL;
}

bool GameEngine::setSize(size_t x, size_t y)
{
	if (!field)
	{
		height = x + 2;
		width = y + 2;
		return true;
	}

	return false;
}

GameEngine::Field** GameEngine::getField()
{
	Field** temp = new Field* [height];
	for (int i = 0; i < height; i++)
		temp[i] = new Field[width];

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			temp[i][j] = field[i][j];

	return temp;
}

int GameEngine::generateField(size_t bombCount, size_t exceptionX, size_t exceptionY)
{
	if (!field)
		return -1;

	if (bombCount > height * width - 1 or
		exceptionX < 0 or exceptionX >= height or
		exceptionY < 0 or exceptionY >= width)
		return -1;



	srand(time(0));

	for (int iter = 0; iter < bombCount; iter++)
	{
		int i = 1 + rand() % (height - 2);
		int j = 1 + rand() % (width - 2);
		if (field[i][j].count != 9 and i != exceptionX and j != exceptionY)
			field[i][j].count = 9;
		else
			bombCount++;
	}

	incAround();
	return bombCount;
}

void GameEngine::incAround()
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (field[i][j].count == 9)
				for (int x = -1; x < 2; x++)
					for (int y = -1; y < 2; y++)
						if (field[i + x][j + y].count != 9)
							field[i + x][j + y].count++;
}

size_t GameEngine::getHeight()
{
	return height;
}

size_t GameEngine::getWidth()
{
	return width;
}

GameEngine::GameState GameEngine::openCell(size_t x, size_t y)
{
	if (!isAlive or !field)
		return DEFAULT;

	if (x > height - 2 or y > width - 2 or x < 0 or y < 0)
		return DEFAULT;

	if (field[x][y].state == OPENED or field[x][y].state == FLAG)
		return DEFAULT;

	if (field[x][y].count == 9)
	{
		field[x][y].count = 10;
		isAlive = false;

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				field[i][j].state = OPENED;
	}

	if (field[x][y].count == 0)
	{
		field[x][y].isProcess = true;
		for (int i = -1; i < 2; i++)
			for (int j = -1; j < 2; j++)
			{
				if (field[x + i][y + j].isProcess or field[x + i][y + j].state == OPENED)
					continue;

				openCell(x + i, y + j);
			}
	}

	field[x][y].isProcess = false;
	field[x][y].state = OPENED;
	return gameStateCheck();
}

void GameEngine::flagCell(size_t x, size_t y)
{
	if (!field or !isAlive)
		return;

	if (x > height - 2 or y > width - 2 or x < 0 or y < 0)
		return;

	if (field[x][y].state == FLAG)
	{
		field[x][y].state = CLOSED;
		return;
	}

	if (field[x][y].state == CLOSED)
		field[x][y].state = FLAG;
}

GameEngine::GameState GameEngine::gameStateCheck()
{
	if (!field)
		return DEFAULT;

	if (!isAlive)
		return LOSE;

	for (int i = 1; i < height - 1; i++)
		for (int j = 1; j < width - 1; j++)
			if (field[i][j].state == CLOSED)
				return NONE;

	return WIN;
}
