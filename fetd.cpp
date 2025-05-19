// fetd.cpp
#include "fetd.h"
#include "bagel.h"

/**
 * @file fetd.cpp
 * @brief ECS module implementations for Flash Element TD game.
 */

namespace fetd {
    /**
     * @brief Handles spawning of new creeps at the start of each wave.
     * 
     * This system is responsible for creating new enemy entities at designated
     * spawn points when a new wave begins. It manages the timing and quantity
     * of different creep types based on the current wave number.
     */
    void SpawnSystem() {
        bagel::Mask m = bagel::MaskBuilder{}.build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }

    /**
     * @brief Updates creep velocity based on pathfinding data.
     * 
     * This system calculates the direction and speed of creeps as they
     * navigate along predefined waypoint paths. When a creep reaches a waypoint,
     * it updates its PathProgress to target the next waypoint in sequence.
     */
    void PathNavigationSystem() {
        bagel::Mask m = bagel::MaskBuilder{}
                .set<PositionComponent>()
                .set<SpeedComponent>()
                .set<PathProgressComponent>()
                .set<CreepEntity>()
                .build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }

    /**
     * @brief Updates entity positions based on velocity components.
     * 
     * This system applies velocity to position for all entities with movement,
     * including creeps and projectiles. It handles basic physics movement
     * and ensures entities move at the correct speed across the game grid.
     */
    void MovementSystem() {
        bagel::Mask m = bagel::MaskBuilder{}
                .set<PositionComponent>()
                .set<VelocityComponent>()
                .build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }

    /**
     * @brief Manages tower targeting of enemy units.
     * 
     * This system identifies valid targets within each tower's range and
     * assigns the closest or highest-priority creep as the tower's current target.
     * It respects tower-specific targeting rules such as air-only or ground-only.
     */
    void TargetingSystem() {
        bagel::Mask m = bagel::MaskBuilder{}
                .set<PositionComponent>()
                .set<RangeComponent>()
                .set<TowerEntity>()
                .build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }

    /**
     * @brief Controls tower attack mechanics and projectile creation.
     * 
     * This system handles the firing logic for towers with valid targets,
     * creating appropriate projectiles and calculating their initial velocity.
     * It also manages tower cooldowns between shots based on attack speed.
     */
    void ShootingSystem() {
        bagel::Mask m = bagel::MaskBuilder{}
                .set<PositionComponent>()
                .set<DamageComponent>()
                .set<TowerEntity>()
                .set<TargetComponent>()
                .build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }

    /**
     * @brief Detects and processes projectile impacts with creeps.
     * 
     * This system checks for collisions between projectiles and enemy units,
     * applying damage and special effects when impacts occur. It also handles
     * splash damage calculations for area-effect projectiles.
     */
    void ProjectileCollisionSystem() {
        bagel::Mask m = bagel::MaskBuilder{}
                .set<PositionComponent>()
                .set<DamageComponent>()
                .set<ProjectileEntity>()
                .build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }

    /**
     * @brief Manages health and death logic for entities.
     * 
     * This system applies pending damage to entities with health components,
     * checking if health has reached zero and triggering appropriate death effects.
     * For creeps, this includes awarding gold bounty to the player.
     */
    void DamageSystem() {
        bagel::Mask m = bagel::MaskBuilder{}
                .set<HealthComponent>()
                .build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }

    /**
     * @brief Processes and maintains slow effects on creeps.
     *
     * This system manages temporary speed reduction effects caused by certain
     * tower types like the Water Tower. It decreases the duration of active
     * slow effects and removes them when they expire.
     */
    void SlowEffectSystem() {
        bagel::Mask m = bagel::MaskBuilder{}
                .set<SlowEffectComponent>()
                .build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }

    /**
     * @brief Handles tower upgrade mechanics and stat improvements.
     * 
     * This system processes tower upgrades when triggered by the player,
     * updating tower components (damage, range, special effects) based on
     * the new upgrade level. It also manages upgrade costs and requirements.
     */
    void UpgradeSystem() {
        bagel::Mask m = bagel::MaskBuilder{}
                .set<UpgradeLevelComponent>()
                .set<TowerEntity>()
                .build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }

    /**
     * @brief Calculates and applies interest to player's gold.
     *
     * This system applies interest bonuses to the player's gold balance
     * at the end of each wave based on the current interest rate. Interest
     * provides passive income that scales with the player's saved gold.
     */
    void InterestSystem() {
        bagel::Mask m = bagel::MaskBuilder{}
                .set<InterestBonusComponent>()
                .build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }

    /**
     * @brief Removes temporary entities that have expired.
     * 
     * This system manages the automatic despawning of temporary entities
     * like projectiles and visual effects based on their lifespan timer.
     * When an entity's lifespan reaches zero, it is destroyed.
     */
    void CleanupSystem() {
        bagel::Mask m = bagel::MaskBuilder{}
                .set<LifespanComponent>()
                .build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }

    /**
     * @brief Handles visual representation of all game entities.
     * 
     * This system draws all visible entities to the screen based on their
     * position and renderable components. It manages sprite selection,
     * animation, and visual effects for the game's graphical presentation.
     */
    void RenderSystem() {
        bagel::Mask m = bagel::MaskBuilder{}
                .set<PositionComponent>()
                .set<RenderableComponent>()
                .build();
        for (bagel::id_type id = 0; id <= bagel::World::maxId().id; ++id) {
            bagel::ent_type ent{id};
            if (bagel::World::mask(ent).test(m)) {
                // pass
            }
        }
    }
    
    // Entity factory implementations
    
    /**
     * @brief Creates an Arrow Tower entity at the specified grid position.
     * 
     * Arrow Towers are basic, all-purpose towers with balanced range and damage.
     * They can only target ground units.
     * 
     * @param x The x-coordinate on the grid
     * @param y The y-coordinate on the grid
     * @return bagel::ent_type The entity ID of the created tower
     */
    bagel::ent_type createArrowTower(float x, float y) {
        bagel::ent_type entity = bagel::World::createEntity();
        
        // Position component with the given coordinates
        PositionComponent pos{x, y};
        
        // Common tower components
        RenderableComponent renderable{1}; // Sprite ID for arrow tower
        TowerEntity tower{};
        RangeComponent range{3.5f}; // Standard range
        DamageComponent damage{10}; // Base damage
        UpgradeLevelComponent level{0}; // Start at level 0
        
        // Add all components
        bagel::World::addComponents(entity, pos, renderable, tower, range, damage, level);
        
        return entity;
    }
    
    /**
     * @brief Creates a Cannon Tower entity at the specified grid position.
     * 
     * Cannon Towers deal splash damage to ground units in an area.
     * They have slightly shorter range than Arrow Towers but deal more damage.
     * 
     * @param x The x-coordinate on the grid
     * @param y The y-coordinate on the grid
     * @return bagel::ent_type The entity ID of the created tower
     */
    bagel::ent_type createCannonTower(float x, float y) {
        bagel::ent_type entity = bagel::World::createEntity();
        
        // Position component with the given coordinates
        PositionComponent pos{x, y};
        
        // Common tower components
        RenderableComponent renderable{2}; // Sprite ID for cannon tower
        TowerEntity tower{};
        RangeComponent range{3.0f}; // Shorter range than arrow
        DamageComponent damage{15}; // Higher base damage
        UpgradeLevelComponent level{0}; // Start at level 0
        
        // Special component: splash damage
        SplashRadiusComponent splash{1.5f}; // Area effect radius
        
        // Add all components
        bagel::World::addComponents(entity, pos, renderable, tower, range, damage, level, splash);
        
        return entity;
    }
    
    /**
     * @brief Creates an Air Tower entity at the specified grid position.
     * 
     * Air Towers are specialized defensive structures that can only target 
     * flying units. They have extended range but can't hit ground units.
     * 
     * @param x The x-coordinate on the grid
     * @param y The y-coordinate on the grid
     * @return bagel::ent_type The entity ID of the created tower
     */
    bagel::ent_type createAirTower(float x, float y) {
        bagel::ent_type entity = bagel::World::createEntity();
        
        // Position component with the given coordinates
        PositionComponent pos{x, y};
        
        // Common tower components
        RenderableComponent renderable{3}; // Sprite ID for air tower
        TowerEntity tower{};
        RangeComponent range{4.0f}; // Longer range for air targets
        DamageComponent damage{12}; // Medium damage
        UpgradeLevelComponent level{0}; // Start at level 0
        
        // Special component: can only hit air targets
        AirOnlyEntity airOnly{};
        
        // Add all components
        bagel::World::addComponents(entity, pos, renderable, tower, range, damage, level, airOnly);
        
        return entity;
    }

