#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "SDL_wrapper.h"

/*
This header provides limited animation functionality for SDL Textures
*/

struct vec2
{
    float x;
    float y;
};

enum AnimationType
{
    ANIMATION_NONE,
    ANIMATION_SPRITESHEET,  // Animates the texture from a spritesheet
    ANIMATION_MOVE,         // Moves the rect from one location to another smoothly
    ANIMATION_SCALE,        // Scales the rect smoothly
    ANIMATION_FADE          // Fades the texture completely in or completely out
};

struct Animation {           // A collection of data about any animation
    AnimationType type = ANIMATION_NONE; // The type of this animation
    bool complete = false; // Whether or not this animation is complete
    bool queued = false; // If true, will wait until this animation is fully complete to start next animation in chain
    union {
        struct {                    // SpriteSheet
            SDL_Texture* sheet;     // Pointer to the entire spritesheet texture. NOTE assumes texture is arranged horizontally
            SDL_Rect frameRect;     // Rect containing current texture of spritesheet. NOTE this width MUST be the exact spacing between frames as well
            uint8_t ticksPerFrame;  // How many ticks to keep any given animation frame
            uint8_t currentTicks;   // Used as incrementor to keep track of ticks
            int currentFrame;       // Used to keep track of the current frame
            int totalFrames;        // The number of frames of the animation
            bool loops;             // Whether or not the animation loops forever or will complete
        };
        struct {                    // Move
            vec2 currentPos;        // The detailed position of the current rect
            vec2 targetPos;         // Where the rect is trying to move to
            vec2 deltaPos;          // How far the rect will move in each direction each update
        };
        struct {                    // Scale
            vec2 currentScale;      // The current, detailed x,y size of the rect
            vec2 targetScale;       // The scale the animation is trying to reach
            vec2 deltaScale;        // How much the scale changes in x and y each tick
        };
        struct {                    // Fade
            float currentFade;      // The current alpha value of the fade
            float deltaFade;        // How much this fade changes every tick
        };
    };
};

Animation anim_spritesheet(SDL_Texture* spritesheet, int framewidth, int frameheight, uint8_t ticksPerFrame, bool loops, bool queued = false)
{
    // Makes and returns a spritesheet anim struct
    Animation d;
    d.type = ANIMATION_SPRITESHEET;
    d.sheet = spritesheet;
    d.frameRect = {0, 0, framewidth, frameheight};
    d.ticksPerFrame = ticksPerFrame;
    d.currentTicks = 0;
    d.currentFrame = 0;
    int sheetWidth = 0;
    SDL_QueryTexture(spritesheet, nullptr, nullptr, &sheetWidth, nullptr);
    d.totalFrames = sheetWidth / framewidth;
    d.loops = loops;
    d.queued = queued;
    return d;
}

Animation anim_move(vec2 startPos, vec2 targetPos, float fractionPerTick, bool queued = false)
{
    // Makes and returns a move anim struct
    Animation d;
    d.type = ANIMATION_MOVE;
    d.currentPos = startPos;
    d.targetPos = targetPos;
    d.deltaPos.x = (targetPos.x - startPos.x) * fractionPerTick;
    d.deltaPos.y = (targetPos.y - startPos.y) * fractionPerTick;
    d.queued = queued;
    return d;
}

Animation anim_scale(vec2 startDimensions, vec2 targetDimensions, float fractionPerTick, bool queued = false)
{
    // Makes and returns a scale anim struct
    Animation d;
    d.type = ANIMATION_SCALE;
    d.currentScale = startDimensions;
    d.targetScale = targetDimensions;
    d.deltaScale.x = (targetDimensions.x - startDimensions.x) * fractionPerTick;
    d.deltaScale.y = (targetDimensions.y - startDimensions.y) * fractionPerTick;
    d.queued = queued;
    return d;
}

Animation anim_fade(float startAlpha, float deltaAlpha, bool queued = false)
{
    // Makes and returns a fade anim struct. NOTE that alphas ar egiven in range [0.0, 1.0]
    Animation d;
    d.type = ANIMATION_FADE;
    d.currentFade = startAlpha * 255.0;
    d.deltaFade = deltaAlpha * 255.0;
    d.queued = queued;
    return d;
}

/// /// ///

class Animatic
{
    // A wrapper for a series of animations, as well as a texture and position (rect)
public:
    // Functions
    Animatic(SDL* SDLInstance, SDL_Texture* Texture); // Constructor
    void addAnimation(const Animation& anim); // Adds an animation given a bunch of data
    void update(); // Ticks all of the animations
    // Variables
    SDL_Texture* texture = nullptr; // The texture as the animation system sees it
    SDL_Rect rect = {0, 0, 0, 0}; // The position and size as the animation system sees it
    //bool purgeOnComplete = true; // Whether or not this Animatic is destroyed after all animations finish
    std::vector<Animation> animations; // The collection of all animations currently playing on this Animatic
private:
    // Functions
    // Variables
    SDL* sdl; // SDL instance
};

