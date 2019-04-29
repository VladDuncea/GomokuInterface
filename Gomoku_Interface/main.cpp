#include <iostream>
#include "GameWindow.h"
#include "Texture.h"
#include "GridSquare.h"
#include "Viewport.h"
#include <vector>
#include <string>
#include <windows.h>
#include "TextBox.h"
#include "Button.h"

constexpr auto SCALING = 0.9;
//Grid variables
constexpr auto GRID_WIDTH = 20;
constexpr auto GRID_HEIGHT = 23;
//Square variables
constexpr auto SQUARE_SPRITE_SIZE = 400;
const int SQUARE_WIDTH =(int) (30 * SCALING);
const int SQUARE_HEIGHT = SQUARE_WIDTH;
//Button variables
constexpr auto BUTTON_SPRITE_SIZE_W = 400;
constexpr auto BUTTON_SPRITE_SIZE_H = 200;
const int BUTTON_SIZE_W = (int) (120 * SCALING);
const int BUTTON_SIZE_H = (int) (60 * SCALING);
//Menu variables
const int MENU_WIDTH = (int) (500 * SCALING);
constexpr auto TOTAL_SQUARES = GRID_HEIGHT * GRID_WIDTH;
constexpr auto FONT = "timesbd.ttf";

//Global fonts
TTF_Font* fontLarge = NULL;
TTF_Font* fontBig = NULL;
TTF_Font* fontNormal = NULL;

//Global textures
Texture * buttonSpriteSheet;
Texture * SquareSpriteSheet;

enum TextBoxes
{
	TEXTBOX_TITLE =0,
	TEXTBOX_PONE,
	TEXTBOX_PTWO,
	TEXTBOX_PONE_SCORE,
	TEXTBOX_PTWO_SCORE,
	TEXTBOX_WINMSG,
	TEXTBOX_PIECENUM,
	TEXTBOX_PTURN,
	TEXTBOX_STRAT_CHOICE,
	TEXTBOX_STRAT_START
};

