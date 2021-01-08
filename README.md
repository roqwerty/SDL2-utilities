# SDL2-utilities
A small collection of quality-of-life SDL2 libraries.

# File List


### SDL_wrapper
This file defines a master SDL class that handles things such as window creation, fullscreen toggling, pixel querying, easy surface and texture loading, and FPS control.

### SDL_text_wrapper
This file defines a Terminal class that provides console-like text handling and storage, as well as the SDL_Text namespace that lets the user create SDL2 textures from lines or blocks of text using the open-source pixel monogram font as defined in the neighboring monogram.png file.

### CRT_filter
This file creates the CRT namespace, housing a collection of visual filters for SDL2 textures to mimic various effects of a CRT monitor such as screen curvature, chromatic aberration, and scanlines.
>Currently, the CRT screen curvature / bending effect is ridiculously computationally intensive. As such, no stable release exists.
