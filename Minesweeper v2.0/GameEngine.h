#pragma once

#include <cstdlib>

class GameEngine
{
public:
	enum CellState
	{
		CLOSED, OPENED, FLAG, QUESTION
	};

	enum GameState
	{
		NONE, LOSE, WIN, DEFAULT
	};

	struct Field
	{
		int count = 0;
		CellState state = CLOSED;
		bool isProcess = false;						// ���� ��������� ������
	};

	GameEngine();
	GameEngine(size_t x, size_t y);
	~GameEngine();

	// ������ ������� ��� ������� ��������� true, ���� �� �� ����������.
	bool setSize(size_t x, size_t y);

	// ������� ����� ������� � ���������� ��������� �� ����
	GameEngine::Field** getField();

	// ������� ������ ������ ������
	void createField();

	// ������� ������
	void deleteField();

	// ��������� �� ���� ���� � ���������� bombCount, �������� ������ exceptionX;exceptionY.
	// ���������� ���������� �������� ���������� ����, ���� -1, ���� ������ �� ����������.
	int generateField(size_t bombCount = 10, size_t exceptionX = 0, size_t exceptionY = 0);

	// ���������� ������ ����
	size_t getHeight();

	// ���������� ������ ����
	size_t getWidth();

	// ����������� ������� �������� ������.
	// ������ ��������� � ���� �������� ����� � ������ 0 � OPENED
	// ���������� ��������� ���������� ������ winCheck()
	GameEngine::GameState openCell(size_t x, size_t y);

	// ����������� ��������� ��������� ������ � ���������� FLAG � OPENED
	void flagCell(size_t x, size_t y);

	// ���������� ��������� ���� GameState
	GameEngine::GameState gameStateCheck();


private:
	Field** field;
	unsigned int height;
	unsigned int width;
	bool isAlive;

	void incAround();
};