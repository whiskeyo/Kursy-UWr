// Include standard headers, GLEW, GLFW, GLM
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "common/shader.hpp"
#include "common/controls.hpp"

#include "point.hpp"
#include "linesegment.hpp"
#include "triangle.hpp"
#include "extrafunctions.hpp"

static int WINDOW_WIDTH = 1024;
static int WINDOW_HEIGHT = 768;
static float LINE_LENGTH_MIN = 100.0f;

const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout(location = 0) in vec2 vertexPos;\n"
    "out vec2 ourPosition;\n"
    "out vec3 FragColorShapes;\n"
    "void main()\n"
    "{\n"
    "   gl_Position.xy = vertexPos;\n"
    "   ourPosition = vertexPos;\n"
    "   if (gl_VertexID % 6 == 0 || gl_VertexID % 6 == 1) FragColorShapes = vec3(0.7f, 0.7f, 0.7f);\n"
    "   else if (gl_VertexID % 6 == 2 || gl_VertexID % 6 == 3) FragColorShapes = vec3(0.4f, 0.4f, 0.4f);\n"
    "   else if (gl_VertexID % 6 == 4 || gl_VertexID % 6 == 5) FragColorShapes = vec3(0.1f, 0.1f, 0.1f);\n"
    "}\n\0";

const char *fragmentShaderShapesSource = 
    "#version 330 core\n"
    "out vec3 color;\n"
    "in  vec3 FragColorShapes;\n"
    "void main()\n"
    "{\n"
    "   color = FragColorShapes;\n"
    "}\n\0";

const char *fragmentShaderBackgroundSource = "#version 330 core\n"
    "out     vec3 FragColor;\n"
    "uniform vec3 ourColor; // it is now global variable which we can change in the code\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

GLFWwindow* window;

void cursorPositionCallback(GLFWwindow* window, double l_xpos, double l_ypos); // pos relative to the window
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void windowSizeCallback(GLFWwindow* window, int width, int height);

