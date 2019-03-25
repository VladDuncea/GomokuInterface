#include <iostream>
#include "GameWindow.h"
#include "Texture.h"
#include "GridSquare.h"
#include "Viewport.h"
#include <vector>

#define GRID_WIDTH  19
#define GRID_HEIGHT  19
#define MENU_WIDTH  300
#define TOTAL_BUTTONS GRID_HEIGHT*GRID_WIDTH

enum SquareSprite
{
	SQUARE_DEFAULT = 0,
	SQUARE_BLUE_PIECE= 1,
	SQUARE_RED_PIECE = 2,
	SQUARE_SPRITE_TOTAL = 3
};

enum Players
{
	PLAYER_BLUE = 0,
	PLAYER_RED = 1,
	PLAYER_TOTAL = 2
};


bool checkForWin(int x, int y, std::vector<GridSquare> &gs,int squareId)
{
	//Check if there are 5 or more pieces togheter
	int piecesCount = 1;
	int cx = x, cy = y;

	//Check up-down
	while (--cy >= 0 && gs[cy*GRID_WIDTH + cx].getSprite() == squareId)
		piecesCount++;
	cy = y;
	while (++cy <GRID_HEIGHT && gs[cy*GRID_WIDTH + cx].getSprite() == squareId)
		piecesCount++;
	cy = y;
	if (piecesCount >= 5)
		return true;

	//Check left-right
	piecesCount = 1;
	while (--cx >= 0 && gs[cy*GRID_WIDTH + cx].getSprite() == squareId)
		piecesCount++;
	cx = x;
	while (++cx < GRID_WIDTH && gs[cy*GRID_WIDTH + cx].getSprite() == squareId)
		piecesCount++;
	cx = x;
	if (piecesCount >= 5)
		return true;

	//Check left to right diagonally
	piecesCount = 1;
	while (--cx >= 0 && --cy >=0 && gs[cy*GRID_WIDTH + cx].getSprite() == squareId)
		piecesCount++;
	cx = x; cy = y;
	while (++cx < GRID_WIDTH && ++cy < GRID_HEIGHT && gs[cy*GRID_WIDTH + cx].getSprite() == squareId)
		piecesCount++;
	cx = x; cy = y; 
	if (piecesCount >= 5)
		return true;

	//Check right to left diagonally
	piecesCount = 1;
	while (	++cx >= 0 && --cy >= 0 && gs[cy*GRID_WIDTH + cx].getSprite() == squareId)
		piecesCount++;
	cx = x; cy = y;
	while ( --cx < GRID_WIDTH && ++cy < GRID_HEIGHT && gs[cy*GRID_WIDTH + cx].getSprite() == squareId)
		piecesCount++;
	cx = x; cy = y;
	if (piecesCount >= 5)
		return true;

	return false;
}

void resetSquares(std::vector<GridSquare> &gs)
{
	for (auto &square : gs)
	{
		square.locked(false);
		square.setSprite(0);
	}

}


int main(int argc, char* args[])
{
	
	//Start up SDL and create game window
	GameWindow gw(GRID_WIDTH*30 + MENU_WIDTH, GRID_HEIGHT*30);

	//Create the 2 viewports (game grid + menu)
	Viewport gameViewport(gw, 0, 0, GRID_WIDTH * 30, gw.height());
	Viewport menuViewport(gw, GRID_WIDTH*30, 0, MENU_WIDTH , gw.height());
	

	//Load assets

	//Square sprites
	Texture SquareSpriteSheet(gw);
	try
	{
		SquareSpriteSheet.loadFromFile("piece.png");
	}
	catch (int e)
	{
		printf("Failed to load square sprite texture!\n");
		exit(-1);
	}

	//The clips from the sprite sheet
	SDL_Rect spriteClips[SQUARE_SPRITE_TOTAL];
	
	//Select sprites in sprite sheet
	for (int i = 0; i < SQUARE_SPRITE_TOTAL; ++i)
	{
		spriteClips[i].x = 0;
		spriteClips[i].y = i * 50;
		spriteClips[i].w = 50;
		spriteClips[i].h = 50;
	}

	//All the squares in the grid
	std::vector<GridSquare> gridSquares;
	//Set the squares to coresponding positions to create the grid
	GridSquare gs(gameViewport, gameViewport.height() / GRID_HEIGHT, gameViewport.width() / GRID_WIDTH, SQUARE_SPRITE_TOTAL, SquareSpriteSheet,spriteClips);
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			gs.setPosition(gs.width()*j, gs.height()*i);
			gridSquares.push_back(gs);
		}
	}

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//Set win variable
	bool win = false;

	//Set player turn to a random number between 0 and 1
	Players playerTurn = PLAYER_BLUE;

	//While application is running
	while (!quit)
	{
		
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			//Check for key-presses
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_r:
					win = false;
					resetSquares(gridSquares);
					break;

				case SDLK_ESCAPE:
					quit = true; 
					break;

				default:
					break;
				}
			}
			//Clear window
			gw.clear();

			//If mouse event happened and the game is not over
			if (!win & (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN))
			{
				
				//Get mouse position
				int x, y;
				//Get mouse
				SDL_GetMouseState(&x, &y);
				
				printf("x: %d y:%d\n", x, y);

				//Set all unlocked grid squares to empty
				for (int i = 0; i < GRID_HEIGHT*GRID_WIDTH; i++)
				{
					gridSquares[i].setSprite(SQUARE_DEFAULT);
				}

				//Inside play area
				if (x < gameViewport.width())
				{
					//Get square coordinates
					x /= gridSquares[0].width();
					y /= gridSquares[0].height();
					int squareNr = y * GRID_WIDTH + x;

					

					//Account for possible out of bounds
					if (squareNr <= 360)
					{
						//Show the color of the current player
						if (playerTurn == PLAYER_BLUE)
							gridSquares[squareNr].setSprite(SQUARE_BLUE_PIECE);
						else
							gridSquares[squareNr].setSprite(SQUARE_RED_PIECE);
						if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							//Verify if the square is empty
							if (!gridSquares[squareNr].locked())
							{
								//Lock the square
								gridSquares[squareNr].locked(true);
								//Check if the player has won
								win = checkForWin(x, y, gridSquares, playerTurn + 1);

								if (win)
									std::cout << "Player won";
								//If the current player did not win change players
								if (!win)
									playerTurn = playerTurn == PLAYER_RED ? PLAYER_BLUE : PLAYER_RED;
							}
						}
					}
				}
			}

			//Render the grid
			for (int i = 0; i < GRID_HEIGHT*GRID_WIDTH; i++)
			{
				gridSquares[i].render(gameViewport.width() / GRID_WIDTH, gameViewport.height() / GRID_HEIGHT);
			}

			//Render window
			gw.update();

		}
	}

	return 0;
}