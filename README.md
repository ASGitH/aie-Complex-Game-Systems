# [Complex Game Systems](https://asgith.github.io/aie-complex-game-systems-page)

![alt text](https://asgith.github.io/images/aie/complex-game-systems.png "Complex Game Systems Screenshot")

[Complex Game Systems](https://asgith.github.io/aie-complex-game-systems-page) is a Game Boy project created at the [Academy of Interactive Entertainment](https://seattle.aie.edu/campuses/seattle/). Everything created, whether that would be something minimal, to the more complex and thought out of, creations tend to make themselves more complicated, ending up with something very complex. The goal of this project, create a complex system and implement it into a game. A random procedurally generated dungeon crawler, was my choice for a complex game system.

Latest Update:

* Janurary 10th, 2020
  - The last update before I decided to take a different approach to the random procedurally generated floor/room.
  - Way things worked before was that the dungeon rooms were created by hand, then when the game began, a floor was created with different rooms placed into it.
  - For testing purposes, you could select the size of a floor, and if the current room you were in had another room next to it, touching this arrow would send you to the next     room.

  - My new plan of approach would be to randomly create a room from scratch. From the room itself (each individual tile in a room), to the floor with all newly created rooms,      would not only improve the quality of the game itself, but show the aspect of a complex game system.

![alt text](https://asgith.github.io/images/aie/complex-game-systems-01.png "Complex Game Systems Screenshot")

## How to Play

Movement: directional pad

To Switch to a Different Room: step into the arrow (when present)

![alt text](https://asgith.github.io/images/aie/complex-game-systems-03.png "Complex Game Systems Screenshot")

## Miscellaneous

Tools used:

* Backgrounds/Sprites/Tiles - [Game Boy Map Builder & Game Boy Tile Designer](https://github.com/gbdk-2020/GBTD_GBMB)

* Developer Kit - [Game Boy Development Kit](https://github.com/gbdk-2020/gbdk-2020)

* Emulator - [BGB GameBoy Emulator](http://bgb.bircd.org/)

## To Run it

To get it up and running, you'll need to have a emulator that can run Game Boy games. My preference is to use [BGB GameBoy Emulator](http://bgb.bircd.org/)

Once you have an emulator of choice, then:

* Download the .gb file from the [releases section](https://github.com/ASGitH/aie-Complex-Game-Systems/releases/tag/build-00).

* Load up the .gb file and have fun!
