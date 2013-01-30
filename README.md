Mir
=================================

Mir was a project to implement a virtual simulation in which ships with DCPU-16 hardware could be simulated on a large scale; the idea being that developers could safely test their programs under various environments before using them in-game.

Unfortunately due to other commitments, we no longer have the resources to bring this to a full implementation (and the last real work that was done on it was for a university project).

Building
-----------
You will need to clone this underneath DCPUToolchain as the "dtgame" folder.  Then just run cmake as normal for the toolchain and it will build the Mir client and server as part of it.

Things that do work
---------------------
* Multiplayer infrastructure
* Rendering and game loop

Things that don't work
------------------------
* No DCPU-16 VM emulation hooked in
* Players and accounts aren't handled properly at all; nor is there an infrastructure for supporting accounts or players when clients join
