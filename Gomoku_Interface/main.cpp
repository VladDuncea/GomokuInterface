#include <iostream>
#include "GameWindow.h"
#include "Texture.h"
#include "GridSquare.h"
#include "Viewport.h"
#include <vector>
#include <string>
#include <windows.h>
#include "TextBox.h"

#define GRID_WIDTH  20
#define GRID_HEIGHT  23
#define SQUARE_WIDTH 30
#define SQUARE_HEIGHT  SQUARE_WIDTH
#define MENU_WIDTH  SQUARE_WIDTH*15
#define TOTAL_BUTTONS GRID_HEIGHT*GRID_WIDTH
#define FONT "timesbd.ttf"

enum SquareSprite
{
	SQUARE_DEFAULT = 0,
	SQUARE_BLACK_PIECE= 1,
	SQUARE_WHITE_PIECE = 2,
	SQUARE_SPRITE_TOTAL = 3
};

enum Players
{
	PLAYER_ONE = 0,
	PLAYER_TWO = 1,
	PLAYER_TOTAL = 2
};

enum StartStrategy
{
	STRATEGY_NOT_SET = 0,
	STRATEGY_SWAP = 1,
	STRATEGY_SWAP2 = 2
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
	GameWindow gw(GRID_WIDTH*SQUARE_WIDTH + MENU_WIDTH, GRID_HEIGHT*SQUARE_HEIGHT);

	//Create the 2 viewports (game grid + menu)
	Viewport gameViewport(gw, 0, 0, GRID_WIDTH * SQUARE_WIDTH, gw.height());
	Viewport menuViewport(gw, GRID_WIDTH*SQUARE_WIDTH, 0, MENU_WIDTH, gw.height());

	//Load assets

		//Square sprites
		Texture SquareSpriteSheet(gw);
		try
		{
			SquareSpriteSheet.loadFromFile("pieces3.png");
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
			spriteClips[i].y = i * 400;
			spriteClips[i].w = 400;
			spriteClips[i].h = 400;
		}

		//Load font
		TTF_Font *fontLarge = NULL;
		TTF_Font *fontBig = NULL;
		TTF_Font *fontNormal = NULL;
		fontLarge = TTF_OpenFont(FONT, 20 +MENU_WIDTH/20);
		fontBig = TTF_OpenFont(FONT, 10 + MENU_WIDTH / 40);
		fontNormal = TTF_OpenFont(FONT, 8 + MENU_WIDTH / 40);
		if (fontBig == NULL || fontNormal == NULL)
		{
			printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
			exit(-1);
		}
		
		//Store all the renderable objects
		std::vector<Renderable *> rendVect;

		//Create title
		TextBox textTitlee(gw, "Gomoku Interface", fontLarge,{ 214, 118, 40 }); //TODO error checking
		textTitlee.setRendCentered(true);
		textTitlee.setRenderPos(menuViewport.width() / 2, menuViewport.height() / 100 + textTitlee.getHeight() / 2);
		rendVect.push_back(&textTitlee);
		/*if (!textTitle.loadFromRenderedText("GomokuInterface", fontLarge, { 214, 118, 40 }))
		{
			printf("Failed to render text texture!\n");
		}*/
		Texture textPlayerTwo(gw);
		Texture textPlayerOne(gw);
		Texture textPlayerTwoScore(gw);
		Texture textPlayerOneScore(gw);
		Texture textWinMessage(gw);
		Texture textContinueMessage(gw);
		Texture textPieceNum(gw);
		Texture textPlayerTurn(gw);
		Texture textStrategyChoice(gw);
		Texture textStartingStrategy(gw);
		Texture textStartingStrategyOptions(gw);

		textPlayerOne.loadFromRenderedText("Player one", fontNormal);
		textPlayerTwo.loadFromRenderedText("Player two", fontNormal);
		textPlayerOneScore.loadFromRenderedText("0", fontNormal);
		textPlayerTwoScore.loadFromRenderedText("0", fontNormal);
		textWinMessage.loadFromRenderedText("", fontBig);
		textContinueMessage.loadFromRenderedText("Press 'R' to start another game", fontNormal);
		textPieceNum.loadFromRenderedText("Piece : 1", fontNormal);
		textStartingStrategy.loadFromRenderedText("Please choose a starting strategy !", fontNormal);
		textStartingStrategyOptions.loadFromRenderedText("1 - SWAP || 2 - SWAP2", fontNormal);

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
	//
	//Game variables
	//

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//Set win variable
	bool win = false;

	//Set player turn
	//TODO: ask how the first player is chosen
	Players playerTurn = PLAYER_ONE;
	textPlayerTurn.loadFromRenderedText("Player's ONE turn", fontNormal);

	int turnNr = 0;
	bool swap2choice3 = false;

	int playerBlackScore = 0;
	int playerWhiteScore = 0;

	StartStrategy startStrat = STRATEGY_NOT_SET;
	bool stratChosen = false;

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
					if (win)
					{
						win = false;
						turnNr = 0;
						swap2choice3 = false;
						stratChosen = false;
						textPieceNum.loadFromRenderedText("Piece : 1", fontNormal);
						resetSquares(gridSquares);
					}
					break;
				case SDLK_1:
					if (startStrat == STRATEGY_NOT_SET)
					{
						startStrat = STRATEGY_SWAP;
						textStartingStrategy.loadFromRenderedText("Chosen starting rule : SWAP", fontNormal);
					}
					else if (turnNr == 3 || (turnNr==5&&startStrat==STRATEGY_SWAP2))
					{
						stratChosen = true;
					}
					break;
				case SDLK_2:
					if (startStrat == STRATEGY_NOT_SET)
					{
						startStrat = STRATEGY_SWAP2;
						textStartingStrategy.loadFromRenderedText("Chosen starting rule : SWAP2", fontNormal);
					}
					else if (!stratChosen && turnNr == 3|| (turnNr == 5 && swap2choice3))
					{
						playerTurn = playerTurn == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
						stratChosen = true;
					}
					break;
				case SDLK_3:
					if (!stratChosen && turnNr == 3 && startStrat ==STRATEGY_SWAP2)
					{
						swap2choice3 = true;
						stratChosen = true;
					}
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

