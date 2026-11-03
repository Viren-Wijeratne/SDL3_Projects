#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WIDTH 1280 //320
#define HEIGHT 720 //200

uint32_t framebuffer[WIDTH * HEIGHT];	//Static Array

void put_pixel(int x, int y, uint32_t color);
void draw_line(int x1, int y1, int x2, uint32_t color);
void draw_rectangle(int x1, int y1, int x2, int y2, uint32_t color);
void fill_rectangle(int x1, int y1, int x2, int y2, uint32_t color);
void columns(uint32_t color);
void clear(uint32_t color);

int main (void){

//The Window is the base in which the renderer sites (renderer can be OpenGl or Vulcan even) and the texture is placed inside the renderer which loads the framebuffer
SDL_Window *window;	
SDL_Renderer *renderer;
SDL_Texture *texture;
SDL_Event event;

const double target_frame = 1.0/60.0; //Which is 0.016
	
if(!SDL_Init(SDL_INIT_VIDEO)){
	fprintf(stderr, "SDL Init failed: %s \n", SDL_GetError());
	return EXIT_FAILURE;
}	
	
window = SDL_CreateWindow(
		"SDL Framebuffer", 
		WIDTH, //Changed
		HEIGHT,  //changed
		0
);

if(window == NULL){ 		//Error checking for good measure
	fprintf(stderr, "SDL_CreateWindow failed: %s \n", SDL_GetError());
	SDL_Quit();
	return EXIT_FAILURE;
}

renderer = SDL_CreateRenderer(
		window, 
		NULL
);

if(renderer == NULL){		//Error checking for good measure
	fprintf(stderr, "SDL_CreateRenderer failed: %s \n", SDL_GetError());
	SDL_DestroyWindow(window); 	//Need to close the window at this breakpoint window is already initiallized
	SDL_Quit();			//Need to quite from SDL_Init
	return EXIT_FAILURE;
}

texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_XRGB8888, 
		SDL_TEXTUREACCESS_STREAMING, 
		WIDTH, 
		HEIGHT
);

if(texture == NULL){		//Error checking for good measure
	fprintf(stderr, "SDL_CreateTexture failed: %s \n", SDL_GetError());
	SDL_DestroyRenderer(renderer); //Need to quite from renderer and the window at this point if something breaks
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_FAILURE;
}


SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

uint8_t isRunning = 1;
uint32_t frame = 0;	//Start of the animation frame count

while(isRunning){
		
	uint64_t start = SDL_GetPerformanceCounter(); 
	//SDL_GetPerformanceCounter returns a high-resolution harwdware counter, giving is microsecond or nanosecond-level precision

	//Poll the events (User Input)
	while(SDL_PollEvent(&event)){
		if(event.type == SDL_EVENT_QUIT){	
		//Listening until a event occurs to make a change in the program (Pressing the "X" button to exit a program == SDL_EVENT_QUIT)
			isRunning = 0;			//This breaks the while loop
		}
	}


/**********************************************************************************************************************************************************/
//Here we can manipulate our framebuffer array as we wish...

	//clear(0xFFFFFF);
	//draw_line(40, 20, 100, 0xFF0000);
	//draw_rectangle(50, 20, 300, 100, 0x00FF00);
	//columns(0x00FFFF);
	
/*** Animate a pixel moving from left to right, using varible "frame" as an increment for x (x%WIDTH is a safeguard) ***/
	clear(0x000000); //This is essential as it needs to clear the previous frame from the buffer, If not that frame will show up repeatedly
	int x = frame % WIDTH;
	//int y = HEIGHT/2;
	for(int y = 0; y <= HEIGHT; y++){
		if(y % 10 ==0){
			put_pixel(x, y, 0x00FFFF);
		}
	}

/**********************************************************************************************************************************************************/



	// Copy the content of the framebuffer to the texture
	SDL_UpdateTexture(
		texture, 
		NULL, 
		framebuffer,
		WIDTH * sizeof(uint32_t)	//Pitch: the width of the texture in bytes
	);


		/*** Display the window and the renderer ***/
	SDL_RenderClear(renderer);	//Clear the current render
	SDL_RenderTexture(renderer, texture, NULL, NULL); //Draw texture to the renderer canvas
	SDL_RenderPresent(renderer);	//Displays the render
	
	uint64_t end = SDL_GetPerformanceCounter();
	double elapsed =(double)(end - start)/(double)SDL_GetPerformanceFrequency(); 
	//SDL_GetPerformanceFrequency() tells you how fast the performance counter increments. You can think of it as the "ticks per second" of the counter
	
	//Cap the Framerate to 60 FPS
	if (elapsed < target_frame){
		SDL_Delay((target_frame - elapsed)*1000.0); //SDL_Delay is used to sleep until we reach the target for 60 FPS
	}

frame += 1;	//Increment of the animation frame count
}


SDL_DestroyTexture(texture);	//Releases from the memory before closing the program
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();

return EXIT_SUCCESS;

}


void put_pixel(int x, int y, uint32_t color){
	if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT){	//Buffer-overflow Protection
		return;
	}
	framebuffer[WIDTH * y + x] = color;
	}

void draw_line(int x1, int y1, int x2, uint32_t color){
	if (x1<0||x1>=WIDTH||x2<0||x2>=WIDTH||y1<0||y1>=HEIGHT){	//Buffer-overflow Protection
		return;
	}
	else if(x1 < x2){
		for(int x = x1; x <= x2; x++){
			framebuffer[WIDTH * y1 + x] = color;
		}
	}
}

void draw_rectangle(int x1, int y1, int x2, int y2, uint32_t color){
	if (x1<0||x1>=WIDTH||x2<0||x2>=WIDTH||y1<0||y1>=HEIGHT||y2<0||y2>=HEIGHT){	//Buffer-overflow Protection
		return;
	}
	else if (x1 < x2 && y1 < y2){
		for(int x = x1; x <= x2; x++){
			framebuffer[WIDTH * y1 + x] = color;
			framebuffer[WIDTH * y2 + x] = color;
		}
		for(int y = y1; y <= y2; y++){
			framebuffer[WIDTH * y + x1] = color;
			framebuffer[WIDTH * y + x2] = color;
		}
	}
}

void fill_rectangle(int x1, int y1, int x2, int y2, uint32_t color){
	if (x1<0||x1>=WIDTH||x2<0||x2>=WIDTH||y1<0||y1>=HEIGHT||y2<0||y2>=HEIGHT){	//Buffer-overflow Protection
		return;
	}
	else if (x1 < x2 && y1 < y2){
		for(int x = x1; x <= x2; x++){
			for(int y = y1; y <= y2; y++){
				framebuffer[WIDTH * y + x] = color;
			}
		}
	}	
}

void columns(uint32_t color){
	for(int y = 0; y < HEIGHT; y++){
		for(int x = 0; x < WIDTH; x++){
			if(x % 5 == 0){
				put_pixel(x, y, color);
			}
		}
	}
}

void clear(uint32_t color){
	for(int i = 0; i < WIDTH * HEIGHT; i++){
		framebuffer[i] = color;
	}	
}
