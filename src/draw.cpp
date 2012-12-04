#include "draw.h"

Draw::Draw():width(800),height(600){
	if(!glfwInit()){
		fprintf (stderr, "Failed to initialize GLFW\n");
		exit(-1);
	}
	
	setupWindow();
	//setupGL();
}

Draw::~Draw(){
	
}

void Draw::draw(Type type){
	//glClear(GL_COLOR_BUFFER_BIT);
	//glLoadIdentity();

	//glPushMatrix();
	//glOrtho(0, width, height , 0, -1.0, 1.0);
	switch (type){
	case pot:
		drawPot();
		break;
	case sky:
		drawSky();
		break;
	case test:
		drawTest();

	}
	//glPopMatrix();
	//glfwSwapBuffers();
}

void Draw::setupWindow(){
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES,4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR,3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( width, height, 0,0,0,0, 32,0, GLFW_WINDOW ) ){
		fprintf( stderr, "Failed to open GLFW window\n" );
		glfwTerminate();
	}
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}
	glfwSetWindowTitle( "skyGesture Pre-Alpha GLFW Build v0.001a" );
	glfwEnable(GLFW_STICKY_KEYS);
	glfwEnable(GLFW_STICKY_MOUSE_BUTTONS);
	glfwDisable(GLFW_MOUSE_CURSOR);

	
}

void Draw::setupGL(){
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

}

void Draw::drawTest(){
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	
	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
	do{
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );
		// An array of 3 vectors which represents 3 vertices

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
		glColor3f( 1.0f, 1.0f, 0.0f );
		glutSolidTeapot(20.0f);
		// Swap buffers
		glfwSwapBuffers();
	}
	while(glfwGetKey(GLFW_KEY_ESC)!= GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));


}
void Draw::drawPot(){
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex2f(10.0,10.0);
	glVertex2f(50.0,50.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0,1.0,0.0);
	glVertex2f(100.0,250.0);
	glVertex2f(450.0,500.0);
	glEnd();
}

void Draw::drawSky(){
	float x[150];
	float y[150];
	srand(54654);

	glBegin(GL_POINTS);
	glColor3f(0.3,0.7,0.9);
	for (int i = 0; i < 150; i++){
		x[i] = rand()%width;
		y[i] = rand()%height;

		glVertex2f(x[i],y[i]);
	}
	glEnd();
}

void Draw::drawLIDAR(GLdouble *data[]){
	cout<<data[0]<<data[1]<<data[2]<<endl;
}