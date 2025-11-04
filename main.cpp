#include "main.h"

class AABB
{
public:
	vector_3 min_location;
	vector_3 max_location;
};

AABB apparatus_bounds;
AABB double_slit_boundaries[3];
AABB receiver;


class photon
{
public:
	vector_3 velocity;
	vector_3 position;
};

vector<photon> hit_receiver;
vector<photon> hit_boundaries;
vector<photon> hit_apparatus_bounds;




bool intersect_AABB_2D_xz(const AABB &aabb, const vector_3& point)
{
	if (aabb.min_location.x <= point.x && aabb.max_location.x >= point.x &&
		aabb.min_location.z <= point.z && aabb.max_location.z >= point.z)
	{
		return true;
	}

	return false;
}

int main(int argc, char** argv)
{
	apparatus_bounds.min_location = vector_3(-1.0, 0, -1.0);
	apparatus_bounds.max_location = vector_3(1.0, 0, 1.0);
	receiver.min_location = vector_3(-1.0, 0, 1.0);
	receiver.max_location = vector_3(1.0, 0, 1.2);
	double_slit_boundaries[0].min_location = vector_3(0.2, 0.01, -0.1);
	double_slit_boundaries[0].max_location = vector_3(1.0, 0.01, 0.1);
	double_slit_boundaries[1].min_location = vector_3(-0.1, 0.01, -0.1);
	double_slit_boundaries[1].max_location = vector_3(0.1, 0.01, 0.1);
	double_slit_boundaries[2].min_location = vector_3(-1.0, 0.01, -0.1);
	double_slit_boundaries[2].max_location = vector_3(-0.2, 0.01, 0.1);

	cout << setprecision(20) << endl;

	glutInit(&argc, argv);
	init_opengl(win_x, win_y);
	glutReshapeFunc(reshape_func);
	glutIdleFunc(idle_func);
	glutDisplayFunc(display_func);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);
	glutMotionFunc(motion_func);
	glutPassiveMotionFunc(passive_motion_func);
	//glutIgnoreKeyRepeat(1);
	glutMainLoop();
	glutDestroyWindow(win_id);



}


// Add this function to draw an AABB using triangles
void draw_AABB(const AABB& aabb, float r, float g, float b, float alpha = 1.0f)
{
	vector_3 min = aabb.min_location;
	vector_3 max = aabb.max_location;

	glColor4f(r, g, b, alpha);

	// Enable blending if using transparency
	if (alpha < 1.0f)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glBegin(GL_TRIANGLES);

	// Front face (z = max.z)
	glVertex3d(min.x, min.y, max.z);
	glVertex3d(max.x, min.y, max.z);
	glVertex3d(max.x, max.y, max.z);

	glVertex3d(min.x, min.y, max.z);
	glVertex3d(max.x, max.y, max.z);
	glVertex3d(min.x, max.y, max.z);

	// Back face (z = min.z)
	glVertex3d(min.x, min.y, min.z);
	glVertex3d(min.x, max.y, min.z);
	glVertex3d(max.x, max.y, min.z);

	glVertex3d(min.x, min.y, min.z);
	glVertex3d(max.x, max.y, min.z);
	glVertex3d(max.x, min.y, min.z);

	// Left face (x = min.x)
	glVertex3d(min.x, min.y, min.z);
	glVertex3d(min.x, min.y, max.z);
	glVertex3d(min.x, max.y, max.z);

	glVertex3d(min.x, min.y, min.z);
	glVertex3d(min.x, max.y, max.z);
	glVertex3d(min.x, max.y, min.z);

	// Right face (x = max.x)
	glVertex3d(max.x, min.y, min.z);
	glVertex3d(max.x, max.y, max.z);
	glVertex3d(max.x, min.y, max.z);

	glVertex3d(max.x, min.y, min.z);
	glVertex3d(max.x, max.y, min.z);
	glVertex3d(max.x, max.y, max.z);

	// Top face (y = max.y)
	glVertex3d(min.x, max.y, min.z);
	glVertex3d(min.x, max.y, max.z);
	glVertex3d(max.x, max.y, max.z);

	glVertex3d(min.x, max.y, min.z);
	glVertex3d(max.x, max.y, max.z);
	glVertex3d(max.x, max.y, min.z);

	// Bottom face (y = min.y)
	glVertex3d(min.x, min.y, min.z);
	glVertex3d(max.x, min.y, max.z);
	glVertex3d(min.x, min.y, max.z);

	glVertex3d(min.x, min.y, min.z);
	glVertex3d(max.x, min.y, min.z);
	glVertex3d(max.x, min.y, max.z);

	glEnd();

	if (alpha < 1.0f)
	{
		glDisable(GL_BLEND);
	}
}


void idle_func(void)
{


	glutPostRedisplay();
}

