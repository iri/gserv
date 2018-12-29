#include <SDL.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "logger.h"
#include "time.h"
#include "udp.h"
#include "udplib.h"
#include "gs_api.h"
#include "gserv.h"

#define SCREEN_WIDTH 		1000
#define SCREEN_HEIGHT		600

Logger *LOG;
int started;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

SDL_Surface *screen = NULL;

extern item_t obj[MAXOBJ];
extern int objcnt;

extern gp_color_t bgcolor;


void drawThickPoint(int x, int y)
{
	SDL_RenderDrawPoint( gRenderer, x-1, y-1 );
	SDL_RenderDrawPoint( gRenderer, x-1, y   );
	SDL_RenderDrawPoint( gRenderer, x-1, y+1 );
	SDL_RenderDrawPoint( gRenderer, x  , y-1 );
	SDL_RenderDrawPoint( gRenderer, x  , y   );
	SDL_RenderDrawPoint( gRenderer, x  , y+1 );
	SDL_RenderDrawPoint( gRenderer, x+1, y-1 );
	SDL_RenderDrawPoint( gRenderer, x+1, y   );
	SDL_RenderDrawPoint( gRenderer, x+1, y+1 );
}


int init()
{
	//Initialization flag
	int success = 1;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = 0;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "GServ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = 0;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = 0;
			}
			else
			{
				screen = SDL_GetWindowSurface(gWindow);

				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
			}
		}
	}

	return success;
}


int loadMedia()
{
	return 1;
}


int main( int argc, char* args[] )
{
	int err;

   LOG = Logger_create();
   LOG->level = LOG_DEBUG;

   // Start udp_listener_loop() process
   pthread_t tid_udp_listener_loop;
   started = 0;
   err = pthread_create(&tid_udp_listener_loop, NULL, &udp_listener_loop, NULL);
   if (err != 0)
   {
      log_error(LOG, "Can't create udp_listener_loop thread :[%s]", strerror(err));
      return 1;
   } else {
      while (started!=1) {
         usleep(10000);
      }
      log_info(LOG, "udp_listener_loop thread started on port %lu",UDP_PORT);
   }


	objcnt = 0;

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			MICROSEC FPS = 20000;					// 1000000/20000us = 50FPS
			MICROSEC T_refresh_offset;
			TIMER T_refresh;

			int i;

			//Main loop flag
			int quit = 0;

			//Event handler
			SDL_Event e;

			// timer for refresh
		   timer_start(&T_refresh);

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = 1;
					}
					else if( e.type == SDL_KEYDOWN )
					{
						printf("Pressed %d\n",e.key.keysym.sym);
					}
				}

		      T_refresh_offset = timer_stop(&T_refresh);

				if (T_refresh_offset >= FPS)
				{
					//Clear screen

					SDL_SetRenderDrawColor( gRenderer, bgcolor.r, bgcolor.g, bgcolor.b, bgcolor.alpha );
					SDL_RenderClear( gRenderer );

					// Draw content
					for (i=0;i<objcnt;i++) {

						switch(obj[i].t) {

						   case GP_POINT  :
						   	SDL_RenderDrawPoint( gRenderer, obj[i].d.point.x, obj[i].d.point.y );
						      break;
								  
						   case GP_LINE  :
								SDL_RenderDrawLine(gRenderer, obj[i].d.line.x1, obj[i].d.line.y1, obj[i].d.line.x2, obj[i].d.line.y2);
						      break;

						   case GP_COLOR  :
								SDL_SetRenderDrawColor( gRenderer, obj[i].d.color.r, obj[i].d.color.g, obj[i].d.color.b, obj[i].d.color.alpha );
						      break;

						}   

					}

					//Update screen
					SDL_RenderPresent( gRenderer );

					//printf("Render, ts=%lu\n",T_refresh_offset);

				   timer_start(&T_refresh);
				}

				SDL_Delay(1);
			}
		}
	}

	//Free resources and close SDL
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	SDL_Quit();

	return 0;
}

