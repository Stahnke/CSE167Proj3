#include "window.h"

const char* window_title = "GLFW Starter Project";

MatrixTransform * robotArmy;
Skybox * skybox;
GLint shaderProgram;
GLint skyboxShaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

#define SKYBOX_VERTEX_SHADER_PATH "../skyboxShader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skyboxShader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, -1.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

glm::vec3 NEARPPos = cam_pos;
glm::vec3 FARPPos = { cam_pos.x - 0.0f, cam_pos.y - 0.0f, cam_pos.z - 1000.0f };
glm::vec3 TOPPos = { 0.0f, 20.0f, 0.0f };
glm::vec3 BOTTOMPos = { 0.0f, -20.0f, 0.0f };
glm::vec3 LEFTPos = { -20.0f, 0.0f, 0.0f };
glm::vec3 RIGHTPos = { 20.0f, 0.0f, 0.0f };

glm::vec3 NEARPNorm = { 0.0f, 0.0f, 1.0f };
glm::vec3 FARPNorm = { 0.0f,0.0f,-1.0f };
glm::vec3 TOPNorm = { 0.0f, 1.0f, 0.0f };
glm::vec3 BOTTOMNorm = { 0.0f, -1.0f, 0.0f };
glm::vec3 LEFTNorm = { -1.0f, 0.0f, 0.0f };
glm::vec3 RIGHTNorm = { 1.0f, 0.0f, 0.0f };


int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;
glm::mat4 Window::camTransform;
glm::mat4 Window::camRotate;
glm::mat4 Window::camRotatePos;

int Movement;
const int NONE = 0;
const int ROTATE = 1;
const int TRANSLATE = 2;
const float m_ROTSCALE = 1.0f;
const float m_ZOOMSCALE = 1.0f;
const float m_TRANSSCALE = 0.1f;

int Mode = 1;
const int CAMERA = 1;
const int ARMY = 2;


Cube * cubeGrey;
Cube * cubeBlue;
Cube * cubeYellow;


SphereSolid * sphereWhite;
Sphere * sphereBlack;

glm::vec2 mouse_point;
glm::vec3 lastPoint;

//Frustum
glm::vec3 Window::planePos[6];
glm::vec3 Window::planeNorm[6];

bool Window::on = true;

