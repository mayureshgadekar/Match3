#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <String.h>
#include <Stdlib.h>
#include <SFML/Window.hpp>
using namespace sf;
using namespace std;

int tsize = 54; //tile size
Vector2i offset(48, 24);

struct game
{
	int x, y, c, r, way, same, alpha;			// alpha for color 
	game() { same = 0; alpha = 255; }
} g[10][10];

void swap(game p1, game p2)
{
	std::swap(p1.c, p2.c);
	std::swap(p1.r, p2.r);

	g[p1.r][p1.c] = p1;
	g[p2.r][p2.c] = p2;
}


int main()
{
	srand(time(0));
	RenderWindow Window;

	RenderWindow app(VideoMode(740, 480), "Game");
	app.setFramerateLimit(60);

	Texture pic1, pic2;
	pic1.loadFromFile("board.png");
	pic2.loadFromFile("shapes.png");

	Sprite board(pic1), shapes(pic2);

	for (int i = 1;i <= 8;i++)
		for (int j = 1;j <= 8;j++)
		{
			g[i][j].way = rand() % 3;
			g[i][j].c = j;
			g[i][j].r = i;
			g[i][j].x = j*tsize;
			g[i][j].y = i*tsize;
		}

	int x0, y0, x, y; int touch = 0; Vector2i pos;
	bool isswap = false, isMoving = false;

	while (app.isOpen())
	{	
		Event e;
		while (app.pollEvent(e))	
		{
			if (e.type == Event::Closed)
				app.close();

			if (e.type == Event::MouseButtonPressed)
				if (e.key.code == Mouse::Left)
				{
					if (!isswap && !isMoving) touch++;
					pos = Mouse::getPosition(app) - offset;
				}
		}

	
		if (touch == 1)
		{									// touch the tile to move
			x0 = pos.x / tsize + 1;
			y0 = pos.y / tsize + 1;
		}
		if (touch == 2)
		{
			x = pos.x / tsize + 1;
			y = pos.y / tsize + 1;
			if (abs(x - x0) + abs(y - y0) == 1)
			{
				swap(g[y0][x0], g[y][x]); isswap = 1; touch = 0;
			}
			else touch = 1;
		}

		//find same 
		for (int i = 1;i <= 8;i++)
			for (int j = 1;j <= 8;j++)
			{
				if (g[i][j].way == g[i + 1][j].way)	//horizontal
					if (g[i][j].way == g[i - 1][j].way)
						for (int n = -1;n <= 1;n++) g[i + n][j].same++;	// for loop for destroying 3 tiles  

				if (g[i][j].way == g[i][j + 1].way)		// vertical 
					if (g[i][j].way == g[i][j - 1].way)
						for (int n = -1;n <= 1;n++) g[i][j + n].same++;
			}

	
		isMoving = false;
		for (int i = 1;i <= 8;i++)
			for (int j = 1;j <= 8;j++)
			{
				game &p = g[i][j];		// New tiles moving 
				int dx, dy;
				for (int n = 0;n<7;n++)   // 7- speed
				{
					dx = p.x - p.c*tsize;
					dy = p.y - p.r*tsize;
					if (dx) p.x -= dx / abs(dx);
					if (dy) p.y -= dy / abs(dy);
				}
				if (dx || dy) isMoving = 1;
			}

		
		int score = 0; 
		int score1 = 0;
		for (int i = 1;i <= 8;i++)
		{
			for (int j = 1;j <= 8;j++)		// score
			{
				score += g[i][j].same;
				score1 += 10;
				string Text1 = to_string(score1);
//				RenderWindow Window;
			// Some issues trying to display it on the center 
				Text text;
				text.setString(Text1);
				text.setCharacterSize(56);
				text.setFillColor(Color(44,127,255));
				text.setOrigin((text.getGlobalBounds().width / 2), (text.getGlobalBounds().height / 2));
				//text.setPosition(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
				Window.draw(text);
			}
		}
		if (isswap && !isMoving)				// if not same then again on same location 
		{
			if (!score) swap(g[y0][x0], g[y][x]); isswap = 0;
		}

	
		if (!isMoving)
		{
			for (int i = 8;i>0;i--) // After updation if there is any same again
				for (int j = 1;j <= 8;j++)
					if (g[i][j].same)
						for (int n = i;n>0;n--)
							if (!g[n][j].same) { swap(g[n][j], g[i][j]); break; };

			for (int j = 1;j <= 8;j++)				// Respawn 
				for (int i = 8, n = 0;i>0;i--)
					if (g[i][j].same)
					{
						g[i][j].way = rand() % 7;
						g[i][j].y = -tsize*n++;
						g[i][j].same = 0;
						g[i][j].alpha = 255;
					}
		}


		
		app.draw(board);

		for (int i = 1;i <= 8;i++)
			for (int j = 1;j <= 8;j++)				// paint
			{											// set all attributes of shapes and and then paint
				game p = g[i][j];
				shapes.setTextureRect(IntRect(p.way * 49, 0, 49, 49));
				shapes.setColor(Color(255, 255, 255, p.alpha));
				shapes.setPosition(p.x, p.y);
				shapes.move(offset.x - tsize, offset.y - tsize);
				app.draw(shapes);
			}

		app.display();
	}
	return 0;
}
