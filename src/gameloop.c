#include "gameloop.h"
#include "colors.h"
#include "font.h"
#include "general_utils.h"
#include "keyboard.h"
#include "line.h"
#include "quad.h"
#include "random.h"
#include "timer.h"
#include "alloc.h"
#include "sprites.h"

const uint32_t LOOP_FREQUENCY = 60;

const uint32_t BEAR_X = 80;
const uint32_t BEAR_WIDTH = 32;
const uint32_t BEAR_STANDING_HEIGHT = 48;
const uint32_t BEAR_DUCKING_HEIGHT = 25;
const float BEAR_DUCKING_VELOCITY = -600;

const uint32_t OBSTACLE_WIDTH = 30;
const uint32_t OBSTACLE_HEIGHT = 30;
const uint32_t OBSTACLE_Y_INC = 35;
const float BEE_RATIO = 0.5;

const uint32_t OBSTACLE_SPAWN_MIN_MILLIS = 500;
const uint32_t OBSTACLE_SPAWN_MAX_MILLIS = 1200;

const uint32_t GROUND = 200;
const uint32_t GROUND_THICKNESS = 2;

const float GRAVITY = -2500.0;
const float JUMP_VELOCITY = 700.0;

const float START_RUN_VELOCITY = 250.0;
const float MAX_RUN_VELOCITY = 1500.0;
const float RUN_VELOCITY_INCREMENT_RATE = 15.0;

