#ifndef SDL_WRAPPER_H_INCLUDED
#define SDL_WRAPPER_H_INCLUDED

#include <SDL2/SDL.h>
//#include <SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL_image.h>
#include <string>

/// VERSION 1.0
/*
Changelog:
    -1.1-
        Added toggleFullscreen() function
    -1.0-
        Implemented versioning system
        SDL constructor now allows for explicit width/height assignment at instance creation
            FUTURE: Protect these and use getWidth() and getHeight() functions to access members, as they are no longer immutable constants
        Added newBlankTexture(int width, int height)
            Returns a new, optimized blank texture based on the size parameters passed in to the function
        Added multiplyTextureSize(SDL_Texture* sourceTexture, int scale, bool destructive = false)
            Returns a new texture that is actually the passed texture with dimensions multiplied by the scale factor. If destructive, destroys the old texture as well
    -Old-
        SDL wrapper class exists that contains many useful features, such as:
            An SDL_Window* and SDL_Renderer*, named window and renderer, respectively
            WIDTH and HEIGHT class-wide declaration
            Streamlined surface and texture loading
            FPS control
            Functions:
                clear() - clears the rendered screen, using the current render draw color
                update() - updates the rendered screen
                loadTexture(std::string filepath) - Loads and returns an optimized texture from filepath
                loadSurface(std::string filepath) - Loads and returns an optimized surface from filepath
                    This function also exists as a SEPARATE loadSurface(std::string filepath) function (outside of the class), so it CAN be used without an SDL instance!
                FPSinit(int framesPerSecond) - Initializes FPS submodule and caps framerate at the passed frames per second
                FPSlog(); - Halts the game until the given framerate has been reached. After initializing using FPSinit(), call FPSlog() every game cycle to smooth framerate.
*/

class SDL
{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    //SDL_Surface* surface;
    int WIDTH;
    int HEIGHT;
    unsigned int msPerFrame = 0;
    unsigned int lastTick = 0; // Used as a log point for FPS calculation
    bool windowed = true; // Whether this sdl instance is fullscreen or not
    int fullscreenWidth; // The width of the screen when fullscreen. Used for positioning things
    int fullscreenHeight; // ^^^
    // Functions
    SDL(int width = 1280, int height = 800); // Constructor with default arguments
    virtual ~SDL();
    void clear(); // clears the screen
    void update(); // updates the screen (use SDL_RenderCopy in between)
    SDL_Texture* loadTexture(std::string filepath); // Loads the given filepath as an optimized texture
    SDL_Surface* loadSurface(std::string filepath); // Loads and returns a surface from a filepath
    SDL_Texture* newBlankTexture(int width, int height); // Creates and returns a new, optimized, blank texture of the given size
    SDL_Texture* multiplyTextureSize(SDL_Texture* sourceTexture, int scale, bool destructive = false); // Returns a new texture, scaled by the given constant
    void FPSinit(int framesPerSecond); // Starts the FPS submodule and caps framerate at a given number
    void FPSlog(); // Pauses the game until a given framerate is reached
    void toggleFullscreen(); // Toggles fullscreen of this SDL
    //SDL_Texture* mergeTexture(SDL_Texture* t1, int t1_pos[2], SDL_Texture* t2, int t2_pos[2]); // Merges both textures onto a 32x32 texture to return
};

SDL::SDL(int width, int height)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    // Set up variables
    WIDTH = width;
    HEIGHT = height;
    //Create window
    window = SDL_CreateWindow("Ex Gaea", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    //surface = SDL_GetWindowSurface(window);

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    IMG_Init(imgFlags);
}

SDL::~SDL()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    //SDL_FreeSurface(surface);
    //SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