Animatic::Animatic(SDL* SDLInstance, SDL_Texture* Texture)
{
    sdl = SDLInstance;
    texture = Texture;
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
}

void Animatic::addAnimation(const Animation& anim)
{
    // Adds an animation given a bunch of data
    animations.push_back(anim);
}

void Animatic::update()
{
    // Ticks all of the animations
    for (auto anim = animations.begin(); anim != animations.end(); )
    {
        if (anim->complete) {anim = animations.erase(anim);} // Erase the animation if complete
        else
        {
            // Do animation things
            switch(anim->type) {
            case ANIMATION_SPRITESHEET:
                anim->currentTicks++;
                if (anim->currentTicks >= anim->ticksPerFrame) {
                    anim->currentTicks = 0;
                    anim->currentFrame++;
                    if (anim->currentFrame >= anim->totalFrames) {
                        anim->currentFrame = 0;
                        if (!anim->loops) {anim->complete = true;}
                    }
                    anim->frameRect.x = anim->frameRect.w * anim->currentFrame;
                    SDL_Texture* originalTexture = SDL_GetRenderTarget(sdl->renderer);
                    SDL_SetRenderTarget(sdl->renderer, texture);
                    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 0);
                    SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_NONE);
                    SDL_RenderClear(sdl->renderer);
                    SDL_RenderCopy(sdl->renderer, anim->sheet, &anim->frameRect, nullptr);
                    SDL_SetRenderTarget(sdl->renderer, originalTexture);
                }
                break;
            case ANIMATION_MOVE:
                anim->currentPos.x += anim->deltaPos.x;
                anim->currentPos.y += anim->deltaPos.y;
                if (anim->currentPos.x > anim->targetPos.x && anim->deltaPos.x >= 0.0) {anim->currentPos.x = anim->targetPos.x;}
                else if (anim->currentPos.x < anim->targetPos.x && anim->deltaPos.x < 0.0) {anim->currentPos.x = anim->targetPos.x;}
                if (anim->currentPos.y > anim->targetPos.y && anim->deltaPos.y >= 0.0) {anim->currentPos.y = anim->targetPos.y;}
                else if (anim->currentPos.y < anim->targetPos.y && anim->deltaPos.y < 0.0) {anim->currentPos.y = anim->targetPos.y;}
                if (anim->currentPos.x == anim->targetPos.x && anim->currentPos.y == anim->targetPos.y) {
                    anim->complete = true;
                }
                rect.x = anim->currentPos.x;
                rect.y = anim->currentPos.y;
                break;
            case ANIMATION_SCALE:
                anim->currentScale.x += anim->deltaScale.x;
                anim->currentScale.y += anim->deltaScale.y;
                if (anim->currentScale.x > anim->targetScale.x && anim->deltaScale.x >= 0.0) {anim->currentScale.x = anim->targetScale.x;}
                else if (anim->currentScale.x < anim->targetScale.x && anim->deltaScale.x < 0.0) {anim->currentScale.x = anim->targetScale.x;}
                if (anim->currentScale.y > anim->targetScale.y && anim->deltaScale.y >= 0.0) {anim->currentScale.y = anim->targetScale.y;}
                else if (anim->currentScale.y < anim->targetScale.y && anim->deltaScale.y < 0.0) {anim->currentScale.y = anim->targetScale.y;}
                if (anim->currentScale.x == anim->targetScale.x && anim->currentScale.y == anim->targetScale.y) {
                    anim->complete = true;
                }
                rect.w = anim->currentScale.x;
                rect.h = anim->currentScale.y;
                break;
            case ANIMATION_FADE:
                anim->currentFade += anim->deltaFade;
                if (anim->currentFade <= 0.0 || anim->currentFade >= 255.0) {
                    anim->complete = true;
                }
                SDL_SetTextureAlphaMod(texture, (uint8_t)anim->currentFade);
                break;
            default:
                break;
            }

            if (anim->queued) {return;} // If this is queued animation, skip all the rest
            ++anim; // Increment animation count
        }
    }
}

/*/// /// ///

class AnimaticWrapper
{
    // A wrapper for a set of Animatics, handling creation/destruction as well as batch updating
public:
    // Functions
    AnimaticWrapper(SDL_Renderer* Renderer); // Constructor
    // Variables
private:
    // Functions
    // Variables
    SDL_Renderer* renderer = nullptr; // Pointer to the current SDL renderer
};

AnimaticWrapper::AnimaticWrapper(SDL_Renderer* Renderer)
{
    renderer = Renderer;
}*/