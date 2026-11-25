#ifndef _GAMELOOP_H_
#define _GAMELOOP_H_

#include "frame_buffer_utils.h"
#include "random.h"

typedef struct {
  FrameBuffer *fb;
  random_state_t rng_state;
  float dt;

  float bear_y;
  float bear_velocity;
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

/*
 * Set the game state to reasonable defaults. FrameBuffer is never
 * touched.
 *
 * @param state pointer to current game state
*/
void reset_game_state(gamestate_t *state);

#endif