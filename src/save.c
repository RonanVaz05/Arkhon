#include "../include/save.h"
#include "../include/engine.h" // For game_t definition
#include "../include/world.h"  // For Room ID for player location
#include "../include/player.h" // For player struct members
#include <stdio.h>
#include <string.h>

// A simplified save structure. For complex worlds, this needs to be more robust.
// For example, saving/loading Room connections or dynamic world state is tricky.
typedef struct {
    char current_room_id[64];
    Player player_state;
    // We are not saving the entire world state here, only the player's progress
    // and current location. The world itself is re-initialized on load.
    // This is simpler but means changes to the world (e.g. opened doors not tied to player flags)
    // might not persist unless also stored in player_state or a separate world_delta save.
} SaveData;

bool save_game(const game_t *game, const char *path) {
    FILE *file = fopen(path, "wb");
    if (!file) {
        perror("Error opening save file");
        return false;
    }

    SaveData data_to_save;
    memset(&data_to_save, 0, sizeof(SaveData));

    if (game->player.location) {
        strncpy(data_to_save.current_room_id, game->player.location->id, sizeof(data_to_save.current_room_id) - 1);
    } else {
        data_to_save.current_room_id[0] = '\0'; // No location
    }
    memcpy(&data_to_save.player_state, &game->player, sizeof(Player));

    size_t written = fwrite(&data_to_save, sizeof(SaveData), 1, file);
    fclose(file);

    if (written != 1) {
        fprintf(stderr, "Error writing save data.\n");
        return false;
    }
    return true;
}

bool load_game(game_t *game, const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("Error opening load file");
        return false;
    }

    SaveData loaded_data;
    size_t read = fread(&loaded_data, sizeof(SaveData), 1, file);
    fclose(file);

    if (read != 1) {
        fprintf(stderr, "Error reading save data.\n");
        return false;
    }

    // Restore player state
    memcpy(&game->player, &loaded_data.player_state, sizeof(Player));

    // Important: Re-initialize the world to its default state first
    // This ensures all room pointers etc. are valid before trying to find the saved room.
    init_world(&game->world); 

    // Find the room by ID and set player location
    if (loaded_data.current_room_id[0] != '\0') {
        game->player.location = world_find_room(&game->world, loaded_data.current_room_id);
        if (!game->player.location) {
            fprintf(stderr, "Warning: Saved room ID '%s' not found in current world. Player may be in an unexpected state.\n", loaded_data.current_room_id);
            // Optionally, move player to a default starting room if current one is invalid
            if (game->world.room_count > 0) game->player.location = &game->world.rooms[0];
        }
    } else {
        // If no room ID was saved, or it was intentionally null (e.g. intro sequence)
        // place player at default start, or handle as per game design
        if (game->world.room_count > 0) game->player.location = &game->world.rooms[0];
        else game->player.location = NULL;
    }
    
    // The game state should be running after a successful load.
    game->state = GAME_RUNNING;

    return true;
} 