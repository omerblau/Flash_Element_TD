#pragma once
#include <SDL3/SDL.h>
#include "res/atlas.h"

namespace element {
    /// components
    // @formatter:off              //  components
    using Transform = struct {SDL_FPoint p; float a;};
    using Drawable = struct {SDL_FRect part; SDL_FPoint size;};
    using Gold = struct {int current;};

    using HP = struct {int current; int initial;};
    using Gold_Bounty = struct {int value;};
    using Speed = struct {float value;};
    using Velocity  = struct { SDL_FPoint v; };       // vector per second
    using WaypointIndex = struct { int idx; };        // next target in TURNS[]

    using Creep_Tag = struct {};
    using Player_Tag = struct {};
    // @formatter:on               //  ← formatter back on


    class Element {
    public:
        Element();

        ~Element();

        void run(); // main loop


        static constexpr float MAP_TEX_PAD_X = 20.0f;
        static constexpr float MAP_TEX_PAD_Y = 20.0f;
        static constexpr float TEX_SCALE = 1.8f;

    private:
        void draw_system() const;
        void path_navigation_system() const;   // sets Velocity + WaypointIndex
        void movement_system() const;          // applies Velocity to Transform
        void endpoint_system() const;

        bool prepareWindowAndTexture();

        void createMap() const;
        void createPlayer() const;
        void createSheep() const;

        static constexpr int WIN_WIDTH = 1280;
        static constexpr int WIN_HEIGHT = 800;

        static constexpr int FPS = 60;
        static constexpr float DT = 1.f / FPS;      // seconds per logic step (0.016 666…)
        static constexpr float GAME_FRAME = 1000.f / FPS;
        static constexpr float RAD_TO_DEG = 57.2958f;

        static constexpr float PAD = 0.f;
        static constexpr SDL_FRect SHEEP_TEX = {
            sprite_1.x + PAD + 5, sprite_1.y + PAD, sprite_1.w - 2 * PAD, sprite_1.h - 2 * PAD
        };
        static constexpr SDL_FRect MAP_TEX = {sprite_map.x, sprite_map.y, sprite_map.w, sprite_map.h};

        SDL_Window *win = nullptr;
        SDL_Renderer *ren = nullptr;
        SDL_Texture *tex = nullptr;
    };

    // -----------------------------------------------------------------------------
    // Way-point helpers (screen coordinates, macro-free)
    // -----------------------------------------------------------------------------

    // Convert map-pixel coords to on-screen pixels (includes padding + scale)
    constexpr SDL_FPoint toScreen(float mapX, float mapY) {
        return {
            Element::MAP_TEX_PAD_X + mapX * Element::TEX_SCALE,
            Element::MAP_TEX_PAD_Y + mapY * Element::TEX_SCALE
        };
    }

    // Create a TurnPt from map coords + direction
    struct TurnPt {
        float x, y;
        char dir;
    };

    constexpr TurnPt TP(float mapX, float mapY, char dir) {
        SDL_FPoint s = toScreen(mapX, mapY);
        return {s.x, s.y, dir};
    }

    // Way-points for the sheep path // @formatter:off
    constexpr TurnPt TURNS[] = {
        TP(125,  1, 's'),
        TP(125, 50, 'w'),
        TP( 48, 53, 's'),
        TP( 45,190, 'e'),
        TP(125,190, 'n'),
        TP(128,131, 'e'),
        TP(244,131, 's'),
        TP(247,271, 'w'),
        TP( 48,273, 's'),
        TP( 48,352, 'e'),
        TP(325,352, 'n'),
        TP(324, 52, 'w'),
        TP(204, 50, 'n'),
        TP(204,  1, 's')
    };    // @formatter:on

    constexpr int TURN_COUNT = sizeof(TURNS) / sizeof(TURNS[0]);

    // -----------------------------------------------------------------------------
    // (end waypoint helpers)
    // -----------------------------------------------------------------------------
}