void Window::initialize_objects()
{

	resetCamera();

	
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);

	glm::mat4 toWorld = glm::mat4(1.0f);
	robotArmy = new MatrixTransform(toWorld, false);
	
	const int COL_SIZE = 50;
	const int ROW_SIZE = 20;
	const float SPACING = 15.0f;

	//Create our prefab cube
	cubeGrey = new Cube(shaderProgram, { 0.5f, 0.0f, 0.5f });
	cubeBlue = new Cube(shaderProgram, { 0.0f, 0.0f, 1.0f });
	cubeYellow = new Cube(shaderProgram, { 1.0f, 1.0f, 0.0f });

	sphereWhite = new SphereSolid(shaderProgram, { 0.0f, 0.7f, 0.4f }, 1);
	sphereBlack = new Sphere(shaderProgram, { 0.0f, 0.0f, 0.0f }, 0);


	for (int i = 0; i < COL_SIZE; i++)
	{
		for (int j = 0; j < ROW_SIZE; j++)
		{
			MatrixTransform * curRobot = createRobot(toWorld);
			robotArmy->addChild(curRobot);
			toWorld = glm::translate(glm::mat4(1.0f), { SPACING, 0.0f, 0.0f }) * toWorld;
		}
		toWorld = glm::translate(glm::mat4(1.0f), { ROW_SIZE * -SPACING, 0.0f, -SPACING }) * toWorld;
	}

	//Set the skybox
	skybox = new Skybox();
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(robotArmy);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyboxShaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		//setCamInternals(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		//setCamDef(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	//cube->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Transform the camera
	//transformCamera();

	// Use the shader of programID
	glUseProgram(skyboxShaderProgram);

	// Render the skybox
	skybox->draw(skyboxShaderProgram);
	// Use the shader of programID
	glUseProgram(shaderProgram);


	robotArmy->update(on, planePos, planeNorm);
	// Render the robot army
	milliseconds ms1 = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());
	robotArmy->draw(V, 0, { 0.0f,0.0f,0.0f });
	milliseconds ms2 = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());

	cout << "Render Time: " << ms2.count() - ms1.count() << "ms" << endl;
	
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{

		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		//Reset camera
		else if (key == GLFW_KEY_R)
		{
			switch (Mode)
			{
			case CAMERA:
				resetCamera();
				break;
			case ARMY:
				resetArmy();
				break;
			}
		}

		//Camera movement mode
		else if (key == GLFW_KEY_1)
		{
			Mode = CAMERA;
		}

		//Army movement mode
		else if (key == GLFW_KEY_2)
		{
			Mode = ARMY;
		}

		else if (key == GLFW_KEY_C)
		{
			if (mods == GLFW_MOD_SHIFT)
				on = true;
			else
				on = false;
		}
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{

		//Camera movement controls (FPS style)
		if (key == GLFW_KEY_W)	//Forward
		{
			translateCamera({ 0.0f,0.0f,-1.0f });
			//cout << "Z" << mods << endl;
		}
		else if (key == GLFW_KEY_S) //Back
		{
			switch (Mode)
			{
			case CAMERA:
				translateCamera({ 0.0f,0.0f,1.0f });
				break;
			case ARMY:
				if(mods == GLFW_MOD_SHIFT)
					scaleArmy({ 2.0f,2.0f,2.0f });
				else
					scaleArmy({ 0.5f,0.5f,0.5f });
				break;
			}
		}
			//cout << "z" << endl;
		else if (key == GLFW_KEY_A) //Left
		{
			translateCamera({ -1.0f,0.0f,0.0f });
			//cout << "X" << mods << endl;
		}
		else if (key == GLFW_KEY_D) //Right
		{
			translateCamera({ 1.0f,0.0f,0.0f });
			//cout << "x" << endl;
		}
		else if (key == GLFW_KEY_SPACE) //Up
		{
			translateCamera({ 0.0f,1.0f,0.0f });
			//cout << "Y" << mods << endl;
		}
		else if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) //Down
		{
			translateCamera({ 0.0f,-1.0f,0.0f });
			//cout << "y" << endl;
		}
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Set the last know mouse_point
	mouse_point = { xpos, ypos };

	glm::vec3 direction;
	glm::vec3 curPoint;
	float rot_angle;

	// Handle any necessary mouse movements
	switch (Movement)
	{

		// Left-mouse button is being held down
	case ROTATE:
	{
		curPoint = trackBallMapping(mouse_point);
		direction = curPoint - lastPoint;
		float velocity = glm::length(direction);

		if (velocity > 0.0001) // If little movement - do nothing.
		{
			
			// Rotate about the axis that is perpendicular to the great circle connecting the mouse movements.
			glm::vec3 rotAxis;
			rotAxis = glm::cross(lastPoint, curPoint);
			rot_angle = velocity * m_ROTSCALE;

			//Apply the rotation
			switch (Mode)
			{
			case CAMERA:
				//rotateCamera(rot_angle, rotAxis);
				rotateCamera( direction.x * m_ROTSCALE, {0.0f, 1.0f, 0.0f});
				rotateCamera( -direction.y * m_ROTSCALE, {1.0f, 0.0f, 0.0f});
				break;
			case ARMY:
				rotateArmy(rot_angle, rotAxis);
				break;
			}

		}
		break;
	}

	//Right-mouse button is being held down
	case TRANSLATE:
	{
		curPoint = trackBallMapping(mouse_point);

		//Get the translation vector
		direction = curPoint - lastPoint;
		float velocity = glm::length(direction);
		if (velocity > 0.0001) // If little movement - do nothing.
		{
			//Apply the translation
			switch (Mode)
			{
			case CAMERA:
				translateCamera({ direction.x * m_TRANSSCALE, direction.y * m_TRANSSCALE, 0 });
				break;
			case ARMY:
				translateArmy({ direction.x * m_TRANSSCALE, direction.y * m_TRANSSCALE, 0 });
				break;
			}
		}
		break;
	}
	// Save the location of the current point for the next movement.
	lastPoint = curPoint;
	}
}