int main (void)
{
    if (!glfwInit()) // Initialize GLFW
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // antialising
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Line segments - 2D game", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);        // input from keyboard
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  // input from mouse
    
    glfwPollEvents();
    glClearColor(0.85f, 0.85f, 0.85f, 0.0f); // background color
    glLineWidth(1.0f);

    // Build and compile shader program (from const char* above)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderShapes = glCreateShader(GL_FRAGMENT_SHADER);     // fragment shader for lines / triangles
    GLuint fragmentShaderBackground = glCreateShader(GL_FRAGMENT_SHADER); // fragment shader for background
    GLuint shaderProgramShapes  = glCreateProgram();
    GLuint shaderProgramBackground = glCreateProgram(); // background shader program
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(fragmentShaderShapes, 1, &fragmentShaderShapesSource, NULL);
    glCompileShader(fragmentShaderShapes);
    glShaderSource(fragmentShaderBackground, 1, &fragmentShaderBackgroundSource, NULL);
    glCompileShader(fragmentShaderBackground);

    // Check for compilation errors:
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error: vertexShader compilation failed." << std::endl;
    }

    glGetShaderiv(fragmentShaderShapes, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderShapes, 512, NULL, infoLog);
        std::cout << "Error: fragmentShaderShapes compilation failed." << std::endl;
    }

    glGetShaderiv(fragmentShaderBackground, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderBackground, 512, NULL, infoLog);
        std::cout << "Error: vertexShader compilation failed." << std::endl;
    }

    // Linking program objects:
    glAttachShader(shaderProgramShapes, vertexShader);
    glAttachShader(shaderProgramShapes, fragmentShaderShapes);
    glLinkProgram(shaderProgramShapes);
    glAttachShader(shaderProgramBackground, vertexShader);
    glAttachShader(shaderProgramBackground, fragmentShaderBackground);
    glLinkProgram(shaderProgramBackground);

    // Set up vertex data and buffer(s), configure vertex attributes:
    static GLfloat g_vertex_buffer_data[2000] = {}; // array of vertices being sent to GPU
    static GLfloat g_vertex_background_data[] = {
        -1.0f,  1.0f,
         1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f,
    };

    GLuint VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
    
    // setup of background
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_background_data), g_vertex_background_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    // setup of objects (lines / triangles)
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    /* ALL SETTINGS & NECCESSARY FEATURES OF THE GAME */
    srand(time(NULL));
    static std::vector<LineSegment> m_lines;        // array of line segments created by 4 following vertices
    static std::vector<Triangle>    m_triangles;    // array of triagnels created by 6 following vertices
    static int m_level = 3, m_lives = 3, m_record_lines = 0, m_record_triangles = 0;
    static int m_gametype = 1; // 0 - lines, 1 - triangles
    static int m_restarted = 1;

    GLuint m_vertices = 0;              // number of vertices in g_vertex_buffer_data
    static GLdouble m_xpos, m_ypos;     // current cursor position (relative to window)

    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    do{ // basically gameloop
        //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        // INPUT FROM KEYBOARD: (std::fill_n(array, size, value) wypelnia tablice wartosciami 0)
        // 1...5 - quantity of lives
        // 6 - turn wireframe mode on
        // 7 - turn wireframe mode off
        // o - play with lines
        // t - play with triangles
        // n - new game
        // i - current info about game
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) { m_level = 1; }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) { m_level = 2; }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) { m_level = 3; }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) { m_level = 4; }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) { m_level = 5; }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) { std::fill_n(g_vertex_buffer_data, 2*m_vertices, 0.0f); m_lines.clear(); m_gametype = 0; m_vertices = 0; m_lives = m_level; m_restarted = 1; }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) { std::fill_n(g_vertex_buffer_data, 2*m_vertices, 0.0f); m_triangles.clear(); m_gametype = 1; m_vertices = 0; m_lives = m_level; m_restarted = 1; }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) 
        {   
            std::fill_n(g_vertex_buffer_data, 2 * m_vertices, 0.0f); 
            m_lines.clear();
            m_triangles.clear();
            m_vertices = 0; 
            m_lives = m_level; 
            m_restarted = 1; 
            system("clear");
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) { system("clear"); std::cout << "Game type: " << ((m_gametype > 0) ? "triangles" : "lines") << ", level: " << m_level << ", lives: " << m_lives << ", vertices = " << m_vertices << std::endl; }

		//glClear(GL_COLOR_BUFFER_BIT);           // Clear the screen
		
        glfwGetCursorPos(window, &m_xpos, &m_ypos);
        static int m_mouse_oldstate = GLFW_RELEASE;
        int m_mouse_newstate = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (m_mouse_newstate == GLFW_RELEASE && m_mouse_oldstate == GLFW_PRESS && m_restarted == 1)
        {
            system("clear");
            if (m_gametype == 0) // lines
            {
                // Convert screen coordinates to normalized [-1,1]x[-1,1]
                GLfloat m_vertex_x1 =  2.0f * m_xpos / (double)WINDOW_WIDTH  - 1.0f,
                        m_vertex_y1 = -2.0f * m_ypos / (double)WINDOW_HEIGHT + 1.0f;
                g_vertex_buffer_data[2 * m_vertices]     =  m_vertex_x1;
                g_vertex_buffer_data[2 * m_vertices + 1] =  m_vertex_y1;

                m_vertices++;
                //std::cout << "Vertices count: " << m_vertices << "\t Position: " << m_vertex_x1 << ", " << m_vertex_y1 << std::endl;
                
                // Generate the other end of the line
                GLfloat m_length =  (rand() % 150 + LINE_LENGTH_MIN) / (WINDOW_HEIGHT / 2.0f);
                GLfloat m_angle  =  rand() % 360 * M_PI / 180.0f;

                GLfloat m_vertex_x2 = m_vertex_x1 + cos(m_angle) * m_length,
                        m_vertex_y2 = m_vertex_y1 + sin(m_angle) * m_length;  

                g_vertex_buffer_data[2 * m_vertices]     =  m_vertex_x2;
                g_vertex_buffer_data[2 * m_vertices + 1] =  m_vertex_y2;           

                m_vertices++;
                //std::cout << "Vertices count: " << m_vertices << "\t Position: " << m_vertex_x2 << ", " << m_vertex_y2 << std::endl;
                
                // COLLISION DETECTION:
                m_lines.push_back(LineSegment(m_vertex_x1, m_vertex_y1, m_vertex_x2, m_vertex_y2));
                int m_lines_count = m_lines.size(); 
                int iter = 0;
                while (iter < m_lines_count)
                {
                    if (doIntersect(m_lines.at(iter), m_lines.at(m_lines_count - 1)))
                        std::cout << "Collision detected!! Intersecting lines: " << iter << " and " << m_lines_count -1 << ". You have " << --m_lives << " lives now." << std::endl;

                    iter++;
                }
                std::cout << "Score: " << m_lines_count << ", lives: " << m_lives << std::endl;
                if (m_record_lines <= m_lines_count) m_record_lines = m_lines_count;
            }
            else if (m_gametype == 1) // triangles
            {
                // Convert screen coordinates to normalized [-1,1]x[-1,1]
                GLfloat m_vertex_x1 =  2.0f * m_xpos / (double)WINDOW_WIDTH  - 1.0f,
                        m_vertex_y1 = -2.0f * m_ypos / (double)WINDOW_HEIGHT + 1.0f,
                        m_vertex_x2, m_vertex_y2, m_vertex_x3, m_vertex_y3;

                g_vertex_buffer_data[2 * m_vertices]     =  m_vertex_x1;
                g_vertex_buffer_data[2 * m_vertices + 1] =  m_vertex_y1;

                m_vertices++;
                //std::cout << "Vertices count: " << m_vertices << "\t Position: " << m_vertex_x1 << ", " << m_vertex_y1 << std::endl;
                
                // Generate the first line
                GLfloat m_length1 =  (rand() % 150 + LINE_LENGTH_MIN) / (WINDOW_HEIGHT / 2.0f);
                GLfloat m_length2 =  (rand() % 150 + LINE_LENGTH_MIN) / (WINDOW_HEIGHT / 2.0f);
                GLfloat m_angle   =  rand() % 360 * M_PI / 180.0f;

                m_vertex_x2 = m_vertex_x1 + cos(m_angle) * m_length1;
                m_vertex_y2 = m_vertex_y1 + sin(m_angle) * m_length1;  
                m_vertex_x3 = m_vertex_x1 + cos(m_angle + M_PI_2) * m_length2;
                m_vertex_y3 = m_vertex_y1 + sin(m_angle + M_PI_2) * m_length2;  

                g_vertex_buffer_data[2 * m_vertices]     =  m_vertex_x2;
                g_vertex_buffer_data[2 * m_vertices + 1] =  m_vertex_y2;           

                m_vertices++;
                //std::cout << "Vertices count: " << m_vertices << "\t Position: " << m_vertex_x2 << ", " << m_vertex_y2 << std::endl;
                
                g_vertex_buffer_data[2 * m_vertices]     =  m_vertex_x3;
                g_vertex_buffer_data[2 * m_vertices + 1] =  m_vertex_y3; 

                m_vertices++;
                //std::cout << "Vertices count: " << m_vertices << "\t Position: " << m_vertex_x3 << ", " << m_vertex_y3 << std::endl;

                // COLLISION DETECTION:
                m_triangles.push_back(Triangle(m_vertex_x1, m_vertex_y1, m_vertex_x2, m_vertex_y2, m_vertex_x3, m_vertex_y3));
                int m_triangles_count = m_triangles.size();
                int iter = 0;
                while (iter < m_triangles_count - 1)
                {
                    if (doTrianglesIntersect(m_triangles.at(iter), m_triangles.at(m_triangles_count - 1)))
                        std::cout << "Collision detected!! Intersecting triangles: " << iter << " and " << m_triangles_count -1 << ". You have " << --m_lives << " lives now." << std::endl;

                    iter++;
                }
                std::cout << "Score: " << m_triangles_count << ", lives: " << m_lives << std::endl;
                if (m_record_triangles <= m_triangles_count) m_record_triangles = m_triangles_count;
            }
        }
        m_mouse_oldstate = m_mouse_newstate;

        if (m_lives <= 0 && m_restarted == 1)
        {
            std::cout << "YOU LOST!!" << std::endl << 
                         "Your score: " << ((m_gametype == 1) ? m_vertices / 3 : m_vertices / 2) << std::endl <<
                         "Your record: " << ((m_gametype == 1) ? m_record_triangles : m_record_lines) << std::endl <<
                         "Press N to play again." << std::endl; 
            m_restarted = 0;
        }

        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

        // Set the background and its color by using uniform:
        float timeValue = glfwGetTime() * 0.75f;
        float colorValue = (sin(timeValue) / 2.0f) + 0.5f;
        float rValue, gValue, bValue;
        int vertexColorLocation = glGetUniformLocation(shaderProgramBackground, "ourColor");
        glUseProgram(shaderProgramBackground);

        if (m_restarted == 1) // nice colors during game changed according to time
        {
            rValue = 1.0f - 0.98f * colorValue; 
            gValue = 0.25f + abs(0.5f - colorValue);
            bValue = colorValue;
        }
        else // if you lose, colors switch to red
        {
            if (rValue < 1.0f)   rValue *= 1.01f;
            if (gValue > 0.0f)   gValue *= 0.95f;
            if (bValue > 0.0f)   bValue *= 0.95f;

            if (rValue >= 0.98f) rValue = 1.0f;
            if (gValue <= 0.05f) gValue = 0.0f;
            if (bValue <= 0.05f) bValue = 0.0f;

            for (int n = 0; n < 2 * m_vertices; n++)
                g_vertex_buffer_data[n] *= 0.96f;
        }

        // Color of background (for debugging):
        //std::cout << "RGB: (" << (int)(rValue*255.0f) << ", " << (int)(gValue*255.0f) << ", " << (int)(bValue*255.0f) << "), m_restarted = " << m_restarted << std::endl;

        glUniform3f(vertexColorLocation, rValue, gValue, bValue);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBindVertexArray(VAOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_background_data), g_vertex_background_data, GL_STREAM_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glEnableVertexAttribArray(0);
        
        // Set lines / triangles:
        glUseProgram(shaderProgramShapes);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBindVertexArray(VAOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

		// Draw the lines
        if (m_gametype == 0)
		    glDrawArrays(GL_LINES, 0, m_vertices); // (type, starting_index, number_of_vertices)
        else if (m_gametype == 1)
            glDrawArrays(GL_TRIANGLES, 0, m_vertices); // (type, starting_index, number_of_vertices)
		
        glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	glDeleteBuffers(2, VBOs);
	glDeleteVertexArrays(2, VAOs);
	//glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void cursorPositionCallback(GLFWwindow* window, double l_xpos, double l_ypos)
{
    //std::cout << "(x,y) = (" << l_xpos << ", " << l_ypos << ")" << std::endl;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{   /*
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        std::cout << "LEFT MOUSE BUTTON IS PRESSED" << std::endl;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        std::cout << "LEFT MOUSE BUTTON IS RELEASED" << std::endl;
    */
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    WINDOW_HEIGHT = height;
    WINDOW_WIDTH  = width;
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}