    /**
     * @brief Creates a Water Tower entity at the specified grid position.
     *
     * Water Towers have a slowing effect and can target both ground and air units.
     * They deal less direct damage but strategically reduce enemy movement speed.
     *
     * @param x The x-coordinate on the grid
     * @param y The y-coordinate on the grid
     * @return bagel::ent_type The entity ID of the created tower
     */
    bagel::ent_type createWaterTower(float x, float y) {
        bagel::ent_type entity = bagel::World::createEntity();

        // Position component with the given coordinates
        PositionComponent pos{x, y};

        // Common tower components
        RenderableComponent renderable{4}; // Sprite ID for water tower
        TowerEntity tower{};
        RangeComponent range{3.0f}; // Standard range
        DamageComponent damage{8}; // Lower damage but applies slow
        UpgradeLevelComponent level{0}; // Start at level 0

        // Special component: can target air units
        CanHitAirEntity canHitAir{};

        // Add all components
        bagel::World::addComponents(entity, pos, renderable, tower, range, damage, level, canHitAir);

        return entity;
    }

    /**
     * @brief Creates a Fire Tower entity at the specified grid position.
     *
     * Fire Towers are versatile defensive structures that deal splash damage,
     * can hit both ground and air units, and deal bonus damage to immune creeps.
     * They are more expensive but extremely effective against mixed unit types.
     *
     * @param x The x-coordinate on the grid
     * @param y The y-coordinate on the grid
     * @return bagel::ent_type The entity ID of the created tower
     */
    bagel::ent_type createFireTower(float x, float y) {
        bagel::ent_type entity = bagel::World::createEntity();
        
        // Position component with the given coordinates
        PositionComponent pos{x, y};
        
        // Common tower components
        RenderableComponent renderable{5}; // Sprite ID for fire tower
        TowerEntity tower{};
        RangeComponent range{3.0f}; // Standard range
        DamageComponent damage{12}; // Medium-high damage
        UpgradeLevelComponent level{0}; // Start at level 0
        
        // Special components
        SplashRadiusComponent splash{1.2f}; // Area effect radius
        CanHitAirEntity canHitAir{};
        BonusVsImmuneComponent bonusVsImmune{1.5f}; // 50% bonus vs immune

        // Add all components
        bagel::World::addComponents(entity, pos, renderable, tower, range, damage, level,
                                   splash, canHitAir, bonusVsImmune);

        return entity;
    }

    /**
     * @brief Creates an Earth Tower entity at the specified grid position.
     *
     * Earth Towers deal massive single-target damage to ground units.
     * They have shorter range but compensate with extremely high damage output.
     * Effective against high-health creeps but cannot target air units.
     *
     * @param x The x-coordinate on the grid
     * @param y The y-coordinate on the grid
     * @return bagel::ent_type The entity ID of the created tower
     */
    bagel::ent_type createEarthTower(float x, float y) {
        bagel::ent_type entity = bagel::World::createEntity();

        // Position component with the given coordinates
        PositionComponent pos{x, y};

        // Common tower components
        RenderableComponent renderable{6}; // Sprite ID for earth tower
        TowerEntity tower{};
        RangeComponent range{2.5f}; // Shorter range
        DamageComponent damage{25}; // Very high damage
        UpgradeLevelComponent level{0}; // Start at level 0

        // Add all components
        bagel::World::addComponents(entity, pos, renderable, tower, range, damage, level);

        return entity;
    }

    /**
     * @brief Creates a Rocket Tower entity at the specified grid position.
     *
     * Rocket Towers have exceptional range and high damage, capable of hitting
     * both ground and air units from far away. They're expensive but provide
     * strategic coverage across large areas of the map.
     *
     * @param x The x-coordinate on the grid
     * @param y The y-coordinate on the grid
     * @return bagel::ent_type The entity ID of the created tower
     */
    bagel::ent_type createRocketTower(float x, float y) {
        bagel::ent_type entity = bagel::World::createEntity();

        // Position component with the given coordinates
        PositionComponent pos{x, y};

        // Common tower components
        RenderableComponent renderable{7}; // Sprite ID for rocket tower
        TowerEntity tower{};
        RangeComponent range{5.0f}; // Very long range
        DamageComponent damage{20}; // High damage
        UpgradeLevelComponent level{0}; // Start at level 0

        // Special component: can target air units
        CanHitAirEntity canHitAir{};

        // Add all components
        bagel::World::addComponents(entity, pos, renderable, tower, range, damage, level, canHitAir);

        return entity;
    }

