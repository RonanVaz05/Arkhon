#include "../include/engine.h"
#include "../include/world.h"
#include "../include/player.h"
#include "../include/story.h"
#include "../include/save.h"
#include "../include/utils.h" // For str_trim
#include <stdio.h>
#include <string.h>
#include <ctype.h> // For tolower

// Command handler function prototypes
static void cmd_look(game_t *g, const command_t *c);
static void cmd_go(game_t *g, const command_t *c);
static void cmd_take(game_t *g, const command_t *c);
static void cmd_use(game_t *g, const command_t *c);
static void cmd_talk(game_t *g, const command_t *c); // Assuming NPC interaction
static void cmd_cast(game_t *g, const command_t *c);
static void cmd_inventory(game_t *g, const command_t *c);
static void cmd_save(game_t *g, const command_t *c);
static void cmd_load(game_t *g, const command_t *c);
static void cmd_quit(game_t *g, const command_t *c);
static void cmd_help(game_t *g, const command_t *c);


void engine_run(game_t *game) {
    char buffer[128];
    printf("Welcome to Arkhon's Awakening!\n");
    // Initial look around
    if (game->player.location) {
        cmd_look(game, NULL); // NULL command as it's an implicit action
    }

    while (true) {
        printf("\n> ");
        if (!fgets(buffer, sizeof buffer, stdin)) {
            printf("Exiting due to input error or EOF.\n");
            break; // EOF or error
        }

        // Trim whitespace (including newline)
        str_trim(buffer);

        if (strlen(buffer) == 0) { // Empty input
            continue;
        }

        command_t cmd;
        if (parse_command(buffer, &cmd)) {
            execute_command(game, &cmd);
            if (game->state == GAME_OVER) {
                printf("Game Over.\n");
                break;
            }
            check_events(game); // Check for story events after each command
        } else {
            printf("I don't understand that. Type 'help' for available commands.\n");
        }
    }
}

bool parse_command(const char *input, command_t *out) {
    char lower_input[128];
    strncpy(lower_input, input, sizeof(lower_input) - 1);
    lower_input[sizeof(lower_input) - 1] = '\0';
    for (int i = 0; lower_input[i]; i++) {
        lower_input[i] = tolower(lower_input[i]);
    }

    char *verb = strtok(lower_input, " \t\n");
    char *noun = strtok(NULL, " \t\n"); // Get the next token

    if (verb) {
        strncpy(out->verb, verb, sizeof(out->verb) - 1);
        out->verb[sizeof(out->verb) - 1] = '\0';

        if (noun) {
            strncpy(out->noun, noun, sizeof(out->noun) - 1);
            out->noun[sizeof(out->noun) - 1] = '\0';
        } else {
            out->noun[0] = '\0'; // No noun provided
        }
        return true;
    }
    return false;
}

void execute_command(game_t *game, const command_t *cmd) {
    if (strcmp(cmd->verb, "look") == 0) cmd_look(game, cmd);
    else if (strcmp(cmd->verb, "go") == 0) cmd_go(game, cmd);
    else if (strcmp(cmd->verb, "take") == 0) cmd_take(game, cmd);
    else if (strcmp(cmd->verb, "use") == 0) cmd_use(game, cmd);
    else if (strcmp(cmd->verb, "talk") == 0) cmd_talk(game, cmd);
    else if (strcmp(cmd->verb, "cast") == 0) cmd_cast(game, cmd);
    else if (strcmp(cmd->verb, "inventory") == 0 || strcmp(cmd->verb, "i") == 0) cmd_inventory(game, cmd);
    else if (strcmp(cmd->verb, "save") == 0) cmd_save(game, cmd);
    else if (strcmp(cmd->verb, "load") == 0) cmd_load(game, cmd);
    else if (strcmp(cmd->verb, "quit") == 0 || strcmp(cmd->verb, "q") == 0) cmd_quit(game, cmd);
    else if (strcmp(cmd->verb, "help") == 0) cmd_help(game, cmd);
    else {
        printf("I don't know how to '%s'.\n", cmd->verb);
    }
}

// --- Command Handler Implementations (Stubs) ---

static void cmd_look(game_t *g, const command_t *c) {
    if (!g->player.location) {
        printf("You are floating in a void. There is nothing to see.\n");
        return;
    }
    printf("You are in: %s\n", g->player.location->id);
    printf("%s\n", g->player.location->description);
    // List items in the room
    if (g->player.location->item_count > 0) {
        printf("You see here: ");
        for (int i = 0; i < g->player.location->item_count; ++i) {
            printf("%s%s", g->player.location->items[i], (i == g->player.location->item_count - 1) ? "" : ", ");
        }
        printf(".\n");
    }
    // List exits
    printf("Exits: ");
    if (g->player.location->north) printf("north ");
    if (g->player.location->south) printf("south ");
    if (g->player.location->east) printf("east ");
    if (g->player.location->west) printf("west ");
    printf("\n");
}

