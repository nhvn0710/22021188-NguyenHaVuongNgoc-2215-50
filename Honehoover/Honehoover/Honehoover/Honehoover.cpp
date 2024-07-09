#include <SDL.h>
#include <cstdio>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
	SDL_Window* window = NULL;
	
	SDL_Surface* screenSurface = NULL;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
		window = SDL_CreateWindow( "SDL Game - Honehoover", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
		else
		{
			screenSurface = SDL_GetWindowSurface( window );

			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xAA, 0xFF, 0xDD ) );

			SDL_UpdateWindowSurface( window );

            SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
		}
	}

	SDL_DestroyWindow( window );

	SDL_Quit();

	return 0;
}
