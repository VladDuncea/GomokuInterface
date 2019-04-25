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

enum TextBoxes
{
	TEXTBOX_TITLE =0,
	TEXTBOX_PONE,
	TEXTBOX_PTWO,
	TEXTBOX_PONE_SCORE,
	TEXTBOX_PTWO_SCORE,
	TEXTBOX_WINMSG,
	TEXTBOX_CONTMSG,
	TEXTBOX_PIECENUM,
	TEXTBOX_PTURN,
	TEXTBOX_STRAT_CHOICE,
	TEXTBOX_STRAT_START,
	TEXTBOX_STRAT_OPT
};

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

bool initTextBoxes(GameWindow &gw, std::vector<TextBox*> & textBoxVect,Viewport &menuViewport)
{
	//Usefull variables
	int middleOfViewport = menuViewport.height() / 2;

	//Load font
	TTF_Font* fontLarge = NULL;
	TTF_Font* fontBig = NULL;
	TTF_Font* fontNormal = NULL;
	fontLarge = TTF_OpenFont(FONT, 20 + MENU_WIDTH / 20);
	fontBig = TTF_OpenFont(FONT, 10 + MENU_WIDTH / 40);
	fontNormal = TTF_OpenFont(FONT, 8 + MENU_WIDTH / 40);
	if (fontBig == NULL || fontNormal == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		exit(-1);
	}

	//Static text
		//Title
	TextBox* textTitle = new TextBox(gw, "Gomoku Interface", fontLarge, { 214, 118, 40 }); //TODO error checking
	textTitle->setRendCentered(true);
	textTitle->setRenderPos(menuViewport.width() / 2, menuViewport.height() / 100 + textTitle->getHeight() / 2);
	textBoxVect.push_back(textTitle);
		//Player text
	TextBox* textPlayerOne = new TextBox(gw,"Player one",fontNormal);
	int OneX = menuViewport.width() / 10 + textPlayerOne->getWidth() / 2;
	textPlayerOne->setRendCentered(true);
	textPlayerOne->setRenderPos(OneX, middleOfViewport);
	textBoxVect.push_back(textPlayerOne);
	TextBox* textPlayerTwo = new TextBox(gw,"Player two",fontNormal);
	int TwoX = menuViewport.width() - textPlayerTwo->getWidth() / 2 - menuViewport.width() / 10;
	textPlayerTwo->setRendCentered(true);
	textPlayerTwo->setRenderPos(TwoX, middleOfViewport);
	textBoxVect.push_back(textPlayerTwo);

	//Changing text
		//Player score
	TextBox* textPlayerOneScore = new TextBox(gw,"0",fontNormal);
	textPlayerOneScore->setRendCentered(true);
	textPlayerOneScore->setRenderPos(OneX, textPlayerOne->getRenderPos().y + textPlayerOne->getHeight()*2);
	textBoxVect.push_back(textPlayerOneScore);
	TextBox* textPlayerTwoScore = new TextBox(gw, "0",fontNormal);
	textPlayerTwoScore->setRendCentered(true);
	textPlayerTwoScore->setRenderPos(TwoX, textPlayerTwo->getRenderPos().y + textPlayerOne->getHeight() * 2);
	textBoxVect.push_back(textPlayerTwoScore);

		//Text messages
	//Win message
	TextBox* textWinMessage = new TextBox(gw,"placeholder",fontBig);
	textWinMessage->setRendCentered(true);
	textWinMessage->setRenderPos(menuViewport.width() / 2, menuViewport.height() / 3);
	textWinMessage->setRendEnabled(false);
	textBoxVect.push_back(textWinMessage);
	//Continue message
	TextBox* textContinueMessage = new TextBox(gw, "Press 'R' to start another game", fontNormal);
	textContinueMessage->setRendCentered(true);
	textContinueMessage->setRenderPos(menuViewport.width() / 2, menuViewport.height() / 3 + textWinMessage->getHeight());
	textContinueMessage->setRendEnabled(false);
	textBoxVect.push_back(textContinueMessage);
	//Piece counter
	TextBox* textPieceNum = new TextBox(gw, "Piece : 1",fontNormal);
	textPieceNum->setRenderPos(menuViewport.width() / 10, menuViewport.height() / 25 + textTitle->getHeight());
	textPieceNum->setRendEnabled(false);
	textBoxVect.push_back(textPieceNum);
	//Player turn indicator
	TextBox* textPlayerTurn = new TextBox(gw, "placeholder",fontNormal);
	textPlayerTurn->setRendCentered(true);
	textPlayerTurn->setRenderPos(menuViewport.width() / 2, middleOfViewport - textPlayerTurn->getHeight() * 2);
	textBoxVect.push_back(textPlayerTurn);
	//Strategy choices
	TextBox* textStrategyChoice = new TextBox(gw,"placeholder",fontNormal);
	textStrategyChoice->setRendCentered(true);
	textStrategyChoice->setRenderPos(menuViewport.width() / 2, middleOfViewport - textPlayerTurn->getHeight() * 4);
	textStrategyChoice->setRendEnabled(false);
	textBoxVect.push_back(textStrategyChoice);
	//Initial starting strategy text
	TextBox* textStartingStrategy = new TextBox(gw, "Please choose a starting strategy !",fontNormal);
	textStartingStrategy->setRendCentered(true);
	textStartingStrategy->setRenderPos(menuViewport.width() / 2, menuViewport.height() / 50 + textTitle->getHeight());
	textBoxVect.push_back(textStartingStrategy);
	//Strategy options
	TextBox* textStartingStrategyOptions = new TextBox(gw, "1 - SWAP || 2 - SWAP2",fontNormal);
	textStartingStrategyOptions->setRendCentered(true);
	textStartingStrategyOptions->setRenderPos(menuViewport.width() / 2, textStartingStrategy->getRenderPos().y +textStartingStrategy->getHeight()*2);
	textBoxVect.push_back(textStartingStrategyOptions);
	return true;
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

	//Store all the renderable objects
	std::vector<Renderable*> rendVect;

	//Store all the text obj
	std::vector<TextBox*> textBoxVect;
	//Initialise them
	initTextBoxes(gw, textBoxVect, menuViewport);

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

	//Event handler
	SDL_Event e;

	//Main loop flag
	bool quit = false;
	//Set win variable
	bool win = false;

	//Set player turn
	//TODO: ask how the first player is chosen
	Players playerTurn = PLAYER_ONE;
	textBoxVect[TEXTBOX_PTURN]->chageText("Player ONE turn");

	int turnNr = 0;
	bool swap2choice3 = false;

	int playerBlackScore = 0;
	int playerWhiteScore = 0;

	StartStrategy startStrat = STRATEGY_NOT_SET;
	bool hasStratToChose = false;

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
						textBoxVect[TEXTBOX_WINMSG]->setRendEnabled(false);
						textBoxVect[TEXTBOX_CONTMSG]->setRendEnabled(false);
						win = false;
						turnNr = 0;
						swap2choice3 = false;
						hasStratToChose = false;
						(textBoxVect[TEXTBOX_PIECENUM])->chageText("Piece: 1");
						resetSquares(gridSquares);
					}
					break;
				case SDLK_1:
					if (startStrat == STRATEGY_NOT_SET)
					{
						startStrat = STRATEGY_SWAP;
						(textBoxVect[TEXTBOX_STRAT_START])->chageText("Chosen starting rule : SWAP");
						//Starting strategy options are hidden now
						(textBoxVect[TEXTBOX_STRAT_OPT])->setRendEnabled(false);
						//Piece counter is now shown
						(textBoxVect[TEXTBOX_PIECENUM])->setRendEnabled(true);
					}
					else if (hasStratToChose &&(turnNr == 3 || (turnNr==5&&startStrat==STRATEGY_SWAP2)))
					{
						textBoxVect[TEXTBOX_STRAT_CHOICE]->setRendEnabled(false);
						hasStratToChose = false;
					}
					break;
				case SDLK_2:
					if (startStrat == STRATEGY_NOT_SET)
					{
						startStrat = STRATEGY_SWAP2;
						(textBoxVect[TEXTBOX_STRAT_START])->chageText("Chosen starting rule : SWAP2");
						//Starting strategy options are hidden now
						(textBoxVect[TEXTBOX_STRAT_OPT])->setRendEnabled(false);
						//Piece counter is now shown
						(textBoxVect[TEXTBOX_PIECENUM])->setRendEnabled(true);
					}
					else if (hasStratToChose &&( turnNr == 3|| (turnNr == 5 && swap2choice3)))
					{
						playerTurn = playerTurn == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
						hasStratToChose = false;
						textBoxVect[TEXTBOX_STRAT_CHOICE]->setRendEnabled(false);
					}
					break;
				case SDLK_3:
					if (hasStratToChose && (turnNr == 3 && startStrat ==STRATEGY_SWAP2))
					{
						swap2choice3 = true;
						hasStratToChose = false;
						textBoxVect[TEXTBOX_STRAT_CHOICE]->setRendEnabled(false);
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
			if ( !hasStratToChose && !win&& startStrat!=STRATEGY_NOT_SET && (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN))
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
										(textBoxVect[TEXTBOX_PONE_SCORE])->chageText(std::to_string(playerBlackScore).c_str());
										(textBoxVect[TEXTBOX_WINMSG])->chageText("Player One won!");
									}
									else
									{
										playerWhiteScore++;
										(textBoxVect[TEXTBOX_PTWO_SCORE])->chageText(std::to_string(playerWhiteScore).c_str());
										(textBoxVect[TEXTBOX_WINMSG])->chageText("Player Two won!");
									}
									textBoxVect[TEXTBOX_WINMSG]->setRendEnabled(true);
									textBoxVect[TEXTBOX_CONTMSG]->setRendEnabled(true);
								}
								else
								{
									//If the current player did not win advance game
									turnNr++;
									if (turnNr > 2 && (!swap2choice3 || turnNr > 4))
									{
										playerTurn = playerTurn == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
									}
									std::string text("Piece : ");
									text += std::to_string(turnNr + 1);
									(textBoxVect[TEXTBOX_PIECENUM])->chageText(text.c_str());

									if (turnNr == 3)
									{
										if (startStrat == STRATEGY_SWAP)
											textBoxVect[TEXTBOX_STRAT_CHOICE]->chageText("Do you want to 1-do nothing || 2-swap ");
										else
											textBoxVect[TEXTBOX_STRAT_CHOICE]->chageText("Do you want to 1-do nothing || 2-swap || 3-yes ");
										textBoxVect[TEXTBOX_STRAT_CHOICE]->setRendEnabled(true);
										hasStratToChose = true;
									}
									else if (!hasStratToChose && turnNr == 5 && swap2choice3)
									{
										hasStratToChose = true;
										textBoxVect[TEXTBOX_STRAT_CHOICE]->chageText("Do you want to 1-do nothing || 2-swap ");
										textBoxVect[TEXTBOX_STRAT_CHOICE]->setRendEnabled(true);
									}

									
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

				//Turn indicator
			if (playerTurn == PLAYER_ONE)
				textBoxVect[TEXTBOX_PTURN]->chageText("Player's One turn");
			else
				textBoxVect[TEXTBOX_PTURN]->chageText("Player's Two turn");
			
			//Render textboxes
			for (auto t : textBoxVect)
				t->render();

			//Render everything
			for (auto r : rendVect)
				r->render();

			//Render window
			gw.update();
		}
	}

	return 0;
}