void init_opengl(const int& width, const int& height)
{
	win_x = width;
	win_y = height;

	if (win_x < 1)
		win_x = 1;

	if (win_y < 1)
		win_y = 1;

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(win_x, win_y);
	win_id = glutCreateWindow("orbit");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor((float)background_colour.x, (float)background_colour.y, (float)background_colour.z, 1);
	glClearDepth(1.0f);

	main_camera.Set(0, 0, camera_w, camera_fov, win_x, win_y, camera_near, camera_far);
}

void reshape_func(int width, int height)
{
	win_x = width;
	win_y = height;

	if (win_x < 1)
		win_x = 1;

	if (win_y < 1)
		win_y = 1;

	glutSetWindow(win_id);
	glutReshapeWindow(win_x, win_y);
	glViewport(0, 0, win_x, win_y);

	main_camera.Set(main_camera.u, main_camera.v, main_camera.w, main_camera.fov, win_x, win_y, camera_near, camera_far);
}

// Text drawing code originally from "GLUT Tutorial -- Bitmap Fonts and Orthogonal Projections" by A R Fernandes
void render_string(int x, const int y, void* font, const string& text)
{
	for (size_t i = 0; i < text.length(); i++)
	{
		glRasterPos2i(x, y);
		glutBitmapCharacter(font, text[i]);
		x += glutBitmapWidth(font, text[i]) + 1;
	}
}
// End text drawing code.

void draw_objects(void)
{
	glDisable(GL_LIGHTING);

	glPushMatrix();


	glPointSize(5.0f);
	glLineWidth(1.0f);



	draw_AABB(apparatus_bounds, 1.0, 0.5, 0.0, 1.0f);
	draw_AABB(receiver, 1.0, 0.5, 1.0, 1.0f);
	draw_AABB(double_slit_boundaries[0], 1.0, 0.0, 0.0, 1.0f);
	draw_AABB(double_slit_boundaries[1], 0.0, 1.0, 0.0, 1.0f);
	draw_AABB(double_slit_boundaries[2], 1.0, 0.0, 0.0, 1.0f);


	glBegin(GL_POINTS);

	glColor3f(0, 0, 1);

	for (size_t i = 0; i < hit_receiver.size(); i++)
		glVertex3d(hit_receiver[i].position.x, hit_receiver[i].position.y, hit_receiver[i].position.z);

	for (size_t i = 0; i < hit_boundaries.size(); i++)
		glVertex3d(hit_boundaries[i].position.x, hit_boundaries[i].position.y, hit_boundaries[i].position.z);

	for (size_t i = 0; i < hit_apparatus_bounds.size(); i++)
		glVertex3d(hit_apparatus_bounds[i].position.x, hit_apparatus_bounds[i].position.y, hit_apparatus_bounds[i].position.z);

	glEnd();


	if (true == draw_axis)
	{
		glLineWidth(1.0f);

		glBegin(GL_LINES);

		glColor3f(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(1, 0, 0);
		glColor3f(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 1, 0);
		glColor3f(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, 1);

		glColor3f(0.5, 0.5, 0.5);
		glVertex3d(0, 0, 0);
		glVertex3d(-1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, -1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, -1);

		glEnd();
	}

	glPopMatrix();
}




void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the model's components using OpenGL/GLUT primitives.
	draw_objects();

	if (true == draw_control_list)
	{
		// Text drawing code originally from "GLUT Tutorial -- Bitmap Fonts and Orthogonal Projections" by A R Fernandes
		// http://www.lighthouse3d.com/opengl/glut/index.php?bmpfontortho
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, win_x, 0, win_y);
		glScaled(1, -1, 1); // Neat. :)
		glTranslated(0, -win_y, 0); // Neat. :)
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glColor3d(control_list_colour.x, control_list_colour.y, control_list_colour.z);

		size_t break_size = 22;
		size_t start = 20;
		ostringstream oss;

		render_string(10, static_cast<int>(start), GLUT_BITMAP_HELVETICA_18, string("Mouse controls:"));
		render_string(10, static_cast<int>(start + 1 * break_size), GLUT_BITMAP_HELVETICA_18, string("  LMB + drag: Rotate camera"));
		render_string(10, static_cast<int>(start + 2 * break_size), GLUT_BITMAP_HELVETICA_18, string("  RMB + drag: Zoom camera"));

		render_string(10, static_cast<int>(start + 4 * break_size), GLUT_BITMAP_HELVETICA_18, string("Keyboard controls:"));
		render_string(10, static_cast<int>(start + 5 * break_size), GLUT_BITMAP_HELVETICA_18, string("  w: Draw axis"));
		render_string(10, static_cast<int>(start + 6 * break_size), GLUT_BITMAP_HELVETICA_18, string("  e: Draw text"));
		render_string(10, static_cast<int>(start + 7 * break_size), GLUT_BITMAP_HELVETICA_18, string("  u: Rotate camera +u"));
		render_string(10, static_cast<int>(start + 8 * break_size), GLUT_BITMAP_HELVETICA_18, string("  i: Rotate camera -u"));
		render_string(10, static_cast<int>(start + 9 * break_size), GLUT_BITMAP_HELVETICA_18, string("  o: Rotate camera +v"));
		render_string(10, static_cast<int>(start + 10 * break_size), GLUT_BITMAP_HELVETICA_18, string("  p: Rotate camera -v"));



		custom_math::vector_3 eye = main_camera.eye;
		custom_math::vector_3 eye_norm = eye;
		eye_norm.normalize();

		oss.clear();
		oss.str("");
		oss << "Camera position: " << eye.x << ' ' << eye.y << ' ' << eye.z;
		render_string(10, static_cast<int>(win_y - 2 * break_size), GLUT_BITMAP_HELVETICA_18, oss.str());

		oss.clear();
		oss.str("");
		oss << "Camera position (normalized): " << eye_norm.x << ' ' << eye_norm.y << ' ' << eye_norm.z;
		render_string(10, static_cast<int>(win_y - break_size), GLUT_BITMAP_HELVETICA_18, oss.str());

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		// End text drawing code.
	}

	glFlush();
	glutSwapBuffers();
}


