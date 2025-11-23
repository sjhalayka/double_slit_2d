#ifndef main_H
#define main_H

#include "uv_camera.h"
#include "custom_math.h"
using custom_math::vector_3;
using custom_math::vector_4;

using custom_math::line_segment_3;


#include <cstdlib>
#include <GL/glut.h>       //GLUT Library

#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>
using std::setprecision;

#include <vector>
using std::vector;

#include <string>
using std::string;
using std::to_string;

#include <sstream>
using std::ostringstream;
using std::istringstream;

#include <fstream>
using std::ofstream;
using std::ifstream;

#include <set>
using std::set;

#include <map>
using std::map;

#include <utility>
using std::pair;

#include <mutex>
using std::mutex;

#include <thread>
using std::thread;


#include <random>

#include <optional>
#include <utility>
using namespace std;

const real_type pi = 4.0 * atan(1.0);

const real_type dt = 0.001;
vector_3 light_position;
const size_t x_res = 50;
const size_t z_res = 50;
const size_t num_direction_vectors_per_aabb = 100;

std::mt19937 generator_int(static_cast<unsigned>(0));
std::uniform_int_distribution<long unsigned int> dis_int(0, num_direction_vectors_per_aabb - 1);

std::mt19937 generator_real(static_cast<unsigned>(0));
std::uniform_real_distribution<real_type> dis_real(0, 1);




void idle_func(void);
void init_opengl(const int& width, const int& height);
void reshape_func(int width, int height);
void display_func(void);
void keyboard_func(unsigned char key, int x, int y);
void mouse_func(int button, int state, int x, int y);
void motion_func(int x, int y);
void passive_motion_func(int x, int y);

void render_string(int x, const int y, void* font, const string& text);
void draw_objects(void);


custom_math::vector_3 background_colour(1.0f, 1.0f, 1.0f);
custom_math::vector_3 control_list_colour(0, 0, 0);

bool draw_axis = true;
bool draw_control_list = true;

uv_camera main_camera;

GLint win_id = 0;
GLint win_x = 800, win_y = 600;
real_type camera_w = 1;

real_type camera_fov = 45;
real_type camera_x_transform = 0;
real_type camera_y_transform = 0;
real_type u_spacer = 0.01f;
real_type v_spacer = 0.5f * u_spacer;
real_type w_spacer = 0.1f;
real_type camera_near = 0.01f;
real_type camera_far = 100.0f;

bool lmb_down = false;
bool mmb_down = false;
bool rmb_down = false;
int mouse_x = 0;
int mouse_y = 0;


#endif
