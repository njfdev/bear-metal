#include "gameloop.h"
#include "colors.h"
#include "font.h"
#include "general_utils.h"
#include "keyboard.h"
#include "quad.h"
#include "timer.h"

const uint32_t LOOP_FREQUENCY = 60;

const uint32_t BEAR_X = 80;
const uint32_t BEAR_WIDTH = 25;
const uint32_t BEAR_HEIGHT = 30;

const uint32_t GROUND = 200;

const float GRAVITY = -1150.0;
const float JUMP_VELOCITY = 500.0;

void gameloop(FrameBuffer *fb, PixelList *pixels) {

  int phase = 0;
  
	Color background = {0, 0, 0, 0};

  clear_screen(fb, background);

  gamestate_t state = {.fb = fb, .pixels = pixels};
  reset_game_state(&state);

  uint32_t last_millis = millis();
  while (1) {
    clear_screen(fb, background);

    if (phase == 0) {
      title_tick(&state);

      // if up pressed, then start
      if (check_pressed(Up)) {
        phase = 1;
        reset_game_state(&state);
      }
    } else {
      game_tick(&state);

      if (check_pressed(Enter)) {
        phase = 0;
      }
    }

    // wait until time has past for this frame
    uint32_t cur = millis();
    while (1000/LOOP_FREQUENCY > millis()-last_millis) {
      __asm__ volatile ("hlt");
      cur = millis();
    }
    state.dt = (cur-last_millis)/1000.0;
    last_millis = cur;
  }
}

void title_tick(gamestate_t *state) {
  draw_text(state->pixels, "Press up arrow to start!", (Point){80, 210}, WHITE, 2);

}

void game_tick(gamestate_t *state) {
  // compute bear height
  if (state->bear_y == 0) {
    // if jump, then give bear upwards velocity
    if (check_pressed(Up)) {
      state->bear_velocity = JUMP_VELOCITY;
    }
  }

  // compute velocity change and offset
  state->bear_velocity += GRAVITY*state->dt;
  state->bear_y += state->bear_velocity*state->dt;

  if (state->bear_y <= 0) {
    state->bear_y = 0;
    state->bear_velocity = 0;
  }

  // draw everything
  draw_text(state->pixels, "Running!", (Point){20, GROUND+40}, WHITE, 1);

  uint32_t left = BEAR_X-(BEAR_WIDTH/2);
  uint32_t right = BEAR_X+(BEAR_WIDTH/2);
  uint32_t top = GROUND-BEAR_HEIGHT-state->bear_y;
  uint32_t bottom = GROUND-state->bear_y;

	Point corners[4] = {(Point){left, bottom}, (Point){right, bottom}, (Point){right, top}, (Point){left, top}};
	fill_quad(state->pixels, corners, RED, 0);
}

void reset_game_state(gamestate_t *state) {
  state->dt = 1.0/LOOP_FREQUENCY;

  state->bear_y = 0;
  state->bear_velocity = 0;
}
