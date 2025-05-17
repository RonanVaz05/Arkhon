#ifndef STORY_H
#define STORY_H

// Forward declaration
struct Game; // From engine.h (implicitly, via game_t)

void check_events(struct Game *game);
void conclude_game(struct Game *game);

#endif // STORY_H 