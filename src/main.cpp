#ifdef _WIN32
#include <windows.h>
#endif


#include <iostream>
#include <vector>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Program.h"
#include "SDL/SDL.h"
#include <OpenGL/gl.h>
#include <OPenGL/glu.h>

#include <GLKit/GLKMatrix4.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


using std::vector;

tdogl::Program* gProgram = NULL;

GLuint gVAO = 0;

//these are the VBOs for the triangle
GLuint triangleVertices;
GLuint triangleColors;


void error(const std::string& s) {
#ifdef _WIN32
	MessageBox(NULL, s.c_str(), "An error occurred", MB_ICONINFORMATION | MB_OK);
#else
	std::cerr << s << std::endl;
#endif
}

class SimpleApp
{
public:
    static const int WINDOW_WIDTH = 1024;
    static const int WINDOW_HEIGHT = 768;

    SimpleApp();
    bool initialize();
    void LoadShaders();
    void LoadTriangles();
    void LoadUniform();
    void RenderScene();
    void resize(int x, int y);
    void prepare();
    void render();
};

void SimpleApp::LoadUniform()
{
    glm::mat4 ident;
    
    // set the "camera" uniform
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 12.0f));
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), 4.0f/3.0f, 1.0f, 100.0f);
    gProgram->setUniform("projection_matrix", projectionMatrix);
    //gProgram->setUniform("camera", gCamera.matrix());
    
    // set the "model" uniform in the vertex shader, based on the gDegreesRotated global
    gProgram->setUniform("modelview_matrix", glm::rotate(translate, glm::radians(0.75f), glm::vec3(0,1,0)));
}

void SimpleApp::LoadTriangles()
{
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    glGenBuffers(1, &triangleVertices);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVertices);
    
    GLfloat vertexData[] =
    {
        2.0, 2.5, -1.0,
        -3.5, -2.5, -1.0,
        2.0, -4.0, -1.0
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(gProgram->attrib("a_Vertex"));
    glVertexAttribPointer(gProgram->attrib("a_Vertex"), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);
    
    vector<GLfloat> m_colrs;
    //color #1
    m_colrs.push_back(1.0f);
    m_colrs.push_back(0.0f);
    m_colrs.push_back(0.0f);
    //color #2
    m_colrs.push_back(0.0f);
    m_colrs.push_back(1.0f);
    m_colrs.push_back(0.0f);
    //color #3
    m_colrs.push_back(0.0f);
    m_colrs.push_back(0.0f);
    m_colrs.push_back(1.0f);
    
    glGenBuffers(1, &triangleColors); //Generate a buffer for the colors
    glBindBuffer(GL_ARRAY_BUFFER, triangleColors); //Bind the color buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_colrs.size(), &m_colrs[0], GL_STATIC_DRAW);
    
    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(gProgram->attrib("a_Color"));
    glVertexAttribPointer(gProgram->attrib("a_Color"), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);
    
    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SimpleApp::LoadShaders()
{
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile("basic-fixed.vert", GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile("basic-fixed.frag", GL_FRAGMENT_SHADER));
    gProgram = new tdogl::Program(shaders);
}

SimpleApp::SimpleApp()
{

}

bool SimpleApp::initialize()
{
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
    //Set up the projection matrix
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    return true;
}

void SimpleApp::RenderScene()
{
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //uncomment for wireframe mode
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    // bind the program (the shaders)
    gProgram->use();
    
    LoadUniform();
    
    glBindVertexArray(gVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, triangleVertices);
    //glVertexPointer(3, GL_FLOAT, 0, 0);
    //glEnableVertexAttribArray(gProgram->attrib("a_Vertex"));
    glVertexAttribPointer(gProgram->attrib("a_Vertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    
    glBindBuffer(GL_ARRAY_BUFFER, triangleColors);
    //glEnableVertexAttribArray(gProgram->attrib("a_Color"));
    glVertexAttribPointer(gProgram->attrib("a_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    //draw the triangle
    // draw the VAO
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glBindVertexArray(0);
    gProgram->stopUsing();
}

void SimpleApp::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0.0, 1.0, 6.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    
    //GLKMatrix4MakeLookAt(0.0f,1.0f,6.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
    
    glBegin(GL_TRIANGLES);
        //Send the vertices and colors for the triangle
        glColor4f(1.0, 0.0, 0.0, 1.0);
        glVertex3f(2.0, 2.5, -1.0);
        glColor4f(0.0, 1.0, 0.0, 1.0);
        glVertex3f(-3.5, -2.5, -1.0);
        glColor4f(0.0, 0.0, 1.0, 1.0);
        glVertex3f(2.0, -4.0, -1.0);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
        //Send the vertices and colors for the pentagon
        glColor4f(1.0, 1.0, 1.0, 1.0);
        glVertex3f(-1.0, 2.0, 0.0);
        glColor4f(1.0, 1.0, 0.0, 1.0);
        glVertex3f(-3.0, -0.5, 0.0);
        glColor4f(0.0, 1.0, 1.0, 1.0);
        glVertex3f(-1.5, -3.0, 0.0);
        glColor4f(0.0, 0.0, 0.0, 1.0);
        glVertex3f(1.0, -2.0, 0.0);
        glColor4f(1.0, 0.0, 1.0, 1.0);
        glVertex3f(1.0, 1.0, 0.0);
    glEnd();
}

void SimpleApp::resize(int w, int h)
{
    //Prevent a divide by zero error
    if (h <= 0)
    {
        h = 1;
    }

    //When we resize the window, we tell OpenGL about the new viewport size
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION); //deprecated
    glLoadIdentity();
    //Then we set up our projection matrix with the correct aspect ratio
    
    gluPerspective(60.0f, float(w) / float(h), 1.0f, 100.0f); //deprecated
    
    //GLKMatrix4MakePerspective(60.0f, float(w) / float(h) , 1.0f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW); //deprecated
    glLoadIdentity(); //deprecated
}

void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}

void TryGLEW()
{
    glfwSetErrorCallback(OnError);
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *gWindow = glfwCreateWindow((int)1024, (int)768, "OpenGL Tutorial", NULL, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
    
    // GLFW settings
    glfwMakeContextCurrent(gWindow);
    
    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
}

int main(int argc, char** argv)
{
    SimpleApp myApp;

    // TryGLEW();

    //not working
    //load the shaders
    // myApp.LoadShaders();
    
    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
        std::cerr << "Could not initialize SDL" << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    std::string title = "BOGLGP Chapter 1";
    SDL_WM_SetCaption(title.c_str(), title.c_str());

    // Create the window
    if (!SDL_SetVideoMode(SimpleApp::WINDOW_WIDTH, SimpleApp::WINDOW_HEIGHT, 0, SDL_OPENGL | SDL_RESIZABLE))
    {
        std::cerr << "Could not create the window" << std::endl;
        return false;
    }
    
   
    
    //initilize and bind VAO and VBO for the triangle
    // myApp.LoadTriangles();
    
    if (!myApp.initialize())
    {
        SDL_Quit();
        return 1;
    }

    bool done = false;

    while (!done)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {

			switch(event.type) {
				case SDL_QUIT:
					done = true;
				break;
				case SDL_VIDEORESIZE:
					myApp.resize(event.resize.w, event.resize.h);
				break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) {
						done = true;
					}
				break;
				default:
					break;
			}
        }

        myApp.render();
       // with shader not working
        // myApp.RenderScene();
        SDL_GL_SwapBuffers();
    }

    SDL_Quit();
	
    return 0;
}
