// Game.cpp

#include "Game.h"
#include <cmath>

static constexpr bool DEBUG = true;

//std::optional<std::pair<int, int>> hoveredCell;
int hoveredCellX = -1;
int hoveredCellY = -1;



static void drawCircle(SDL_Renderer *ren, const float cx, const float cy, float radius) {
    constexpr int SEGMENTS = 32;
    constexpr float TWO_PI = 2.0f * 3.14159265f;
    constexpr float STEP = TWO_PI / SEGMENTS;
    float theta = 0.0f;

    for (int i = 0; i < SEGMENTS; ++i) {
        const float x1 = cx + radius * cosf(theta);
        const float y1 = cy + radius * sinf(theta);
        const float x2 = cx + radius * cosf(theta + STEP);
        const float y2 = cy + radius * sinf(theta + STEP);

        // use SDL_RenderLine (float version)
        SDL_RenderLine(ren, x1, y1, x2, y2);

        theta += STEP;
    }
}

Game::Game() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        cout << SDL_GetError() << endl;
        return;
    }

    if (!SDL_CreateWindowAndRenderer("Flash Element Tower Defence", 1000, 750, 0, &win, &ren)) {
        cout << SDL_GetError() << endl;
        return;
    }

    SDL_Surface *surf = IMG_Load("res/atlas.png");
    if (surf == nullptr) {
        cout << SDL_GetError() << endl;
        return;
    }

    tex = SDL_CreateTextureFromSurface(ren, surf);
    if (tex == nullptr) {
        cout << SDL_GetError() << endl;
        return;
    }
    SDL_DestroySurface(surf);

    // creating world
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0, 0};
    world = b2CreateWorld(&worldDef);

    // 2) Define & create the sheep body
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_kinematicBody;
    bodyDef.position = {TURNS[0].x / BOX_SCALE, TURNS[0].y / BOX_SCALE};
    nextTurn = 1;
    ballBody = b2CreateBody(world, &bodyDef);

    // 3) Attach a shape so Box2D will track it (collision isn’t used here)
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1;
    b2Circle circle = {0, 0, SHEEP.w * TEX_SCALE / BOX_SCALE};
    b2CreateCircleShape(ballBody, &shapeDef, &circle);
}

Game::~Game() {
    if (tex != nullptr)
        SDL_DestroyTexture(tex);
    if (ren != nullptr)
        SDL_DestroyRenderer(ren);
    if (win != nullptr)
        SDL_DestroyWindow(win);

    SDL_Quit();
}

// — Helpers for run() —

// 1) Step the Box2D world
void Game::stepPhysics(float deltaTime) {
    b2World_Step(world, deltaTime, 4);
}

// 2) Update our sprite destination rect from the body’s position
void Game::updateSpriteRect(SDL_FRect &r) {
    b2Vec2 p = b2Body_GetPosition(ballBody);
    float screenX = p.x * BOX_SCALE;
    float screenY = p.y * BOX_SCALE;

    // subtract half the sprite’s width/height so its centre is at (screenX,screenY)
    r.x = screenX - (r.w * 0.5f);
    r.y = screenY - (r.h * 0.5f);
}

// 3) Aim at (tx,ty), set the kinematic velocity, and return the angle
float Game::aimAt(float tx, float ty) {
    b2Vec2 p = b2Body_GetPosition(ballBody);
    // compute the sprite’s visual centre in screen px
    float cx = p.x * BOX_SCALE;
    float cy = p.y * BOX_SCALE;

    float vx = tx - cx;
    float vy = ty - cy;
    float len = std::sqrt(vx * vx + vy * vy);
    if (len < 1e-4f) len = 1.0f;

    b2Body_SetLinearVelocity(
        ballBody, {
            (vx / len) * (SHEEP_SPEED / BOX_SCALE),
            (vy / len) * (SHEEP_SPEED / BOX_SCALE)
        }
    );

    return atan2f(vy, vx);
}

// 4) Check for waypoint snap, advance nextTurn, and re-aim if needed
float Game::handleWaypoint(float currentAngle) {
    // get the true body-center in screen px
    b2Vec2 p = b2Body_GetPosition(ballBody);
    float cx = p.x * BOX_SCALE;
    float cy = p.y * BOX_SCALE;

    // current target
    const TurnPt &t = TURNS[nextTurn];

    if (std::fabs(cx - t.x) < EPS && std::fabs(cy - t.y) < EPS) {
        // 1) snap to this waypoint
        b2Body_SetTransform(
            ballBody,
            {t.x / BOX_SCALE, t.y / BOX_SCALE},
            b2MakeRot(0.0f)
        );

        // Are we at the very last point?
        if (nextTurn == TURN_COUNT - 1) {
            // 2a) teleport back to the spawn (index 0)
            b2Body_SetTransform(
                ballBody,
                {TURNS[0].x / BOX_SCALE, TURNS[0].y / BOX_SCALE},
                b2MakeRot(0.0f)
            );
            // 3a) reset to head toward the first real waypoint
            nextTurn = 1;
        } else {
            // 2b) just advance to the next waypoint
            ++nextTurn;
        }

        // 4) re-aim at whichever is now TURNS[nextTurn]
        return aimAt(
            TURNS[nextTurn].x,
            TURNS[nextTurn].y
        );
    }

    // not yet reached: keep the old angle
    return currentAngle;
}


