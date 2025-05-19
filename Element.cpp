#include "Element.h"
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

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
        const float w  = MAP_TEX.w * TEX_SCALE;
        const float h  = MAP_TEX.h * TEX_SCALE;
        const float cx = MAP_TEX_PAD_X + w * 0.5f;   // centre-x
        const float cy = MAP_TEX_PAD_Y + h * 0.5f;   // centre-y

        Entity mapEntity = Entity::create();
        mapEntity.addAll(
            Transform{{cx, cy},0.f},
            Drawable{MAP_TEX, {MAP_TEX.w * TEX_SCALE, MAP_TEX.h * TEX_SCALE}}
        );
    }


    void Element::draw_system() const {
        static const Mask mask = MaskBuilder()
                .set<Transform>() // where to draw
                .set<Drawable>() // what to draw
                .build();

        SDL_RenderClear(ren);

        for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
            if (World::mask(e).test(mask)) {
                const auto& d = World::getComponent<Drawable>(e);
                const auto& t = World::getComponent<Transform>(e);

                const SDL_FRect dst = {
                    t.p.x-d.size.x/2,
                    t.p.y-d.size.y/2,
                    d.size.x, d.size.y};

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
