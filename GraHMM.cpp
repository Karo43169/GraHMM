#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <string.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"


//Variables definitions for the grid cells
#define GRID_CELL_SIZE 50
#define Grid_cells_x 15
#define Grid_cells_y 11

//Variables for setting screen resolution depending on cell amount
int WINDOW_SIZE_WIDTH = 15 * GRID_CELL_SIZE;
int WINDOW_SIZE_HEIGHT = 11 * GRID_CELL_SIZE;


struct Vec2i
{
	int x;
	int y;
};

//Setting up a structure for a grid and it's properties
struct Board_grid
{
	//Setting textures for elements of the grid
	SDL_Texture* Texture_obstacle;
	SDL_Texture* Texture_cell_default;
	//two dimensional array for creating and storing grid informations
	unsigned char Grid_cells[Grid_cells_y][Grid_cells_x];
	unsigned char Grassfire_Grid_cells[Grid_cells_y][Grid_cells_x];
};

//function for generating board (not random)
void Board_create(Board_grid* Board)
{
	memset(Board->Grid_cells, 0, sizeof(Board->Grid_cells));

	Board->Grid_cells[2][5] = 255;
	Board->Grid_cells[5][9] = 255;
	Board->Grid_cells[5][10] = 255;
	Board->Grid_cells[5][11] = 255;
	Board->Grid_cells[6][9] = 255;
	Board->Grid_cells[6][11] = 255;
	Board->Grid_cells[7][9] = 255;
	Board->Grid_cells[7][10] = 255;
	Board->Grid_cells[7][11] = 255;
}



//setting up an funtion for signing textures to simplify the mess
SDL_Texture* Texture_set(SDL_Surface* surface, SDL_Renderer* renderer, const char* filename)
{
	// Here the surface is the information about the image. It contains the color data, width, height and other info.
	surface = IMG_Load(filename);
	if (!surface)
	{
		printf("Unable to load an image %s. Error: %s", filename, IMG_GetError());
		return 0;
	}

	// Now we use the renderer and the surface to create a texture which we later can draw on the screen.
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture)
	{
		printf("Unable to create a texture. Error: %s", SDL_GetError());
		return 0;
	}
	SDL_FreeSurface(surface);

	return texture;
}

void Rectangle_Set(SDL_Rect* rect, int x, int y, int w, int h)
{

	// Here is the rectangle where the image will be on the screen
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
}

//function that renders images in our game
void Render(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect rect)
{


	SDL_RenderCopyEx(renderer, // Already know what is that
		texture, // The image
		0, // A rectangle to crop from the original image. Since we need the whole image that can be left empty (nullptr)
		&rect, // The destination rectangle on the screen.
		0, // An angle in degrees for rotation
		0, // The center of the rotation (when nullptr, the rect center is taken)
		SDL_FLIP_NONE); // We don't want to flip the image


}

typedef struct Board_grid Board;
//creating a structure for units
struct Unit
{
	SDL_Texture* unit_texture;

	Vec2i startPosition;
	Vec2i destiny;
	Vec2i currpos;

	float speedUnit;
};

