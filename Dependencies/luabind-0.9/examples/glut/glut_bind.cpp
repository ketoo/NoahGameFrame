extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/class.hpp>
#include <luabind/function.hpp>
#include <luabind/object.hpp>


#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

struct glut_constants {};
struct gl_constants {};

using luabind::object;

namespace glut_bindings
{
	object displayfunc;

	void displayfunc_callback()
	{
		displayfunc();
	}

	void set_displayfunc(object const& fun)
	{
		glutDisplayFunc(&displayfunc_callback);
		displayfunc = fun;
	}

	object idlefunc;

	void idlefunc_callback()
	{
		idlefunc();
	}

	void set_idlefunc(object const& fun)
	{
		glutIdleFunc(&idlefunc_callback);
		idlefunc = fun;
	}


	object reshapefunc;

	void reshapefunc_callback(int w, int h)
	{
		reshapefunc(w, h);
	}

	void set_reshapefunc(object const& fun)
	{
		reshapefunc = fun;
	}

	object keyboardfunc;

	void keyboardfunc_callback(unsigned char key, int x, int y)
	{
		keyboardfunc(key, x, y);
	}

	void set_keyboardfunc(object const& fun)
	{
		glutKeyboardFunc(&keyboardfunc_callback);
		keyboardfunc = fun;
	}

	object mousefunc;

	void mousefunc_callback(int button, int state, int x, int y)
	{
		mousefunc(button, state, x, y);
	}

	void set_mousefunc(object const& fun)
	{
		mousefunc = fun;
	}
}

void bind_glut(lua_State* L)
{
	using namespace luabind;
	using namespace glut_bindings;

	open(L);

	module(L)
	[	
		def("glutInitWindowSize", &glutInitWindowSize),
		def("glutInitWindowPosition", &glutInitWindowPosition),
		def("glutInitDisplayMode", &glutInitDisplayMode),

		class_<glut_constants>("glut")
			.enum_("constants")
			[
				value("RGB", GLUT_RGB),
				value("RGBA", GLUT_RGBA),
				value("INDEX", GLUT_INDEX),
				value("SINGLE", GLUT_SINGLE),
				value("DOUBLE", GLUT_DOUBLE),
				value("DEPTH", GLUT_DEPTH),
				value("STENCIL", GLUT_STENCIL),
				value("LEFT_BUTTON", GLUT_LEFT_BUTTON),
				value("MIDDLE_BUTTON", GLUT_MIDDLE_BUTTON),
				value("RIGHT_BUTTON", GLUT_RIGHT_BUTTON),
				value("UP", GLUT_UP),
				value("DOWN", GLUT_DOWN),
				value("ELAPSED_TIME", GLUT_ELAPSED_TIME)
			],

		def("glutCreateWindow", &glutCreateWindow),
		def("glutDestroyWindow", &glutDestroyWindow),
		def("glutFullScreen", &glutFullScreen),
		def("glutDisplayFunc", &set_displayfunc),
		def("glutKeyboardFunc", &set_keyboardfunc),
		def("glutReshapeFunc", &set_reshapefunc),
		def("glutIdleFunc", &set_idlefunc),
		def("glutMainLoop", &glutMainLoop),
		def("glutSwapBuffers", &glutSwapBuffers),
		def("glutGet", &glutGet),
		def("glutSolidSphere", &glutSolidSphere),
		def("glutWireSphere", &glutWireSphere),
		def("glutWireTeapot", &glutWireTeapot),
		def("glutSolidTeapot", &glutSolidTeapot),

		// -- opengl

		class_<gl_constants>("gl")
			.enum_("constants")
			[
				value("COLOR_BUFFER_BIT", GL_COLOR_BUFFER_BIT),
				value("DEPTH_BUFFER_BIT", GL_DEPTH_BUFFER_BIT),
				value("TRIANGLES", GL_TRIANGLES),
				value("MODELVIEW", GL_MODELVIEW),
				value("PROJECTION", GL_PROJECTION)
			],

		def("glBegin", &glBegin),
		def("glVertex3", &glVertex3f),
		def("glEnd", &glEnd),
		def("glClear", &glClear),
		def("glPushMatrix", &glPushMatrix),
		def("glPopMatrix", &glPopMatrix),
		def("glRotate", &glRotatef),
		def("glColor3", &glColor3f),
		def("glColor4", &glColor4f),
		def("glMatrixMode", &glMatrixMode),
		def("glLoadIdentity", &glLoadIdentity),
		def("glViewport", &glViewport),
		def("glTranslate", &glTranslatef),

		// -- glu

		def("gluPerspective", &gluPerspective)
	];
}

int main(int argc, char* argv[])
{
	lua_State* L = luaL_newstate();
	lua_baselibopen(L);
	lua_mathlibopen(L);
	bind_glut(L);

	glutInit (&argc, argv);

	lua_dofile(L, "glut_bindings.lua");

	lua_close(L);
	return 0;
}

