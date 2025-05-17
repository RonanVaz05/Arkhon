#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "world.h" // For Room type

#define MAX_INVENTORY_ITEMS 10
#define MAX_LORE_SHARDS 10 // Example value

// Define Player structure
typedef struct {
    Room *location; // Current room the player is in
    char inventory[MAX_INVENTORY_ITEMS][64]; // Array of item IDs
    int inv_count;
    bool lore_found[MAX_LORE_SHARDS]; // Tracks found lore shards
    int lore_count; // Number of lore shards found
    // Add spells later
    // int health, mana, etc. as needed
} Player;

void init_player(Player *player);
bool player_take_item(Player *player, const char *item_id, World *world); // Added World for context
void player_use_item(Player *player, const char *item_id);
// Add functions like player_add_lore, player_learn_spell etc.

#endif // PLAYER_H 