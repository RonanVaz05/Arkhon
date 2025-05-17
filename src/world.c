#include "../include/world.h"
#include <string.h>
#include <stdio.h> // For NULL if not included by string.h or world.h indirectly

// Helper to create a room (can be expanded)
static void create_room(World *world, const char *id, const char *description) {
    if (world->room_count < (sizeof(world->rooms) / sizeof(world->rooms[0]))) {
        Room *room = &world->rooms[world->room_count++];
        strncpy(room->id, id, sizeof(room->id) - 1);
        room->id[sizeof(room->id) - 1] = '\0';
        strncpy(room->description, description, sizeof(room->description) - 1);
        room->description[sizeof(room->description) - 1] = '\0';
        room->north = room->south = room->east = room->west = NULL;
        room->item_count = 0;
    } else {
        // Handle error: too many rooms
        fprintf(stderr, "Error: Maximum number of rooms reached.\n");
    }
}

static void add_item_to_room(Room *room, const char *item_id) {
    if (room->item_count < (sizeof(room->items) / sizeof(room->items[0]))) {
        // In a real game, you'd probably want to copy the string or use a global item registry.
        // For now, assuming item_id is a persistent string literal.
        room->items[room->item_count++] = item_id;
    } else {
        fprintf(stderr, "Error: Maximum items in room %s reached.\n", room->id);
    }
}

void init_world(World *world) {
    world->room_count = 0;

    // Create some rooms
    create_room(world, "Dungeon Entrance", "You stand at the cold, stone entrance to a forgotten dungeon. A heavy wooden door, slightly ajar, leads north into darkness.");
    create_room(world, "Hall of Echoes", "A long hall stretches before you, a faint green glow emanating from the north. The air is damp and smells of mildew. A rusted lever is set into the west wall.");
    create_room(world, "Altar Chamber", "This circular chamber is dominated by a cracked stone altar. Strange symbols cover its surface. A single torch flickers on the east wall, casting dancing shadows.");

    // Define exits (assuming rooms are indexed 0, 1, 2...)
    if (world->room_count >= 2) { // Check if at least two rooms exist
        world->rooms[0].north = &world->rooms[1]; // Dungeon Entrance -> Hall of Echoes
        world->rooms[1].south = &world->rooms[0]; // Hall of Echoes -> Dungeon Entrance
    }
    if (world->room_count >= 3) { // Check if at least three rooms exist
        world->rooms[1].north = &world->rooms[2]; // Hall of Echoes -> Altar Chamber
        world->rooms[2].south = &world->rooms[1]; // Altar Chamber -> Hall of Echoes
    }

    // Place items
    if (world->room_count >= 2) {
        add_item_to_room(&world->rooms[1], "rusty_lever");
    }
    if (world->room_count >= 3) {
         add_item_to_room(&world->rooms[2], "lore_shard_1");
         add_item_to_room(&world->rooms[2], "torch");
    }

    // TODO: Place NPCs
}

Room *world_find_room(World *world, const char *id) {
    for (int i = 0; i < world->room_count; ++i) {
        if (strcmp(world->rooms[i].id, id) == 0) {
            return &world->rooms[i];
        }
    }
    return NULL; // Not found
} 