quit = false

function resize_func(w, h)
	local ratio = w / h

	print('====== resize')

	glMatrixMode(gl.PROJECTION)
	glLoadIdentity()

	glViewport(0,0,w,h)

	gluPerspective(45,ratio,1,1000)

	glMatrixMode(gl.MODELVIEW)
	glLoadIdentity()
end

angle = 0
angle2 = 0
previous_time = 0

function display_func()

	if quit then return end

	local cur_time = glutGet(glut.ELAPSED_TIME)
	local delta = (cur_time - previous_time) / 1000
	previous_time = cur_time

	glClear(gl.COLOR_BUFFER_BIT + gl.DEPTH_BUFFER_BIT)

	glPushMatrix()

	glTranslate(0,0,-5)
	glRotate(angle, 0, 1, 0)
	glRotate(angle2, 0, 0, 1)
	glColor3(1,0,0)
--	glutWireSphere(0.75, 10, 10)
	glutSolidTeapot(0.75)
--	glColor3(1,1,1)
--	glutWireTeapot(0.75)

	glPopMatrix()

	angle = angle + 200 * delta
	angle2 = angle2 + 170 * delta

	frames = frames + 1

	if math.mod(frames, 100) == 0 then
		local fps = frames * 1000 / (glutGet(glut.ELAPSED_TIME) - start_time);
		print('fps: ' .. fps .. ' time: ' .. glutGet(glut.ELAPSED_TIME) .. ' frames: ' .. frames)
	end

	glutSwapBuffers()
end

function keyboard_func(key,x,y)
	print('keyboard' .. key)
	if key == 27 then glutDestroyWindow(window) quit = true end
end

glutInitWindowSize(600,600)
glutInitWindowPosition(0,0)
glutInitDisplayMode(glut.RGB + glut.DOUBLE + glut.DEPTH)
window = glutCreateWindow("luabind, glut-bindings")
glutDisplayFunc(display_func)
glutIdleFunc(display_func)
glutKeyboardFunc(keyboard_func)
glutReshapeFunc(resize_func)

resize_func(600,600)

start_time = glutGet(glut.ELAPSED_TIME)
frames = 0

glutMainLoop()

