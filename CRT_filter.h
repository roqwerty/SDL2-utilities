#ifndef CRT_FILTER_H_INCLUDED
#define CRT_FILTER_H_INCLUDED

#include <SDL2/SDL.h> // For SDL...
#include <cstdlib> // For abs() function

/*
This file allows the modification of a texture passed through it to more closely mimic a CRT monitor.
Specifically, this file bends the edges of the image to appear like a curved screen
FUTURE can also handle plaintext inputs and colorization and pixel light bloom
*/

namespace CRT
{
    SDL_Texture* lastBentTexture = nullptr; // This stores location of last bent texture, for reasons

    // NOTE For best results, set to the same as the SDL instance in SDL_wrapper
    int WIDTH = 1280;
    int HEIGHT = 800; // TEMP width and height declaration

    SDL_Point bendPoint(SDL_Point sourcePoint)
    {
        // Best used for bending the four corners of a button, for example
        // TEMP just copy-pasted from renderBend function. FUTURE can pass all renderBend operations through something like this?
        int bendX = 0;
        int bendY = 0;
        int centerX = WIDTH / 2;
        int centerY = HEIGHT / 2;
        int denominator = WIDTH * HEIGHT / ( ( (WIDTH + HEIGHT) / 32 ) );
        bendX = ( abs(centerY - sourcePoint.y) * abs(centerY - sourcePoint.y) ) / denominator;
        bendY = ( abs(centerX - sourcePoint.x) * abs(centerX - sourcePoint.x) ) / denominator;

        // Move pixel towards center
        // x
        if (sourcePoint.x < centerX)
        {
            sourcePoint.x += bendX;
        }
        else
        {
            sourcePoint.x -= bendX;
        }
        // y
        if (sourcePoint.y < centerY)
        {
            sourcePoint.y += bendY;
        }
        else
        {
            sourcePoint.y -= bendY;
        }

        return sourcePoint;
    }

    void renderBend(SDL_Renderer* renderer, SDL_Texture* sourceTexture)
    {
        // "Bends" the source texture according to CRT things and renders to the full size of renderer
        // FUTURE add source and destination rects?
        /// TAKES FOREVER...... FUTURE: Store all the bend pixel amounts as points in a massive vector? Might improve speed at cost of like 3MB of memory...
        SDL_DestroyTexture(lastBentTexture);
        lastBentTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
        SDL_SetRenderTarget(renderer, lastBentTexture);

        // Iterate through the pixels and bend to texture
        //int bendNum = 0; // Used in calculating extent of bend
        int bendX = 0;
        int bendY = 0;
        int centerX = WIDTH / 2;
        int centerY = HEIGHT / 2;
        int denominator = WIDTH * HEIGHT / ( ( (WIDTH + HEIGHT) / 32 ) );
        //int denominator = WIDTH * HEIGHT * 64;
        //int denominator = 1280;

        // Used to move a single pixel at a time
        SDL_Rect source_rect;
        source_rect.w = 1;
        source_rect.h = 1;
        source_rect.x = 0;
        source_rect.y = 0;
        SDL_Rect destination_rect;
        destination_rect.w = 1;
        destination_rect.h = 1;

        for (int y = 0; y < HEIGHT; y++)
        {
            source_rect.y = y;

            for (int x = 0; x < WIDTH; x++)
            {
                source_rect.x = x;

                /// Do the bend algorithm
                // Get the bendnum
                //bendNum = ( abs(centerX - x) * abs(centerY - y) ) / ( WIDTH * 4 );
                //bendNum = ( ( abs(centerX - x) * abs(centerX - x) ) + ( abs(centerY - y) * abs(centerY - y) ) ) / ( WIDTH * HEIGHT * .0125 );
                //bendNum = ( ( abs(centerX - x) * abs(centerX - x) ) + ( abs(centerY - y) * abs(centerY - y) ) ) / denominator; /// MAIN
                //bendNum = ( ( abs(centerX - x) * abs(centerX - x) ) * ( abs(centerY - y) * abs(centerY - y) ) ) / denominator; // WEIRD INVERSE CURVE
                // Make depend on whichever is lesser: distance from centerX or centerY
                //bendNum = ( std::min( abs(centerX - x), abs(centerY - y) ) * std::min( abs(centerX - x), abs(centerY - y) ) ) / denominator; // Middle Bubble
                bendX = ( abs(centerY - y) * abs(centerY - y) ) / denominator;
                bendY = ( abs(centerX - x) * abs(centerX - x) ) / denominator;
                /// FUTURE make overlap pixels less absolutely annihilate everything they touch
                // Move pixel the bendnum towards the center
                // x
                if (x < centerX)
                {
                    //destination_rect.x = x + bendNum;
                    destination_rect.x = x + bendX;
                }
                else
                {
                    //destination_rect.x = x - bendNum;
                    destination_rect.x = x - bendX;
                }
                // y
                if (y < centerY)
                {
                    //destination_rect.y = y + bendNum;
                    destination_rect.y = y + bendY;
                }
                else
                {
                    //destination_rect.y = y - bendNum;
                    destination_rect.y = y - bendY;
                }
                // Render the pixel to the bent texture
                SDL_RenderCopy(renderer, sourceTexture, &source_rect, &destination_rect);
            }
        }

        // Reset the render target
        SDL_SetRenderTarget(renderer, NULL);

        // Render to the render target
        SDL_SetTextureBlendMode(lastBentTexture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, lastBentTexture, NULL, NULL);
    }