enum Buttons
{
	BUTTON_DRAW_1 =0,
	BUTTON_DRAW_2,
	BUTTON_STRATEGY_SWAP,
	BUTTON_STRATEGY_SWAP2,
	BUTTON_DO_NOTHING,
	BUTTON_SWAP,
	BUTTON_PLACE_TWO,
	BUTTON_CONTINUE
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

void loadFonts()
{
	//Load font
	fontLarge = TTF_OpenFont(FONT,(int) ( 50 * SCALING));
	fontBig = TTF_OpenFont(FONT,(int) (30 * SCALING));
	fontNormal = TTF_OpenFont(FONT,(int) (25 * SCALING));
	if (fontBig == NULL || fontNormal == NULL || fontLarge == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		exit(-1);	//TODO throw error
	}
}

void loadTextures(GameWindow & gw)
{
	//Square sprites
	SquareSpriteSheet = new Texture(gw);
	try
	{
		SquareSpriteSheet->loadFromFile("GomokuPieces.png");
	}
	catch (int e)
	{
		printf("Failed to load square sprite texture!\n");
		exit(-1);
	}


	//Button sprites
	buttonSpriteSheet = new Texture(gw);
	try
	{
		buttonSpriteSheet->loadFromFile("GomokuButtons.png");
	}
	catch (int e)
	{
		printf("Failed to load square sprite texture!\n");
		exit(-1);
	}


}

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
	while (	++cx <GRID_WIDTH && --cy >= 0 && gs[cy*GRID_WIDTH + cx].getSprite() == squareId)
		piecesCount++;
	cx = x; cy = y;
	while ( --cx >=0 && ++cy < GRID_HEIGHT && gs[cy*GRID_WIDTH + cx].getSprite() == squareId)
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

void hideUI(std::vector<TextBox*> & textBoxVect, std::vector<Button*>& buttonVect)
{
	textBoxVect[TEXTBOX_PTURN]->setRendEnabled(false);
	buttonVect[BUTTON_DRAW_1]->setRendEnabled(false);
	buttonVect[BUTTON_DRAW_2]->setRendEnabled(false);
	buttonVect[BUTTON_DRAW_2]->unToggle();
}

void showUI(std::vector<TextBox*> & textBoxVect, std::vector<Button*>& buttonVect)
{
	textBoxVect[TEXTBOX_PTURN]->setRendEnabled(true);
	buttonVect[BUTTON_DRAW_1]->setRendEnabled(true);
	buttonVect[BUTTON_DRAW_2]->setRendEnabled(true);
}

bool initMenuUI(std::vector<TextBox*> & textBoxVect, std::vector<Button*>& buttonVect,Viewport &menuViewport)
{
	//Usefull variables
	int middleOfViewport = menuViewport.height() / 2;

	//Static text
		//Title
	TextBox* textTitle = new TextBox(menuViewport, "Gomoku Interface", fontLarge, { 214, 118, 40 }); //TODO error checking
	textTitle->setRendCentered(true);
	textTitle->setRenderPos(menuViewport.width() / 2, menuViewport.height() / 100 + textTitle->getHeight() / 2);
	textBoxVect.push_back(textTitle);
		//Player text
	TextBox* textPlayerOne = new TextBox(menuViewport,"Player one",fontNormal);
	int OneX = menuViewport.width() / 10 + textPlayerOne->getWidth() / 2;
	textPlayerOne->setRendCentered(true);
	textPlayerOne->setRenderPos(OneX, middleOfViewport);
	textBoxVect.push_back(textPlayerOne);
	TextBox* textPlayerTwo = new TextBox(menuViewport,"Player two",fontNormal);
	int TwoX = menuViewport.width() - textPlayerTwo->getWidth() / 2 - menuViewport.width() / 10;
	textPlayerTwo->setRendCentered(true);
	textPlayerTwo->setRenderPos(TwoX, middleOfViewport);
	textBoxVect.push_back(textPlayerTwo);

	//Changing text
		//Player score
	TextBox* textPlayerOneScore = new TextBox(menuViewport,"0",fontNormal);
	textPlayerOneScore->setRendCentered(true);
	textPlayerOneScore->setRenderPos(OneX, textPlayerOne->getRenderPos().y + textPlayerOne->getHeight()*2);
	textBoxVect.push_back(textPlayerOneScore);
	TextBox* textPlayerTwoScore = new TextBox(menuViewport, "0",fontNormal);
	textPlayerTwoScore->setRendCentered(true);
	textPlayerTwoScore->setRenderPos(TwoX, textPlayerTwo->getRenderPos().y + textPlayerOne->getHeight() * 2);
	textBoxVect.push_back(textPlayerTwoScore);

		//Text messages
	//Win message
	TextBox* textWinMessage = new TextBox(menuViewport,"placeholder",fontBig);
	textWinMessage->setRendCentered(true);
	textWinMessage->setRenderPos(menuViewport.width() / 2, menuViewport.height() / 3);
	textWinMessage->setRendEnabled(false);
	textBoxVect.push_back(textWinMessage);
	//Piece counter
	TextBox* textPieceNum = new TextBox(menuViewport, "Piece : 1",fontNormal);
	textPieceNum->setRenderPos(menuViewport.width() / 10, menuViewport.height() / 25 + textTitle->getHeight());
	textPieceNum->setRendEnabled(false);
	textBoxVect.push_back(textPieceNum);
	//Player turn indicator
	TextBox* textPlayerTurn = new TextBox(menuViewport, "placeholder",fontNormal);
	textPlayerTurn->setRendCentered(true);
	textPlayerTurn->setRenderPos(menuViewport.width() / 2, middleOfViewport - textPlayerTurn->getHeight() * 2);
	textBoxVect.push_back(textPlayerTurn);
	//Strategy choices
	TextBox* textStrategyChoice = new TextBox(menuViewport,"Choose an action",fontNormal);
	textStrategyChoice->setRendCentered(true);
	textStrategyChoice->setRenderPos(menuViewport.width() / 2, textPieceNum->getRenderPos().y +textPieceNum->getHeight()*2);
	textStrategyChoice->setRendEnabled(false);
	textBoxVect.push_back(textStrategyChoice);
	//Initial starting strategy text
	TextBox* textStartingStrategy = new TextBox(menuViewport, "Please choose a starting strategy !",fontNormal);
	textStartingStrategy->setRendCentered(true);
	textStartingStrategy->setRenderPos(menuViewport.width() / 2, menuViewport.height() / 50 + textTitle->getHeight());
	textBoxVect.push_back(textStartingStrategy);

	//Buttons
	//The clips from the sprite sheet
	SDL_Rect buttonSpriteClips[BUTTON_TOTAL_STATES];
	//Select sprites in sprite sheet
	for (int i = 0; i < BUTTON_TOTAL_STATES; i++)
	{
		buttonSpriteClips[i].x = 0;
		buttonSpriteClips[i].y = i * BUTTON_SPRITE_SIZE_H;
		buttonSpriteClips[i].w = BUTTON_SPRITE_SIZE_W;
		buttonSpriteClips[i].h = BUTTON_SPRITE_SIZE_H;
	}

	//Draw buttons
	Button * buttonDraw1 = new Button(menuViewport, BUTTON_SIZE_H, BUTTON_SIZE_W, buttonSpriteSheet, buttonSpriteClips, fontNormal, "Draw");
	OneX = menuViewport.width() / 10 + BUTTON_SIZE_W / 2;
	buttonDraw1->setRendCentered(true);
	buttonDraw1->setToggle(true);
	buttonDraw1->setRenderPos(OneX, (int)(menuViewport.height() * 0.9));
	buttonVect.push_back(buttonDraw1);
	Button * buttonDraw2 = new Button(menuViewport, BUTTON_SIZE_H, BUTTON_SIZE_W, buttonSpriteSheet, buttonSpriteClips, fontNormal, "Draw");
	TwoX = (int)(menuViewport.width()*0.9 - BUTTON_SIZE_W / 2);
	buttonDraw2->setRendCentered(true);
	buttonDraw2->setToggle(true);
	buttonDraw2->setRenderPos(TwoX,(int) (menuViewport.height()* 0.9));
	buttonVect.push_back(buttonDraw2);

	//Starting strategy buttons
	Button * buttonSWAP = new Button(menuViewport, BUTTON_SIZE_H, BUTTON_SIZE_W, buttonSpriteSheet, buttonSpriteClips, fontNormal, "SWAP");
	buttonSWAP->setRendCentered(true);
	buttonSWAP->setRenderPos(OneX, textStartingStrategy->getRenderPos().y + textStartingStrategy->getHeight() * 2);
	buttonVect.push_back(buttonSWAP);
	Button * buttonSWAP2 = new Button(menuViewport, BUTTON_SIZE_H, BUTTON_SIZE_W, buttonSpriteSheet, buttonSpriteClips, fontNormal, "SWAP2");
	buttonSWAP2->setRendCentered(true);
	buttonSWAP2->setRenderPos(TwoX, textStartingStrategy->getRenderPos().y + textStartingStrategy->getHeight() * 2);
	buttonVect.push_back(buttonSWAP2);
	//Chosing strategy buttons
	Button * buttonDoNothing = new Button(menuViewport, BUTTON_SIZE_H, BUTTON_SIZE_W, buttonSpriteSheet, buttonSpriteClips, fontNormal, "nothing");
	buttonDoNothing->setRendCentered(true);
	buttonDoNothing->setRendEnabled(false);
	buttonDoNothing->setRenderPos(OneX, textStrategyChoice->getRenderPos().y + BUTTON_SIZE_H);
	buttonVect.push_back(buttonDoNothing);
	Button * buttonChoiceSWAP = new Button(menuViewport, BUTTON_SIZE_H, BUTTON_SIZE_W, buttonSpriteSheet, buttonSpriteClips, fontNormal, "swap");
	buttonChoiceSWAP->setRendCentered(true);
	buttonChoiceSWAP->setRendEnabled(false);
	buttonChoiceSWAP->setRenderPos(menuViewport.width()/2, textStrategyChoice->getRenderPos().y + BUTTON_SIZE_H);
	buttonVect.push_back(buttonChoiceSWAP);
	Button * buttonPlace2 = new Button(menuViewport, BUTTON_SIZE_H, BUTTON_SIZE_W, buttonSpriteSheet, buttonSpriteClips, fontNormal, "place 2");
	buttonPlace2->setRendCentered(true);
	buttonPlace2->setRendEnabled(false);
	buttonPlace2->setRenderPos(TwoX, textStrategyChoice->getRenderPos().y + BUTTON_SIZE_H);
	buttonVect.push_back(buttonPlace2);
	//Continue button
	Button * buttonContinue = new Button(menuViewport, BUTTON_SIZE_H, BUTTON_SIZE_W, buttonSpriteSheet, buttonSpriteClips, fontNormal, "continue");
	buttonContinue->setRendCentered(true);
	buttonContinue->setRendEnabled(false);
	buttonContinue->setRenderPos(menuViewport.width()/2, textWinMessage->getRenderPos().y +BUTTON_SIZE_H);
	buttonVect.push_back(buttonContinue);
	
	hideUI(textBoxVect, buttonVect);
	return true;
}

void draw(std::vector<TextBox*> & textBoxVect, std::vector<Button*>& buttonVect)
{
	hideUI(textBoxVect, buttonVect);
	//Display draw
	textBoxVect[TEXTBOX_WINMSG]->chageText("Draw !");
	textBoxVect[TEXTBOX_WINMSG]->setRendEnabled(true);
	//Show info to continue game
	buttonVect[BUTTON_CONTINUE]->setRendEnabled(true);
}

int main(int argc, char* args[])
{
	//Start up SDL and create game window
	GameWindow gw(GRID_WIDTH*SQUARE_WIDTH + MENU_WIDTH, GRID_HEIGHT*SQUARE_HEIGHT);

	//Load textures
	loadTextures(gw);

	//Initialise fonts
	loadFonts();

	//Create the 2 viewports (game grid + menu)
	Viewport gameViewport(gw, 0, 0, GRID_WIDTH * SQUARE_WIDTH, gw.height());
	Viewport menuViewport(gw, GRID_WIDTH*SQUARE_WIDTH, 0, MENU_WIDTH, gw.height());

	//Load assets

	//Store all the renderable objects
	std::vector<Renderable*> rendVect;

	//The clips from the sprite sheet
	SDL_Rect squareSpriteClips[SQUARE_SPRITE_TOTAL];
	//Select sprites in sprite sheet
	for (int i = 0; i < SQUARE_SPRITE_TOTAL; ++i)
	{
		squareSpriteClips[i].x = 0;
		squareSpriteClips[i].y = i * SQUARE_SPRITE_SIZE;
		squareSpriteClips[i].w = SQUARE_SPRITE_SIZE;
		squareSpriteClips[i].h = SQUARE_SPRITE_SIZE;
	}

	//All the squares in the grid
	std::vector<GridSquare> gridSquares;
	//Set the squares to coresponding positions to create the grid
	GridSquare gs(gameViewport, gameViewport.height() / GRID_HEIGHT, gameViewport.width() / GRID_WIDTH, SQUARE_SPRITE_TOTAL, *SquareSpriteSheet,squareSpriteClips);
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			gs.setRenderPos(SQUARE_WIDTH*j, SQUARE_HEIGHT*i);
			gridSquares.push_back(gs);
		}
	}

	//UI initialisation
	//Store all the text obj
	std::vector<TextBox*> textBoxVect;
	
	//All the buttons
	std::vector<Button*> buttonVect;

	//Initialise them
	initMenuUI(textBoxVect, buttonVect, menuViewport);

	//Game variables

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

			//Key-presses handling
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_r:
					if (win)
					{
						//Show ui
						showUI(textBoxVect, buttonVect);
						//Hide continue messages
						textBoxVect[TEXTBOX_WINMSG]->setRendEnabled(false);
						buttonVect[BUTTON_CONTINUE]->setRendEnabled(false);
						//Reset variables
						win = false;
						turnNr = 0;
						swap2choice3 = false;
						hasStratToChose = false;
						textBoxVect[TEXTBOX_PIECENUM]->chageText("Piece: 1");
						resetSquares(gridSquares);
						//Reset draw buttons
						buttonVect[BUTTON_DRAW_1]->unToggle();
						buttonVect[BUTTON_DRAW_2]->unToggle();
					}
					break;
				case SDLK_1:
					if (startStrat == STRATEGY_NOT_SET)
					{
						startStrat = STRATEGY_SWAP;
						(textBoxVect[TEXTBOX_STRAT_START])->chageText("Chosen starting rule : SWAP");
						//Starting strategy options are hidden now
						buttonVect[BUTTON_STRATEGY_SWAP]->setRendEnabled(false);
						buttonVect[BUTTON_STRATEGY_SWAP2]->setRendEnabled(false);
						//Piece counter is now shown
						(textBoxVect[TEXTBOX_PIECENUM])->setRendEnabled(true);
						showUI(textBoxVect,buttonVect);
					}
					else if (hasStratToChose &&(turnNr == 3 || (turnNr==5&&startStrat==STRATEGY_SWAP2)))
					{
						textBoxVect[TEXTBOX_STRAT_CHOICE]->setRendEnabled(false);
						for(int i = BUTTON_DO_NOTHING;i<=BUTTON_PLACE_TWO;i++)
							buttonVect[i]->setRendEnabled(false);
						hasStratToChose = false;
					}
					break;
				case SDLK_2:
					if (startStrat == STRATEGY_NOT_SET)
					{
						startStrat = STRATEGY_SWAP2;
						(textBoxVect[TEXTBOX_STRAT_START])->chageText("Chosen starting rule : SWAP2");
						//Starting strategy options are hidden now
						buttonVect[BUTTON_STRATEGY_SWAP]->setRendEnabled(false);
						buttonVect[BUTTON_STRATEGY_SWAP2]->setRendEnabled(false);
						//Piece counter is now shown
						(textBoxVect[TEXTBOX_PIECENUM])->setRendEnabled(true);
						showUI(textBoxVect, buttonVect);
					}
					else if (hasStratToChose &&( turnNr == 3|| (turnNr == 5 && swap2choice3)))
					{
						playerTurn = playerTurn == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
						hasStratToChose = false;
						textBoxVect[TEXTBOX_STRAT_CHOICE]->setRendEnabled(false);
						for (int i = BUTTON_DO_NOTHING; i <= BUTTON_PLACE_TWO; i++)
							buttonVect[i]->setRendEnabled(false);
					}
					break;
				case SDLK_3:
					if (hasStratToChose && (turnNr == 3 && startStrat ==STRATEGY_SWAP2))
					{
						swap2choice3 = true;
						hasStratToChose = false;
						textBoxVect[TEXTBOX_STRAT_CHOICE]->setRendEnabled(false);
						for (int i = BUTTON_DO_NOTHING; i <= BUTTON_PLACE_TWO; i++)
							buttonVect[i]->setRendEnabled(false);
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
			//If mouse event happened
			if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type==SDL_MOUSEBUTTONUP)
			{
				//Get mouse position
				int x, y;
				//Get mouse
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					//Accurate position from click
					x = e.motion.x;
					y = e.motion.y;
				}
				//Lag free input for showing sprite
				else
					SDL_GetMouseState(&x, &y);

				//Inside play area do smth only if the starting strategy is chosen and the game is not over
				if (x < gameViewport.width() && !hasStratToChose && startStrat != STRATEGY_NOT_SET && !win)
				{
					//Set all unlocked grid squares to empty
					for (int i = 0; i < GRID_HEIGHT*GRID_WIDTH; i++)
					{
						gridSquares[i].setSprite(SQUARE_DEFAULT);
					}

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
									hideUI(textBoxVect, buttonVect);
									std::cout << "Player won";
									if (playerTurn == PLAYER_ONE)
									{
										playerBlackScore++;
										textBoxVect[TEXTBOX_PONE_SCORE]->chageText(std::to_string(playerBlackScore).c_str());
										textBoxVect[TEXTBOX_WINMSG]->chageText("Player One won!");
									}
									else
									{
										playerWhiteScore++;
										(textBoxVect[TEXTBOX_PTWO_SCORE])->chageText(std::to_string(playerWhiteScore).c_str());
										(textBoxVect[TEXTBOX_WINMSG])->chageText("Player Two won!");
									}
									textBoxVect[TEXTBOX_WINMSG]->setRendEnabled(true);
									buttonVect[BUTTON_CONTINUE]->setRendEnabled(true);
								}
								else
								{
									//If the current player did not win advance game
									turnNr++;

									//Check if board is full
									if (turnNr >= TOTAL_SQUARES)
									{
										draw(textBoxVect,buttonVect);
										win = true;
									}

									//Display piece nr
									std::string text("Piece : ");
									text += std::to_string(turnNr + 1);
									(textBoxVect[TEXTBOX_PIECENUM])->chageText(text.c_str());

									//Advance turn if conditions are met
									if (turnNr > 2 && (!swap2choice3 || turnNr > 4))
									{
										playerTurn = playerTurn == PLAYER_ONE ? PLAYER_TWO : PLAYER_ONE;
									}
									
									//Start of turn 4 choices
									if (turnNr == 3)
									{
										buttonVect[BUTTON_DO_NOTHING]->setRendEnabled(true);
										buttonVect[BUTTON_SWAP]->setRendEnabled(true);
										textBoxVect[TEXTBOX_STRAT_CHOICE]->setRendEnabled(true);
										if (startStrat == STRATEGY_SWAP2)
										{
											buttonVect[BUTTON_PLACE_TWO]->setRendEnabled(true);
										}
										hasStratToChose = true;
									}
									//Start of turn 6 choices
									else if (turnNr == 5 && swap2choice3)
									{
										hasStratToChose = true;
										buttonVect[BUTTON_DO_NOTHING]->setRendEnabled(true);
										buttonVect[BUTTON_SWAP]->setRendEnabled(true);
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
					for (auto b : buttonVect)
					{
						b->handleMouse(e);
					}

					//Check for draw
					if (buttonVect[BUTTON_DRAW_1]->isClicked() && buttonVect[BUTTON_DRAW_2]->isClicked())
					{
						draw(textBoxVect,buttonVect);
						win = true;
					}

					//SDL event used to simulate a key press
					SDL_Event sdlevent = {};
					sdlevent.type = SDL_KEYDOWN;
					sdlevent.key.keysym.sym = NULL;

					//Starting strategy
					if (buttonVect[BUTTON_STRATEGY_SWAP]->isClicked())
						sdlevent.key.keysym.sym = SDLK_1;
					if (buttonVect[BUTTON_STRATEGY_SWAP2]->isClicked())
						sdlevent.key.keysym.sym = SDLK_2;

					//Strategy choices
					if (buttonVect[BUTTON_DO_NOTHING]->isClicked())
						sdlevent.key.keysym.sym = SDLK_1;
					if (buttonVect[BUTTON_SWAP]->isClicked())
						sdlevent.key.keysym.sym = SDLK_2;
					if (buttonVect[BUTTON_PLACE_TWO]->isClicked())
						sdlevent.key.keysym.sym = SDLK_3;
					
					//Continue button
					if (buttonVect[BUTTON_CONTINUE]->isClicked())
						sdlevent.key.keysym.sym = SDLK_r;
					//Send keypress to event queue
					if(sdlevent.key.keysym.sym != NULL)
						SDL_PushEvent(&sdlevent);
				}
			}

			//Render the grid
			gw.setViewport(gameViewport.viewportRect());
			for (auto gs:gridSquares)
			{
				gs.render();
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

			//Render everything else
			for (auto r : rendVect)
				r->render();

			for (auto b : buttonVect)
				b->render();

			//Render window
			
		}
		gw.update();
	}

	return 0;
}