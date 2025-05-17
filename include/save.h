#ifndef SAVE_H
#define SAVE_H

#include <stdbool.h>

// Forward declaration
struct Game; // From engine.h (implicitly, via game_t)

bool save_game(const struct Game *game, const char *path);
bool load_game(struct Game *game, const char *path);

#endif // SAVE_H 