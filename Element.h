#pragma once
#include <SDL3/SDL.h>
#include "res/atlas.h"

namespace element {
    enum class UIAction {
        None, BuyArrow, BuyCannon, BuyAir,
        NextLevel,
    };

    /// components
    // @formatter:off              //  components
    using Transform = struct {SDL_FPoint p; float a;};
    using Drawable = struct {SDL_FRect part; SDL_FPoint size;};
    using Gold = struct {int current;};
    using HP = struct {int current; int initial;};
    using Gold_Bounty = struct {int value;};
    using Speed = struct {float value;};
    using Velocity  = struct { SDL_FPoint v; };          // vector per second
    using WaypointIndex = struct { int idx; };          // next target in TURNS[]
    using CurrentLevel = struct { int level; };         // which entry in WAVES[]
    using SpawnState = struct {int waveIndex; int remaining; float timeLeft;};
    using MouseInput = struct {int x; int y; bool clicked;};
    using UIIntent = struct {UIAction action = UIAction::None;};
    using Range = struct {float value;};
    using Damage = struct {int value;};
    using FireRate = struct {float interval; float timeLeft;};
    using Target = struct {int id;};

    /// Tags
    using Creep_Tag = struct {};
    using Player_Tag = struct {};
    using Mouse_Tag = struct {};
    using UIButton_Tag = struct {};
    using Arrow_Tag = struct {};
    using Cannon_Tag = struct {};
    using Air_Tag = struct {};
    using NextLevel_Tag = struct {};
    using GameState_Tag = struct {};
    using SpawnManager_Tag = struct {};

    class Element {
    public:
        Element();
        ~Element();

        [[noreturn]] void run(); // main loop

        static constexpr float MAP_TEX_PAD_X = 20.0f;
        static constexpr float MAP_TEX_PAD_Y = 20.0f;
        static constexpr float TEX_SCALE = 1.8f;
        static constexpr float PAD = 0.f;

        static constexpr SDL_FRect SHEEP_TEX = {
            sprite_1.x + PAD + 5, sprite_1.y + PAD, sprite_1.w - 2 * PAD, sprite_1.h - 2 * PAD
        };
        static constexpr SDL_FRect RABID_TEX = {
            sprite_2.x + PAD + 5, sprite_2.y + PAD, sprite_2.w - 2 * PAD, sprite_2.h - 2 * PAD
        };

    private:
        /// init helpers
        bool prepareWindowAndTexture();

        void createMap() const;
        void createBuyArrow() const;
        void createBuyCannon() const;
        void createBuyAir() const;
        void createNextLevelButton() const;
        void createUI() const;

        void createMouse() const;
        void createPlayer() const;
        void createGameState() const;

        void createSpawnManager() const;
        void createCreep(float speed, int hp, int goldBounty, SDL_FRect spriteRect) const;
        void createTower(float x, float y, float a, float range, int healthDamage,
                             float fire_rate, SDL_FRect spriteRect) const;

        /// systems
        void input_system();
        void ui_system() const;
        void path_navigation_system() const;// sets Velocity + WaypointIndex
        void movement_system() const;       // applies Velocity to Transform
        void endpoint_system() const;
        void placing_tower_system() const;
        void wave_system() const;
        void print_status_bar() const; //helper for draw
        void draw_system() const;

        static constexpr int WIN_WIDTH = 1280;
        static constexpr int WIN_HEIGHT = 800;

        static constexpr int FPS = 60;
        static constexpr float DT = 1.f / FPS; // seconds per logic step (0.016 666…)
        static constexpr float GAME_FRAME = 1000.f / FPS;
        static constexpr float RAD_TO_DEG = 57.2958f;

        static constexpr SDL_FRect MAP_TEX =        {sprite_map.x, sprite_map.y, sprite_map.w, sprite_map.h};
        static constexpr SDL_FRect UI_NEXT_LEVEL =  {sprite_ui_next_level.x, sprite_ui_next_level.y, sprite_ui_next_level.w, sprite_ui_next_level.h};
        static constexpr SDL_FRect BUY_ARROW_TEX =  {sprite_buy_arrow.x, sprite_buy_arrow.y, sprite_buy_arrow.w, sprite_buy_arrow.h};
        static constexpr SDL_FRect BUY_CANNON_TEX = {sprite_buy_cannon.x, sprite_buy_cannon.y, sprite_buy_cannon.w, sprite_buy_cannon.h};
        static constexpr SDL_FRect BUY_AIR_TEX =    {sprite_buy_air.x, sprite_buy_air.y, sprite_buy_air.w, sprite_buy_air.h};

        static constexpr SDL_FRect SPRITE_HOVER = {sprite_ui_can_place_tower.x, sprite_ui_can_place_tower.y, sprite_ui_can_place_tower.w, sprite_ui_can_place_tower.h};
        static constexpr SDL_FRect SPRITE_HOVER_CANT_PLACE = {sprite_ui_cant_place_tower.x, sprite_ui_cant_place_tower.y, sprite_ui_cant_place_tower.w, sprite_ui_cant_place_tower.h};


        static constexpr SDL_FRect TOWER_TEX_AIR = {
            sprite_tower_air.x , sprite_tower_air.y, sprite_tower_air.w , sprite_tower_air.h
        };
        static constexpr SDL_FRect TOWER_TEX_ARROW = {
            sprite_tower_arrow.x , sprite_tower_arrow.y, sprite_tower_arrow.w , sprite_tower_arrow.h
        };

        static constexpr SDL_FRect TOWER_TEX_CANNON = {
            sprite_tower_cannon_1.x , sprite_tower_cannon_1.y, sprite_tower_cannon_1.w , sprite_tower_cannon_1.h
        };


        SDL_Window *win = nullptr;
        SDL_Renderer *ren = nullptr;
        SDL_Texture *tex = nullptr;
    };  // @formatter:on

    // -----------------------------------------------------------------------------
    // Way-point helpers (screen coordinates, macro-free)
    // -----------------------------------------------------------------------------
    // @formatter:off
    // Convert map-pixel coords to on-screen pixels (includes padding + scale)
    constexpr SDL_FPoint toScreen(float mapX, float mapY) {
        return {Element::MAP_TEX_PAD_X + mapX * Element::TEX_SCALE, Element::MAP_TEX_PAD_Y + mapY * Element::TEX_SCALE};
    }

    // Create a TurnPt from map coords + direction
    struct TurnPt {float x, y; char dir;};

    constexpr TurnPt TP(float mapX, float mapY, char dir) {
        SDL_FPoint s = toScreen(mapX, mapY); return {s.x, s.y, dir};
    }

    // Way-points for the sheep path
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


    ///------------------------------------------------------------------------
    /// Wave config (static data, not ECS components)
    ///------------------------------------------------------------------------
    struct Wave {
        int count; // how many to spawn
        float delay; // seconds between spawns
        float speed; // movement speed
        int hp; // hit points
        int gold; // bounty
        SDL_FRect sprite; // source rect in your atlas
    };

    static constexpr Wave WAVES[] = {
        {10, 0.5f, 100.f, 10, 1, Element::SHEEP_TEX},
        {20, 0.5f, 120.f, 20, 3, Element::RABID_TEX}
    };
    static constexpr int WAVE_COUNT = sizeof(WAVES) / sizeof(WAVES[0]);

    ///------------------------------------------------------------------------
    /// Your ECS components and Element class
    ///------------------------------------------------------------------------
}