SDL_Texture* SDL::loadTexture(std::string filepath)
{
    SDL_Surface* loadedSurface = IMG_Load(filepath.c_str());
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

SDL_Surface* SDL::loadSurface(std::string filepath)
{
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(filepath.c_str());
    if(loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", filepath.c_str(), IMG_GetError());
    }
    else
    {
        return loadedSurface;
    }
    return nullptr;
}
/// NOTE This function is here twice so you can use it without an SDL instance...
SDL_Surface* loadSurface(std::string filepath)
{
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(filepath.c_str());
    if(loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", filepath.c_str(), IMG_GetError());
    }
    else
    {
        return loadedSurface;
    }
    return nullptr;
}

SDL_Texture* SDL::newBlankTexture(int width, int height)
{
    return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
}

SDL_Texture* SDL::multiplyTextureSize(SDL_Texture* sourceTexture, int scale, bool destructive)
{
    // Multiplies the dimension of the existing sourceTexture and returns.
    // If destructive, destroys the old source texture

    // Make a rect of the needed size
    SDL_Rect rect = {0, 0, 0, 0};
    SDL_QueryTexture(sourceTexture, NULL, NULL, &rect.w, &rect.h);
    rect.w *= scale;
    rect.h *= scale;

    // Copy the texture to a new texture of the needed size, deleting the old one if needed
    SDL_Texture* holdTexture = newBlankTexture(rect.w, rect.h); //SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
    SDL_SetRenderTarget(renderer, holdTexture);
    SDL_SetTextureBlendMode(holdTexture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, sourceTexture, NULL, &rect);
    SDL_SetRenderTarget(renderer, NULL);

    if (destructive)
    {
        SDL_DestroyTexture(sourceTexture);
    }

    return holdTexture;
}

void SDL::clear()
{
    SDL_RenderClear(renderer);

    // Old surface rendering things
    // Clear the window with a white background
    //SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
    //Update the surface
    //SDL_UpdateWindowSurface(window);
}

void SDL::update()
{
    SDL_RenderPresent(renderer);
}

void SDL::FPSinit(int framesPerSecond)
{
    // Calculate the amount of msPerFrame based on the given max framerate
    msPerFrame = 1000 / framesPerSecond;
    if (framesPerSecond == 60)
    {
        msPerFrame = 17; // Rounding
    }
    // Log the first tick value
    lastTick = SDL_GetTicks();
}

void SDL::FPSlog()
{
    // Cap the game to a given framerate (wait until the given msPerFrame is reached, then log the new ms count)
    // Also handles resetting after a given number of ticks so C++ doesn't explode

    // Wait until the given frame is equal to the amount waited
    while ((lastTick + msPerFrame) > SDL_GetTicks())//((lastTick + msPerFrame) % 1000000 < (SDL_GetTicks()) % 1000000)
    {
        // Literally does nothing but wait and reset counters
        //lastTick %= 1000000;
    }
    // Now has actually waited the correct amount of time
    lastTick = SDL_GetTicks();
}

void SDL::toggleFullscreen()
{
    // This function toggles fullscreen
    // Source: https://gist.github.com/Cheeseness/8a49668d0a43278b72b2
    // Grab the mouse so that we don't end up with unexpected movement when the dimensions/position of the window changes.
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	windowed = !windowed;
	if (windowed)
	{
		int i = SDL_GetWindowDisplayIndex(window);
        // Recalc window dimensions
		//screenWidth = windowedWidth;
		//screenHeight = windowedHeight;
		SDL_SetWindowFullscreen(window, 0);
	}
	else
	{
		int i = SDL_GetWindowDisplayIndex(window);
        // Recalc window dimensions
		SDL_Rect j;
		SDL_GetDisplayBounds(i, &j);
		fullscreenWidth = j.w;
		fullscreenHeight = j.h;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
    // Anything that needs to be done if resolution changes can go here
}

/* Currently unused, opted to just blit all to the screen at different positions instead (allowed for actual subpixel movement)
SDL_Texture* mergeTexture(SDL_Texture* t1, int[2] t1_pos, SDL_Texture* t2, int[2] t2_pos)
{
    // Merges both textures onto a new texture, does NOT destroy the old ones, and returns the new one
    SDL_Texture* newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 32, 32);
    SDL_SetRenderTarget(renderer, newTexture); // Starts the render to the texture

    // Render the textures to the texture
    SDL_RenderCopy(renderer, t1, t1_pos[0], t1_pos[1]);
    SDL_RenderCopy(renderer, t2, t2_pos[0], t2_pos[1]);

    SDL_SetRenderTarget(renderer, NULL); // Resets the render target to the screen
    SDL_RendererPresent(renderer);
    return newTexture;
} */

/// Functions below can be used without an SDL instance ///

Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    // Here p is the address to the pixel we want to retrieve
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;       // shouldn't happen, but avoids warnings
    }
        /* Usage:
        SDL_Color rgb;
        Uint32 data = getPixel(gSurface, 200, 200);
        SDL_GetRGB(data, gSurface->format, &rgb.r, &rgb.g, &rgb.b);
        */
}



#endif // SDL_WRAPPER_H_INCLUDED
