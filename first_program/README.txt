- The header files are located at: /usr/include/SDL3

**The basics of putting pixel on the screen**

- There are mainly 3 parts to be initialized,
    1. Window
    2. Renderer
    3. Texture

- First we initialize pointers using,
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;

- Where error handling is also done using if statements

- Then we define,

    window = (SDL_Window * SDL_CreateW*title, int w, int h, SDL_WindowFlags flags);)
