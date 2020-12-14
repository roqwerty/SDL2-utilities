#ifndef SDL_TEXT_WRAPPER_H_INCLUDED
#define SDL_TEXT_WRAPPER_H_INCLUDED

#include <SDL2/SDL.h> //For SDL...
#include <map> // For maps (dictionaries)
#include "SDL_wrapper_v1.1.h" // For loading / using optimized textures
//#include "Universals.h" // For various things, including points

/*
This file allows writing of texture objects from a string input
*/

/// File should be placed in same directory as included "monogram.png" file!
// Text characters are arranged as follows:
/*
ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghijklmnopqrstuvwxyz
1234567890+-=/\*:;()[]{}<>
!?.,'"&�#%^~�`|�$�����@��_
*/

/// VERSION 0.5
/*
Changelog:
    -0.5-
        Made everything use SDL_Points for, well, points. Removes dependency on old file
        Made writeLine and writeBlock pass strings by reference, because it's better memory management
        writeBlock is now no longer recursive and MUCH faster. Still not super 60FPS fast with large texts, but should be fine for occasional calls (i.e. not every frame)
        fixed Terminal adding a newline to the beginning of its body when printLine() was called after a clear()
        Added SDL_Text functions
            int countLines(const std::string& block) - Returns the total numer of lines the text is (1, +1 for every newline)
            int getMaximumLineLength(const std::string& block) - Returns the length of the longest line in the block
    -Old-
        Terminal class with support for maximum lines, line rewriting, blanking, and more.
            clear() - Clears the text of the terminal
            setText(std::string text) - Sets the text of the terminal to text
            printLine(std::string line) - Adds the line of text to the bottom of the terminal. Erases top line if there is not room
            updateLastLine(std::string line) - Changes the last line of the terminal to be the given text
        SDL_Text namespace with SDL2 texture support. Location of "monogram.png" must be hardcoded
            init(SDL* sdl) - MUST BE CALLED FIRST. This sets up the namespace with the declaration of an SDL elsewhere in the program
            writeLine(std::string text) - Turns a std::string into a SDL_Texture*. Returns the pointer to the texture
            writeBlock(std::string block) - Turns an std::string into a block of SDL_Texture*. Recognizes newlines. As of right now, is recursive and slow
*/

