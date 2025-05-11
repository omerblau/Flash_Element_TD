//fetd.h
#pragma once
#include "bagel.h"
// Doxygen treats the first sentence as the brief by default
/**
 * @file fetd.h
 * @brief ECS module for Flash Element TD game: components, systems, entity factories.
 */

namespace fetd {

// Components

/** Position: x,y grid coordinates. */
struct PositionComponent {
    float x = 0.f;
    float y = 0.f;
};

/** @brief Target: current enemy being aimed at. */
struct TargetComponent {
    bagel::ent_type entity = {0};
};

/** @brief GoldBounty: gold awarded when a creep dies. */
struct GoldBountyComponent {
    int amount = 0;
};

/** @brief UpgradeLevel: tower tier (0–3). */
struct UpgradeLevelComponent {
    int level = 0;
};

/** @brief Renderable: sprite or animation handle. */
struct RenderableComponent {
    int spriteId = 0;
};

/** InterestBonus: extra interest percent at end of wave. */
struct InterestBonusComponent {
    float percent = 0.f;
};

/** BonusVsImmune: damage multiplier vs immune creeps. */
struct BonusVsImmuneComponent {
    float multiplier = 1.f;
};

/** Velocity: movement vector per tick. */
struct VelocityComponent {
    float dx = 0.f;
    float dy = 0.f;
};

/** Speed: tiles per second creep travels. */
struct SpeedComponent {
    float value = 0.f;
};

/** Health: current hit points. */
struct HealthComponent {
    int current = 0;
};

/** Damage: amount of damage dealt. */
struct DamageComponent {
    int amount = 0;
};

/** Range: attack or effect radius in tiles. */
struct RangeComponent {
    float radius = 0.f;
};

/** PathProgress: index along waypoint path. */
struct PathProgressComponent {
    int index = 0;
};

/** SlowEffect: slow percent and remaining duration. */
struct SlowEffectComponent {
    float percent = 0.f;
    float remaining = 0.f;
};

/** SplashRadius: area-of-effect radius. */
struct SplashRadiusComponent {
    float radius = 0.f;
};

/** Lifespan: seconds until automatic despawn. */
struct LifespanComponent {
    float seconds = 0.f;
};

// Entity tags

struct ProjectileEntity {};
struct TowerEntity {};
struct CreepEntity {};
struct ImmuneEntity {};
struct AirUnitEntity {};
struct CanHitAirEntity {};
struct AirOnlyEntity {};
struct PlacingTowerEntity {};

// System declarations

/**  SpawnSystem: spawn new creeps at the start of each wave. */
void SpawnSystem();

/**  PathNavigationSystem: update creep velocity toward next waypoint. */
void PathNavigationSystem();

/**  MovementSystem: move entities based on their velocity. */
void MovementSystem();

/**  TargetingSystem: towers lock onto valid creeps in range. */
void TargetingSystem();

/**  ShootingSystem: towers with a target spawn projectiles. */
void ShootingSystem();

/**  ProjectileCollisionSystem: detect projectile hits. */
void ProjectileCollisionSystem();

/**  DamageSystem: apply damage and handle deaths. */
void DamageSystem();

/**  SlowEffectSystem: update and remove slow effects. */
void SlowEffectSystem();

/**  UpgradeSystem: upgrade tower stats when triggered. */
void UpgradeSystem();

/**  InterestSystem: grant interest gold at wave end. */
void InterestSystem();

/** CleanupSystem: despawn entities whose lifespan expired. */
void CleanupSystem();

/** RenderSystem: render all entities with sprites. */
void RenderSystem();

// Entity factory declarations

bagel::ent_type createArrowTower(float x, float y);
bagel::ent_type createCannonTower(float x, float y);
bagel::ent_type createAirTower(float x, float y);
bagel::ent_type createWaterTower(float x, float y);
bagel::ent_type createFireTower(float x, float y);
bagel::ent_type createEarthTower(float x, float y);
bagel::ent_type createRocketTower(float x, float y);

bagel::ent_type createGroundCreep(float x, float y);
bagel::ent_type createAirCreep(float x, float y);

bagel::ent_type createProjectile(float x, float y, float vx, float vy);
bagel::ent_type createGameState();

} // namespace fetd
