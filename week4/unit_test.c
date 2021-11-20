/*
 * unit_test.c
 *
 *  Created on: Nov 3, 2021
 *      Author: maxpettit
 */


#include "unit_test.h"


static int time0, time1;
//***********************************************************************************
// function prototypes
//***********************************************************************************
static void angle_task(void);
static void throttle_task(void);
static void rocket_display(void);
static void config_input(void);
static void pwm_task(void);
static void lcd_display(void);
static void physics_task(void);
static void pwm_test_callback(OS_TMR * p_tmr, void * p_arg);



/***************************************************************************//**
 * @brief
 *      Runs unit tests
 *
 * @details
 *      Calls respective unit testing functions.
 *
 * @return
 *      Error status. 0 indicates no error and all tests passed.
 *
 ******************************************************************************/
int run_tests(void){
  // each test has if statement so tests can be
  // switched on and off allowing for targeting
  // of specific tests

  bool run_all = true;

  if(run_all) angle_task();

  if(run_all) throttle_task();

  if(run_all) rocket_display();

  if(run_all) lcd_display();

  if(false) config_input();

  if(false) pwm_task();

  if(run_all) physics_task();


  return UNIT_NO_ERROR;
}


void angle_task(void){

//**********************************************************
// Angle, Unit Test: 1
//**********************************************************
  init_quanta(ANGLE_DELTA);
  int old_angle, new_angle;
  old_angle = angle_get();

  angle_increment();
  new_angle = angle_get();
  EFM_ASSERT(old_angle + ANGLE_DELTA == new_angle);

  old_angle = new_angle;
  angle_increment();
  new_angle = angle_get();
  EFM_ASSERT(old_angle + ANGLE_DELTA == new_angle);

  old_angle = new_angle;
  angle_decrement();
  new_angle = angle_get();
  EFM_ASSERT(old_angle - ANGLE_DELTA == new_angle);

  old_angle = new_angle;
  angle_decrement();
  new_angle = angle_get();
  EFM_ASSERT(old_angle - ANGLE_DELTA == new_angle);


  return;
}


void throttle_task(void){

//**********************************************************
// Fuel Throttle, Unit Test: 2
//**********************************************************

  int fuel;
  uint32_t pos;

  pos = 0;
  fuel_set(pos);
  fuel = fuel_get();
  EFM_ASSERT(fuel == 0);

  pos = 1;
  fuel_set(pos);
  fuel = fuel_get();
  EFM_ASSERT(fuel == 1);

  pos = 2;
  fuel_set(pos);
  fuel = fuel_get();
  EFM_ASSERT(fuel == 2);

  pos = 3;
  fuel_set(pos);
  fuel = fuel_get();
  EFM_ASSERT(fuel == 3);

  pos = 4;
  fuel_set(pos);
  fuel = fuel_get();
  EFM_ASSERT(fuel == 4);

  return;
}


void rocket_display(void){

//**********************************************************
// Rocket, Unit Test: 3
//**********************************************************

  // rocket test 1 translation (not rotation)
  // check expected vertices

  ROCKET_STRUCT test_rocket;
  test_rocket.x_loc = 20;
  test_rocket.y_loc = 20;
  test_rocket.angle = 0;

  POLYGON_STRUCT test_tri;

  get_vertices(&test_tri, &test_rocket);
  EFM_ASSERT(test_tri.vf_x == test_rocket.x_loc);
  EFM_ASSERT(test_tri.vf_y == test_rocket.y_loc);
  EFM_ASSERT(test_tri.vL_x == test_rocket.x_loc - SHIP_HLF_WIDTH);
  EFM_ASSERT(test_tri.vL_y == test_rocket.y_loc - SHIP_HEIGHT);
  EFM_ASSERT(test_tri.vR_x == test_rocket.x_loc + SHIP_HLF_WIDTH);
  EFM_ASSERT(test_tri.vR_y == test_rocket.y_loc - SHIP_HEIGHT);


  // rocket test 2
  // check expected vertices after translation

  test_rocket.x_loc = 40;
  test_rocket.y_loc = 40;
  test_rocket.angle = 0;

  get_vertices(&test_tri, &test_rocket);
  EFM_ASSERT(test_tri.vf_x == test_rocket.x_loc);
  EFM_ASSERT(test_tri.vf_y == test_rocket.y_loc);
  EFM_ASSERT(test_tri.vL_x == test_rocket.x_loc - SHIP_HLF_WIDTH);
  EFM_ASSERT(test_tri.vL_y == test_rocket.y_loc - SHIP_HEIGHT);
  EFM_ASSERT(test_tri.vR_x == test_rocket.x_loc + SHIP_HLF_WIDTH);
  EFM_ASSERT(test_tri.vR_y == test_rocket.y_loc - SHIP_HEIGHT);


//**********************************************************
// Rocket, Unit Test 4
//**********************************************************

  // Rotation test

  test_rocket.x_loc = 40;
  test_rocket.y_loc = 40;
  test_rocket.angle = 90;

  get_vertices(&test_tri, &test_rocket);
  EFM_ASSERT(test_tri.vf_x == test_rocket.x_loc);
  EFM_ASSERT(test_tri.vf_y == test_rocket.y_loc);
  EFM_ASSERT(test_tri.vL_x == test_rocket.x_loc + SHIP_HEIGHT);
  EFM_ASSERT(test_tri.vL_y == test_rocket.y_loc - SHIP_HLF_WIDTH);
  EFM_ASSERT(test_tri.vR_x == test_rocket.x_loc + SHIP_HEIGHT);
  EFM_ASSERT(test_tri.vR_y == test_rocket.y_loc + SHIP_HLF_WIDTH);


  // rocket test 4
  // check expected vertices after CCW rotation

  test_rocket.x_loc = 40;
  test_rocket.y_loc = 40;
  test_rocket.angle = -90;

  get_vertices(&test_tri, &test_rocket);
  EFM_ASSERT(test_tri.vf_x == test_rocket.x_loc);
  EFM_ASSERT(test_tri.vf_y == test_rocket.y_loc);
  EFM_ASSERT(test_tri.vL_x == test_rocket.x_loc - SHIP_HEIGHT);
  EFM_ASSERT(test_tri.vL_y == test_rocket.y_loc + SHIP_HLF_WIDTH);
  EFM_ASSERT(test_tri.vR_x == test_rocket.x_loc - SHIP_HEIGHT);
  EFM_ASSERT(test_tri.vR_y == test_rocket.y_loc - SHIP_HLF_WIDTH);


  // rocket test 5
  // check expected vertices after non 90 deg rotation

  test_rocket.x_loc = 40;
  test_rocket.y_loc = 40;
  test_rocket.angle = ANGLE_DELTA;

  get_vertices(&test_tri, &test_rocket);
  EFM_ASSERT(test_tri.vf_x == test_rocket.x_loc);
  EFM_ASSERT(test_tri.vf_y == test_rocket.y_loc);
  EFM_ASSERT(test_tri.vL_x > test_rocket.x_loc - SHIP_HLF_WIDTH);
  EFM_ASSERT(test_tri.vL_y < test_rocket.y_loc - SHIP_HEIGHT);
  EFM_ASSERT(test_tri.vR_x > test_rocket.x_loc + SHIP_HLF_WIDTH);
  EFM_ASSERT(test_tri.vR_y > test_rocket.y_loc - SHIP_HEIGHT);

  return;
}


