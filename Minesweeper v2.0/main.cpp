#include <iostream>
#include <SFML/Graphics.hpp>

#include "GameEngine.h"

using namespace sf;
using namespace std;

int main()
{
	try
	{
		setlocale(LC_ALL, "Rus");

		Texture text;
		text.loadFromFile("sprites.png");
		Sprite s;
		s.setTexture(text);

		GameEngine game;
		int height, width;

		int SPRITE_SCALE = 32;
		int SPRITE_SIZE = text.getSize().y;
		float SCALE_CONST = (float)SPRITE_SCALE / text.getSize().y;
		
		

		int FIELD_H = 15, FIELD_W = 25;
		int BOMB_COUNT = 55;

		cout << "[Console]: SCALE_CONST задан: " << SCALE_CONST << endl;
		cout << "[Console]: SPRITE_SIZE задан: " << SPRITE_SIZE << endl;
		cout << "[Console]: SPRITE_SCALE задан: " << SPRITE_SCALE << endl;

		bool isFirstClick = true;

		height = FIELD_W;
		width = FIELD_H;

		s.setScale(SCALE_CONST, SCALE_CONST);
		
		RenderWindow window(VideoMode(height * SPRITE_SCALE, width * SPRITE_SCALE), "Minesweeper v2.0");

		cout << "[Console]: Отрисовка окна" << endl;
		cout << "[Console]: Размер поля: " << height << " x " << width << endl;

		game.setSize(height, width) ? cout << "[Console]: Размер поля установлен" << endl : cout << "[Console]: Поле уже существует" << endl;
		game.createField();

		auto show = [&game]()
		{
			GameEngine::Field** arr = game.getField();

			cout << "[Main]: Вызов лямбда-функции с псевдонимом show" << endl;
			for (int i = 1; i < game.getWidth() - 1; i++)
			{
				cout << "[Console]: ";
				for (int j = 1; j < game.getHeight() - 1; j++)
				{
					if (arr[j][i].state == GameEngine::CLOSED)
						cout << 'X' << ' ';
					if (arr[j][i].state == GameEngine::OPENED)
						cout << arr[j][i].count << ' ';
				}
				cout << endl;
			}

			if (arr)
			{
				for (int i = 0; i < game.getHeight(); i++)
					delete arr[i];
				delete arr;
			}
		};

		auto generate = [&game, &isFirstClick](size_t bombCount, size_t x, size_t y)
		{
			if (!isFirstClick)
				return;

			cout << "[Main]: Вызов лямбда-функции с псевдонимом generate" << endl;
			cout << "[Main]: Клик в точке (" << x << ";" << y << ")" << endl;
			cout << "[GameEngine]: Количество вызовов метода generateField(): " << game.generateField(bombCount, x, y) << endl;		
			isFirstClick = false;
		};

		show();

		bool mouseLeftPressed = false;
		bool mouseRightPressed = false;

		while (window.isOpen())
		{
			Event e;
			while (window.pollEvent(e))
				if (e.type == Event::Closed)
					window.close();

			Vector2i mousePos = Mouse::getPosition(window);
			Vector2f pos = window.mapPixelToCoords(mousePos);

			int posX = pos.x / SPRITE_SCALE + 1;
			int posY = pos.y / SPRITE_SCALE + 1;

			if (Mouse::isButtonPressed(Mouse::Left) and !mouseLeftPressed)
			{
				mouseLeftPressed = true;

				generate(BOMB_COUNT, posX, posY);
				switch (game.openCell(posX, posY))
				{
				case GameEngine::NONE: cout << "[Console]: Выбрана ячейка (" << posX << ";" << posY << ")" << endl;
					break;
				case GameEngine::WIN: cout << "[Console]: Победа!" << endl;
					break;
				case GameEngine::LOSE: cout << "[Console]: Поражение! Вы подарвались на мине, и Ваши кишки разлетелись по всей округе." << endl;
					break;
				}
			}
			if (e.type == Event::MouseButtonReleased and e.mouseButton.button == Mouse::Left)
				mouseLeftPressed = false;

			if (Mouse::isButtonPressed(Mouse::Right) and !mouseRightPressed)
			{
				mouseRightPressed = true;

				generate(BOMB_COUNT, posX, posY);
				if (game.gameStateCheck() == GameEngine::NONE)
				{
					game.flagCell(posX, posY);
					cout << "[Console]: Ячейка (" << posX << ";" << posY << ")" << " отмечена" << endl;
				}
				if (game.gameStateCheck() == GameEngine::WIN)
					cout << "[Console]: Победа!" << endl;
			}
			if (e.type == Event::MouseButtonReleased and e.mouseButton.button == Mouse::Right)
				mouseRightPressed = false;

			window.clear();

			////// draw //////

			GameEngine::Field** temp = game.getField();

			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					s.setPosition(i * SPRITE_SCALE, j * SPRITE_SCALE);
					if (temp[i + 1][j + 1].state == GameEngine::CLOSED)
						s.setTextureRect(IntRect(0, 0, SPRITE_SIZE, SPRITE_SIZE));
					if (temp[i + 1][j + 1].state == GameEngine::OPENED)
						s.setTextureRect(IntRect(SPRITE_SIZE * (temp[i + 1][j + 1].count + 2), 0, SPRITE_SIZE, SPRITE_SIZE));
					if (temp[i + 1][j + 1].state == GameEngine::FLAG)
						s.setTextureRect(IntRect(SPRITE_SIZE, 0, SPRITE_SIZE, SPRITE_SIZE));
					window.draw(s);
				}

			if (temp)
			{
				for (int i = 0; i < game.getHeight(); i++)
					delete temp[i];
				delete temp;
			}

			window.display();
		}
	}
	catch (const exception& ex)
	{
		cout << "[Console]: Ошибка при отрисовке окна" << endl;
	}

	cout << "[Console]: Цикл отрисовки окна завершен!" << endl;
	return 1;
}