    SDL_Texture* chromaticAberration(SDL_Renderer* renderer, SDL_Texture* sourceTexture, bool destructive = false)
    {
        // Applies subtle chromatic aberration to a texture (one blue pixel left, one red pixel right)
        // Returns the new texture
        // If destructive, also destroys the old source texture

        // Make a rect of the needed size (+2 pixel width, 1 for blue and 1 for red)
        SDL_Rect rect = {0, 0, 0, 0};
        SDL_QueryTexture(sourceTexture, NULL, NULL, &rect.w, &rect.h);
        rect.w += 2;

        // Make a new texture of the needed width (+2 pixel width, 1 for blue and 1 for red)
        SDL_Texture* holdTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
        SDL_SetRenderTarget(renderer, holdTexture);
        SDL_SetTextureBlendMode(holdTexture, SDL_BLENDMODE_BLEND);

        // Use the existing rect to draw 3 layers of CA (left blue, right red, then center normal)
        rect.w -= 2;
        // Blue
        //rext.x = 0; // Is already 0...
        SDL_SetTextureColorMod(sourceTexture, 0, 0, 255); // For color modulation!
        SDL_RenderCopy(renderer, sourceTexture, NULL, &rect);
        // Red
        rect.x = 2;
        SDL_SetTextureColorMod(sourceTexture, 255, 0, 0); // For color modulation!
        SDL_RenderCopy(renderer, sourceTexture, NULL, &rect);
        // Normal
        rect.x = 1;
        SDL_SetTextureColorMod(sourceTexture, 255, 255, 255); // For color modulation!
        SDL_RenderCopy(renderer, sourceTexture, NULL, &rect);

        // Cleanup
        SDL_SetRenderTarget(renderer, NULL);
        if (destructive)
        {
            SDL_DestroyTexture(sourceTexture);
        }
        return holdTexture;
    }

    SDL_Texture* addScanLines(SDL_Renderer* renderer, SDL_Texture* sourceTexture, int scanSpacing, bool destructive = false)
    {
        // Duplicate the passed texture
        SDL_Rect rect = {0, 0, 0, 0};
        SDL_QueryTexture(sourceTexture, NULL, NULL, &rect.w, &rect.h);
        SDL_Texture* holdTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);
        SDL_SetRenderTarget(renderer, holdTexture);
        SDL_SetTextureBlendMode(holdTexture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, sourceTexture, NULL, NULL);

        // Add scanlines (50% visibility) every scanSpacingrd line
        int iterations = rect.h;
        rect.h = 1;
        SDL_SetTextureColorMod(sourceTexture, 127, 127, 127);
        for (int i = 0; i < iterations; i++)
        {
            if (i % scanSpacing == 0)
            {
                rect.y = i;
                SDL_RenderCopy(renderer, sourceTexture, &rect, &rect);
            }
        }

        // Reset and return
        SDL_SetRenderTarget(renderer, NULL);
        if (destructive)
        {
            SDL_DestroyTexture(sourceTexture);
        }

        return holdTexture;
    }

    void reset()
    {
        // Does all the good cleanup/reset things
        SDL_DestroyTexture(lastBentTexture);
        lastBentTexture = nullptr;
    }
}

#endif // CRT_FILTER_H_INCLUDED
