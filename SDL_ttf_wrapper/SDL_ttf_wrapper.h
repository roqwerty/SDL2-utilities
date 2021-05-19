#pragma once

#include <SDL2/SDL_ttf.h>

class SDL_TTF
{
    // Manages everything for SDL_TTF loading and unloading
public:
    // Functions
    SDL_TTF(std::string fontpath, int fontSize)
    {
        init_ttf();
        font = TTF_OpenFont(fontpath.c_str(), fontSize);
    }
    ~SDL_TTF()
    {
        TTF_CloseFont(font);
        TTF_Quit();
    }
    //Text* write(std::string text)
    // Variables
    TTF_Font* font = nullptr;
private:
    // Functions
    bool init_ttf()
    {
        // Does all of the necessary starting things
        //Initialize SDL_ttf
        if (TTF_Init() == -1)
        {
            std::cout << "Failed to initialize SDL_ttf!" << std::endl;
            return false;    
        }
        return true;
    }
    // Variables
};

class Text
{
    // A very basic wrapper class for single text textures. Make lots
    // FEATURES: only updates when text changes
public:
    // Functions
    Text(TTF_Font* baseFont, const std::string& initialText)
    {
        font = baseFont;
        setText(initialText);
    }
    std::string& getText() {return text;} // So that we can't change it without updating
    void setText(const std::string& newText)
    {
        text = newText;
        SDL_DestroyTexture(texture);
        SDL_Surface* messageS = TTF_RenderText_Solid(font, text.c_str(), textColor);
        renderMutex.lock();
        texture = SDL_CreateTextureFromSurface(sdl.renderer, messageS);
        renderMutex.unlock();
        SDL_FreeSurface(messageS);
        SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    }
    // Variables
    SDL_Texture* texture = nullptr;
    SDL_Color textColor = {255, 255, 255};
    SDL_Rect rect = {0, 0, 0, 0};
    TTF_Font* font = nullptr;
private:
    // Functions
    // Variables
    std::string text = "";
};