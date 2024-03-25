#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
//#include "SDL_ttf.h"
#include "SDL2/SDL_ttf.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

 
struct{
  SDL_Window* window; 
  SDL_Renderer* renderer;
  TTF_Font* Sans;
  SDL_Surface* surfaceMessage;
  SDL_Texture* Message;
  SDL_Rect Message_rect; //create a rect
} screen;

int fileSize(char file_name[]){
  // opening the file in read mode 
  FILE* fp = fopen(file_name, "r"); 

  // checking if the file exist or not 
  if (fp == NULL) { 
      printf("File Not Found!\n"); 
      return -1; 
  } 

  fseek(fp, 0L, SEEK_END); 

  // calculating the size of the file 
  int res = ftell(fp); 

  // closing the file 
  fclose(fp); 

  return res; 

}

void changeDim(char *a){
  int size= sizeof(a);
  printf("%d", size);
  a = realloc(a, (size+1) * sizeof(char));
}

int main(int argc, char **argv) {
  FILE *inputFile;
  char *inputText;
  char c; 
  int  i, resizeDim;
  int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

  if(argc == 1){
    printf("No file input");
    return 1;
  }
  if(argc > 2){
    printf("Only 1 input allowed");
    return 1;
  }

  inputFile = fopen(argv[1], "r");
 
  if(inputFile == NULL){
    printf("File doesn't exist\n");
    return 1;
  }

  inputText = (char*)calloc(fileSize(argv[1]), sizeof(char));
  i = 0; 
  while(1){
    c = fgetc(inputFile);
    if(feof(inputFile)){
      break;
    }
    inputText[i] = c; 
    i++;
  }
  fclose(inputFile);
//  printf("%lu", strlen(inputText));
  //printf("%lu", strlen(inputText)+1);
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
      return 1;
  }

  if (TTF_Init() < 0)
  {
    printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
    exit(1);
  }
  screen.window = SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

  if (!screen.window)
  {
      printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
      exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  screen.renderer = SDL_CreateRenderer(screen.window, -1, rendererFlags);

  if (!screen.renderer)
  {
      printf("Failed to create renderer: %s\n", SDL_GetError());
      exit(1);
  }

  SDL_Color White = {255, 255, 255};
  screen.Sans = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 24);
   screen.surfaceMessage = TTF_RenderText_Solid(screen.Sans, inputText, White); 
    if( screen.surfaceMessage == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );char greetings[] = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\0'};
printf("%s", greetings);
        exit(1);
    }
  screen.Message = SDL_CreateTextureFromSurface(screen.renderer, screen.surfaceMessage);
    if( screen.Message == NULL )
    {
        printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        exit(1);
    }
  screen.Message_rect.x = 0;  //controls the rect's x coordinate 
  screen.Message_rect.y = 0; // controls the rect's y coordinte
  screen.Message_rect.w = 200; // controls the width of the rect
  screen.Message_rect.h = 200; // controls the height of the rect
 SDL_FreeSurface(screen.surfaceMessage);
  //printf("%c", *inputText); 
  SDL_StartTextInput();
  bool quit = false;
  while(!quit){
    bool renderText = false;
    SDL_Event e;
    while(SDL_PollEvent(&e)){
    switch( e.type){
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        strcat(inputText, SDL_GetKeyName(e.key.keysym.sym));
        renderText = true;
        break;
      case SDL_QUIT:
        quit=1;
        break;
      defualt:
        break;
    }




    /*

      if(e.type == SDL_QUIT){
        quit= true;
      }
      else if(e.type = SDL_KEYUP){
        //Handle backspace
        if( e.key.keysym.sym == SDLK_BACKSPACE && strlen(inputText) > 0 )
        {
            //lop off character
            inputText[strlen(inputText)-1] = '\0';
            renderText = true;
        }        
        else if (e.key.keysym.sym >= SDLK_SPACE && e.key.keysym.sym <= SDLK_z) {
                    // Append the character to the input text
                    changeDim( inputText);
                    strcat(inputText, SDL_GetKeyName(e.key.keysym.sym));
                    renderText = true;
        }
      }
      */

    }


      //Rerender text if needed
      if( renderText )
      {
          //Text is not empty
          if( inputText != "" )
          {
              //Render new text
              screen.surfaceMessage = TTF_RenderText_Solid(screen.Sans, inputText, White); 
              screen.Message = SDL_CreateTextureFromSurface(screen.renderer, screen.surfaceMessage);
              //SDL_RenderCopy(screen.renderer, screen.Message, NULL, &screen.Message_rect);
              SDL_FreeSurface(screen.surfaceMessage);
          }
          //Text is empty
          else
          {
              screen.surfaceMessage = TTF_RenderText_Solid(screen.Sans, "", White); 
              screen.Message = SDL_CreateTextureFromSurface(screen.renderer, screen.surfaceMessage);
              //SDL_RenderCopy(screen.renderer, screen.Message, NULL, &screen.Message_rect);
              SDL_FreeSurface(screen.surfaceMessage);
          }
      }

    SDL_SetRenderDrawColor(screen.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(screen.renderer);
    screen.Message_rect.w = SCREEN_WIDTH - screen.Message_rect.w / 2;
    SDL_RenderCopy(screen.renderer, screen.Message, NULL, &screen.Message_rect);
    SDL_RenderPresent(screen.renderer);
  }
  SDL_DestroyTexture(screen.Message);
  SDL_DestroyRenderer(screen.renderer);
  SDL_DestroyWindow(screen.window);
  SDL_StopTextInput();
  TTF_CloseFont(screen.Sans);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
