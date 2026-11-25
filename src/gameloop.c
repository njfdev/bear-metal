#include "gameloop.h"
#include "colors.h"
#include "font.h"
#include "general_utils.h"
#include "keyboard.h"
#include "quad.h"
#include "random.h"
#include "timer.h"
#include "alloc.h"

const uint32_t LOOP_FREQUENCY = 60;

const uint32_t BEAR_X = 80;
const uint32_t BEAR_WIDTH = 25;
const uint32_t BEAR_HEIGHT = 30;

const uint32_t OBSTACLE_WIDTH = 30;
const uint32_t OBSTACLE_HEIGHT = 30;
const uint32_t OBSTACLE_Y = 0;

const uint32_t OBSTACLE_SPAWN_MIN_MILLIS = 500;
const uint32_t OBSTACLE_SPAWN_MAX_MILLIS = 1500;

const uint32_t GROUND = 200;

const float GRAVITY = -1150.0;
const float JUMP_VELOCITY = 500.0;
const float RUN_VELOCITY = 350.0;

void gameloop(FrameBuffer *fb) {

  int phase = 0;
  
	Color background = {0, 0, 0, 0};

  clear_screen(fb, background);

  gamestate_t state = {.fb = fb};
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
  draw_text(state->fb, "Press up arrow to start!", (Point){80, 210}, WHITE, 2);
  char text[20] = "RNG: ";
  uint_to_string((uint32_t)(random_next(&state->rng_state)*4294967296), text+5, 15);
  draw_text(state->fb, text, (Point){80, 310}, WHITE, 2);
}

void game_tick(gamestate_t *state) {
  // generate obstacle if necessary
  if (state->next_obstacle_millis < millis()) {
    add_obstacle(state, (obstacle_state_t) {
      .type = Spike,
      .x = state->fb->width-OBSTACLE_WIDTH
    });
    state->next_obstacle_millis = millis()+(random_next(&state->rng_state)*(OBSTACLE_SPAWN_MAX_MILLIS-OBSTACLE_SPAWN_MIN_MILLIS))+OBSTACLE_SPAWN_MIN_MILLIS;
  }

  // update game obstacles
  for (int i = 0; i < state->obstacles_count; i++) {
    obstacle_state_t* obs = state->obstacles[i];
    obs->x -= RUN_VELOCITY*state->dt;

    // if obstacle done, remove if and shift everything else down by 1
    if (obs->x < OBSTACLE_WIDTH/2.0) {
      remove_obstacle(state, i);

      i--;
    }
  }

  // update and render all obstacles
  for (int i = 0; i < state->obstacles_count; i++) {    
    obstacle_state_t* obs = state->obstacles[i];

    uint32_t left = obs->x-(OBSTACLE_WIDTH/2.0);
    uint32_t right = obs->x+(OBSTACLE_WIDTH/2.0);
    uint32_t top = GROUND-OBSTACLE_HEIGHT-OBSTACLE_Y;
    uint32_t bottom = GROUND-OBSTACLE_Y;

    Point corners[4] = {(Point){left, bottom}, (Point){right, bottom}, (Point){right, top}, (Point){left, top}};
    fill_quad(state->fb, corners, BLUE, 0);
  }

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
  draw_text(state->fb, "Running!", (Point){20, GROUND+40}, WHITE, 1);

  uint32_t left = BEAR_X-(BEAR_WIDTH/2);
  uint32_t right = BEAR_X+(BEAR_WIDTH/2);
  uint32_t top = GROUND-BEAR_HEIGHT-state->bear_y;
  uint32_t bottom = GROUND-state->bear_y;

	Point corners[4] = {(Point){left, bottom}, (Point){right, bottom}, (Point){right, top}, (Point){left, top}};
	fill_quad(state->fb, corners, RED, 0);
}

void reset_game_state(gamestate_t *state) {
  state->rng_state = (random_state_t) {.seed = millis()};
  state->dt = 1.0/LOOP_FREQUENCY;

  state->bear_y = 0;
  state->bear_velocity = 0;

  // free obstacles if already allocated
  state->next_obstacle_millis = millis();
  if (state->obstacles_allocated != 0) {
    for (int i = 0; i < state->obstacles_count; i++) {
      free(state->obstacles[i]);
    }

    free(state->obstacles);
  }
  state->obstacles_allocated = 4;
  state->obstacles_count = 0;
  state->obstacles = malloc(state->obstacles_allocated*sizeof(char*));
}

void add_obstacle(gamestate_t *state, obstacle_state_t obs_state) {
  if (state->obstacles_count == state->obstacles_allocated) {
    state->obstacles_allocated *= 2;
    state->obstacles = realloc(state->obstacles, state->obstacles_allocated*sizeof(void*));
  }

  state->obstacles[state->obstacles_count] = malloc(sizeof(obstacle_state_t));
  *state->obstacles[state->obstacles_count] = obs_state;

  state->obstacles_count++;
}

void remove_obstacle(gamestate_t *state, uint32_t index) {
  free(state->obstacles[index]);

  for (int j = index+1; j < state->obstacles_count; j++) {
    state->obstacles[j-1] = state->obstacles[j];
  }
  state->obstacles[state->obstacles_count] = 0;
  
  state->obstacles_count -= 1;
}