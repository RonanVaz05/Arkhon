#include "../include/story.h"
#include "../include/engine.h" // For game_t and game_state_t
#include "../include/player.h" // For Player struct access
#include "../include/world.h"  // For Room struct access
#include <stdio.h>
#include <string.h> // For strcmp if needed

static bool final_chamber_reached = false; // Example flag

void check_events(game_t *game) {
    // Example: Unlock a new area if a certain lore count is reached.
    if (game->player.lore_count >= 1 && game->player.location) {
        // Let's say lore_shard_1 (index 0) reveals a new exit in the Altar Chamber
        if (game->player.lore_found[0] && strcmp(game->player.location->id, "Altar Chamber") == 0) {
            // Find Altar Chamber and Hall of Echoes to potentially link them if a condition is met.
            Room* altar_chamber = world_find_room(&game->world, "Altar Chamber");
            Room* secret_passage = world_find_room(&game->world, "Secret Passage"); // Assuming this exists

            if(altar_chamber && secret_passage && altar_chamber->east == NULL) { // Check if exit not already opened
                // This is a simple example. A real game might have a more robust way to handle this.
                // For now, let's imagine using the "rusty_lever" (if it was a world object) opens this.
                // The current setup is more about lore count triggering things.
            }
        }
    }

    // Example: Trigger end game if player reaches a specific room
    if (game->player.location && strcmp(game->player.location->id, "Throne of the Twin") == 0) {
        // Assuming "Throne of the Twin" is the final chamber ID
        if (!final_chamber_reached) {
             printf("You have reached the Throne of the Twin!\n");
             final_chamber_reached = true;
        }
        conclude_game(game);
    }
}

void conclude_game(game_t *game) {
    printf("\n--- Epilogue ---\n");

    // Based on lore_count and other flags (not fully implemented yet)
    if (game->player.lore_count >= MAX_LORE_SHARDS) { // Example: True ending if all lore found
        printf("With all the lore shards gathered, Arkhon remembers everything.\n");
        printf("The ritual of Reunification is clear. The mage steps into the swirling portal, shadow and light merging.\n");
        printf("Arkhon is whole again, a being of immense power, free from the dungeon's curse.\n");
        printf("True Ending: Reunite\n");
    } else if (game->player.lore_count >= MAX_LORE_SHARDS / 2) { // Neutral ending
        printf("Arkhon, with fragmented memories, confronts the dark twin.\n");
        printf("Unable to fully restore the lost self, Arkhon chooses to seal the rift, containing the darkness within.\n");
        printf("The dungeon crumbles, but the mage remains a guardian, forever bound.\n");
        printf("Neutral Ending: Seal\n");
    } else { // Bad ending
        printf("Weakened and confused, Arkhon faces the dark twin and is overwhelmed.\n");
        printf("The shadow consumes what little light remained. The dungeon claims another soul.\n");
        printf("Dark Ending: Destroy\n");
    }

    game->state = GAME_OVER;
} 