// res/waves.h
#pragma once

#include <SDL3/SDL.h>
#include "atlas.h"          // gives sprite_1 … sprite_39

// FRECT convenience (safe: identical re-definition is allowed)
#ifndef FRECT
#  define FRECT(s) SDL_FRect{ (s).x, (s).y, (s).w, (s).h }
#endif

namespace element {
    /* ───────────────────────── 1. sprite rectangles ─────────────────────────── */

    inline constexpr SDL_FRect WAVE_1_TEX = FRECT(sprite_1);
    inline constexpr SDL_FRect WAVE_2_TEX = FRECT(sprite_2);
    inline constexpr SDL_FRect WAVE_3_TEX = FRECT(sprite_3);
    inline constexpr SDL_FRect WAVE_4_TEX = FRECT(sprite_4);
    inline constexpr SDL_FRect WAVE_5_TEX = FRECT(sprite_5);
    inline constexpr SDL_FRect WAVE_6_TEX = FRECT(sprite_6);
    inline constexpr SDL_FRect WAVE_7_TEX = FRECT(sprite_7);
    inline constexpr SDL_FRect WAVE_8_TEX = FRECT(sprite_8);
    inline constexpr SDL_FRect WAVE_9_TEX = FRECT(sprite_9);
    inline constexpr SDL_FRect WAVE_10_TEX = FRECT(sprite_10);
    inline constexpr SDL_FRect WAVE_11_TEX = FRECT(sprite_11);
    inline constexpr SDL_FRect WAVE_12_TEX = FRECT(sprite_12);
    inline constexpr SDL_FRect WAVE_13_TEX = FRECT(sprite_13);
    inline constexpr SDL_FRect WAVE_14_TEX = FRECT(sprite_14);
    inline constexpr SDL_FRect WAVE_15_TEX = FRECT(sprite_15);
    inline constexpr SDL_FRect WAVE_16_TEX = FRECT(sprite_16);
    inline constexpr SDL_FRect WAVE_17_TEX = FRECT(sprite_17);
    inline constexpr SDL_FRect WAVE_18_TEX = FRECT(sprite_18);
    inline constexpr SDL_FRect WAVE_19_TEX = FRECT(sprite_19);
    inline constexpr SDL_FRect WAVE_20_TEX = FRECT(sprite_20);
    inline constexpr SDL_FRect WAVE_21_TEX = FRECT(sprite_21);
    inline constexpr SDL_FRect WAVE_22_TEX = FRECT(sprite_22);
    inline constexpr SDL_FRect WAVE_23_TEX = FRECT(sprite_23);
    inline constexpr SDL_FRect WAVE_24_TEX = FRECT(sprite_24);
    inline constexpr SDL_FRect WAVE_25_TEX = FRECT(sprite_25);
    inline constexpr SDL_FRect WAVE_26_TEX = FRECT(sprite_26);
    inline constexpr SDL_FRect WAVE_27_TEX = FRECT(sprite_27);
    inline constexpr SDL_FRect WAVE_28_TEX = FRECT(sprite_28);
    inline constexpr SDL_FRect WAVE_29_TEX = FRECT(sprite_29);
    inline constexpr SDL_FRect WAVE_30_TEX = FRECT(sprite_30);
    inline constexpr SDL_FRect WAVE_31_TEX = FRECT(sprite_31);
    inline constexpr SDL_FRect WAVE_32_TEX = FRECT(sprite_32);
    inline constexpr SDL_FRect WAVE_33_TEX = FRECT(sprite_33);
    inline constexpr SDL_FRect WAVE_34_TEX = FRECT(sprite_34);
    inline constexpr SDL_FRect WAVE_35_TEX = FRECT(sprite_35);
    inline constexpr SDL_FRect WAVE_36_TEX = FRECT(sprite_36);
    inline constexpr SDL_FRect WAVE_37_TEX = FRECT(sprite_37);
    inline constexpr SDL_FRect WAVE_38_TEX = FRECT(sprite_38);
    inline constexpr SDL_FRect WAVE_39_TEX = FRECT(sprite_39);


    /* ───────────────────────── 2. wave table ────────────────────────────────── */

    struct Wave {
        int level;
        const char *name;
        int hp;
        const char *type; // "", "FAST", "AIR", …
        int goldBounty; // bounty
        int count;
        bool wood;
        int speed; // px / sec
        SDL_FRect sprite; // source rect in atlas
    };