			//If mouse event happened and the game is not over and a starting strategy is set
			if (!win&& startStrat!=STRATEGY_NOT_SET && (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN))
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
					if (squareNr <= GRID_HEIGHT*GRID_WIDTH)
					{
						//Show the color of the current player
						if (turnNr % 2 == 0)
							gridSquares[squareNr].setSprite(SQUARE_BLACK_PIECE);
						else
							gridSquares[squareNr].setSprite(SQUARE_WHITE_PIECE);
						if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							//Verify if the square is empty
							if (!gridSquares[squareNr].locked())
							{
								//Lock the square
								gridSquares[squareNr].locked(true);
								//Check if the player has won
								win = checkForWin(x, y, gridSquares, turnNr%2 ==0 ? SQUARE_BLACK_PIECE : SQUARE_WHITE_PIECE);

								if (win)
								{
									std::cout << "Player won";
									if (playerTurn == PLAYER_ONE)
									{
										playerBlackScore++;
										textPlayerOneScore.loadFromRenderedText(std::to_string(playerBlackScore), fontNormal);
										textWinMessage.loadFromRenderedText("Player One won!", fontNormal);
									}
									else
									{
										playerWhiteScore++;
										textPlayerTwoScore.loadFromRenderedText(std::to_string(playerWhiteScore), fontNormal);
										textWinMessage.loadFromRenderedText("Player Two won!", fontBig);
									}
								}
								else
								{
									//If the current player did not win advance game
									turnNr++;
									if (turnNr>2 && (!swap2choice3 || turnNr >4))
									{
										playerTurn = playerTurn == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
									}
									std::string text ("Piece : ");
									text+=std::to_string(turnNr+1);
									textPieceNum.loadFromRenderedText(text, fontNormal);
								}

							}
						}
					}
				}

				//Inside menu area
				else
				{

				}
			}

			//Render the grid
			gw.setViewport(gameViewport.viewportRect());
			for (int i = 0; i < GRID_HEIGHT*GRID_WIDTH; i++)
			{
				gridSquares[i].render(gameViewport.width() / GRID_WIDTH, gameViewport.height() / GRID_HEIGHT);
			}

			//Render the menu
			gw.setViewport(menuViewport.viewportRect());

				//Starting strategy
			textStartingStrategy.renderCentered(menuViewport.width() / 2, menuViewport.height() / 50 + textTitlee.getHeight());
			if (startStrat == STRATEGY_NOT_SET)
			{
				textStartingStrategyOptions.renderCentered(menuViewport.width() / 2, menuViewport.height() / 25 + textTitlee.getHeight() + textTitlee.getHeight());
			}
				//Piece counter
			else
			{
				textPieceNum.render(menuViewport.width() / 10, menuViewport.height() / 25 + textTitlee.getHeight() + textTitlee.getHeight());
			}

				//Usefull variables
			int middleOfViewport = menuViewport.height() / 2;
			int blackX = menuViewport.width() / 10 + textPlayerOne.getWidth() / 2;
			int whiteX = menuViewport.width() - textPlayerTwo.getWidth()/2 - menuViewport.width() / 10;
				
				//Swap choices
			if (!stratChosen && turnNr == 3)
			{
				if (startStrat == STRATEGY_SWAP)
					textStrategyChoice.loadFromRenderedText("Do you want to 1-do nothing || 2-swap ", fontNormal);
				else
					textStrategyChoice.loadFromRenderedText("Do you want to 1-do nothing || 2-swap || 3-yes ", fontNormal);
				textStrategyChoice.renderCentered(menuViewport.width() / 2, middleOfViewport - textPlayerTurn.getHeight() * 4);
			}
			else if (!stratChosen && turnNr == 5 && swap2choice3)
			{
				textStrategyChoice.loadFromRenderedText("Do you want to 1-do nothing || 2-swap ", fontNormal);
				textStrategyChoice.renderCentered(menuViewport.width() / 2, middleOfViewport - textPlayerTurn.getHeight() * 4);
			}
			else if (turnNr == 4)
				stratChosen = false;

				//Turn indicator
			if (playerTurn == PLAYER_ONE)
				textPlayerTurn.loadFromRenderedText("Player's ONE turn",fontNormal);
			else
				textPlayerTurn.loadFromRenderedText("Player's TWO turn", fontNormal);
			textPlayerTurn.renderCentered(menuViewport.width() / 2, middleOfViewport - textPlayerTurn.getHeight() * 2);
				//Player black,white text
			textPlayerOne.renderCentered(blackX, middleOfViewport);
			textPlayerTwo.renderCentered(whiteX, middleOfViewport);
				//Player black,white score
			textPlayerOneScore.renderCentered(blackX, middleOfViewport + menuViewport.height() / 10);
			textPlayerTwoScore.renderCentered(whiteX, middleOfViewport + menuViewport.height() / 10);

				//Render win message when players win
			if (win)
			{
				textWinMessage.renderCentered(menuViewport.width() / 2, menuViewport.height() / 3);
				textContinueMessage.renderCentered(menuViewport.width() / 2, menuViewport.height() / 3 + textWinMessage.getHeight());
			}

			//Render everything
			for (auto r : rendVect)
			{
				r->render();
			}

			//Render window
			gw.update();
		}
	}

	return 0;
}