static void cmd_go(game_t *g, const command_t *c) {
    if (c->noun[0] == '\0') {
        printf("Go where?\n");
        return;
    }
    Room *current_room = g->player.location;
    Room *next_room = NULL;

    if (strcmp(c->noun, "north") == 0) next_room = current_room->north;
    else if (strcmp(c->noun, "south") == 0) next_room = current_room->south;
    else if (strcmp(c->noun, "east") == 0) next_room = current_room->east;
    else if (strcmp(c->noun, "west") == 0) next_room = current_room->west;
    else {
        printf("You can't go '%s'.\n", c->noun);
        return;
    }

    if (next_room) {
        g->player.location = next_room;
        printf("You go %s.\n", c->noun);
        cmd_look(g, NULL); // Automatically look after moving
    } else {
        printf("You can't go that way.\n");
    }
}

static void cmd_take(game_t *g, const command_t *c) {
    if (c->noun[0] == '\0') {
        printf("Take what?\n");
        return;
    }
    if (player_take_item(&g->player, c->noun, &g->world)) {
        printf("You take the %s.\n", c->noun);
    } else {
        // player_take_item should print its own error if item not found or inventory full
        // If it doesn't, add a generic message here.
        // For now, assuming player_take_item handles it.
    }
}

static void cmd_use(game_t *g, const command_t *c) {
    if (c->noun[0] == '\0') {
        printf("Use what?\n");
        return;
    }
    player_use_item(&g->player, c->noun);
    // player_use_item should print messages about the outcome.
}

static void cmd_talk(game_t *g, const command_t *c) {
    if (c->noun[0] == '\0') {
        printf("Talk to whom?\n");
        return;
    }
    printf("There is no one named '%s' to talk to here.\n", c->noun);
    // TODO: Implement NPC interactions
}

static void cmd_cast(game_t *g, const command_t *c) {
    // Example: cast firebolt goblin
    // Here, cmd->noun would be "firebolt", and you might need a third part for the target.
    // The current command_t only has verb and noun.
    // For simplicity, let's assume spells don't need targets or the target is the noun.
    if (c->noun[0] == '\0') {
        printf("Cast what?\n");
        return;
    }
    printf("You don't know any spell called '%s'.\n", c->noun);
    // TODO: Implement spell system
}

static void cmd_inventory(game_t *g, const command_t *c) {
    if (g->player.inv_count == 0) {
        printf("You are not carrying anything.\n");
        return;
    }
    printf("You are carrying:\n");
    for (int i = 0; i < g->player.inv_count; ++i) {
        printf("- %s\n", g->player.inventory[i]);
    }
}

static void cmd_save(game_t *g, const command_t *c) {
    // Default save file name if none provided
    const char* path = (c->noun[0] != '\0') ? c->noun : "savegame.dat";
    if (save_game(g, path)) {
        printf("Game saved to %s.\n", path);
    } else {
        printf("Failed to save game.\n");
    }
}

static void cmd_load(game_t *g, const command_t *c) {
    const char* path = (c->noun[0] != '\0') ? c->noun : "savegame.dat";
    if (load_game(g, path)) {
        printf("Game loaded from %s.\n", path);
        cmd_look(g, NULL); // Look around after loading
    } else {
        printf("Failed to load game from %s. Starting new game or check file.\n", path);
        // Optionally, re-initialize or quit here if load is critical and fails
    }
}

static void cmd_quit(game_t *g, const command_t *c) {
    printf("Are you sure you want to quit? (yes/no)\n> ");
    char confirm_buf[10];
    if (fgets(confirm_buf, sizeof(confirm_buf), stdin)) {
        str_trim(confirm_buf);
        if (strcmp(confirm_buf, "yes") == 0) {
            g->state = GAME_OVER;
            printf("Quitting game. Farewell, Arkhon!\n");
        }
    }
}

static void cmd_help(game_t *g, const command_t *c) {
    printf("Available commands:\n");
    printf("  look                    - Describe the current room and its contents.\n");
    printf("  go [direction]          - Move in a direction (e.g., 'go north').\n");
    printf("  take [item]             - Pick up an item.\n");
    printf("  use [item]              - Use an item from your inventory.\n");
    printf("  inventory (or i)        - List items you are carrying.\n");
    // printf("  talk [character]        - Speak to a character.\n");
    // printf("  cast [spell] [target] - Cast a magical spell.\n");
    printf("  save [filename]         - Save your game (default: savegame.dat).\n");
    printf("  load [filename]         - Load your game (default: savegame.dat).\n");
    printf("  quit (or q)             - Exit the game.\n");
    printf("  help                    - Show this help message.\n");
} 