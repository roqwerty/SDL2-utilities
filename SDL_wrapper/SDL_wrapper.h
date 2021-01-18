#ifndef SDL_WRAPPER_H_INCLUDED
#define SDL_WRAPPER_H_INCLUDED

#include <SDL2/SDL.h>
//#include <SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL_image.h>
#include <string>
#include <math.h> // Only really used in the ellipse function

/// VERSION 1.11
/*
Changelog:
    -1.14-
        SDL constructor changed to SDL(std::string title, int width = 1280, int height = 800). This allows passing of dynamically, but WILL break older versions if ported directly.
            File itself renamed to SDL_wrapper.h to make this breakage more apparent, and to remove versioning from title
    -1.13-
        Added FPSlog(float& FPS) function: Same as FPSlog(), but stores the current FPS into the passed variable
    -1.12-
        Added SDL_DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius) function (not original, credit above function)
        Added SDL_DrawEllipse(SDL_Renderer* r, int x0, int y0, int radiusX, int radiusY) function (not original, credit above function)
    -1.11-
        Made all functions inline to appease the .h file format ideals
    -1.1-
        Added relevant error messages when a loaded texture/surface cannot be found
        Added toggleFullscreen() function
        Added saveTextureToFile() function
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
    inline SDL(std::string title, int width = 1280, int height = 800); // Constructor with default arguments
    inline virtual ~SDL();
    inline void clear(); // clears the screen
    inline void update(); // updates the screen (use SDL_RenderCopy in between)
    inline SDL_Texture* loadTexture(std::string filepath); // Loads the given filepath as an optimized texture
    inline SDL_Surface* loadSurface(std::string filepath); // Loads and returns a surface from a filepath
    inline SDL_Texture* newBlankTexture(int width, int height); // Creates and returns a new, optimized, blank texture of the given size
    inline SDL_Texture* multiplyTextureSize(SDL_Texture* sourceTexture, int scale, bool destructive = false); // Returns a new texture, scaled by the given constant
    inline void FPSinit(int framesPerSecond); // Starts the FPS submodule and caps framerate at a given number
    inline void FPSlog(); // Pauses the game until a given framerate is reached
    inline void FPSlog(float& FPS); // ^ plus stores FPS into passed variable
    inline void toggleFullscreen(); // Toggles fullscreen of this SDL
    inline void saveTextureToFile(std::string filepath, SDL_Texture* texture); // Saves the given texture to file as a PNG
    //SDL_Texture* mergeTexture(SDL_Texture* t1, int t1_pos[2], SDL_Texture* t2, int t2_pos[2]); // Merges both textures onto a 32x32 texture to return
};

SDL::SDL(std::string title, int width, int height)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    // Set up variables
    WIDTH = width;
    HEIGHT = height;
    //Create window
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
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
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", filepath.c_str(), IMG_GetError());
    }
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
inline SDL_Surface* loadSurface(std::string filepath)
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
    //std::cout << (1000.0)/(SDL_GetTicks()-lastTick) << std::endl; // Print FPS to console
    lastTick = SDL_GetTicks();
}

void SDL::FPSlog(float& FPS)
{
    // This version stores the FPS for use elsewhere

    // Cap the game to a given framerate (wait until the given msPerFrame is reached, then log the new ms count)
    // Also handles resetting after a given number of ticks so C++ doesn't explode

    // Wait until the given frame is equal to the amount waited
    while ((lastTick + msPerFrame) > SDL_GetTicks())//((lastTick + msPerFrame) % 1000000 < (SDL_GetTicks()) % 1000000)
    {
        // Literally does nothing but wait and reset counters
        //lastTick %= 1000000;
    }
    // Now has actually waited the correct amount of time
    FPS = 1000.0 / (SDL_GetTicks() - lastTick); // Store the FPS for later use elsewhere
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

void SDL::saveTextureToFile(std::string filepath, SDL_Texture* texture)
{
    // Saves a texture to file as a png.
    //Source: https://stackoverflow.com/questions/34255820/save-sdl-texture-to-file
    SDL_Texture* target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, texture);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, filepath.c_str());
    SDL_FreeSurface(surface);
    SDL_SetRenderTarget(renderer, target);
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

inline Uint32 getPixel(SDL_Surface *surface, int x, int y)
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

// Circle drawing function ( credit: https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl )
inline void SDL_DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

// Ellipse drawing function ( credit: https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl )
inline void SDL_DrawEllipse(SDL_Renderer* r, int x0, int y0, int radiusX, int radiusY)
{
    float pi  = 3.14159265358979323846264338327950288419716939937510;
    float pih = pi / 2.0; //half of pi

    //drew  28 lines with   4x4  circle with precision of 150 0ms
    //drew 132 lines with  25x14 circle with precision of 150 0ms
    //drew 152 lines with 100x50 circle with precision of 150 3ms
    const int prec = 27; // precision value; value of 1 will draw a diamond, 27 makes pretty smooth circles.
    float theta = 0;     // angle that will be increased each loop

    //starting point
    int x  = (float)radiusX * cos(theta);//start point
    int y  = (float)radiusY * sin(theta);//start point
    int x1 = x;
    int y1 = y;

    //repeat until theta >= 90;
    float step = pih/(float)prec; // amount to add to theta each time (degrees)
    for(theta=step;  theta <= pih;  theta+=step)//step through only a 90 arc (1 quadrant)
    {
        //get new point location
        x1 = (float)radiusX * cosf(theta) + 0.5; //new point (+.5 is a quick rounding method)
        y1 = (float)radiusY * sinf(theta) + 0.5; //new point (+.5 is a quick rounding method)

        //draw line from previous point to new point, ONLY if point incremented
        if( (x != x1) || (y != y1) )//only draw if coordinate changed
        {
            SDL_RenderDrawLine(r, x0 + x, y0 - y,    x0 + x1, y0 - y1 );//quadrant TR
            SDL_RenderDrawLine(r, x0 - x, y0 - y,    x0 - x1, y0 - y1 );//quadrant TL
            SDL_RenderDrawLine(r, x0 - x, y0 + y,    x0 - x1, y0 + y1 );//quadrant BL
            SDL_RenderDrawLine(r, x0 + x, y0 + y,    x0 + x1, y0 + y1 );//quadrant BR
        }
        //save previous points
        x = x1;//save new previous point
        y = y1;//save new previous point
    }
    //arc did not finish because of rounding, so finish the arc
    if(x!=0)
    {
        x=0;
        SDL_RenderDrawLine(r, x0 + x, y0 - y,    x0 + x1, y0 - y1 );//quadrant TR
        SDL_RenderDrawLine(r, x0 - x, y0 - y,    x0 - x1, y0 - y1 );//quadrant TL
        SDL_RenderDrawLine(r, x0 - x, y0 + y,    x0 - x1, y0 + y1 );//quadrant BL
        SDL_RenderDrawLine(r, x0 + x, y0 + y,    x0 + x1, y0 + y1 );//quadrant BR
    }
}

#endif // SDL_WRAPPER_H_INCLUDED
