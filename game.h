// Game.h

#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <iostream>
#include <box2d/box2d.h>
#include "res/atlas.h"
#include <SDL3_image/SDL_image.h>

using namespace std;

constexpr float RAD_TO_DEG = 57.2958f; // 180/π
constexpr float DEG_TO_RAD = 1.f / RAD_TO_DEG; // ≈ 0.01745f
constexpr float SHEEP_SPEED = 30.0f;
static constexpr float TEX_SCALE = 1.5f;

struct TurnPt {
    float x, y;
    char dir;
};

constexpr float PAD_X = 20.0f;
constexpr float PAD_Y = 20.0f;

static constexpr TurnPt TURNS[] = {
#define TP(x, y, d) { PAD_X + (x) * TEX_SCALE, \
PAD_Y + (y) * TEX_SCALE, d }
    TP(125, 1, 's'),
    TP(125, 50, 'w'),
    TP(48, 53, 's'),
    TP(45, 190, 'e'),
    TP(125, 190, 'n'),
    TP(128, 131, 'e'),
    TP(244, 131, 's'),
    TP(247, 271, 'w'),
    TP(48, 273, 's'),
    TP(48, 352, 'e'),
    TP(325, 352, 'n'),
    TP(324, 52, 'w'),
    TP(204, 50, 'n'),
    TP(204, 1, 's')
#undef TP
};

static constexpr int TURN_COUNT = sizeof(TURNS) / sizeof(TURNS[0]);
static constexpr float EPS = 0.5f;

inline void drawDot(SDL_Renderer *r, float x, float y) {
    SDL_FRect dot{x - 2.0f, y - 2.0f, 4.0f, 4.0f}; // 4×4 px
    SDL_RenderFillRect(r, &dot);
}

class Game {
public:
    Game();

    ~Game();

    void stepPhysics(float deltaTime);

    void updateSpriteRect(SDL_FRect &r);

    float aimAt(float tx, float ty);

    float handleWaypoint(float currentAngle);

    void renderFrame(const SDL_FRect &r, const SDL_FRect &mapDst, float angle, const SDL_FRect &buy_air,
                     const SDL_FRect &buy_cannon, const SDL_FRect &buy_arrow);

    void run();

private:

    static constexpr int FPS = 60;
    static constexpr float BOX_SCALE = 10;
    static constexpr SDL_FRect SHEEP = {sprite_1.x+10, sprite_1.y, sprite_1.w-10, sprite_1.h};
    static constexpr SDL_FRect MAP = {sprite_map.x, sprite_map.y, sprite_map.w, sprite_map.h};

    static constexpr SDL_FRect ARROW_BUY = {
        sprite_buy_arrow.x, sprite_buy_arrow.y, sprite_buy_arrow.w, sprite_buy_arrow.h
    };
    static constexpr SDL_FRect CANNON_BUY = {
        sprite_buy_cannon.x, sprite_buy_cannon.y, sprite_buy_cannon.w, sprite_buy_cannon.h
    };
    static constexpr SDL_FRect AIR_BUY = {sprite_buy_air.x, sprite_buy_air.y, sprite_buy_air.w, sprite_buy_air.h};

    SDL_Texture *tex;
    SDL_Renderer *ren;
    SDL_Window *win;

    b2WorldId world;
    b2BodyId ballBody;

    int nextTurn = 0; // index into TURNS
};

#endif // GAME_H
