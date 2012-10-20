/*
 * The main entry point for the physical simulation client.
 */

#include "Engine.h"

int main(int argc, char** argv)
{
    Engine engine;

    while (engine.IsRunning())
    {
        engine.Run();
    }

    engine.Cleanup();
}