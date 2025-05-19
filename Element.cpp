#include "Element.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

// #define DEBUG_DRAW   // ← uncomment while tuning padding


using namespace std;

#include "bagel.h"
using namespace bagel;

namespace element {
    bool Element::prepareWindowAndTexture() {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            cout << SDL_GetError() << endl;
            return false;
        }

        if (!SDL_CreateWindowAndRenderer(
            "Flash Element Tower Defence", WIN_WIDTH, WIN_HEIGHT, 0, &win, &ren)) {
            cout << SDL_GetError() << endl;
            return false;
        }
        SDL_Surface *surf = IMG_Load("res/atlas.png");
        if (surf == nullptr) {
            cout << SDL_GetError() << endl;
            return false;
        }

        tex = SDL_CreateTextureFromSurface(ren, surf);
        if (tex == nullptr) {
            cout << SDL_GetError() << endl;
            return false;
        }
        SDL_DestroySurface(surf);
        return true;
    }

    void Element::createMap() const {
        const float w = MAP_TEX.w * TEX_SCALE;
        const float h = MAP_TEX.h * TEX_SCALE;
        const float cx = MAP_TEX_PAD_X + w * 0.5f; // centre-x
        const float cy = MAP_TEX_PAD_Y + h * 0.5f; // centre-y

        Entity mapEntity = Entity::create();
        mapEntity.addAll(
            Transform{{cx, cy}, 0.f},
            Drawable{MAP_TEX, {MAP_TEX.w * TEX_SCALE, MAP_TEX.h * TEX_SCALE}}
        );
    }

    void Element::createSheep() const {
        Entity sheepEntity = Entity::create();
        sheepEntity.addAll(
            Transform{{TURNS[0].x, TURNS[0].y}, 90.f},
            Drawable{SHEEP_TEX, {SHEEP_TEX.w * TEX_SCALE, SHEEP_TEX.h * TEX_SCALE}},
            WaypointIndex{1}, // head to waypoint #1
            Velocity{{0.f, 0.f}},
            Speed{20.f},
            HP{10, 10},
            Gold_Bounty{2},
            Creep_Tag{}
        );
    }

    void Element::createPlayer() const {
        Entity playerEntity = Entity::create();
        playerEntity.addAll(
            HP{20, 20},
            Gold{100},
            Player_Tag{}
        );
    }

    void Element::path_navigation_system() const {
    }

    void Element::movement_system() const {
    }


    void Element::draw_system() const {
        static const Mask mask = MaskBuilder()
                .set<Transform>() // where to draw
                .set<Drawable>() // what to draw
                .build();

        SDL_RenderClear(ren);

        for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
            if (World::mask(e).test(mask)) {
                const auto &d = World::getComponent<Drawable>(e);
                const auto &t = World::getComponent<Transform>(e);

                const SDL_FRect dst = {
                    t.p.x - d.size.x / 2,
                    t.p.y - d.size.y / 2,
                    d.size.x, d.size.y
                };

                /* ----------  DEBUG PADDING OVERLAY  ---------- */
#ifdef DEBUG_DRAW
                // solid magenta backdrop
                SDL_SetRenderDrawColor(ren, 255, 0, 255, 255);
                SDL_RenderFillRect(ren, &dst);

                // 1-px green outline
                SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
                SDL_RenderRect(ren, &dst);
#endif
                /* --------------------------------------------- */

                SDL_RenderTextureRotated(
                    ren, tex, &d.part, &dst, t.a,
                    nullptr, SDL_FLIP_NONE);
            }
        }

        SDL_RenderPresent(ren);
    }


    Element::Element() {
        if (!prepareWindowAndTexture())
            return;
        SDL_srand(time(nullptr));

        createMap();
        createPlayer();
        createSheep();
    }

    Element::~Element() {
        if (tex != nullptr)
            SDL_DestroyTexture(tex);
        if (ren != nullptr)
            SDL_DestroyRenderer(ren);
        if (win != nullptr)
            SDL_DestroyWindow(win);

        SDL_Quit();
    }

    void Element::run() {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        auto start = SDL_GetTicks();
        bool quit = false;

        while (!quit) {
            // input_system();
            // move_system();
            // box_system();
            // score_system();
            //
            draw_system();

            auto end = SDL_GetTicks();
            if (end - start < GAME_FRAME) {
                SDL_Delay(GAME_FRAME - (end - start));
            }
            start += GAME_FRAME;

            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_EVENT_QUIT)
                    quit = true;
                else if ((e.type == SDL_EVENT_KEY_DOWN) && (e.key.scancode == SDL_SCANCODE_ESCAPE))
                    quit = true;
            }
        }
    }
}
