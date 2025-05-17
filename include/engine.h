#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

// Forward declarations
struct Game; // Assuming game_t will be a typedef for struct Game
struct Command; // Assuming command_t will be a typedef for struct Command

// Typedefs from your document (will need actual struct definitions later)
typedef struct Game game_t;
typedef struct Command command_t;

// Define Game structure (game_t)
// This was forward-declared earlier
#include "world.h"   // For World type
#include "player.h"  // For Player type
// story.h and save.h might not be directly needed in game_t itself
// but are used by functions that take game_t*

struct Game {
    World world;
    Player player;
    game_state_t state; // Current state of the game (e.g., RUNNING, GAME_OVER)
    // Potentially other global game flags or states
};

// Game states (example)
typedef enum {
    GAME_RUNNING,
    GAME_OVER
} game_state_t;


// Placeholder for command_t structure
// This will be defined more concretely, likely in engine.c or a shared types.h
struct Command {
    char verb[64];
    char noun[64];
    // Add other fields if necessary, e.g., target for 'cast firebolt goblin'
};


void engine_run(game_t *game);
bool parse_command(const char *input, command_t *out);
void execute_command(game_t *game, const command_t *cmd);

#endif // ENGINE_H 