// Set up the map (dictionary) for point lookup
std::map<char, SDL_Point> characterLocation =
{
    // Capitals (Line 0)
    {'A', (SDL_Point){0, 0}},
    {'B', (SDL_Point){1, 0}},
    {'C', (SDL_Point){2, 0}},
    {'D', (SDL_Point){3, 0}},
    {'E', (SDL_Point){4, 0}},
    {'F', (SDL_Point){5, 0}},
    {'G', (SDL_Point){6, 0}},
    {'H', (SDL_Point){7, 0}},
    {'I', (SDL_Point){8, 0}},
    {'J', (SDL_Point){9, 0}},
    {'K', (SDL_Point){10, 0}},
    {'L', (SDL_Point){11, 0}},
    {'M', (SDL_Point){12, 0}},
    {'N', (SDL_Point){13, 0}},
    {'O', (SDL_Point){14, 0}},
    {'P', (SDL_Point){15, 0}},
    {'Q', (SDL_Point){16, 0}},
    {'R', (SDL_Point){17, 0}},
    {'S', (SDL_Point){18, 0}},
    {'T', (SDL_Point){19, 0}},
    {'U', (SDL_Point){20, 0}},
    {'V', (SDL_Point){21, 0}},
    {'W', (SDL_Point){22, 0}},
    {'X', (SDL_Point){23, 0}},
    {'Y', (SDL_Point){24, 0}},
    {'Z', (SDL_Point){25, 0}},
    // Lowercase (Line 1)
    {'a', (SDL_Point){0, 1}},
    {'b', (SDL_Point){1, 1}},
    {'c', (SDL_Point){2, 1}},
    {'d', (SDL_Point){3, 1}},
    {'e', (SDL_Point){4, 1}},
    {'f', (SDL_Point){5, 1}},
    {'g', (SDL_Point){6, 1}},
    {'h', (SDL_Point){7, 1}},
    {'i', (SDL_Point){8, 1}},
    {'j', (SDL_Point){9, 1}},
    {'k', (SDL_Point){10, 1}},
    {'l', (SDL_Point){11, 1}},
    {'m', (SDL_Point){12, 1}},
    {'n', (SDL_Point){13, 1}},
    {'o', (SDL_Point){14, 1}},
    {'p', (SDL_Point){15, 1}},
    {'q', (SDL_Point){16, 1}},
    {'r', (SDL_Point){17, 1}},
    {'s', (SDL_Point){18, 1}},
    {'t', (SDL_Point){19, 1}},
    {'u', (SDL_Point){20, 1}},
    {'v', (SDL_Point){21, 1}},
    {'w', (SDL_Point){22, 1}},
    {'x', (SDL_Point){23, 1}},
    {'y', (SDL_Point){24, 1}},
    {'z', (SDL_Point){25, 1}},
    // Line 2
    {'1', (SDL_Point){0, 2}},
    {'2', (SDL_Point){1, 2}},
    {'3', (SDL_Point){2, 2}},
    {'4', (SDL_Point){3, 2}},
    {'5', (SDL_Point){4, 2}},
    {'6', (SDL_Point){5, 2}},
    {'7', (SDL_Point){6, 2}},
    {'8', (SDL_Point){7, 2}},
    {'9', (SDL_Point){8, 2}},
    {'0', (SDL_Point){9, 2}},
    {'+', (SDL_Point){10, 2}},
    {'-', (SDL_Point){11, 2}},
    {'=', (SDL_Point){12, 2}},
    {'/', (SDL_Point){13, 2}},
    {'\\', (SDL_Point){14, 2}},
    {'*', (SDL_Point){15, 2}},
    {':', (SDL_Point){16, 2}},
    {';', (SDL_Point){17, 2}},
    {'(', (SDL_Point){18, 2}},
    {')', (SDL_Point){19, 2}},
    {'[', (SDL_Point){20, 2}},
    {']', (SDL_Point){21, 2}},
    {'{', (SDL_Point){22, 2}},
    {'}', (SDL_Point){23, 2}},
    {'<', (SDL_Point){24, 2}},
    {'>', (SDL_Point){25, 2}},
    // Line 3
    {'!', (SDL_Point){0, 3}},
    {'?', (SDL_Point){1, 3}},
    {'.', (SDL_Point){2, 3}},
    {',', (SDL_Point){3, 3}},
    {'\'', (SDL_Point){4, 3}},
    {'"', (SDL_Point){5, 3}},
    {'&', (SDL_Point){6, 3}},
    //{'�', (SDL_Point){7, 3}},
    {'#', (SDL_Point){8, 3}},
    {'%', (SDL_Point){9, 3}},
    {'^', (SDL_Point){10, 3}},
    {'~', (SDL_Point){11, 3}},
    //{'�', (SDL_Point){12, 3}},
    {'`', (SDL_Point){13, 3}},
    {'|', (SDL_Point){14, 3}},
    //{'�', (SDL_Point){15, 3}},
    {'$', (SDL_Point){16, 3}},
    //{'�', (SDL_Point){17, 3}},
    //{'�', (SDL_Point){18, 3}},
    //{'�', (SDL_Point){19, 3}},
    //{'�', (SDL_Point){20, 3}},
    //{'�', (SDL_Point){21, 3}},
    {'@', (SDL_Point){22, 3}},
    //{'�', (SDL_Point){23, 3}},
    //{'�', (SDL_Point){24, 3}},
    {'_', (SDL_Point){25, 3}},
};

class Terminal
{
    // Holds some text and when more new lines are added than can be held (in maxLines), deletes the topmost line
public:
    std::string body = "";
    int maxLines = 0;
    int currentlyUsedLines = 0;

    // Functions

