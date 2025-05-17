#include "../include/engine.h"
#include "../include/world.h"
#include "../include/player.h"
// story.h might be needed if there's an init_story or similar

// main.c
int main(void) {
    game_t game;
    init_world(&game.world);
    init_player(&game.player);
    // Initialize game state
    game.state = GAME_RUNNING; 

    // Link player to starting room (e.g., the first room created)
    if (game.world.room_count > 0) {
        game.player.location = &game.world.rooms[0]; 
    } else {
        // Handle error: No rooms defined
        // For now, we can let it be NULL and engine_run or look command should handle it.
    }

    engine_run(&game);
    return 0;
} 