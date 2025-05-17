#include "../include/player.h"
#include "../include/world.h" // For World and Room, used in player_take_item
#include <string.h>
#include <stdio.h> // For printf

void init_player(Player *player) {
    player->location = NULL; // Will be set in main after world init
    player->inv_count = 0;
    player->lore_count = 0;
    memset(player->lore_found, 0, sizeof(player->lore_found));
    // Initialize other player stats like health, spells etc. here
}

// Helper function to find and remove an item from a room
static bool remove_item_from_room(Room *room, const char *item_id) {
    for (int i = 0; i < room->item_count; ++i) {
        if (strcmp(room->items[i], item_id) == 0) {
            // Found it, now remove by shifting subsequent items left
            for (int j = i; j < room->item_count - 1; ++j) {
                room->items[j] = room->items[j+1];
            }
            room->item_count--;
            room->items[room->item_count] = NULL; // Clear the last (now unused) slot
            return true;
        }
    }
    return false;
}

// Helper function to check if player has an item
static bool player_has_item(Player *player, const char *item_id) {
    for (int i = 0; i < player->inv_count; ++i) {
        if (strcmp(player->inventory[i], item_id) == 0) {
            return true;
        }
    }
    return false;
}


bool player_take_item(Player *player, const char *item_id, World *world) {
    if (!player->location) {
        printf("You are not in any specific location to take items from.\n");
        return false;
    }

    if (player->inv_count >= MAX_INVENTORY_ITEMS) {
        printf("Your inventory is full.\n");
        return false;
    }

    // Check if item is in the current room and remove it
    if (remove_item_from_room(player->location, item_id)) {
        strncpy(player->inventory[player->inv_count], item_id, sizeof(player->inventory[0]) - 1);
        player->inventory[player->inv_count][sizeof(player->inventory[0]) - 1] = '\0';
        player->inv_count++;
        return true;
    } else {
        printf("There is no '%s' here to take.\n", item_id);
        return false;
    }
}

void player_use_item(Player *player, const char *item_id) {
    if (!player_has_item(player, item_id)) {
        printf("You don't have a %s.\n", item_id);
        return;
    }

    // Basic item usage logic
    if (strcmp(item_id, "torch") == 0) {
        printf("You light the torch. The area brightens slightly, revealing more details in the shadows.\n");
        // Potentially reveal something or change room description if it was dark
    } else if (strcmp(item_id, "lore_shard_1") == 0) {
        if (!player->lore_found[0]) { // Assuming lore_shard_1 corresponds to index 0
            printf("You examine the strange shard. Ancient glyphs glow faintly as a fragment of forgotten knowledge floods your mind.\n");
            printf("'...the twin souls, one of shadow, one of fading light...'\n");
            player->lore_found[0] = true;
            player->lore_count++;
            // Remove the lore shard from inventory? Or is it a permanent key item?
            // For now, let's assume it's consumed or its knowledge absorbed.
            // To remove: find item in inventory, shift, decrement inv_count.
        } else {
            printf("You've already gleaned the knowledge from this shard.\n");
        }
    } else if (strcmp(item_id, "rusty_lever") == 0) {
        // This item is in the world, not inventory. So `use rusty_lever` might mean interacting with it in the room.
        // The current `player_use_item` assumes the item is in inventory.
        // This suggests `use` might need to check room items too, or have a different command for world objects.
        // For now, this will fail if `rusty_lever` is not taken first.
        // If it *is* taken (which it shouldn't be if it's fixed to a wall):
        printf("You fiddle with the rusty lever in your hands. It doesn't seem to do anything here.\n");
        // Proper implementation would be: if (player->location->id is "Hall of Echoes" and item is "rusty_lever") then trigger event.
    } else {
        printf("You can't figure out how to use the %s.\n", item_id);
    }
} 