// 5) Render one frame: map, dots, debug‐boxes, sheep
void Game::renderFrame(const SDL_FRect &r, const SDL_FRect &mapDst, float angle, const SDL_FRect &buy_air,
                       const SDL_FRect &buy_cannon, const SDL_FRect &buy_arrow) {
    // clear & draw map
    SDL_RenderClear(ren);
    SDL_RenderTexture(ren, tex, &MAP, &mapDst);

    SDL_RenderTexture(ren, tex, &AIR_BUY, &buy_air);
    SDL_RenderTexture(ren, tex, &ARROW_BUY, &buy_arrow);
    SDL_RenderTexture(ren, tex, &CANNON_BUY, &buy_cannon);

    if constexpr (DEBUG) {
        // 1) Waypoints
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        for (const auto &t: TURNS) {
            drawDot(ren, t.x, t.y);
        }

        // 2) Sprite bounding-box
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
        SDL_RenderLine(ren, r.x, r.y, r.x + r.w, r.y);
        SDL_RenderLine(ren, r.x + r.w, r.y, r.x + r.w, r.y + r.h);
        SDL_RenderLine(ren, r.x + r.w, r.y + r.h, r.x, r.y + r.h);
        SDL_RenderLine(ren, r.x, r.y + r.h, r.x, r.y);

        // 3) Sprite centre dot
        {
            float cx = r.x + r.w * 0.5f;
            float cy = r.y + r.h * 0.5f;
            drawDot(ren, cx, cy);
        }

        // 4) Physics circle + centre
        {
            float physCx = r.x + r.w * 0.5f;
            float physCy = r.y + r.h * 0.5f;
            float physRad = (SHEEP.w * TEX_SCALE) * 0.5f;
            SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
            drawCircle(ren, physCx, physCy, physRad);
            drawDot(ren, physCx, physCy);
        }
    }

    // draw the sheep itself
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    float angleDeg = angle * RAD_TO_DEG;
    SDL_RenderTextureRotated(
        ren, tex,
        &SHEEP, &r,
        angleDeg,
        nullptr,
        SDL_FLIP_NONE
    );

    //SDL_RenderPresent(ren);
}

// — The new, slimmed‐down run() —
void Game::run() {
    // clear‐color once
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

    // prepare our two dest‐rects
    SDL_FRect sheepRect = {0, 0, SHEEP.w * TEX_SCALE, SHEEP.h * TEX_SCALE};
    SDL_FRect mapDst = {PAD_X, PAD_Y, MAP.w * TEX_SCALE, MAP.h * TEX_SCALE};

    SDL_FRect buy_air = {740, 350, sprite_buy_air.w * TEX_SCALE, sprite_buy_air.h * TEX_SCALE};
    SDL_FRect buy_cannon = {680, 350, sprite_buy_cannon.w * TEX_SCALE, sprite_buy_cannon.h * TEX_SCALE};
    SDL_FRect buy_arrow = {620, 350, sprite_buy_arrow.w * TEX_SCALE, sprite_buy_arrow.h * TEX_SCALE};


    //
    SDL_FRect SPRITE_HOVER = {sprite_ui_can_place_tower.x, sprite_ui_can_place_tower.y, sprite_ui_can_place_tower.w, sprite_ui_can_place_tower.h};

    constexpr float STEP = 1.0f / FPS;
    float spriteAngle = aimAt(
        TURNS[nextTurn].x,
        TURNS[nextTurn].y
    );
    // main loop
    for (int i = 0; i < 7000; ++i) {



        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            //change this with "quit" when switching to while loop:
            if (e.type == SDL_EVENT_QUIT){}
                //quit = true;
            else if ((e.type == SDL_EVENT_KEY_DOWN) && (e.key.scancode == SDL_SCANCODE_ESCAPE)){}
                //quit = true;


            if (e.type == SDL_EVENT_MOUSE_MOTION) {
                handleMouseMove(e.motion.x, e.motion.y);
            }
        }





        stepPhysics(STEP);
        updateSpriteRect(sheepRect); // positions r so the sprite’s centre = body centre
        spriteAngle = handleWaypoint(spriteAngle); // snaps _body_ centre to the dot
        renderFrame(sheepRect, mapDst, spriteAngle, buy_air, buy_cannon, buy_arrow);




        if (hoveredCellX >= 0 && hoveredCellY >= 0) {
            constexpr int CELL_SIZE = 10;
            SDL_FRect dst = {
                PAD_X + hoveredCellX * CELL_SIZE,
                PAD_Y + hoveredCellY * CELL_SIZE,
                SPRITE_HOVER.w * TEX_SCALE,
                SPRITE_HOVER.h * TEX_SCALE
            };
            SDL_RenderTexture(ren, tex, &SPRITE_HOVER, &dst);
        }



        SDL_RenderPresent(ren);


        SDL_Delay(2);

    }

}

void Game::handleMouseMove(int mouseX, int mouseY) {
    constexpr int CELL_SIZE = 10;

    int col = (mouseX - PAD_X - sprite_ui_can_place_tower.w / 2.0f) / CELL_SIZE;
    int row = (mouseY - PAD_Y - sprite_ui_can_place_tower.h / 2.0f) / CELL_SIZE;

    if (col >= 0 && row >= 0) {
        hoveredCellX = col;
        hoveredCellY = row;
    } else {
        hoveredCellX = hoveredCellY = -1;
    }
}

