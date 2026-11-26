#ifndef _GAMELOOP_H_
#define _GAMELOOP_H_

#include "frame_buffer_utils.h"
#include "random.h"

typedef enum {
  Title,
  Game,
  EndScreen,
} game_phase_t;

typedef enum {
  Hive,
  Bee,
  HighBee
} obstacle_type_t;

typedef struct {
  float x;
  obstacle_type_t type;
} obstacle_state_t;

typedef struct {
  game_phase_t phase;

  FrameBuffer *fb;
  random_state_t rng_state;
  float dt;

  float score;

  float bear_y;
  float bear_velocity_y;
  float bear_velocity_x;
  float bear_height;
  
  uint32_t next_obstacle_millis;
  uint32_t obstacles_allocated;
  uint32_t obstacles_count;
  obstacle_state_t** obstacles;
} gamestate_t;

/*
 * Runs the Bear Metal game. Never returns.
 *
 * @param fb pointer to previously initialized FrameBuffer
*/
void gameloop(FrameBuffer *fb);

/*
 * Tick the gameloop during the title phase.
 *
 * @param state pointer to current game state
*/
void title_tick(gamestate_t *state);

/*
 * Tick the gameloop during the game phase.
 *
 * @param state pointer to current game state
*/
void game_tick(gamestate_t *state);

void endscreen_tick(gamestate_t *state);

/*
 * Set the game state to reasonable defaults. FrameBuffer is never
 * touched.
 *
 * @param state pointer to current game state
*/
void reset_game_state(gamestate_t *state);

void render_game(gamestate_t *state);
void add_obstacle(gamestate_t *state, obstacle_state_t obs_state);
void remove_obstacle(gamestate_t *state, uint32_t index);
uint8_t check_collisions(gamestate_t *state);
uint32_t obstacle_y(obstacle_type_t type);

#endif