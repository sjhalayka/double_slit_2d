#ifndef uv_camera_h
#define uv_camera_h

#include <cstdlib>
#include <GL/glut.h>       //GLUT Library

#include "custom_math.h"




// UV camera
//
// latitude:     | longitude:    | radius:       |
//       *_*_    |        ___    |        ___    |
//      */   \   |       /   \   |       /   \   |
// u:  *|  x  |  |  v:  |**x**|  |  w:  |  x**|  |
//      *\___/   |       \___/   |       \___/   |
//       * *     |               |               |
// 

class uv_camera
{
public:
	// Use as read-only
	real_type u, v, w, fov;
	real_type win_x, win_y;
    custom_math::vector_3 eye, look_at, up, right;
	real_type near_plane;
	real_type far_plane;

public:
	uv_camera(void);

	// Must initialize or change camera settings through these two functions
	void Set(const real_type u_rad, const real_type v_rad, const real_type w_metres, const real_type fov_deg, const int width_px, const int height_px, real_type src_near, real_type src_far);
	void Set(void);
	void Set_Large_Screenshot(size_t num_cams, size_t cam_num_x, size_t cam_num_y);
protected:
	void Transform(void);
	void Reset(void);
	void Rotate(void);
	void Translate(void);
};


#endif
