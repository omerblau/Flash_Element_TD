# Flash Element TD — BAGEL ECS Demo

A bite-size recreation of the classic **Flash Element TD** tower-defence game, built as a semester project to showcase our custom **BAGEL** Entity-Component-System (ECS) engine.

## What’s an ECS?

In an ECS architecture, you separate _data_ (**components**) from _behavior_ (**systems**), and let anonymous _entities_ simply be “bags of components.”  
This keeps code cache-friendly, allows features to be bolted on by mixing components, and makes parallel updates trivial.

## Project Highlights

-   **BAGEL engine** — header-only, no STL containers at runtime, configurable storage back-ends (sparse / packed / tagged).
-   **Data-driven towers & waves** (`res/towers.h`, `res/waves.h`) so balancing is a spreadsheet edit, not a code change.
-   **ECS-driven gameplay loop** — each frame runs a series of clean systems:  
    input → UI → spawning → movement / targeting / shooting → drawing
-   **Physics emulation without Box2D** — all movement, collision, and targeting use plain math and SDL-level sprite positioning, fully manual.

## Running the Demo

A ready-to-run `.exe` is available on the [Releases](https://github.com/omerblau/Flash_Element_TD/tree/v0.9) page.  
No install required — just run the file to play the demo.

## Demo Limitations (by design)

This MVP was built for an in-class demonstration, not as a polished full game.  
Several features from the original were intentionally skipped:

-   No **main menu**, **loss screen**, or endgame condition
-   No **wood/research** mechanics (only gold)
-   Only a few basic **tower types** are implemented
-   No **tower placement restrictions**  
    (You can place towers anywhere — including on top of each other or on the path)
-   No **gold checks** — placing towers is always allowed, even with negative gold

## Credits

-   Game concept by David Scott (Flash Element TD, 2007)
-   Code & ECS design by our CS-2025 semester team
-   BAGEL ECS engine by our **"Advanced Programming Topics for the Honors Program"** course professor, **Moshe Sulami**
-   Artwork ripped from the original Flash game for educational purposes only
