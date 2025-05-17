#ifndef WORLD_H
#define WORLD_H

// Forward declaration to avoid circular dependencies if Room uses World or vice-versa indirectly
struct World_t;

// Define Room structure
typedef struct Room {
    char id[64];
    char description[256];
    // Example: pointers to other rooms (N, S, E, W, etc.)
    struct Room *north;
    struct Room *south;
    struct Room *east;
    struct Room *west;
    // Pointers or IDs to items and NPCs in the room
    // For simplicity, let's assume item IDs for now
    const char* items[10]; // Max 10 items per room
    int item_count;
    // Add NPCs later
} Room;

// Define World structure
typedef struct World_t {
    Room rooms[50]; // Assuming a max of 50 rooms
    int room_count;
    // Could also have a pointer to the current room the player is in,
    // but often player.location is preferred.
} World;

void init_world(World *world);
Room *world_find_room(World *world, const char *id);

#endif // WORLD_H 