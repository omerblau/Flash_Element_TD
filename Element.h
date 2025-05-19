#pragma once
#include <SDL3/SDL.h>
#include "res/atlas.h"

namespace element {
    /// components
    using Transform = struct {SDL_FPoint p;float a;};
    using Drawable = struct {SDL_FRect part; SDL_FPoint size;};
    using Player_Tag = struct {};
    using HP = struct {int current; int initial;};
    using Gold = struct {int current;};

    class Element {
    public:
        Element();
        ~Element();

        void run(); // main loop
    private:
        void draw_system() const;

        bool prepareWindowAndTexture();

        void createMap() const;


        static constexpr int WIN_WIDTH = 1280;
        static constexpr int WIN_HEIGHT = 800;
        static constexpr int FPS = 60;

        static constexpr float GAME_FRAME = 1000.f / FPS;
        static constexpr float RAD_TO_DEG = 57.2958f;

        static constexpr float TEX_SCALE = 1.5f;
        static constexpr float MAP_TEX_PAD_X = 20.0f;
        static constexpr float MAP_TEX_PAD_Y = 20.0f;
        static constexpr SDL_FRect MAP_TEX = {sprite_map.x, sprite_map.y, sprite_map.w, sprite_map.h};


        SDL_Window *win = nullptr;
        SDL_Renderer *ren = nullptr;
        SDL_Texture *tex = nullptr;
    };
}