    /**
     * @brief Creates a Ground Creep enemy entity at the specified position.
     * 
     * Ground Creeps are the standard enemy units that move along the path.
     * They have moderate health and speed, and yield a small gold bounty when defeated.
     * They can only be targeted by towers that can hit ground units.
     * 
     * @param x The x-coordinate on the grid to spawn the creep
     * @param y The y-coordinate on the grid to spawn the creep
     * @return bagel::ent_type The entity ID of the created creep
     */
    bagel::ent_type createGroundCreep(float x, float y) {
        bagel::ent_type entity = bagel::World::createEntity();
        
        // Position component with the given coordinates
        PositionComponent pos{x, y};
        
        // Common creep components
        RenderableComponent renderable{20}; // Sprite ID for ground creep
        CreepEntity creep{};
        HealthComponent health{50}; // Base health
        GoldBountyComponent bounty{10}; // Gold when killed
        
        // Movement components
        VelocityComponent velocity{0.f, 0.f}; // Initial velocity
        SpeedComponent speed{1.0f}; // Base speed in tiles/sec
        PathProgressComponent pathProgress{0}; // Start at waypoint 0
        
        // Add all components
        bagel::World::addComponents(entity, pos, renderable, creep, health, bounty,
                                   velocity, speed, pathProgress);
        
        return entity;
    }

    /**
     * @brief Creates an Air Creep flying enemy entity at the specified position.
     *
     * Air Creeps are faster flying units with less health than Ground Creeps.
     * They yield more gold when defeated, but can only be targeted by towers
     * with the CanHitAir or AirOnly capabilities. They follow the same path
     * as ground units but can only be damaged by certain tower types.
     *
     * @param x The x-coordinate on the grid to spawn the creep
     * @param y The y-coordinate on the grid to spawn the creep
     * @return bagel::ent_type The entity ID of the created air creep
     */
    bagel::ent_type createAirCreep(float x, float y) {
        bagel::ent_type entity = bagel::World::createEntity();

        // Position component with the given coordinates
        PositionComponent pos{x, y};

        // Common creep components
        RenderableComponent renderable{21}; // Sprite ID for air creep
        CreepEntity creep{};
        HealthComponent health{35}; // Lower health than ground units
        GoldBountyComponent bounty{15}; // More gold than ground units

        // Movement components
        VelocityComponent velocity{0.f, 0.f}; // Initial velocity
        SpeedComponent speed{1.2f}; // Faster than ground units
        PathProgressComponent pathProgress{0}; // Start at waypoint 0

        // Special components
        AirUnitEntity airUnit{}; // Flying unit

        // Add all components
        bagel::World::addComponents(entity, pos, renderable, creep, health, bounty,
                                   velocity, speed, pathProgress, airUnit);

        return entity;
    }

    /**
     * @brief Creates a Projectile entity fired from a tower.
     * 
     * Projectiles travel in a straight line with constant velocity and deal
     * damage to creeps they collide with. They automatically despawn after
     * a set duration if they don't hit anything.
     * 
     * @param x The x-coordinate starting position of the projectile
     * @param y The y-coordinate starting position of the projectile
     * @param vx The x-component of the projectile's velocity vector
     * @param vy The y-component of the projectile's velocity vector
     * @return bagel::ent_type The entity ID of the created projectile
     */
    bagel::ent_type createProjectile(float x, float y, float vx, float vy) {
        bagel::ent_type entity = bagel::World::createEntity();
        
        // Position component with the given coordinates
        PositionComponent pos{x, y};
        
        // Visual and type components
        RenderableComponent renderable{30}; // Sprite ID for projectile
        ProjectileEntity projectile{};
        
        // Movement component
        VelocityComponent velocity{vx, vy}; // Direction and speed
        
        // Combat components
        DamageComponent damage{10}; // Base damage
        LifespanComponent lifespan{3.0f}; // Seconds before auto-despawn
        
        // Add all components
        bagel::World::addComponents(entity, pos, renderable, projectile, velocity, damage, lifespan);
        
        return entity;
    }
    
    /**
     * @brief Creates a singleton GameState entity to track global game parameters.
     * 
     * The GameState entity holds global information that applies to the overall
     * game state rather than specific entities. Currently maintains the interest
     * rate applied to the player's gold at the end of each wave.
     * 
     * @return bagel::ent_type The entity ID of the game state object
     */
    bagel::ent_type createGameState() {
        bagel::ent_type entity = bagel::World::createEntity();
        
        // Game state component
        InterestBonusComponent interest{0.1f}; // 10% interest rate
        
        // Add component
        bagel::World::addComponent(entity, interest);
        
        return entity;
    }
} // namespace fetd