void gameloop(FrameBuffer *fb) {
  Color background = {0, 0, 0, 255};

  clear_screen(fb, background);

  gamestate_t state = {.fb = fb};
  reset_game_state(&state);

  uint32_t last_millis = millis();
  while (1) {
    if (state.phase != EndScreen) {
      clear_screen(fb, background);
    }

    if (state.phase == Title) {
      title_tick(&state);
    } else if (state.phase == Game) {
      game_tick(&state);
    } else {
      endscreen_tick(&state);
    }

    if (check_pressed(Enter) && state.phase != Title) {
      state.phase = Title;
      reset_game_state(&state);
    } else if (check_pressed(Up) && state.phase == Title) {
      state.phase = Game;
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
  render_game(state);
}

void game_tick(gamestate_t *state) {
  // generate obstacle if necessary
  if (state->next_obstacle_millis < millis()) {
    float type_rng = random_next(&state->rng_state);
    add_obstacle(state, (obstacle_state_t) {
      .type = type_rng > BEE_RATIO ? Hive : type_rng > BEE_RATIO/2.0 ? Bee : HighBee,
      .x = state->fb->width-OBSTACLE_WIDTH
    });
    state->next_obstacle_millis = millis()+(random_next(&state->rng_state)*(OBSTACLE_SPAWN_MAX_MILLIS-OBSTACLE_SPAWN_MIN_MILLIS))+OBSTACLE_SPAWN_MIN_MILLIS;
  }

  // update bear speed and score
  state->bear_velocity_x += RUN_VELOCITY_INCREMENT_RATE*state->dt;
  if (state->bear_velocity_x > MAX_RUN_VELOCITY) state->bear_velocity_x = MAX_RUN_VELOCITY;
  state->score += (state->bear_velocity_x*state->dt)/10.0;

  // update game obstacles
  for (int i = 0; i < state->obstacles_count; i++) {
    obstacle_state_t* obs = state->obstacles[i];
    obs->x -= state->bear_velocity_x*state->dt;

    // if obstacle done, remove if and shift everything else down by 1
    if (obs->x < OBSTACLE_WIDTH/2.0) {
      remove_obstacle(state, i);

      i--;
    }
  }

  // compute bear height/y
  if (state->bear_y == 0) {
    // if jump, then give bear upwards velocity
    if (check_pressed(Up)) {
      state->bear_velocity_y = JUMP_VELOCITY;
    }
  }
  if (check_pressed(Down)) {
    state->bear_height = BEAR_DUCKING_HEIGHT;
    state->bear_velocity_y = BEAR_DUCKING_VELOCITY;
  } else {
    state->bear_height = BEAR_STANDING_HEIGHT;
  }

  // compute velocity change and offset
  state->bear_velocity_y += GRAVITY*state->dt;
  state->bear_y += state->bear_velocity_y*state->dt;

  if (state->bear_y <= 0) {
    state->bear_y = 0;
    state->bear_velocity_y = 0;
  }

  render_game(state);

  // if collision, set for return
  if (check_collisions(state) == 1) {
    state->phase = EndScreen;
  }
}

void endscreen_tick(gamestate_t *state) {
  draw_text(&((PixelList){}), "Game Over!", (Point){220, 50}, WHITE, 2);
}

void reset_game_state(gamestate_t *state) {
  state->score = 0;

  state->rng_state = (random_state_t) {.seed = millis()};
  state->dt = 1.0/LOOP_FREQUENCY;

  state->bear_height = BEAR_STANDING_HEIGHT;
  state->bear_y = 0;
  state->bear_velocity_y = 0;
  state->bear_velocity_x = START_RUN_VELOCITY;

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

uint8_t check_collisions(gamestate_t *state) {
  for (int i = 0; i < state->obstacles_count; i++) {
    obstacle_state_t* obs = state->obstacles[i];

    // check if x and y align
    if (
      abs(BEAR_X-obs->x) < (BEAR_WIDTH/2.0)+(OBSTACLE_WIDTH/2.0)
      && abs(state->bear_y-obstacle_y(obs->type)) < (state->bear_height/2.0)+(OBSTACLE_HEIGHT/2.0)
    ) {
      return 1;
    }
  }

  return 0;
}

void render_game(gamestate_t *state) {
  // define PixelLists for each element
  int item_count = 4 + state->obstacles_count; // 4 items always plus the obstacles
  Pixel floor_pixels[(GROUND + GROUND_THICKNESS) * state->fb->width];
  Pixel bear_pixels[BEAR_WIDTH * BEAR_STANDING_HEIGHT];
  PixelList floor = {.pixels = floor_pixels, .count = 0, .capacity = sizeof(floor_pixels) / sizeof(floor_pixels[0])};
  PixelList bear = {.pixels = bear_pixels, .count = 0, .capacity = sizeof(bear_pixels) / sizeof(bear_pixels[0])};
  DrawItem items[item_count];

  // render floor
  draw_line(&floor, (Point){0, GROUND+GROUND_THICKNESS-1}, (Point){state->fb->width, GROUND+GROUND_THICKNESS-1}, WHITE, 0, GROUND_THICKNESS);
  items[0] = (DrawItem){.shape = &floor, .origin = (Point){0, 0}};

  // render bear
  draw_bitmap(&bear, &BEAR_STANDING, (Point){BEAR_X, GROUND + GROUND_THICKNESS}, WHITE, 1);
  items[1] = (DrawItem){.shape = &bear, .origin = (Point){0, 0}};  

  // render score
  char score_string[19] = "Score: ";
  uint_to_string(state->score, score_string+7, 11);
  Pixel score_pixels[FONT_HEIGHT * FONT_WIDTH * 19];
  PixelList score = {.pixels = score_pixels, .count = 0, .capacity = sizeof(score_pixels) / sizeof(score_pixels[0])};
  draw_text(&score, score_string, (Point){BEAR_X-(BEAR_WIDTH/2.0), GROUND+30}, WHITE, 1);
  items[2] = (DrawItem){.shape = &score, .origin = (Point){0, 0}};  

  // render phase specific text
  if (state->phase == Title) {  
    Pixel start_text_pixels[FONT_WIDTH * FONT_HEIGHT * 24 * 16];
    PixelList start_text = {.pixels = start_text_pixels, .count = 0, .capacity = 2048};
    draw_text(&start_text, "Press up arrow to start!", (Point){80, 300}, WHITE, 2);
    items[3] = (DrawItem){.shape = &start_text, .origin = (Point){0, 0}};
  }

  // render all obstacles
  for (int i = 0; i < state->obstacles_count; i++) {    
    obstacle_state_t* obs = state->obstacles[i];

    uint32_t left = obs->x-(OBSTACLE_WIDTH/2.0);
    uint32_t right = obs->x+(OBSTACLE_WIDTH/2.0);
    uint32_t top = GROUND-OBSTACLE_HEIGHT-obstacle_y(obs->type);
    uint32_t bottom = GROUND-obstacle_y(obs->type);

    Point corners[4] = {(Point){left, bottom}, (Point){right, bottom}, (Point){right, top}, (Point){left, top}};
    fill_quad(&floor, corners, WHITE, 0); // TODO: Create new pixellist
  }

  // now render the whole scene
  draw_scene(state->fb, BLACK, items, item_count);
}

uint32_t obstacle_y(obstacle_type_t type) {
  switch (type) {
    case Hive: return 0;
    case Bee: return OBSTACLE_Y_INC;
    case HighBee: return OBSTACLE_Y_INC*2;
  }
  return 0;
}