vector_3 randomPointOnCircle_xz(double radius) 
{
	real_type angle = static_cast<real_type>(rand()) / RAND_MAX * 2.0f * pi;

	// Convert polar coordinates to Cartesian
	vector_3 v;
	v.x = radius * cos(angle);
	v.y = 0;
	v.z = radius * sin(angle);

	return v;
}



void keyboard_func(unsigned char key, int x, int y)
{
	switch (tolower(key))
	{
	case 'z':
	{
		photon p;
		p.position = vector_3(0, 0.0, -0.9);
		p.velocity = randomPointOnCircle_xz(0.001);

		//hit_receiver.push_back(p);
		while (1)
		{
			p.position += p.velocity * dt;

			if (true == intersect_AABB_2D_xz(double_slit_boundaries[0], p.position) ||
				true == intersect_AABB_2D_xz(double_slit_boundaries[1], p.position) ||
				true == intersect_AABB_2D_xz(double_slit_boundaries[2], p.position))
			{
				hit_boundaries.push_back(p);
				break;
			}
			else if (true == intersect_AABB_2D_xz(receiver, p.position))
			{
				hit_receiver.push_back(p);
				break;
			}
			else if (false == intersect_AABB_2D_xz(apparatus_bounds, p.position))
			{
				hit_apparatus_bounds.push_back(p);
				break;
			}

		}

		break;
	}


	case 'w':
	{
		draw_axis = !draw_axis;
		break;
	}
	case 'e':
	{
		draw_control_list = !draw_control_list;
		break;
	}
	case 'u':
	{
		main_camera.u -= u_spacer;
		main_camera.Set();
		break;
	}
	case 'i':
	{
		main_camera.u += u_spacer;
		main_camera.Set();
		break;
	}
	case 'o':
	{
		main_camera.v -= v_spacer;
		main_camera.Set();
		break;
	}
	case 'p':
	{
		main_camera.v += v_spacer;
		main_camera.Set();
		break;
	}

	default:
		break;
	}
}

void mouse_func(int button, int state, int x, int y)
{
	if (GLUT_LEFT_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			lmb_down = true;
		else
			lmb_down = false;
	}
	else if (GLUT_MIDDLE_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			mmb_down = true;
		else
			mmb_down = false;
	}
	else if (GLUT_RIGHT_BUTTON == button)
	{
		if (GLUT_DOWN == state)
			rmb_down = true;
		else
			rmb_down = false;
	}
}

void motion_func(int x, int y)
{
	int prev_mouse_x = mouse_x;
	int prev_mouse_y = mouse_y;

	mouse_x = x;
	mouse_y = y;

	int mouse_delta_x = mouse_x - prev_mouse_x;
	int mouse_delta_y = prev_mouse_y - mouse_y;

	if (true == lmb_down && (0 != mouse_delta_x || 0 != mouse_delta_y))
	{
		main_camera.u -= static_cast<float>(mouse_delta_y) * u_spacer;
		main_camera.v += static_cast<float>(mouse_delta_x) * v_spacer;
	}
	else if (true == rmb_down && (0 != mouse_delta_y))
	{
		main_camera.w -= static_cast<float>(mouse_delta_y) * w_spacer;

		if (main_camera.w < 1.1f)
			main_camera.w = 1.1f;

	}

	main_camera.Set(); // Calculate new camera vectors.
}

void passive_motion_func(int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}