void Window::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		glm::vec2 point = mouse_point;
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			//Rotate mode
			Movement = ROTATE;
			lastPoint = trackBallMapping(point);
		}

		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			//Translate mode
			Movement = TRANSLATE;
			lastPoint = trackBallMapping(point);
		}
	}

	else if (action == GLFW_RELEASE)
	{
		Movement = NONE;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	switch (Mode)
	{
	case CAMERA:
		translateCamera({ 0,0, -1 * yoffset * m_ZOOMSCALE });
		break;
	case ARMY:
		translateArmy({ 0,0, -1 * yoffset * m_ZOOMSCALE });
		break;
	}

}

glm::vec3 Window::trackBallMapping(glm::vec2 point)
{
	glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v.x = (2.0f*point.x - Window::width) / Window::width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (Window::height - 2.0f*point.y) / Window::height;   // this does the equivalent to the above for the mouse Y position
	v.z = 0.0f;   // initially the mouse z position is set to zero, but this will change below
	d = glm::length(v);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d < 1.0f) ? d : 1.0f;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = sqrtf(1.001f - d*d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	glm::normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
}

//Camera transformations
void Window::translateCamera(glm::vec3 transVec) {
	V = glm::translate(glm::mat4(1.0f), transVec) * V;
	//camTransform = glm::translate(glm::mat4(1.0f), transVec) * camTransform;
	//cout << "cam_pos.x = " << cam_pos.x << "cam_pos.y = " << cam_pos.y << "cam_pos.y = " << cam_pos.z << endl;
	//cam_pos = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(cam_pos, 1.0f));
	//cam_look_at = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(cam_look_at, 1.0f));
	//cam_up = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(cam_up, 0.0f));

	//V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	NEARPPos = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(NEARPPos, 1.0f));
	FARPPos = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(FARPPos, 1.0f));
	TOPPos = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(TOPPos, 1.0f));
	BOTTOMPos = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(BOTTOMPos, 1.0f));
	LEFTPos = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(LEFTPos, 1.0f));
	RIGHTPos = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(RIGHTPos, 1.0f));

	NEARPNorm = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(NEARPNorm, 0.0f));
	FARPNorm = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(FARPNorm, 0.0f));
	TOPNorm = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(TOPNorm, 0.0f));
	BOTTOMNorm = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(BOTTOMNorm, 0.0f));
	LEFTNorm = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(LEFTNorm, 0.0f));
	RIGHTNorm = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(RIGHTNorm, 0.0f));

	planePos[NEARP] = NEARPPos;
	planeNorm[NEARP] = NEARPNorm;
	planePos[FARP] = FARPPos;
	planeNorm[FARP] = FARPNorm;
	planePos[TOP] = TOPPos;
	planeNorm[TOP] = TOPNorm;
	planePos[BOTTOM] = BOTTOMPos;
	planeNorm[BOTTOM] = BOTTOMNorm;
	planePos[LEFT] = LEFTPos;
	planeNorm[LEFT] = LEFTNorm;
	planePos[RIGHT] = RIGHTPos;
	planeNorm[RIGHT] = RIGHTNorm;

}

void Window::rotateCamera(float rot_angle, glm::vec3 rotAxis) {
	V = glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * V;
	//camRotate = glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * camRotate;
	//camRotatePos = camRotatePos * glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis);
		
	NEARPPos = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(NEARPPos,1.0f));
	FARPPos = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(FARPPos, 1.0f));
	TOPPos = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(TOPPos, 1.0f));
	BOTTOMPos = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(BOTTOMPos, 1.0f));
	LEFTPos = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(LEFTPos, 1.0f));
	RIGHTPos = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(RIGHTPos, 1.0f));

	NEARPNorm = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(NEARPNorm, 0.0f));
	FARPNorm = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(FARPNorm, 0.0f));
	TOPNorm = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(TOPNorm, 0.0f));
	BOTTOMNorm = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(BOTTOMNorm, 0.0f));
	LEFTNorm = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(LEFTNorm, 0.0f));
	RIGHTNorm = glm::vec3(glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * glm::vec4(RIGHTNorm, 0.0f));

	planePos[NEARP] = NEARPPos;
	planeNorm[NEARP] = NEARPNorm;
	planePos[FARP] = FARPPos;
	planeNorm[FARP] = FARPNorm;
	planePos[TOP] = TOPPos;
	planeNorm[TOP] = TOPNorm;
	planePos[BOTTOM] = BOTTOMPos;
	planeNorm[BOTTOM] = BOTTOMNorm;
	planePos[LEFT] = LEFTPos;
	planeNorm[LEFT] = LEFTNorm;
	planePos[RIGHT] = RIGHTPos;
	planeNorm[RIGHT] = RIGHTNorm;

	//V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}

