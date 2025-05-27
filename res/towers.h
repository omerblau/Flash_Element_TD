/* Auto-generated from Towers and Waves - Towers.csv – DO NOT EDIT MANUALLY */
#pragma once

struct TowerInfo {
    int level;
    int price;
    int damage;
    int range;
    float fireRate;
    const char *element;
    const char *effects;
    const char *special;
    const char *targets;
    const char *unlock;
};

///                                     level    price   damage range  fireRate  element spriteData
inline constexpr TowerInfo arrow_1    {1,       7,       6,   100,    0.2f,   "None",                 "None", "None", "Ground/Air", "Always Available"};
inline constexpr TowerInfo arrow_2    {2,      13,      16,   110,    0.2f,   "None",                 "None", "None", "Ground/Air", "Always Available"};
inline constexpr TowerInfo arrow_3    {3,      32,      26,   125,    0.2f,   "None",                 "None", "None", "Ground/Air", "Always Available"};
inline constexpr TowerInfo cannon_1   {1,       9,       9,    70,    0.2f,   "None",                 "None", "Splash Damage", "Ground", "Always Available"};
inline constexpr TowerInfo cannon_2   {2,      15,      24,    70,    0.2f,   "None",                 "None", "Splash Damage", "Ground", "Always Available"};
inline constexpr TowerInfo cannon_3   {3,      26,      50,    70,    0.2f,   "None",                 "Splash Damage increase to 60", "Splash Damage", "Ground", "Always Available"};
inline constexpr TowerInfo air_1      {1,      12,      20,   120,    0.2f,   "None",                 "None", "None", "Air", "Always Available"};
inline constexpr TowerInfo air_2      {2,      20,      35,   120,    0.2f,   "None",                 "increased attack speed", "None", "Air", "Always Available"};
inline constexpr TowerInfo air_3      {3,      30,      56,   120,    0.2f,   "None",                 "increased attack speed (again)", "None", "Air", "Always Available"};
inline constexpr TowerInfo water_1    {1,      50,      25,    75,    0.2f,   "Water",                "None", "Slows enemies (except IMMUTE)", "Ground/Air", "reaserch water"};
inline constexpr TowerInfo water_2_   {2,      25,      30,    75,    0.2f,   "Water",                "adds a SMALL splash to the slow", "Slows enemies (except IMMUTE)", "Ground/Air", "reaserch water"};
inline constexpr TowerInfo water_3    {3,      25,      35,    75,    0.2f,   "Water",                "adds a LARGE splash to the slow", "Slows enemies (except IMMUTE)", "Ground/Air", "reaserch water"};
inline constexpr TowerInfo water_4    {4,      25,      40,    75,    0.2f,   "Water",                "adds a HUGE splash to the slow", "Slows enemies (except IMMUTE)", "Ground/Air", "reaserch water"};
inline constexpr TowerInfo earth_1    {1,      50,     144,   100,    1.0f,   "Earth",                "None", "None", "Ground", "reaserch earth"};
inline constexpr TowerInfo earth_2    {2,      75,     288,   110,    1.0f,   "Earth",                "None", "None", "Ground", "reaserch earth"};
inline constexpr TowerInfo earth_3_   {3,     100,     576,   120,    1.0f,   "Earth",                "None", "None", "Ground", "reaserch earth"};
inline constexpr TowerInfo earth_4    {4,     150,    1152,   130,    1.0f,   "Earth",                "None", "None", "Ground", "reaserch earth"};
inline constexpr TowerInfo fire_1     {1,      50,      75,   100,    0.5f,   "Fire",                 "None", "Splash Damage, Extra Damage to IMMUNE", "Ground/Air", "reaserch fire"};
inline constexpr TowerInfo fire_2     {2,      75,     175,   100,    0.5f,   "Fire",                 "Larger Splash , Extra Damage to IMMUNE", "Splash Damage, Extra Damage to IMMUNE", "Ground/Air", "reaserch fire"};
inline constexpr TowerInfo fire_3     {3,     100,     300,   100,    0.5f,   "Fire",                 "Even Larger Splash , Extra Damage to IMMUNE", "Splash Damage, Extra Damage to IMMUNE", "Ground/Air", "reaserch fire"};
inline constexpr TowerInfo fire_4     {4,     150,     400,   100,    0.5f,   "Fire",                 "HUGE Splash , Extra Damage to IMMUNE", "Splash Damage, Extra Damage to IMMUNE", "Ground/Air", "reaserch fire"};
inline constexpr TowerInfo rocket_1   {1,     200,    2000,   170,    0.8f,   "All",                  "None", "None", "Ground/Air", "reaserch all elements"};
inline constexpr TowerInfo rocket_2   {2,     190,    2000,   170,    0.5f,   "All",                  "double the fire rate", "None", "Ground/Air", "reaserch all elements"};