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
		bool isProcess = false;						// Флаг обработки клетки
	};

	GameEngine();
	GameEngine(size_t x, size_t y);
	~GameEngine();

	// Задает размеры для массива возращает true, если он не существует.
	bool setSize(size_t x, size_t y);

	// Создает копию массива и возвращает указатель на него
	GameEngine::Field** getField();

	// Создает массив внутри класса
	void createField();

	// Удаляет массив
	void deleteField();

	// Размещает на поле мины в количестве bombCount, исключая ячейку exceptionX;exceptionY.
	// Возвращает количество итераций размещения бомб, либо -1, если массив не существует.
	int generateField(size_t bombCount = 10, size_t exceptionX = 0, size_t exceptionY = 0);

	// Возвращает высоту поля
	size_t getHeight();

	// Возвращает ширину поля
	size_t getWidth();

	// Рекурсивная функция открытия клетки.
	// Ставит состояние у всех соседних ячеек с числом 0 в OPENED
	// Возвращает результыт выполнения метода winCheck()
	GameEngine::GameState openCell(size_t x, size_t y);

	// Переключает состояние выбранной ячейки в состояниях FLAG и OPENED
	void flagCell(size_t x, size_t y);

	// Возвращает состояние игры GameState
	GameEngine::GameState gameStateCheck();


private:
	Field** field;
	unsigned int height;
	unsigned int width;
	bool isAlive;

	void incAround();
};