void Window::resetCamera() {

	V = glm::lookAt(cam_pos, cam_look_at, cam_up);	
	
	NEARPPos = cam_pos;
	FARPPos = { cam_pos.x - 0.0f, cam_pos.y - 0.0f, cam_pos.z - 10000000.0f };
	TOPPos = { 0.0f, 20.0f * 30, 0.0f };
	BOTTOMPos = { 0.0f, -20.0f * 1000, 0.0f };
	LEFTPos = { -20.0f * 100000, 0.0f, 0.0f };
	RIGHTPos = { 20.0f * 100000, 0.0f, 0.0f };

	NEARPNorm = { 0.0f, 0.0f, 1.0f };
	FARPNorm = { 0.0f,0.0f,-1.0f };
	TOPNorm = { 0.0f, 1.0f, 0.0f };
	BOTTOMNorm = { 0.0f, -1.0f, 0.0f };
	LEFTNorm = { -1.0f, 0.0f, 0.0f };
	RIGHTNorm = { 1.0f, 0.0f, 0.0f };

	planePos[NEARP] = NEARPPos;
	planeNorm[NEARP] = NEARPNorm;
	planePos[FARP] = FARPPos;
	planeNorm[FARP] = FARPNorm;
	planePos[TOP] = TOPPos;
	planeNorm[TOP] = TOPNorm;
	planePos[BOTTOM] = BOTTOMPos;
	planeNorm[BOTTOM] = BOTTOMNorm;
	planePos[LEFT] = LEFTPos;
	planeNorm[LEFT] = LEFTNorm;
	planePos[RIGHT] = RIGHTPos;
	planeNorm[RIGHT] = RIGHTNorm;


	//camTransform = glm::mat4(1.0f);
	//camRotate = glm::mat4(1.0f);
}

//Army transformations
void Window::translateArmy(glm::vec3 transVec) {
	robotArmy->M = glm::translate(glm::mat4(1.0f), transVec) * robotArmy->M;
	//cout << "cam_pos.x = " << cam_pos.x << "cam_pos.y = " << cam_pos.y << "cam_pos.y = " << cam_pos.z << endl;
}

void Window::rotateArmy(float rot_angle, glm::vec3 rotAxis) {
	robotArmy->M = glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * robotArmy->M;
}

void Window::scaleArmy(glm::vec3 transVec) {
	robotArmy->M = glm::scale(glm::mat4(1.0f), transVec) * robotArmy->M;
	//cout << "cam_pos.x = " << cam_pos.x << "cam_pos.y = " << cam_pos.y << "cam_pos.y = " << cam_pos.z << endl;
}

void Window::resetArmy() {
	robotArmy->M = glm::mat4(1.0f);
}


void Window::transformCamera() {

	/*glm::vec3 outCam_pos; // = cam_pos;
	outCam_pos = glm::vec3(camRotate * glm::vec4(cam_pos, 1.0f));
	outCam_pos = glm::vec3(camTransform * glm::vec4(outCam_pos, 1.0f));
	//outCam_pos = glm::vec3(camRotatePos * glm::vec4(outCam_pos, 1.0f));

	glm::vec3 outCam_look_at; //= cam_look_at;
	outCam_look_at = glm::vec3(camRotate * glm::vec4(cam_look_at, 1.0f));
	outCam_look_at = glm::vec3(camTransform * glm::vec4(outCam_look_at, 1.0f));

	glm::vec3 outCam_up = cam_up;
	//outCam_up = glm::vec3(camRotate * glm::vec4(cam_up, 1.0f));
	//outCam_up = glm::vec3(camTransform * glm::vec4(outCam_up, 1.0f));
	
	V = glm::lookAt(outCam_pos, outCam_look_at, outCam_up);*/
	
	//cout << "x = " << outCam_look_at.x << " y = " << outCam_look_at.y << " z = " << outCam_look_at.z << endl;
}

