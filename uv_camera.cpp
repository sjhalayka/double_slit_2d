// Shawn Halayka
// This code and data is in the public domain.


#include "uv_camera.h"

const real_type pi = 4.0f*atan(1.0f);
const real_type pi_half = 0.5f*pi;
const real_type pi_2 = 2.0f*pi;
const real_type epsilon = 1e-6f;



uv_camera::uv_camera(void)
{
	u = v = 0;
	w = 4;
	fov = 45;
	near_plane = 1.0;
	far_plane = 4.0;
	win_x = win_y = 0;
}

void uv_camera::Set(const real_type u_rad, const real_type v_rad, const real_type w_metres, const real_type fov_deg, const int width_px, const int height_px, real_type src_near, real_type src_far)
{
	u = u_rad;
	v = v_rad;
	w = w_metres;
	near_plane = src_near;
	far_plane = src_far;

	static const real_type lock = epsilon * 1000.0f;

	if(u < -pi_half + lock)
		u = -pi_half + lock;
	else if(u > pi_half - lock)
		u = pi_half - lock;

	while(v < 0)
		v += pi_2;

	while(v > pi_2)
		v -= pi_2;

	if(w < 0)
		w = 0;
//	else if(w > 10000)
//		w = 10000;

	fov = fov_deg;
	win_x = width_px;
	win_y = height_px;

	Transform();
}


void uv_camera::Transform(void)
{
	Reset();
	Rotate();
	Translate();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(
		fov, 
		static_cast<GLfloat>(win_x)/static_cast<GLfloat>(win_y), 
		near_plane, far_plane);

	gluLookAt(
		eye.x, eye.y, eye.z, // Eye position.
		eye.x + look_at.x, eye.y + look_at.y, eye.z + look_at.z, // Look at position (not direction).
		up.x, up.y, up.z); // Up direction vector.
}

void uv_camera::Set(void)
{
	// Force a recalculation of the camera vectors and frustum.
	Set(u, v, w, fov, static_cast<int>(win_x), static_cast<int>(win_y), near_plane, far_plane);
}

void uv_camera::Set_Large_Screenshot(size_t num_cams, size_t cam_index_x, size_t cam_index_y)
{
	// No guarantees about the behaviour of this functionality. It wasn't tested a lot.

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Image plane reference:
	// http://www.songho.ca/opengl/gl_transform.html
    const real_type deg_to_rad = (1.0/360.0)*2*pi;
	real_type aspect = win_x/win_y;
    real_type tangent = tan((fov/2.0)*deg_to_rad);
    real_type height = near_plane * tangent; // Half height of near_plane plane.
    real_type width = height * aspect; // Half width of near_plane plane.

	real_type cam_width = 2*width/num_cams;
	real_type cam_height = 2*height/num_cams;

	real_type left = -width + cam_index_x*cam_width;
	real_type right = -width + (cam_index_x + 1)*cam_width;
	real_type bottom = -height + cam_index_y*cam_height;
	real_type top = -height + (cam_index_y + 1)*cam_height;

	// Instead of gluPerspective...
    glFrustum(left, right, bottom, top, near_plane, far_plane);

	gluLookAt(
		eye.x, eye.y, eye.z, // Eye position.
		eye.x + look_at.x, eye.y + look_at.y, eye.z + look_at.z, // Look at position (not direction).
		up.x, up.y, up.z); // Up direction vector.
}

void uv_camera::Reset(void)
{
	eye.zero();
	look_at.zero();
	up.zero();
	right.zero();

	look_at.z = -1;
	up.y = 1;
	right.x = 1;
}

void uv_camera::Rotate(void)
{
	// Rotate about the world x axis
	look_at.rotate_x(u);
	up.rotate_x(u);
	// Right only rotates on the x axis

	// Rotate about the world y axis
	look_at.rotate_y(v);
	up.rotate_y(v);
	right.rotate_y(v);
}

void uv_camera::Translate(void)
{
	// Place the eye directly across the sphere from the look-at vector's "tip",
	// Then scale the sphere radius by w
	eye.x = -look_at.x*w;
	eye.y = -look_at.y*w;
	eye.z = -look_at.z*w;
}
