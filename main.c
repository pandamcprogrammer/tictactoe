#include <stdio.h>
#include "SDL2/SDL.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int BUTTON_WIDTH = 640/3;
const int BUTTON_HEIGHT = 480/3;
const int TOTAL_BUTTONS = 9;

SDL_Window *g_window = NULL;
SDL_Surface *g_screen_surface = NULL;
SDL_Surface *g_stretched_surface = NULL;
SDL_Surface *g_x = NULL;
SDL_Surface *g_o = NULL;

int init();
int load_media();
void app_close();
SDL_Surface *load_surface(char *path);
void mark_x(int, int);
void fill_in_markers();

int cpu_needs_to_move = 0;
void cpu_move();

int main(int argc, char *args[])
{
     if (!init())
          printf("Failed to initialize!\n");
     else {
          if (!load_media())
               printf("Failed to load media!\n");
          else {
               int quit = 0;

               SDL_Event e;

               // Main game loop.
               while (!quit) {
                    while (SDL_PollEvent(&e) != 0) {
                         if (e.type == SDL_QUIT)
                              quit = 1;

                         if (e.type == SDL_MOUSEBUTTONDOWN) {
                              if (e.button.button == SDL_BUTTON_LEFT) {
                                   int x = e.motion.x;
                                   int y = e.motion.y;
                                   if (0 <= x && x <= SCREEN_WIDTH/3 && 0 <= y && y <= SCREEN_HEIGHT/3)
                                        mark_x(1,1);
                                   else if (0 <= x && x <= SCREEN_WIDTH/3 && SCREEN_HEIGHT/3 <= y && y <= SCREEN_HEIGHT*2/3)
                                        mark_x(1,2);
                                   else if (0 <= x && x <= SCREEN_WIDTH/3 && SCREEN_HEIGHT*2/3 <= y && y <= SCREEN_HEIGHT)
                                        mark_x(1,3);
                                   else if (SCREEN_WIDTH/3 <= x && x <= SCREEN_WIDTH*2/3 && 0 <= y && y <= SCREEN_HEIGHT/3)
                                        mark_x(2,1);
                                   else if (SCREEN_WIDTH/3 <= x && x <= SCREEN_WIDTH*2/3 && SCREEN_HEIGHT/3 <= y && y <= SCREEN_HEIGHT*2/3)
                                        mark_x(2,2);
                                   else if (SCREEN_WIDTH/3 <= x && x <= SCREEN_WIDTH*2/3 && SCREEN_HEIGHT*2/3 <= y && y <= SCREEN_HEIGHT)
                                        mark_x(2,3);
                                   else if (SCREEN_WIDTH*2/3 <= x && x <= SCREEN_WIDTH && 0 <= y && y <= SCREEN_HEIGHT/3)
                                        mark_x(3,1);
                                   else if (SCREEN_WIDTH*2/3 <= x && x <= SCREEN_WIDTH && SCREEN_HEIGHT/3 <= y && y <= SCREEN_HEIGHT*2/3)
                                        mark_x(3,2);
                                   else if (SCREEN_WIDTH*2/3 <= x && x <= SCREEN_WIDTH && SCREEN_HEIGHT*2/3 <= y && y <= SCREEN_HEIGHT)
                                        mark_x(3,3);
                              }

                         }
                    }

                    cpu_move();

                    SDL_Rect stretch_rect;
                    stretch_rect.x = 0;
                    stretch_rect.y = 0;
                    stretch_rect.w = SCREEN_WIDTH;
                    stretch_rect.h = SCREEN_HEIGHT;
                    SDL_BlitScaled(g_stretched_surface, NULL, g_screen_surface, &stretch_rect);

                    fill_in_markers();

                    SDL_UpdateWindowSurface(g_window);
               }
          }
     }

     app_close();

     return 0;
}

// 0 for neutral. 1 for player. -1 for computer
int markers[3][3] = {{0,0,0},
                     {0,0,0},
                     {0,0,0}};

void mark_x(int x, int y)
{
     printf("%d, %d\n", x, y);
     if (markers[x-1][y-1] == 0) {
          markers[x-1][y-1] = 1;
          cpu_needs_to_move = 1;
     }
}

void cpu_move()
{
     int x, y;
     while (cpu_needs_to_move) {
          x = rand() % 3;
          y = rand() % 3;
          if (markers[x][y] == 0){
               markers[x][y] = -1;
               cpu_needs_to_move = 0;
          }
     }
}


void fill_in_markers()
{
     for (int i=0; i < 3; i++) {
          for (int j=0; j < 3; j++) {
               if (markers[i][j] != 0) {
                         SDL_Rect stretch_rect;
                         stretch_rect.x = BUTTON_WIDTH * i;
                         stretch_rect.y = BUTTON_HEIGHT * j;
                         stretch_rect.w = BUTTON_WIDTH;
                         stretch_rect.h = BUTTON_HEIGHT;
                         if (markers[i][j] == 1)
                              SDL_BlitScaled(g_x, NULL, g_screen_surface, &stretch_rect);
                         else
                              SDL_BlitScaled(g_o, NULL, g_screen_surface, &stretch_rect);
                    }
          }
     }
}

int init()
{
     int success = 1;

     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
          printf("SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
          success = 0;
     }
     else {
          g_window = SDL_CreateWindow("Tic Tac Toe!",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH,
                                    SCREEN_HEIGHT,
                                    SDL_WINDOW_SHOWN);
          if (g_window == NULL) {
               printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
               success = 0;
          }
          else
               g_screen_surface = SDL_GetWindowSurface(g_window);
     }
     return success;
}

int load_media()
{
     int success = 1;

     g_stretched_surface = load_surface("../snake/bg.bmp");
     if (g_stretched_surface == NULL) {
          printf("Failed to load stretching image!\n");
          success = 0;
     }

     // load icons
     g_x = load_surface("../snake/x_icon.bmp");
     g_o = load_surface("../snake/o_icon.bmp");

     return success;
}

void app_close()
{
     SDL_FreeSurface(g_stretched_surface);
     g_stretched_surface = NULL;

     SDL_DestroyWindow(g_window);
     g_window = NULL;

     SDL_Quit();
}


SDL_Surface *load_surface(char *path)
{
     SDL_Surface *optimized_surface = NULL;

     SDL_Surface *loaded_surface = SDL_LoadBMP( path ); // ???
     if (loaded_surface == NULL)
          printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
     else {
          optimized_surface = SDL_ConvertSurface(loaded_surface, g_screen_surface->format, 0);
          if (optimized_surface == NULL)
               printf("Unable to load image %s! SDL Error: %s\n", path,SDL_GetError());

          SDL_FreeSurface(loaded_surface);
     }
     return optimized_surface;
}


