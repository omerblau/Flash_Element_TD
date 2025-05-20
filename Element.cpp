#include "Element.h"

#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

// #define DEBUG_DRAW   // ← uncomment while tuning padding


using namespace std;

#include "bagel.h"
using namespace bagel;

namespace element {
    /// init helpers  // @formatter:off
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
        const float cy = MAP_TEX_PAD_Y + h * 0.5f; // centre-y
        const float cx = MAP_TEX_PAD_X + w * 0.5f; // centre-x

        Entity mapEntity = Entity::create();
        mapEntity.addAll(
            Transform{{cx, cy}, 0.f},
            Drawable{MAP_TEX, {MAP_TEX.w * TEX_SCALE, MAP_TEX.h * TEX_SCALE}}
        );
    }
    void Element::createBuyArrow() const {
        Entity buyArrowEntity = Entity::create();
        buyArrowEntity.addAll(
            Transform{{800, 380}, 0.f}, // place at (cx, cy)
            Drawable{BUY_ARROW_TEX, {BUY_ARROW_TEX.w * TEX_SCALE, BUY_ARROW_TEX.h * TEX_SCALE}}, // sprite + size
            Buy_Tag{}, // UI tag
            UIButton_Tag{},
            Arrow_Tag{} // tower‐type tag
        );
    }
    void Element::createBuyCannon() const {
        Entity buyCannonEntity = Entity::create();
        buyCannonEntity.addAll(
            Transform{{880, 380}, 0.f}, // place at (cx, cy)
            Drawable{BUY_CANNON_TEX, {BUY_CANNON_TEX.w * TEX_SCALE, BUY_CANNON_TEX.h * TEX_SCALE}}, // sprite + size
            Buy_Tag{}, // UI tag
            UIButton_Tag{},
            Cannon_Tag{} // tower‐type tag
        );
    }
    void Element::createBuyAir() const {
        Entity buyAirEntity = Entity::create();
        buyAirEntity.addAll(
            Transform{{960, 380}, 0.f}, // place at (cx, cy)
            Drawable{BUY_AIR_TEX, {BUY_AIR_TEX.w * TEX_SCALE, BUY_AIR_TEX.h * TEX_SCALE}}, // sprite + size
            Buy_Tag{}, // UI tag
            UIButton_Tag{},
            Air_Tag{});// tower‐type tag
    }

    void Element::createMouse() const {
        Entity mouseEntity = Entity::create();
        mouseEntity.addAll(
        Transform{{0,0}, 0.f},
        Drawable{SPRITE_HOVER, {sprite_ui_cant_place_tower.w * TEX_SCALE, sprite_ui_cant_place_tower.h * TEX_SCALE}},
        Mouse_Tag{}
        );
    }

    void Element::createNextLevelButton() const {
        Entity nextLevelButtonEntity = Entity::create();
        nextLevelButtonEntity.addAll(
            Transform{{790, 694}, 0.f}, // place at (cx, cy)
            Drawable{UI_NEXT_LEVEL, {UI_NEXT_LEVEL.w * TEX_SCALE, UI_NEXT_LEVEL.h * TEX_SCALE}}, // sprite + size
            UIButton_Tag{}
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
    void Element::createGameState() const {
        Entity levelEntity = Entity::create();
        levelEntity.addAll(
            LevelManager_Tag{}, // marks this as the singleton level controller
            CurrentLevel{0} // start at wave 0
        );
    }
    void Element::createSpawnManager() const {
        // look up the first wave config:
        const Wave &w = WAVES[0];

        Entity spawnEntity = Entity::create();
        spawnEntity.addAll(
            SpawnManager_Tag{}, // marks the singleton spawn controller
            SpawnState{
                0, // waveIndex = 0
                w.count, // remaining = number of creeps to spawn
                0.f // timeLeft = spawn immediately
            }
        );
    }
    void Element::createCreep(float x, float y, float a, float speed, int hp,
                              int goldBounty, SDL_FRect spriteRect) const {
        Entity creepEntity = Entity::create();
        creepEntity.addAll(
            Transform{{x, y}, a},
            Drawable{spriteRect, {spriteRect.w * TEX_SCALE, spriteRect.h * TEX_SCALE}},
            WaypointIndex{1}, // head to waypoint #1
            Velocity{{0.f, 0.f}},
            Speed{speed},
            HP{hp, hp},
            Gold_Bounty{goldBounty},
            Creep_Tag{}
        );
    }
    // @formatter:on

    /// systems
    void Element::path_navigation_system() const {
        static const Mask mask = MaskBuilder()
                .set<Transform>()
                .set<Speed>()
                .set<WaypointIndex>()
                .set<Velocity>()
                .set<Creep_Tag>()
                .build();

        constexpr float SNAP = 1.0f; // px distance considered “arrived”

        for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
            if (!World::mask(e).test(mask))
                continue;

            auto &t = World::getComponent<Transform>(e);
            auto &wi = World::getComponent<WaypointIndex>(e);
            const auto &sp = World::getComponent<Speed>(e);
            auto &vel = World::getComponent<Velocity>(e);

            if (wi.idx >= TURN_COUNT)
                continue; // creep already at the end

            float wx = TURNS[wi.idx].x;
            float wy = TURNS[wi.idx].y;

            float dx = wx - t.p.x;
            float dy = wy - t.p.y;
            float distSq = dx * dx + dy * dy;

            if (distSq < SNAP * SNAP) {
                ++wi.idx; // snap & advance
                if (wi.idx >= TURN_COUNT) // reached base – handled elsewhere
                    continue;

                dx = TURNS[wi.idx].x - t.p.x;
                dy = TURNS[wi.idx].y - t.p.y;
            }

            float len = SDL_sqrtf(dx * dx + dy * dy);
            if (len < 1e-4f) len = 1.0f;

            vel.v = {
                dx / len * sp.value, // pixels per second
                dy / len * sp.value
            };

            t.a = RAD_TO_DEG * SDL_atan2f(vel.v.y, vel.v.x); // facing angle
        }
    }

    void Element::movement_system() const {
        // Only entities with a Transform and a Velocity move
        static const Mask mask = MaskBuilder()
                .set<Transform>()
                .set<Velocity>()
                .build();

        for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
            if (!World::mask(e).test(mask))
                continue;

            auto &t = World::getComponent<Transform>(e);
            const auto &vel = World::getComponent<Velocity>(e);

            // DT is seconds per frame (1 / FPS)
            t.p.x += vel.v.x * DT;
            t.p.y += vel.v.y * DT;
        }
    }

    void Element::placing_tower_system() const {
        static const Mask mask = MaskBuilder()
        .set<Transform>()
        .set<Mouse_Tag>()
        .build();

        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        const float  mapX = (mouseX-MAP_TEX_PAD_X) / TEX_SCALE;
        const float  mapY = (mouseY-MAP_TEX_PAD_Y) / TEX_SCALE;

        int col = (mapX) / CELL_SIZE;
        int row = (mapY) / CELL_SIZE;


        if (col >= 0 && row >= 0) {
            SDL_FPoint center = toScreen(
                col * CELL_SIZE + sprite_ui_can_place_tower.h / 2.0f ,
                row * CELL_SIZE + sprite_ui_can_place_tower.w / 2.0f
            );

            for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
                if (World::mask(e).test(mask)) {
                    World::getComponent<Transform>(e).p = center;
                }
            }
        }


    }

    void Element::endpoint_system() const {
        // 1. Build masks
        static const Mask creepMask = MaskBuilder()
                .set<Creep_Tag>()
                .set<WaypointIndex>()
                .set<Transform>()
                .set<HP>()
                .set<Gold_Bounty>()
                .build();

        static const Mask playerMask = MaskBuilder()
                .set<Player_Tag>()
                .set<HP>()
                .set<Gold>()
                .build();

        // 2. Find the player entity (cache once)
        static ent_type player{-1};
        if (player.id == -1) {
            for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
                if (World::mask(e).test(playerMask)) {
                    player = e;
                    break;
                }
            }
        }
        if (player.id == -1) return; // no player found? bail

        auto &playerHP = World::getComponent<HP>(player);
        auto &playerGold = World::getComponent<Gold>(player);

        // 3. Process each creep that’s reached the end
        for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
            if (!World::mask(e).test(creepMask))
                continue;

            auto &wi = World::getComponent<WaypointIndex>(e);
            auto &t = World::getComponent<Transform>(e);
            const auto &bounty = World::getComponent<Gold_Bounty>(e);

            if (wi.idx >= TURN_COUNT) {
                // a) Penalize the player
                playerHP.current = std::max(0, playerHP.current - 1);
                playerGold.current = std::max(0, playerGold.current - bounty.value);

                // b) Respawn the creep at the start
                t.p.x = TURNS[0].x;
                t.p.y = TURNS[0].y;
                wi.idx = 1; // head toward waypoint #1 next frame
            }
        }
    }

    void Element::wave_system() const {
        // 1) Find the singleton SpawnManager entity
        static const Mask mask = MaskBuilder()
                .set<SpawnManager_Tag>()
                .set<SpawnState>()
                .build();

        ent_type mgr{-1};
        for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
            if (World::mask(e).test(mask)) {
                mgr = e;
                break;
            }
        }
        if (mgr.id == -1) return; // no spawn manager?

        // 2) Access its SpawnState
        auto &st = World::getComponent<SpawnState>(mgr);

        // 3) If we've run all waves, do nothing
        if (st.waveIndex >= WAVE_COUNT) {
            return;
        }

        // 4) Grab the current wave config
        const Wave &w = WAVES[st.waveIndex];

        // 5) Countdown timer (only while spawning waves remain)
        st.timeLeft -= DT;
        if (st.timeLeft > 0.f) {
            return;
        }

        // 6) Spawn one creep if any remain in this wave
        if (st.remaining > 0) {
            createCreep(
                TURNS[0].x, TURNS[0].y, // spawn position & initial angle
                0.f, // we’ll orient in navigation
                w.speed, // speed
                w.hp, // hit points
                w.gold, // gold bounty
                w.sprite // sprite rect
            );
            st.remaining -= 1;
            st.timeLeft = w.delay;
            return;
        }

        // 7) This wave is done → advance to the next one (if it exists)
        st.waveIndex += 1;
        if (st.waveIndex < WAVE_COUNT) {
            const Wave &next = WAVES[st.waveIndex];
            st.remaining = next.count;
            st.timeLeft = 0.f; // immediately spawn the first creep of the new wave
        }
        // If st.waveIndex == WAVE_COUNT, we’ve finished _all_ waves and
        // the guard at step (3) will keep us from indexing further.
    }

    void Element::draw_system() const {
        static const Mask mask = MaskBuilder()
                .set<Transform>() // where to draw
                .set<Drawable>() // what to draw
                .build();

        SDL_RenderClear(ren);

        for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
            if (!World::mask(e).test(mask))
                continue;

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

        SDL_RenderPresent(ren);
        print_status_bar();
    }

    void Element::print_status_bar() const {
        // find the player
        static const Mask uiMask = MaskBuilder()
                .set<Player_Tag>()
                .set<HP>()
                .set<Gold>()
                .build();

        ent_type player{-1};
        for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
            if (World::mask(e).test(uiMask)) {
                player = e;
                break;
            }
        }
        if (player.id == -1) return;

        // fetch current values
        int hp = World::getComponent<HP>(player).current;
        int gold = World::getComponent<Gold>(player).current;

        // remember last printed values
        static int lastHP = -1;
        static int lastGold = -1;

        // only print if something changed
        if (hp == lastHP && gold == lastGold)
            return;

        lastHP = hp;
        lastGold = gold;

        // now print one line and newline
        std::cout << "HP: " << hp
                << "   Gold: " << gold
                << std::endl;
    }

    /// game
    Element::Element() {
        if (!prepareWindowAndTexture())
            return;

        createMap();

        createBuyArrow();
        createBuyCannon();
        createBuyAir();
        createNextLevelButton();

        createPlayer();
        createMouse();
        createGameState(); // sets up CurrentLevel{0}
        createSpawnManager(); // sets up SpawnState for WAVE[0]
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
            placing_tower_system();
            // move_system();
            // box_system();
            // score_system();
            //
            // input_system();
            wave_system();
            path_navigation_system();
            movement_system();
            //targeting_system
            //damage_system
            endpoint_system();
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