    Terminal(int newMaxLines)
    {
        maxLines = newMaxLines;
    }

    void clear()
    {
        body = "";
        currentlyUsedLines = 0;
    }

    void setText(std::string text)
    {
        // Sets the text of the terminal, as well as calculating the amount of used lines
        body = text;
        currentlyUsedLines = 1;
        for (int i = 0; i < text.length(); i++)
        {
            if (text[i] == '\n')
            {
                currentlyUsedLines++;
            }
        }
    }

    void printLine(std::string line)
    {
        // Prints a new line to the terminal. If there are too many lines, removes the topmost one
        if (currentlyUsedLines == 0) // This is the first line, don't add a newline at the top
        {
            body = line;
            currentlyUsedLines = 1;
        }
        else if (currentlyUsedLines < maxLines) // There is open room
        {
            body = body + "\n" + line;
            currentlyUsedLines++;
        }
        else // There is no open room
        {
            // Remove the topmost line from the terminal
            for (int i = 0; i < body.length(); i++)
            {
                // Find the first '\n' in the body and remove everything before it
                if (body[i] == '\n')
                {
                    body = body.substr(i+1) + "\n" + line; // Just returns everything after the first newline
                    break;
                }
            }
        }
    }

    void updateLastLine(std::string line)
    {
        // Changes the last line of the terminal to whatever the passed line is
        int remainingLength = body.length();
        for (int i = body.length(); i > 0; i--)
        {
            // Check to see if the character is a newline
            if (body[i] == '\n')
            {
                body = body.substr(0, remainingLength) + "\n" + line;
                break;
            }
            else
            {
                remainingLength--;
            }
        }
    }
};

namespace SDL_Text
{
    /// Variables
    std::string fontpath = "data/monogram.png";
    SDL_Texture* font = nullptr;
    // Uses the popular and open-source monogram font as a base
    const int CHAR_HEIGHT = 9;
    const int CHAR_WIDTH = 5;
    SDL* sdl = nullptr; // INITIALIZE TO MAIN SDL REFERENCE RENDERER BEFORE ANYTHING ELSE. YES I KNOW IS BAD FORM. This is so functions can be called like print("arg") without also passing renderer each time
    //int lookupX = 0; // Used for locating characters in the master image. Is set instead of the function returning anything
    //int lookupY = 0;

    /// Functions

    void init(SDL* newSDL)
    {
        // Is slightly better form to initialize needed SDL instance
        sdl = newSDL;
        font = sdl->loadTexture(fontpath);
    }

    int countLines(const std::string& block)
    {
        // Counts the number of lines of text in a referenced string
        int count = 1; // One line by default
        for (char c : block)
        {
            if (c == '\n') {count++;}
        }
        return count;
    }

    int getMaximumLineLength(const std::string& block)
    {
        // Returns the length of the longest line in the block of text. Counts spaces and special characters
        int longest = 0;
        int current = 0;
        for (char c : block)
        {
            if (c == '\n')
            {
                if (current > longest) {longest = current;}
                current = 0;
            }
            else
            {
                current++;
            }
        }
        if (current > longest) {longest = current;}
        return longest;
    }