typedef struct Unit Player_unit;
//grassfire mój
//void Grassfire(Board* board)
//{
//	printf("pinor");
//	bool S = true;
//	while (S)
//	{
//		//copying tables
//		S = false;
//		memcpy(board->Grassfire_Grid_cells, board->Grid_cells, sizeof(board->Grid_cells));
//		for (int i = 0; i < Grid_cells_y; i++)
//		{
//			for (int j = 0; j < Grid_cells_x; j++)
//			{
//				int A = board->Grassfire_Grid_cells[i][j];
//				// if cell is not 255 and is not marked as starting cell
//				if (A != 255 && A != 0)
//				{
//					int B = A + 1;
//					//setting values of neighboring cells to += 1 thus marking them as "passable"
//					int x;
//					if (j > 0)
//					{
//						x = board->Grassfire_Grid_cells[i][j - 1];
//						if (x == 0)
//						{
//							board->Grid_cells[i][j - 1] = B;
//							S = true;
//						}
//					}
//					if (j < Grid_cells_x - 1)
//					{
//						x = board->Grassfire_Grid_cells[i][j + 1];
//						if (x == 0)
//						{
//							board->Grid_cells[i][j + 1] = B;
//							S = true;
//						}
//					}
//					if (i > 0)
//					{
//						x = board->Grassfire_Grid_cells[i - 1][j];
//						if (x == 0)
//						{
//							board->Grid_cells[i - 1][j] = B;
//							S = true;
//						}
//					}
//					if (i < Grid_cells_y - 1)
//					{
//						x = board->Grassfire_Grid_cells[i + 1][j];
//						if (x == 0)
//						{
//							board->Grid_cells[i + 1][j] = B;
//							S = true;
//						}
//					}
//				}
//			}
//		}
//	}
//}

//Grassfire po¿yczony
void Grassfire(Board* board)
{
	// Grassfire algorithm

	bool S = true;
	while (S)
	{
		S = false;
		memcpy(board->Grassfire_Grid_cells, board->Grid_cells, sizeof(board->Grid_cells));
		for (int i = 0; i < Grid_cells_y; ++i)
		{
			for (int j = 0; j < Grid_cells_x; ++j)
			{
				int A = board->Grassfire_Grid_cells[i][j];
				if (A != 0 && A != 255)
				{
					int B = A + 1;
					// Adjacent top
					if (i > 0)
					{
						int x = board->Grassfire_Grid_cells[i - 1][j];
						if (x == 0)
						{
							board->Grid_cells[i - 1][j] = B;
							S = true;
						}
					}
					// Adjacent right
					if (j < Grid_cells_x - 1)
					{
						int x = board->Grassfire_Grid_cells[i][j + 1];
						if (x == 0)
						{
							board->Grid_cells[i][j + 1] = B;
							S = true;
						}
					}
					// Adjacent bottom
					if (i < Grid_cells_y - 1)
					{
						int x = board->Grassfire_Grid_cells[i + 1][j];
						if (x == 0)
						{
							board->Grid_cells[i + 1][j] = B;
							S = true;
						}
					}
					// Adjacent left
					if (j > 0)
					{
						int x = board->Grassfire_Grid_cells[i][j - 1];
						if (x == 0)
						{
							board->Grid_cells[i][j - 1] = B;
							S = true;
						}
					}
				}
			}
		}
	}
}