void lcd_display(void){

//**********************************************************
// LCD Display, Unit Test: 5
//**********************************************************

  lcd_clear();
  ROCKET_STRUCT test_rocket;
  test_rocket.x_loc = DISP_CENTER;
  test_rocket.y_loc = DISP_CENTER;
  test_rocket.angle = 0;

  char * str = "LCD DISPLAY TEST";
  lcd_print_text(str);
  update_lcd();

  int32_t points[] = {10, 20, 60, 70, 10, 70};
  lcd_polygon(SHIP_NUM_PTS, points);
  update_lcd();

  for(int i = 0; i < 3000000; i++);
  POLYGON_STRUCT test_tri;

  get_vertices(&test_tri, &test_rocket);
  disp_rocket(&test_tri);
  for(int k = 0; k < 100000; k++);

  for(int j = -360; j< 361; j+=5){
      test_rocket.angle = j;
      get_vertices(&test_tri, &test_rocket);
      disp_rocket(&test_tri);
      for(int k = 0; k < 25000; k++);
  }

  return;
}



void config_input(void){

//**********************************************************
// Config, Unit Test: 6
//**********************************************************
  // Given test file input
  // check config stored correctly


  CONFIG_STRUCT *config;
  config = get_config();
  EFM_ASSERT(config->version == 1);
  EFM_ASSERT(config->init_fuel == 100);
  EFM_ASSERT(config->maxspeedx == 10);
  EFM_ASSERT(config->maxspeedy == 10);
  EFM_ASSERT(config->gravity == 10);


  return;
}


static void pwm_test_callback(OS_TMR * p_tmr, void * p_arg){
  PP_UNUSED_PARAM(p_arg);
  PP_UNUSED_PARAM(p_tmr);

  if(time0 == -1) time0 = get_ticks();
  else{
      time1 = get_ticks();
      EFM_ASSERT((time1 - time0) == 1000);
  }
}



void pwm_task(void){

//**********************************************************
// PWM, Unit Test: 7
//**********************************************************
  // Test PWM frequency correct through pwm_create
  // use pwm_callback to test time between timer callbacks
  // time using systick
  int freq = 1;
  int dc = 50;
  int null_cb = 0;

  time0 = -1;

  pwm_create(0, freq, dc, (OS_TMR_CALLBACK_PTR) pwm_test_callback, (OS_TMR_CALLBACK_PTR) null_cb);
  // pwm_test_callback handles test assertion for pass/fail

  return;
}


void physics_task(void){
//**********************************************************
// Physics, Unit Test: 8
//**********************************************************
  // Test read write to rocket struct
  ROCKET_STRUCT test_rocket;

  int newangle = 30;
  int32_t newx = 40, newy = 40;


  // Check get_rocket and initial postion is legal
  test_rocket = get_rocket();
  EFM_ASSERT(0 <= test_rocket.x_loc && test_rocket.x_loc < 2*DISP_CENTER);
  EFM_ASSERT(YSTART == test_rocket.y_loc);
  EFM_ASSERT(0 == test_rocket.angle);

  test_rocket.angle = newangle;
  test_rocket.x_loc = newx;
  test_rocket.y_loc = newy;

  // Check set_rocket
  set_rocket(&test_rocket);
  test_rocket = get_rocket();
  EFM_ASSERT(newx == test_rocket.x_loc);
  EFM_ASSERT(newy == test_rocket.y_loc);
  EFM_ASSERT(newangle == test_rocket.angle);


//**********************************************************
// Physics, Unit Test: 9
//**********************************************************
  // Test thrust
  int fuel_rate = 10;
  int thrust, expected_thrust = fuel_rate*FUEL_WEIGHT*4000;


  thrust = calc_thrust(fuel_rate);
  EFM_ASSERT(thrust == expected_thrust);


//**********************************************************
// PWM, Unit Test: 10
//**********************************************************
  // Test Blackout returns correctly

  /*
  bool blackout;
  thrust = calc_thrust(10*fuel_rate);
  blackout = get_blackout(thrust);
  EFM_ASSERT(blackout);
``*/

  return;
}