    /* one constexpr WaveInfo per wave (level-order) */ // @formatter:off
inline constexpr Wave wave_1  {  1, "Sheep",                       10,  "",            1, 20, false, 100, WAVE_1_TEX };
inline constexpr Wave wave_2  {  2, "Rabid Dogs",                  42,  "",            1, 20, false, 104, WAVE_2_TEX  };
inline constexpr Wave wave_3  {  3, "Small Boys",                  65,  "",            1, 20, false, 108, WAVE_3_TEX  };
inline constexpr Wave wave_4  {  4, "Peasants",                    75,  "",            1, 20, false, 110, WAVE_4_TEX  };
inline constexpr Wave wave_5  {  5, "Firework Throwers",          101,  "",            1, 20, false, 112, WAVE_5_TEX  };
inline constexpr Wave wave_6  {  6, "Drug Runners",                87, "FAST",         1, 20, false, 116, WAVE_6_TEX  };
inline constexpr Wave wave_7  {  7, "Greedy Pigs",                135,  "",            1, 20, true,  110, WAVE_7_TEX  };
inline constexpr Wave wave_8  {  8, "FlyingMachines",             158, "AIR",          1, 20, false, 118, WAVE_8_TEX  };
inline constexpr Wave wave_9  {  9, "Stags",                      189,  "",            1, 20, false, 106, WAVE_9_TEX  };
inline constexpr Wave wave_10 { 10, "Trolls",                     212, "IMMUNE",       2, 20, false, 114, WAVE_10_TEX };
inline constexpr Wave wave_11 { 11, "Angry Clitters",            2000, "BOSS",        45,  1, false, 105, WAVE_11_TEX };
inline constexpr Wave wave_12 { 12, "Water Elementals",           246,  "",            2, 20, false, 115, WAVE_12_TEX };
inline constexpr Wave wave_13 { 13, "Drug Runners",               212, "FAST",         2, 20, false, 117, WAVE_13_TEX };
inline constexpr Wave wave_14 { 14, "Lizards",                    331,  "",            2, 20, true,  101, WAVE_14_TEX };
inline constexpr Wave wave_15 { 15, "Battle Golems",              384,  "",            2, 20, false, 113, WAVE_15_TEX };
inline constexpr Wave wave_16 { 16, "Happy Rogue",                445,  "",            2, 20, false, 109, WAVE_16_TEX };
inline constexpr Wave wave_17 { 17, "Albatross",                  580, "AIR",          2, 20, false, 120, WAVE_17_TEX };
inline constexpr Wave wave_18 { 18, "FatPigs",                    695,  "",            2, 20, false, 107, WAVE_18_TEX };
inline constexpr Wave wave_19 { 19, "Drug Runners",               559, "FAST",         2, 20, false, 118, WAVE_19_TEX };
inline constexpr Wave wave_20 { 20, "Goblins Again",              806,  "",            3, 20, false, 111, WAVE_20_TEX };
inline constexpr Wave wave_21 { 21, "Trolls",                    1125, "IMMUNE",       3, 20, true,  100, WAVE_21_TEX };
inline constexpr Wave wave_22 { 22, "Angry Clitters",           14000, "BOSS",        55,  1, false, 103, WAVE_22_TEX };
inline constexpr Wave wave_23 { 23, "Goblins Again!?",           1075,  "",            3, 20, false, 110, WAVE_23_TEX };
inline constexpr Wave wave_24 { 24, "DarkMinions",               1265,  "",            4, 20, false, 108, WAVE_24_TEX };
inline constexpr Wave wave_25 { 25, "Cannons",                   1468,  "",            4, 20, false, 112, WAVE_25_TEX };
inline constexpr Wave wave_26 { 26, "Drug Runners",              1265, "FAST",         4, 20, false, 116, WAVE_26_TEX };
inline constexpr Wave wave_27 { 27, "Albatross",                 1615, "AIR",          4, 20, false, 119, WAVE_27_TEX };
inline constexpr Wave wave_28 { 28, "Cryptic Fiends",            1935,  "",            4, 20, true,  104, WAVE_28_TEX };
inline constexpr Wave wave_29 { 29, "Sludge Monstrosity",        2165,  "",            5, 20, false, 115, WAVE_29_TEX };
inline constexpr Wave wave_30 { 30, "Ghouls",                    2405,  "",            5, 20, false, 107, WAVE_30_TEX };
inline constexpr Wave wave_31 { 31, "Goblins",                   2655,  "",            5, 20, false, 113, WAVE_31_TEX };
inline constexpr Wave wave_32 { 32, "Trolls Riding Drug Runners",2500,"FAST&IMMUNE",    2, 20, false, 117, WAVE_32_TEX };
inline constexpr Wave wave_33 { 33, "Angry Clitters",           35000, "BOSS",       100,  1, false, 102, WAVE_33_TEX };
inline constexpr Wave wave_34 { 34, "Damn Hard",                 5000,  "",            5, 40, false, 109, WAVE_34_TEX };
inline constexpr Wave wave_35 { 35, "OMG",                       7000,  "",           10, 40, false, 118, WAVE_35_TEX };
inline constexpr Wave wave_36 { 36, "gg",                       10000,  "",           15, 40, false, 111, WAVE_36_TEX };
inline constexpr Wave wave_37 { 37, "HOW ARE YOU STILL ALIVE?!",15000,  "",           20, 40, false, 105, WAVE_37_TEX };
inline constexpr Wave wave_38 { 38, "THE LAST LEVEL",           20000,  "",           25, 40, false, 120, WAVE_38_TEX };
inline constexpr Wave wave_39 { 39, "U B PRO",                  25001,  "",           30, 60, false, 112, WAVE_39_TEX };
    // @formatter:on
    /* aggregate for iteration */
    inline constexpr Wave WAVES[]{
        wave_1, wave_2, wave_3, wave_4, wave_5, wave_6, wave_7, wave_8, wave_9, wave_10,
        wave_11, wave_12, wave_13, wave_14, wave_15, wave_16, wave_17, wave_18, wave_19, wave_20,
        wave_21, wave_22, wave_23, wave_24, wave_25, wave_26, wave_27, wave_28, wave_29, wave_30,
        wave_31, wave_32, wave_33, wave_34, wave_35, wave_36, wave_37, wave_38, wave_39
    };
    static constexpr int WAVE_COUNT = sizeof(WAVES) / sizeof(WAVES[0]);

} // namespace element
