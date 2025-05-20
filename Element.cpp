#include "Element.h"

#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>


using namespace std;

#include "bagel.h"
using namespace bagel;

namespace element {
    //------------------------------------------------------------------------------
    // helper to find the first entity matching a mask
    //------------------------------------------------------------------------------
    static bagel::ent_type findEntity(const bagel::Mask &mask) {
        for (bagel::ent_type e{0}; e.id <= bagel::World::maxId().id; ++e.id) {
            if (bagel::World::mask(e).test(mask))
                return e;
        }
        return bagel::ent_type{-1};
    }

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
        constexpr auto w = MAP_TEX.w * TEX_SCALE;
        constexpr auto h = MAP_TEX.h * TEX_SCALE;
        constexpr auto cy = MAP_TEX_PAD_Y + h * 0.5f; // centre-y
        constexpr auto cx = MAP_TEX_PAD_X + w * 0.5f; // centre-x

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
            UIButton_Tag{},
            Arrow_Tag{} // tower‐type tag
        );
    }
    void Element::createBuyCannon() const {
        Entity buyCannonEntity = Entity::create();
        buyCannonEntity.addAll(
            Transform{{880, 380}, 0.f}, // place at (cx, cy)
            Drawable{BUY_CANNON_TEX, {BUY_CANNON_TEX.w * TEX_SCALE, BUY_CANNON_TEX.h * TEX_SCALE}}, // sprite + size
            UIButton_Tag{},
            Cannon_Tag{} // tower‐type tag
        );
    }
    void Element::createBuyAir() const {
        Entity buyAirEntity = Entity::create();
        buyAirEntity.addAll(
            Transform{{960, 380}, 0.f}, // place at (cx, cy)
            Drawable{BUY_AIR_TEX, {BUY_AIR_TEX.w * TEX_SCALE, BUY_AIR_TEX.h * TEX_SCALE}}, // sprite + size
            UIButton_Tag{},
            Air_Tag{});// tower‐type tag
    }
    void Element::createNextLevelButton() const {
        Entity nextLevelButtonEntity = Entity::create();
        nextLevelButtonEntity.addAll(
            Transform{{790, 694}, 0.f}, // place at (cx, cy)
            Drawable{UI_NEXT_LEVEL, {UI_NEXT_LEVEL.w * TEX_SCALE, UI_NEXT_LEVEL.h * TEX_SCALE}}, // sprite + size
            UIButton_Tag{},
            NextLevel_Tag{}
        );
    }
    void Element::createMouse() const {
        Entity mouseEntity = Entity::create();
        mouseEntity.addAll(
        Transform{{0,0}, 0.f},
        Drawable{{},{}},
        MouseInput{ 0, 0, false },
        Mouse_Tag{}
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
            GameState_Tag{}, // marks this as the singleton level controller
            CurrentLevel{0}, // start at wave 0
            UIIntent{}    // starts at None
        );
    }
    void Element::createSpawnManager() const {
        Entity spawnEntity = Entity::create();
        spawnEntity.addAll(
            SpawnManager_Tag{}, // marks the singleton spawn controller
            SpawnState{
                -1, // waveIndex = 0
                0, // remaining = number of creeps to spawn
                0.f // timeLeft = spawn immediately
            }
        );
    }
    void Element::createCreep(float speed, int hp, int goldBounty, SDL_FRect spriteRect) const {
        Entity creepEntity = Entity::create();
        creepEntity.addAll(
            Transform{{TURNS[0].x, TURNS[0].y}, 0.f,},
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
    void Element::input_system() {
        static const Mask mouseMask = MaskBuilder()
                .set<Mouse_Tag>()
                .set<MouseInput>()
                .set<Transform>()
                .build();

        auto mouseEnt = findEntity(mouseMask);
        if (mouseEnt.id == -1) return;

        auto &mi = World::getComponent<MouseInput>(mouseEnt);
        auto &t = World::getComponent<Transform>(mouseEnt);

        // 2) Clear last frame’s click
        mi.clicked = false;

        // 3) Poll SDL events and feed into that same mouseEnt
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) exit(0);

            if (e.type == SDL_EVENT_MOUSE_MOTION) {
                mi.x = e.motion.x;
                mi.y = e.motion.y;
            } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                mi.x = e.button.x;
                mi.y = e.button.y;
                mi.clicked = true;
            }

            // always update the visual cursor position too
            t.p.x = static_cast<float>(mi.x);
            t.p.y = static_cast<float>(mi.y);
        }
    }

    void Element::ui_system() const {
        // 1) Read the one & only MouseInput entity
        static const Mask mouseMask = MaskBuilder()
                .set<Mouse_Tag>()
                .set<MouseInput>()
                .set<Transform>()
                .build();

        ent_type mouseEnt = findEntity(mouseMask);
        if (mouseEnt.id == -1) return;

        const auto &mi = World::getComponent<MouseInput>(mouseEnt);
        if (!mi.clicked) return; // only respond to an actual click

        // 2) Grab the single GameState (where UIIntent lives)
        static const Mask intentMask = MaskBuilder()
                .set<GameState_Tag>() // I assume this is your GameState tag
                .set<UIIntent>()
                .build();

        ent_type gs = findEntity(intentMask);
        if (gs.id == -1) return;

        auto &intent = World::getComponent<UIIntent>(gs);
        intent.action = UIAction::None; // reset before setting a new one

        // 3) Hit‐test every UIButton_Tag
        static const Mask btnMask = MaskBuilder()
                .set<UIButton_Tag>()
                .set<Transform>()
                .set<Drawable>()
                .build();

        for (ent_type b{0}; b.id <= World::maxId().id; ++b.id) {
            if (!World::mask(b).test(btnMask))
                continue;

            const auto &t = World::getComponent<Transform>(b);
            const auto &d = World::getComponent<Drawable>(b);
            // @formatter:off
            float left   = t.p.x - d.size.x/2;
            float right  = t.p.x + d.size.x/2;
            float top    = t.p.y - d.size.y/2;
            float bottom = t.p.y + d.size.y/2;

            if (mi.x < left || mi.x > right || mi.y < top || mi.y > bottom)
                continue;

            const Mask &m = World::mask(b);
            // if      (m.test(Component<Arrow_Tag>::Bit))       intent.action = UIAction::BuyArrow;
            if      (m.test(Component<Arrow_Tag>::Bit))       intent.action = UIAction::BuyArrow;
            else if (m.test(Component<Cannon_Tag>::Bit))      intent.action = UIAction::BuyCannon;
            else if (m.test(Component<Air_Tag>::Bit))         intent.action = UIAction::BuyAir;
            else if (m.test(Component<NextLevel_Tag>::Bit))   intent.action = UIAction::NextLevel;
            break;
            // @formatter:on
        }
    }

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
        static ent_type player = findEntity(playerMask);
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
        // 1) Find SpawnManager singleton
        static const Mask mgrMask = MaskBuilder()
                .set<SpawnManager_Tag>()
                .set<SpawnState>()
                .build();
        ent_type mgr{-1};
        for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
            if (World::mask(e).test(mgrMask)) {
                mgr = e;
                break;
            }
        }
        if (mgr.id == -1) return;
        auto &st = World::getComponent<SpawnState>(mgr);

        // 2) If we're mid-spawning this wave, continue countdown + spawn
        if (st.remaining > 0) {
            const Wave &w = WAVES[st.waveIndex];
            st.timeLeft -= DT;
            if (st.timeLeft <= 0.f) {
                createCreep(w.speed, w.hp, w.gold, w.sprite);
                st.remaining -= 1;
                st.timeLeft = w.delay;
            }
            return;
        }

        // no new wave until current one’s creeps are all gone and player clicked
        // 3) Ensure no creeps remain alive before allowing next-wave click
        static const Mask creepMask = MaskBuilder().set<Creep_Tag>().build();
        for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
            if (World::mask(e).test(creepMask))
                return;
        }

        // 4) Handle NextLevel click to start the next wave
        static const Mask intentMask = MaskBuilder()
                .set<GameState_Tag>()
                .set<UIIntent>()
                .build();
        ent_type gs{-1};
        for (ent_type e{0}; e.id <= World::maxId().id; ++e.id) {
            if (World::mask(e).test(intentMask)) {
                gs = e;
                break;
            }
        }
        if (gs.id == -1) return;
        auto &intent = World::getComponent<UIIntent>(gs);
        if (intent.action != UIAction::NextLevel)
            return;

        // 5) Advance and initialize next wave
        st.waveIndex += 1;
        if (st.waveIndex < WAVE_COUNT) {
            const Wave &w = WAVES[st.waveIndex];
            st.remaining = w.count;
            st.timeLeft = 0.f;
        }
        intent.action = UIAction::None;
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

    [[noreturn]] void Element::run() {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        auto start = SDL_GetTicks();
        while (true) {
            input_system();
            ui_system();

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
        }
    }
}