void Move_unit_to_cell(Board board, Unit* unit)
{

	int minValue = 255;
	int side = 0;

	int currentCellX = unit->currpos.x;
	int currentCellY = unit->currpos.y;

	int newCellX = currentCellX;
	int newCellY = currentCellY;
	printf("\n%i ", newCellX);
	printf("%i\n", newCellY);
	if (currentCellX > 0)
	{
		side = board.Grid_cells[currentCellY][(currentCellX - 1)];

		if (side < minValue && side != 0)
		{
			printf("1");
			minValue = side;
			newCellX = (currentCellX - 1);
			newCellY = currentCellY;
		}
	}
	if (currentCellX < Grid_cells_x - 1)
	{
		side = board.Grid_cells[currentCellY][(currentCellX + 1)];

		if (side < minValue && side != 0)
		{
			minValue = side;
			printf("2");
			newCellX = (currentCellX + 1);
			newCellY = currentCellY;
		}
	}
	if (currentCellY > 0)
	{
		side = board.Grid_cells[(currentCellY - 1)][currentCellX];

		if (side < minValue && side != 0)
		{
			minValue = side;
			printf("3");
			newCellX = currentCellX;
			newCellY = (currentCellY - 1);
		}
	}
	if (currentCellY < Grid_cells_y - 1)
	{
		side = board.Grid_cells[(currentCellY + 1)][currentCellX];

		if (side < minValue && side != 0)
		{
			minValue = side;
			printf("4");
			newCellX = currentCellX;
			newCellY = (currentCellY + 1);
		}
	}

	unit->startPosition.x = (newCellX * GRID_CELL_SIZE) + (GRID_CELL_SIZE / 2);
	unit->startPosition.y = (newCellY * GRID_CELL_SIZE) + (GRID_CELL_SIZE / 2);

}
int main(int argc, char* argv[])
{
	// Init SDL libraries
	SDL_SetMainReady(); // Just leave it be
	int result = 0;
	result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); // Init of the main SDL library
	if (result) // SDL_Init returns 0 (false) when everything is OK
	{
		printf("Can't initialize SDL. Error: %s", SDL_GetError()); // SDL_GetError() returns a string (as const char*) which explains what went wrong with the last operation
		return -1;
	}

	result = IMG_Init(IMG_INIT_PNG); // Init of the Image SDL library. We only need to support PNG for this project
	if (!(result & IMG_INIT_PNG)) // Checking if the PNG decoder has started successfully
	{
		printf("Can't initialize SDL image. Error: %s", SDL_GetError());
		return -1;
	}

	// Creating the window size proportional to cell amount in a grid
	SDL_Window* window = SDL_CreateWindow("FirstSDL",
		0, 0,
		WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (!window)
		return -1;

	// Creating a renderer which will draw things on the screen
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		return -1;

	SDL_Surface* surface = nullptr;
	SDL_Rect rect;
	// Setting the color of an empty window (RGBA). You are free to adjust it.
	SDL_SetRenderDrawColor(renderer, 20, 150, 39, 255);


	// In a moment we will get rid of the surface as we no longer need that. But let's keep the image dimensions.
	int tex_width = GRID_CELL_SIZE;
	int tex_height = GRID_CELL_SIZE;

	int destiny = 2;

	Player_unit P_Unit_1;
	P_Unit_1.speedUnit = 200.0f;
	P_Unit_1.startPosition = { (3 * GRID_CELL_SIZE) + (GRID_CELL_SIZE / 2), (4 * GRID_CELL_SIZE) + (GRID_CELL_SIZE / 2) };
	P_Unit_1.destiny = { P_Unit_1.startPosition.x, P_Unit_1.startPosition.y };

	P_Unit_1.unit_texture = Texture_set(surface, renderer, "Player.png");

	Board board;
	board.Texture_obstacle = Texture_set(surface, renderer, "Cell_obstacle.png");
	board.Texture_cell_default = Texture_set(surface, renderer, "Cell.png");

	Board_create(&board);

	float deltaTime = 0;
	float lastTick = 0;

	float x = P_Unit_1.startPosition.x;
	float y = P_Unit_1.startPosition.y;

	bool done = false;
	bool reachable = false;
	bool cangrassfire = false;
	bool cell_reached = true;
	SDL_Event sdl_event;
	// The main loop
	// Every iteration is a frame
	while (!done)
	{
		float currentTick = (float)SDL_GetTicks() / 1000.f;
		deltaTime = currentTick - lastTick;
		lastTick = currentTick;
		
		while (SDL_PollEvent(&sdl_event))
		{
			if (sdl_event.type == SDL_QUIT) // The user wants to quit
			{
				done = true;
			}
			else if (sdl_event.type == SDL_KEYDOWN) // A key was pressed
			{
				switch (sdl_event.key.keysym.sym) // Which key?
				{
				case SDLK_ESCAPE: // Posting a quit message to the OS queue so it gets processed on the next step and closes the game
					SDL_Event event;
					event.type = SDL_QUIT;
					event.quit.type = SDL_QUIT;
					event.quit.timestamp = SDL_GetTicks();
					SDL_PushEvent(&event);
					break;
				default:
					break;
				}
			}
			else if (sdl_event.type == SDL_MOUSEBUTTONDOWN)  // new event type for mouse key down
			{
				switch (sdl_event.button.button)
				{
				case SDL_BUTTON_LEFT:
					{
						int Mouse_x, Mouse_y;
						SDL_GetMouseState(&Mouse_x, &Mouse_y);
						P_Unit_1.destiny = { Mouse_x, Mouse_y };
						printf("%i\n", P_Unit_1.destiny.x);
						printf("%i\n", P_Unit_1.destiny.y);
						//sets 255 values, if used as start or end pos, as unreachable and exiting loop
						if (board.Grid_cells[P_Unit_1.startPosition.y / GRID_CELL_SIZE][P_Unit_1.startPosition.x / GRID_CELL_SIZE] == 255 or board.Grid_cells[P_Unit_1.destiny.y/ GRID_CELL_SIZE][P_Unit_1.destiny.x / GRID_CELL_SIZE] == 255)
						{
							reachable = false;
							break;
						}
						Board_create(&board);
						reachable = true;
						cangrassfire = true;
						//board.Grid_cells[P_Unit_1.startPosition.x][P_Unit_1.startPosition.y] = 1;
						board.Grid_cells[P_Unit_1.destiny.y / GRID_CELL_SIZE][P_Unit_1.destiny.x / GRID_CELL_SIZE] = 1;
						Grassfire(&board);
					}
					break;
				default:
					break;
				}
			}
		}
		// Clearing the screen
		SDL_RenderClear(renderer);

		for (int i = 0; i < Grid_cells_y; ++i)
		{
			for (int j = 0; j < Grid_cells_x; ++j)
			{
				Rectangle_Set(&rect, j * tex_width, i * tex_height, tex_width, tex_height);
				Render(renderer, (board.Grid_cells[i][j] == 255 ? board.Texture_obstacle : board.Texture_cell_default), rect );
			}
		}

		Rectangle_Set(&rect, (int)round(x - tex_width / 2), (int)round(y - tex_height / 2), tex_width, tex_height);
		Render(renderer, P_Unit_1.unit_texture, rect);

		// Showing the screen to the player
		SDL_RenderPresent(renderer);
		
		if (reachable)
		{

			P_Unit_1.currpos.x = x / GRID_CELL_SIZE;
			P_Unit_1.currpos.y = y / GRID_CELL_SIZE;
			cell_reached = true;
			if (fabs(x - P_Unit_1.destiny.x) >= destiny) 
			{
				if (x > P_Unit_1.destiny.x) 
				{
					x -= P_Unit_1.speedUnit * deltaTime;
				}
				else 
				{
					x += P_Unit_1.speedUnit * deltaTime;
				}
				cell_reached = false;
			}
			if (fabs(y - P_Unit_1.destiny.y) >= destiny) 
			{
				if (y > P_Unit_1.destiny.y) 
				{
					y -= P_Unit_1.speedUnit * deltaTime;
				}
				else 
				{
					y += P_Unit_1.speedUnit * deltaTime;
				}
				cell_reached = false;
			}

			/*if (cell_reached && cangrassfire)
			{
				printf("//////////asdogfikdhjasfioguefdhroigehroigfvhrdfgihdjsapighadsfoighdfpigejqrpoifjeqrpiofvis true");
				Grassfire(&board);

				cangrassfire = false;
			}*/
			if (cell_reached &&((P_Unit_1.destiny.x/GRID_CELL_SIZE) != P_Unit_1.currpos.x) || ((P_Unit_1.destiny.y/GRID_CELL_SIZE) != P_Unit_1.currpos.y))
			{

				Move_unit_to_cell(board, &P_Unit_1);
			}
		}
	}

	// If we reached here then the main loop stoped
		// That means the game wants to quit

		// Shutting down the renderer
	SDL_DestroyRenderer(renderer);

	// Shutting down the window
	SDL_DestroyWindow(window);

	// Quitting the Image SDL library
	IMG_Quit();
	// Quitting the main SDL library
	SDL_Quit();

	// Done.
	return 0;
}