MatrixTransform * Window::createRobot(glm::mat4 startMat) {

	MatrixTransform * RobotMatrix;
	MatrixTransform * matTransform;
	MatrixTransform * matTransform2;
	//RenderWith * renderWith;

	//Set up temp transform matrix
	glm::mat4 toWorld = startMat;

	//Set up Robot Matrix (full body)
	RobotMatrix = new MatrixTransform(toWorld, true);

	//Set up culling sphere
	toWorld = glm::mat4(1.0f);  //reset matrix
	toWorld = glm::scale(glm::mat4(1.0f), { 5.5f, 5.5f, 5.5f }) * toWorld;	//Scale by 0.5
	toWorld = glm::translate(glm::mat4(1.0f), { 0.0f, -1.0f, 0.0f }) * toWorld;
	matTransform = new MatrixTransform(toWorld); //Set up first coordinate system
	//renderWith = new RenderWith(0);
	//renderWith->addChild(sphereBlack);
	matTransform->addChild(sphereBlack);
	RobotMatrix->addChild(matTransform);

	//Set 1st cube at origin (body)
	toWorld = glm::mat4(1.0f);  //reset matrix
	toWorld = glm::scale(glm::mat4(1.0f), { 1.5f, 1.5f, 1.5f }) * toWorld;	//Scale by 0.5
	matTransform = new MatrixTransform(toWorld); //Set up first coordinate system
	matTransform->addChild(sphereWhite);
	RobotMatrix->addChild(matTransform);

	//Set 2nd cube on top of first cube and shrink it (now in first cubes coordinate system) (head)
	//Set up 2nd coordinate system
	toWorld = glm::mat4(1.0f);  //reset matrix
	toWorld = glm::scale(glm::mat4(1.0f), { 0.5f, 0.5f, 0.5f }) * toWorld;	//Scale by 0.5
	toWorld = glm::rotate(glm::mat4(1.0f), -22.5f / 180.0f * glm::pi<float>(), { 0.0f, 1.0f, 0.0f }) * toWorld;
	toWorld = glm::translate(glm::mat4(1.0f), { 0.0f, 2.75f, 0.0f }) * toWorld; //Translate up by 3.5
	matTransform = new MatrixTransform(toWorld, 0.5f, { 0.0f, 1.0f, 0.0f }, 45.0f);;
	matTransform->addChild(cubeGrey);	//Add cube to coordinate system
	RobotMatrix->addChild(matTransform); //Add coordinat system to higher coordinate system

	//(left shoulder pivot)
	toWorld = glm::mat4(1.0f);  //reset matrix
	toWorld = glm::rotate(glm::mat4(1.0f), 45.0f / 180.0f * glm::pi<float>(), { 1.0f, 0.0f, 0.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::translate(glm::mat4(1.0f), { 2.0f, 1.25f, 0.0f }) * toWorld; //Translate right by 4.5, up by 0.5
	matTransform = new MatrixTransform(toWorld, 1.0f, { 1.0f, 0.0f, 0.0f }, 90.0f);
	RobotMatrix->addChild(matTransform); //Add coordinat system to higher coordinate system
	//(left arm)
	toWorld = glm::mat4(1.0f);  //reset matrix
	toWorld = glm::scale(glm::mat4(1.0f), { 1.0f, 0.25f, 0.25f }) * toWorld;	//Scale by long in x, and shrink in y,z
	//toWorld = glm::rotate(glm::mat4(1.0f), 90.0f / 180.0f * glm::pi<float>(), { 0.0f, 0.0f, 1.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::rotate(glm::mat4(1.0f), -90.0f / 180.0f * glm::pi<float>(), { 0.0f, 1.0f, 0.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f }) * toWorld; //Translate right by 4.5, up by 0.5
	matTransform2 = new MatrixTransform(toWorld);
	matTransform2->addChild(cubeBlue);	//Add cube to coordinate system
	matTransform->addChild(matTransform2); //Add coordinat system to higher coordinate system


	//(right shoulder pivot)
	toWorld = glm::mat4(1.0f);  //reset matrix
	toWorld = glm::rotate(glm::mat4(1.0f), 135.0f / 180.0f * glm::pi<float>(), { 1.0f, 0.0f, 0.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::translate(glm::mat4(1.0f), { -2.0f, 1.25f, 0.0f }) * toWorld; //Translate right by 4.5, up by 0.5
	matTransform = new MatrixTransform(toWorld, -1.0f, { 1.0f, 0.0f, 0.0f }, 90.0f);
	RobotMatrix->addChild(matTransform); //Add coordinat system to higher coordinate system
	//(right arm)
	toWorld = glm::mat4(1.0f);  //reset matrix
	toWorld = glm::scale(glm::mat4(1.0f), { 1.0f, 0.25f, 0.25f }) * toWorld;	//Scale by long in x, and shrink in y,z
	//toWorld = glm::rotate(glm::mat4(1.0f), 90.0f / 180.0f * glm::pi<float>(), { 0.0f, 0.0f, 1.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::rotate(glm::mat4(1.0f), -90.0f / 180.0f * glm::pi<float>(), { 0.0f, 1.0f, 0.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::translate(glm::mat4(1.0f), { -0.0f, 0.0f, 2.0f }) * toWorld; //Translate right by 4.5, up by 0.5
	matTransform2 = new MatrixTransform(toWorld);
	matTransform2->addChild(cubeBlue);	//Add cube to coordinate system
	matTransform->addChild(matTransform2); //Add coordinat system to higher coordinate system



	//(left leg pivot)
	toWorld = glm::mat4(1.0f);  //reset matrix
	toWorld = glm::rotate(glm::mat4(1.0f), 135.0f / 180.0f * glm::pi<float>(), { 1.0f, 0.0f, 0.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::translate(glm::mat4(1.0f), { 1.0f, -2.0f, 0.0f }) * toWorld; //Translate right by 4.5, up by 0.5
	matTransform = new MatrixTransform(toWorld, -1.0f, { 1.0f, 0.0f, 0.0f }, 90.0f);
	RobotMatrix->addChild(matTransform); //Add coordinat system to higher coordinate system
	//(left leg)
	toWorld = glm::mat4(1.0f);  //reset matrix
	toWorld = glm::scale(glm::mat4(1.0f), { 1.0f, 0.25f, 0.25f }) * toWorld;	//Scale by long in x, and shrink in y,z
	//toWorld = glm::rotate(glm::mat4(1.0f), 90.0f / 180.0f * glm::pi<float>(), { 0.0f, 0.0f, 1.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::rotate(glm::mat4(1.0f), -90.0f / 180.0f * glm::pi<float>(), { 0.0f, 1.0f, 0.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f }) * toWorld; //Translate right by 4.5, up by 0.5
	matTransform2 = new MatrixTransform(toWorld);
	matTransform2->addChild(cubeYellow);	//Add cube to coordinate system
	matTransform->addChild(matTransform2); //Add coordinat system to higher coordinate system

	//(right leg pivot)
	toWorld = glm::mat4(1.0f);  //reset matrix
	toWorld = glm::rotate(glm::mat4(1.0f), 45.0f / 180.0f * glm::pi<float>(), { 1.0f, 0.0f, 0.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::translate(glm::mat4(1.0f), { -1.0f, -2.0f, 0.0f }) * toWorld; //Translate right by 4.5, up by 0.5
	matTransform = new MatrixTransform(toWorld, 1.0f, { 1.0f, 0.0f, 0.0f }, 90.0f);
	RobotMatrix->addChild(matTransform); //Add coordinat system to higher coordinate system
	//(right leg)
	toWorld = glm::mat4(1.0f);  //reset matrix
	toWorld = glm::scale(glm::mat4(1.0f), { 1.0f, 0.25f, 0.25f }) * toWorld;	//Scale by long in x, and shrink in y,z
	//toWorld = glm::rotate(glm::mat4(1.0f), 90.0f / 180.0f * glm::pi<float>(), { 0.0f, 0.0f, 1.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::rotate(glm::mat4(1.0f), -90.0f / 180.0f * glm::pi<float>(), { 0.0f, 1.0f, 0.0f }) * toWorld;	//Rotate downward in Z axis
	toWorld = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 2.0f }) * toWorld; //Translate right by 4.5, up by 0.5
	matTransform2 = new MatrixTransform(toWorld);
	matTransform2->addChild(cubeYellow);	//Add cube to coordinate system
	matTransform->addChild(matTransform2); //Add coordinat system to higher coordinate system

	return RobotMatrix;
}