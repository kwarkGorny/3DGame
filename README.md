# 3DGame
2.5D top down space shooter made in 2 weeks using C++17 in OpenGL.

## Gameplay:
Standard top-down shooter just destroy as much asteroids as you can before getting your ship wreck.

## Controls:
Controls are very basics:
```
A - to move left
D - to move right
``` 
## Configuration:
Basic gameplay parameters can be change form <a href="https://github.com/kwarkGorny/3DGame/blob/master/data/lua/config.lua">config.lua</a>
```lua
return {
    canonShootingFrequency = 0.5, -- shoots per second
    canonBulletsVelocity = 15, -- meters per second
    spaceShipForwardVelocity = 10, -- meters per second
    asteroidsAngularVelocityRange = {-math.pi, math.pi}, -- radians per second
    explosionDuration = 0.1, -- seconds
    manoeuveringEnginesThrust = 25, -- newtons
    spaceShipMass = 2, -- kilograms
    initialAsteroidAppearanceFrequency = 1, -- number per seconds
    asteroidAppearanceFrequencyIncrease = 0.01 -- number per seconds
}
```

## Third-party libraries:
All third-party libraries are located in <a href="https://github.com/kwarkGorny/3DGame/tree/master/external">external</a> folder:
* <a href="https://github.com/skypjack/entt">entt</a> - best header-only entity-component-system (ECS).
* <a href="https://github.com/ThePhD/sol2">sol2</a> - best lua binding for C++
* <a href="https://github.com/gabime/spdlog">spdlog</a> - awesome logging library
* <a href="https://github.com/nothings/stb">stb</a> - used for textures loading
* <a href="https://github.com/tinyobjloader/tinyobjloader">tinyobjloader</a> - used for obj loading
* <a href="https://github.com/g-truc/glm">glm</a> - mathematics library for graphics
* <a href="https://www.libsdl.org/download-2.0.php">SDL2</a>
* <a href="https://www.libsdl.org/projects/SDL_mixer/">SDL2 mixer</a>
* <a href="http://glew.sourceforge.net/">glew</a>

## Building:
Project uses submodules so to build project you need to dowload them beside this is normal cmake project:
```
git clone --recursive https://github.com/kwarkGorny/3DGame.git
cd 3DGame
mkdir build
cd build
cmake ..
cmake --build .
```

## Assets:
* <a href="https://free-game-assets.itch.io/free-space-shooter-game-user-interface">Gui</a>
* <a href="https://maxparata.itch.io/voxel-spaceships">Spaceships</a>
* <a href="https://wwwtyro.github.io/space-3d">Skybox</a>
* <a href="https://opengameart.org/content/space-shooter-sound-effects">Sounds</a>
* <a href="https://opengameart.org/content/space-shooter-music">Music</a>
* Asteroids where created by me using <a href="https://ephtracy.github.io/">MagicaVoxel</a>.

## Preview:
ToDo