    SDL_Texture* writeLine (const std::string& line)
    {
        // Turns text into a texture. Also adds a pixel of space in between characters
        // Source and destination rectangles
        SDL_Rect sourceRect;
        sourceRect.w = CHAR_WIDTH;
        sourceRect.h = CHAR_HEIGHT;
        sourceRect.x = 0;
        sourceRect.y = 0;
        SDL_Rect destRect = {0, 0, CHAR_WIDTH, CHAR_HEIGHT}; // Equivalent and shorter declaration
        // Make the initial texture of the right size and set target ready to render
        int width = line.length() * CHAR_WIDTH + line.length() - 1; // For space in between chars (1 for each char, minus one after last char)
        SDL_Texture* lineTexture = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, CHAR_HEIGHT);
        SDL_SetRenderTarget(sdl->renderer, lineTexture);
        SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_NONE);
        SDL_SetTextureBlendMode(lineTexture, SDL_BLENDMODE_NONE);
        // Make explicitly all transparent to fix beraking on some gpu / monitor setups
        SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 0);
        SDL_Rect fullRect = {0, 0, width, CHAR_HEIGHT};
        SDL_RenderFillRect(sdl->renderer, &fullRect); // Fills the entire texture with transparent

        // Iterate through the letters, writing to texture
        for (int i = 0; i < line.length(); i++)
        {
            // Update the destination rect
            destRect.x = CHAR_WIDTH * i + i;

            if (characterLocation.find(line[i]) != characterLocation.end()) // If exists in the map
            {
                // Update the source rect
                sourceRect.x = characterLocation[line[i]].x * CHAR_WIDTH;
                sourceRect.y = characterLocation[line[i]].y * CHAR_HEIGHT;
                // Write to destination
                SDL_RenderCopy(sdl->renderer, font, &sourceRect, &destRect);
            } // All unknown characters are treated like spaces!
        }

        // Cleanup and return
        SDL_SetRenderTarget(sdl->renderer, NULL);
        SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureBlendMode(lineTexture, SDL_BLENDMODE_BLEND);
        return lineTexture;
        // NOTE use SDL_QueryTexture to find a rect the same size as this texture!
    }

    SDL_Texture* writeBlock (const std::string& block)
    {
        // Writes a block of text a single line at a time.
        // Uses the newline '\n' character to separate lines

        // Variable init things
        SDL_Rect sourceRect = {0, 0, CHAR_WIDTH, CHAR_HEIGHT}; // The source rectangle for the text
        SDL_Rect destRect = {0, 0, CHAR_WIDTH, CHAR_HEIGHT}; // The destination render rectangle
        // Make a new texture of the needed size
        int width = getMaximumLineLength(block);
        width = width * CHAR_WIDTH + width - 1; // For space in between chars (1 for each char, minus one after last char)
        int height = countLines(block);
        height = height * CHAR_HEIGHT + height - 1; // For space in between rows (1 for each char, minus one after last row)
        SDL_Texture* blockTexture = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        // Setup the texture with all transparent
        SDL_SetRenderTarget(sdl->renderer, blockTexture);
        SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_NONE);
        SDL_SetTextureBlendMode(blockTexture, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 0);
        SDL_RenderClear(sdl->renderer);
        SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255); // Set to white, used to draw boxes when the character is not known
        // Iterate through all of the characters, drawing them to where they need to go
        for (char c : block)
        {
            if (c == '\n')
            {
                // Newline things
                destRect.x = 0;
                destRect.y += CHAR_HEIGHT + 1;
            }
            else if (characterLocation.count(c) != 0) // If exists in map
            {
                // Update the source rect
                sourceRect.x = characterLocation[c].x * CHAR_WIDTH;
                sourceRect.y = characterLocation[c].y * CHAR_HEIGHT;
                // Write to destination
                SDL_RenderCopy(sdl->renderer, font, &sourceRect, &destRect);
                // Move the destRect
                destRect.x += CHAR_WIDTH + 1;
            }
            else if (c == ' ') // Character is a space
            {
                destRect.x += CHAR_WIDTH + 1;
            }
            /*else if (c != ' ') // Character is not known, and not just a space
            {
                // Draw a mystery box
                SDL_RenderDrawRect(sdl->renderer, &destRect);
                // Move the destRect
                destRect.x += CHAR_WIDTH + 1;
            }*/
        }
        // Cleanup and return
        SDL_SetRenderTarget(sdl->renderer, nullptr);
        SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureBlendMode(blockTexture, SDL_BLENDMODE_BLEND);
        return blockTexture;

        /*/// OLD CODE, recursive and slow
        return nullptr;

        // Go through a string until a newline character is encountered
        // Use writeLine to get a texture of the line
        // If there is already a master texture, add a pixel to the bottom and add this line (taking the maximum total width)
        //  if not, make a master texture of the needed size
        //  also delete the old master texture
        // Repeat until all are together (left-aligned, for now...) and return the result

        int lastBreak = 0; // The position the last break occurred
        int lineLength = 0; // The length of the currently parsed line, in chars
        std::string currentLine = ""; // The current line being worked on
        SDL_Texture* masterTexture = nullptr; // The master texture of the block of text
        SDL_Texture* newLineTexture = nullptr; // The texture used for the new line
        SDL_Rect masterRect = {0, 0, 0, 0}; // Used for rect things
        SDL_Rect newTextRect = {0, 0, 0, 0}; // Used for rect things

        for (int i = 0; i < block.length(); i++)
        {
            if (block[i] == '\n' || i == block.length() - 1) // If it's a newline or the last iteration
            {
                // If is last iteration, add 1 to line length for some reason i guess
                if (i == block.length() - 1)
                {
                    lineLength += 1;
                }

                // Subdivide the string
                currentLine = block.substr(lastBreak, lineLength);
                lastBreak = i + 1; // +1 to skip over newline
                lineLength = 0;

                // Get the texture from the string
                newLineTexture = writeLine(currentLine);

                // Update the texture rect
                SDL_QueryTexture(newLineTexture, NULL, NULL, &newTextRect.w, &newTextRect.h);

                // Add the texture to any existing master texture, if exists
                if (masterTexture == nullptr) // There is no existing master texture
                {
                    // Take over the new text texture
                    masterTexture = newLineTexture;
                    newLineTexture = nullptr;
                    // Update the master rect
                    SDL_QueryTexture(masterTexture, NULL, NULL, &masterRect.w, &masterRect.h);
                }
                else // There is already a master texture
                {
                    // Make a new texture that is big enough to hold both of them
                    int newHeight = masterRect.h + 1 + newTextRect.h; // +1 for a space in between the two
                    int newWidth = std::max(masterRect.w, newTextRect.w); // Finds the widest
                    SDL_Texture* newBigTexture = SDL_CreateTexture(sdl->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, newWidth, newHeight);
                    SDL_SetRenderTarget(sdl->renderer, newBigTexture);
                    // Make explicitly all transparent to fix beraking on some gpu / monitor setups
                    SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_NONE);
                    SDL_SetTextureBlendMode(newBigTexture, SDL_BLENDMODE_NONE);
                    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 0);
                    SDL_Rect fillRect = {0, 0, newWidth, newHeight};
                    SDL_RenderFillRect(sdl->renderer, &fillRect);
                    SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_BLEND);
                    SDL_SetTextureBlendMode(newBigTexture, SDL_BLENDMODE_BLEND);

                    // Copy the two textures over
                    // Master texture
                    SDL_SetTextureBlendMode(masterTexture, SDL_BLENDMODE_NONE);
                    SDL_RenderCopy(sdl->renderer, masterTexture, NULL, &masterRect);
                    // New line texture
                    SDL_SetTextureBlendMode(newLineTexture, SDL_BLENDMODE_NONE);
                    newTextRect.y = masterRect.h + 1; // Making sure everything lines up :)
                    SDL_RenderCopy(sdl->renderer, newLineTexture, NULL, &newTextRect);
                    // Reset
                    SDL_SetRenderTarget(sdl->renderer, NULL);

                    // Destroy the old master texture and newline texture and move pointer over
                    SDL_DestroyTexture(masterTexture);
                    SDL_DestroyTexture(newLineTexture);
                    masterTexture = newBigTexture;
                    newBigTexture = nullptr;
                    // Update the master rect
                    SDL_QueryTexture(masterTexture, NULL, NULL, &masterRect.w, &masterRect.h);
                }
            }
            else // Continue on
            {
                lineLength += 1;
            }
        }

        //SDL_SetTextureBlendMode(newLineTexture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureBlendMode(masterTexture, SDL_BLENDMODE_BLEND);
        return masterTexture;*/
    }
}

#endif // SDL_TEXT_WRAPPER_H_INCLUDED
