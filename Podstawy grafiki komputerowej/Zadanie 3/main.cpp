#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "point.hpp"
#include "linesegment.hpp"
#include "triangle.hpp"
#include "extrafunctions.hpp"

// GAME PARAMETERS
GLFWwindow* window;
static int WINDOW_WIDTH = 1024;
static int WINDOW_HEIGHT = 768;
static int WINDOW_FRAMERATE = 60;

static float m_border_width = 0.1f;

static float m_brick_width   =  0.225f;
static float m_brick_start_x = -0.9f;
static float m_brick_start_y =  0.9f;
static float m_angle         =  M_PI / 3.0f;

static float m_paddle_width  =  0.5f;
static float m_paddle_height =  0.1f;
static float m_LT_paddle_x   = -0.25f;        // left top x coordinate of the paddle
static float m_LT_paddle_y   = -0.8f;         // left top y coordinate of the paddle
static float m_paddle_speed  =  0.012f;
static float m_paddle_noise  =  0.0005f;      // paddle noise, so reflection angle is in range [alpha-noise, alpha+noise]

static float m_ball_width    =  0.1f;
static float m_ball_start_x  = -0.05f;
static float m_ball_start_y  = -0.68f;
static glm::vec2 m_ball_speed = glm::vec2(0.0f, 0.0f);
static glm::vec2 m_ball_middle;

static std::vector<Triangle> m_triangles;   // array of triangles created by 3 following vertices (bricks on top)
static int m_triangles_counter = 0;
static std::vector<Triangle> m_paddle;
static Triangle m_ball( m_ball_start_x, 
                        m_ball_start_y, 
                        m_ball_start_x + m_ball_width, 
                        m_ball_start_y, 
                        m_ball_start_x + m_ball_width / 2, 
                        m_ball_start_y - m_ball_width * sqrt(3) / 2);

static int m_restarted = 1;
static int m_started = 0;
static int m_score = 0;
static glm::vec2 m_triangle_middle;
static float m_side_x1, m_side_x2, m_side_y1, m_side_y2;

// SHADERS
const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout(location = 0) in vec2 vertexPos;\n"
    "out vec2 ourPosition;\n"
    "out vec3 FragColorShapes;\n"
    "void main()\n"
    "{\n"
    "   gl_Position.xy = vertexPos;\n"
    "   ourPosition = vertexPos;\n"
    "   if      (gl_VertexID >= 0  && gl_VertexID < 12) FragColorShapes = vec3(0.7f, 0.7f, 0.7f);\n"        // border color
    "   else if (gl_VertexID >= 12 && gl_VertexID < 16) FragColorShapes = vec3(0.7f, 0.7f, 0.7f);\n"        // paddle color
    "   else if (gl_VertexID >= 16 && gl_VertexID < 19) FragColorShapes = vec3(0.1f, 0.1f, 0.1f);\n"        // ball color
    "   else FragColorShapes = vec3(0.5f, 0.5f, 0.5f);\n"
    "}\n\0";

const char *vertexShaderBricksSource = 
    "#version 330 core\n"
    "layout(location = 0) in vec2 vertexPos;\n"
    "layout(location = 1) in vec3 vertexColor;\n"
    "out vec2 ourPosition;\n"
    "out vec3 FragColorShapes;\n"
    "void main()\n"
    "{\n"
    "   gl_Position.xy = vertexPos;\n"
    "   ourPosition = vertexPos;\n"     
    "   FragColorShapes = vertexColor;\n"
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

void windowSizeCallback(GLFWwindow* window, int width, int height);             // window does scale all objects while resizing to fit it 
void collisionHandler(GLfloat *arr);                                            // with use of triangleIntersectionv2 it makes reflections work perfectly fine
glm::vec2 triangleIntersectionv2(Triangle u, Triangle v);                       // better version of doTrianglesIntersect
void restart(GLfloat *arr);
void fillBricks(GLfloat *arr);

int main()
{
    system("clear");
    srand(time(NULL));

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
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Arkanoid for triangles - Tomasz Woszczynski", NULL, NULL);
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
    glClearColor(0.85f, 0.85f, 0.85f, 0.0f); // background color (pretty useless to define specified color, as we have animated background)
    glLineWidth(1.0f);

    // Build and compile shader program (from const char* above)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);                 // borders, paddle, ball
    GLuint vertexShaderBricks = glCreateShader(GL_VERTEX_SHADER);           // bricks
    GLuint fragmentShaderShapes = glCreateShader(GL_FRAGMENT_SHADER);       // fragment shader for objects
    GLuint fragmentShaderBackground = glCreateShader(GL_FRAGMENT_SHADER);   // fragment shader for background
    GLuint shaderProgramShapes  = glCreateProgram();                        // ball, paddle and border shader program
    GLuint shaderProgramBricks  = glCreateProgram();                        // bricks shader program
    GLuint shaderProgramBackground = glCreateProgram();                     // background shader program
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glShaderSource(vertexShaderBricks, 1, &vertexShaderBricksSource, NULL);
    glCompileShader(vertexShaderBricks);
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

    glGetShaderiv(vertexShaderBricks, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderBricks, 512, NULL, infoLog);
        std::cout << "Error: vertexShaderBricks compilation failed." << std::endl;
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
    glAttachShader(shaderProgramBricks, vertexShaderBricks);
    glAttachShader(shaderProgramBricks, fragmentShaderShapes);
    glLinkProgram(shaderProgramBricks);
    glAttachShader(shaderProgramBackground, vertexShader);
    glAttachShader(shaderProgramBackground, fragmentShaderBackground);
    glLinkProgram(shaderProgramBackground);

    // Check for linking errors:
    glGetProgramiv(shaderProgramShapes, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgramShapes, 512, NULL, infoLog);
        std::cout << "Error: shaderProgramShapes linking failed." << std::endl;
    }

    glGetProgramiv(shaderProgramBricks, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgramBricks, 512, NULL, infoLog);
        std::cout << "Error: shaderProgramBricks linking failed." << std::endl;
    }

    glGetProgramiv(shaderProgramBackground, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgramBackground, 512, NULL, infoLog);
        std::cout << "Error: shaderProgramBackground linking failed." << std::endl;
    }

    // We linked shaders, so we don't need them anymore (they are saved in programs now)
    glDeleteShader(vertexShader);
    glDeleteShader(vertexShaderBricks);
    glDeleteShader(fragmentShaderShapes);
    glDeleteShader(fragmentShaderBackground);

    // Push back triangles to vector to create paddle
    m_paddle.push_back(Triangle(m_LT_paddle_x, m_LT_paddle_y, m_LT_paddle_x + m_paddle_width, m_LT_paddle_y, m_LT_paddle_x, m_LT_paddle_y - m_paddle_height));                                      // upper triangle of the paddle
    m_paddle.push_back(Triangle(m_LT_paddle_x, m_LT_paddle_y - m_paddle_height, m_LT_paddle_x + m_paddle_width, m_LT_paddle_y - m_paddle_height, m_LT_paddle_x + m_paddle_width, m_LT_paddle_y));   // lower triangle of the paddle

    // Set up vertex data and buffer(s), configure vertex attributes:
    static GLfloat g_vertex_buffer_data[2000] = {
        // BORDER (drawing 2 triangles every time) (indexes 0..11 / 0..23)
        -1.00f, -1.00f,
        -1.00f,  1.00f,
        -0.95f, -1.00f,
        -0.95f,  1.00f,

        -0.95f,  1.00f,
         0.95f,  1.00f,
        -0.95f,  0.95f,
         0.95f,  0.95f,

         0.95f, -1.00f,
         0.95f,  1.00f,
         1.00f, -1.00f,
         1.00f,  1.00f,

        // PADDLE (indexes 12..15 / 24..31)
        (float)m_paddle.at(0).getA().getX(), (float)m_paddle.at(0).getA().getY(),
        (float)m_paddle.at(0).getB().getX(), (float)m_paddle.at(0).getB().getY(),
        (float)m_paddle.at(0).getC().getX(), (float)m_paddle.at(0).getC().getY(),
        (float)m_paddle.at(1).getB().getX(), (float)m_paddle.at(1).getB().getY(),

        // "BALL" (indexes 16..18 / 32..37)
        (float)m_ball.getA().getX(), (float)m_ball.getA().getY(),
        (float)m_ball.getB().getX(), (float)m_ball.getB().getY(),
        (float)m_ball.getC().getX(), (float)m_ball.getC().getY(),        
    };

    static GLfloat g_vertex_buffer_triangles[2000];

    static GLfloat g_vertex_background_data[] = {
        -1.0f,  1.0f,
         1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f,
    };

    GLuint VBOs[3], VAOs[3];
    glGenVertexArrays(3, VAOs);
	glGenBuffers(3, VBOs);
    
    // setup of background
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_background_data), g_vertex_background_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    // setup of objects (ball, paddle, borders)
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // setup of bricks
    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_triangles), g_vertex_buffer_triangles, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glfwSetWindowSizeCallback(window, windowSizeCallback);

    double m_last_time = glfwGetTime();
    int nbFrames = 0;

    restart(g_vertex_buffer_triangles);

    std::cout << "Keys: A and D - move the paddle, Space - start the game" << std::endl;

    do
    {
        double m_current_time = glfwGetTime();
        nbFrames++;
        double m_elapsed_time = m_current_time - m_last_time;

        if (m_elapsed_time > 1.0f / WINDOW_FRAMERATE) // UPDATE (GET INPUT)
        {
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && g_vertex_buffer_data[24] >= -0.9f) 
            { 
                g_vertex_buffer_data[24] -= m_paddle_speed; 
                g_vertex_buffer_data[26] -= m_paddle_speed; 
                g_vertex_buffer_data[28] -= m_paddle_speed; 
                g_vertex_buffer_data[30] -= m_paddle_speed;
                m_paddle.at(0).translate(-m_paddle_speed, 0);
                m_paddle.at(1).translate(-m_paddle_speed, 0);
            }

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && g_vertex_buffer_data[26] <= 0.9f) 
            {   
                g_vertex_buffer_data[24] += m_paddle_speed; 
                g_vertex_buffer_data[26] += m_paddle_speed; 
                g_vertex_buffer_data[28] += m_paddle_speed; 
                g_vertex_buffer_data[30] += m_paddle_speed;
                m_paddle.at(0).translate(m_paddle_speed, 0);
                m_paddle.at(1).translate(m_paddle_speed, 0);
            }

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && m_started == 0)
            {
                m_ball_speed.x = (float)(rand() % 80 - 40) / 10000.0f;  // randomize velocity of x (so it can go both left and right at the beginning)
                m_ball_speed.y = (float)(rand() % 80) / 10000.0f;
                m_started = 1;
            }
            
            // Display full shapes or wireframe:
            if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Maybe it isn't very fast (for computing), but at least it works
        m_ball.translate(m_ball_speed.x, m_ball_speed.y);
        g_vertex_buffer_data[32] = (float)m_ball.getA().getX(); 
        g_vertex_buffer_data[33] = (float)m_ball.getA().getY();
        g_vertex_buffer_data[34] = (float)m_ball.getB().getX();
        g_vertex_buffer_data[35] = (float)m_ball.getB().getY();
        g_vertex_buffer_data[36] = (float)m_ball.getC().getX();
        g_vertex_buffer_data[37] = (float)m_ball.getC().getY();

        m_ball_middle = glm::vec2(g_vertex_buffer_data[32] + m_ball_width / 2.0f, 
                                  g_vertex_buffer_data[33] - m_ball_width * sqrt(3) / 3);

        // CHECK COLLISIONS OF BALL WITH: PADDLE / BORDERS / BRICKS
        // 1. BORDERS: first 2 ifs checking horizontal collision, last one vertical (with ceilling)
        if (m_ball.getA().getX() <= -0.95f) m_ball_speed.x = -m_ball_speed.x;
        if (m_ball.getB().getX() >=  0.95f) m_ball_speed.x = -m_ball_speed.x;
        if (m_ball.getB().getY() >=  0.95f) m_ball_speed.y = -m_ball_speed.y;

        if (m_ball.getC().getY() <= -1 - m_ball_width) m_started = 0;   // basically losing condition

        // 2. PADDLE - similarly to borders - we dont check left/right side, as the ball would also fall down there
        if (m_ball.getC().getX() >= m_paddle.at(0).getA().getX() &&
            m_ball.getC().getX() <= m_paddle.at(0).getB().getX() &&
            m_ball.getC().getY() <= m_paddle.at(0).getA().getY() + 0.03f) 
        {
            m_ball_speed.y = -m_ball_speed.y;
            m_ball_speed.x = m_ball_speed.x + (rand() % 3 - 1) * m_paddle_noise; 
        }

        // 3. BRICKS - whole function below
        collisionHandler(g_vertex_buffer_triangles);

        // DISPLAY EVERYTHING ON THE SCREEN!
        glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

        // Set the background and its color by using uniform:
        float timeValue = glfwGetTime() * 0.75f;
        float colorValue = (sin(timeValue) / 2.0f) + 0.5f;
        float rValue, gValue, bValue;
        int vertexColorLocation = glGetUniformLocation(shaderProgramBackground, "ourColor");
        glUseProgram(shaderProgramBackground);

        // Blueish fade wave
        rValue = 0.25f + abs(0.5f - colorValue);
        gValue = 0.25f + abs(0.5f - colorValue);
        bValue = 1.0f;

        // Color of background (for debugging):
        //std::cout << "RGB: (" << (int)(rValue*255.0f) << ", " << (int)(gValue*255.0f) << ", " << (int)(bValue*255.0f) << "), m_restarted = " << m_restarted << std::endl;

        glUniform3f(vertexColorLocation, rValue, gValue, bValue);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
        glBindVertexArray(VAOs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_background_data), g_vertex_background_data, GL_STREAM_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glEnableVertexAttribArray(0);
        
        // Set shaders for shapes (border, paddle, ball)
        glUseProgram(shaderProgramShapes);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBindVertexArray(VAOs[0]);
        glBufferData(GL_ARRAY_BUFFER, 38*sizeof(float), g_vertex_buffer_data, GL_STATIC_DRAW); // draw borders, paddle and ball
        glEnableVertexAttribArray(0);

		// Draw the shapes (type, starting_index, number_of_vertices):
        // BORDER
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4); 
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

        // PADDLE
		glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);   

        // BALL                 
        if (m_started == 1) glDrawArrays(GL_TRIANGLES, 16, 3);      

        // Draw bricks
        glUseProgram(shaderProgramBricks);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
        glBindVertexArray(VAOs[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_triangles), g_vertex_buffer_triangles, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glDrawArrays(GL_TRIANGLES, 0, m_triangles_counter*3);  // (type, starting_index, number_of_vertices) - BRICKS

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();        
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	glDeleteBuffers(3, VBOs);
	glDeleteVertexArrays(3, VAOs);
	glDeleteProgram(shaderProgramShapes);
    glDeleteProgram(shaderProgramBricks);
    glDeleteProgram(shaderProgramBackground);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    WINDOW_HEIGHT = height;
    WINDOW_WIDTH  = width;
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void collisionHandler(GLfloat *arr)
{
    for (int i = 0; i < m_triangles.size(); i++)
    {
        glm::vec2 m_vector = triangleIntersectionv2(m_ball, m_triangles.at(i));

        if (m_vector != glm::vec2(0.0f, 0.0f)) // triangles intersect = there is collision
        {
            float m_vector_length   = sqrt(m_vector.x * m_vector.x + m_vector.y * m_vector.y);
            float m_velocity_length = sqrt(m_ball_speed.x * m_ball_speed.x + m_ball_speed.y * m_ball_speed.y);
            
            glm::vec2 m_vector_norm   = glm::vec2(m_vector.x / m_vector_length, m_vector.y / m_vector_length);
            glm::vec2 m_velocity_norm = glm::vec2(m_ball_speed.x / m_velocity_length, m_ball_speed.y / m_velocity_length);
            glm::vec2 m_perp_CW  = glm::vec2(m_vector_norm.y, -m_vector_norm.x);
            glm::vec2 m_perp_CCW = glm::vec2(-m_vector_norm.y, m_vector_norm.x);

            m_ball_middle = m_ball_middle - m_ball_speed * 2.0f;
            int m_ball_side = glm::sign((m_ball_middle.x - m_side_x1) * (m_side_y2 - m_side_y1) - (m_ball_middle.y - m_side_y1) * (m_side_x2 - m_side_x1));

            glm::vec2 m_determine_temp = m_ball_middle + m_perp_CW * 5.0f;
            int m_determined_side = glm::sign((m_determine_temp.x - m_side_x1) * (m_side_y2 - m_side_y1) - (m_determine_temp.y - m_side_y1) * (m_side_x2 - m_side_x1));
            if (m_determined_side == m_ball_side)
                m_ball_speed = m_velocity_norm - m_perp_CW * (2.0f * glm::dot(m_velocity_norm, m_perp_CW));
            else
                m_ball_speed = m_velocity_norm - m_perp_CCW * (2.0f * glm::dot(m_velocity_norm, m_perp_CCW));

            m_ball_speed *= m_velocity_length;

            m_triangles.erase(m_triangles.begin() + i);
            std::fill_n(arr, m_triangles_counter * 9, 0.0f);
            m_triangles_counter--;
            fillBricks(arr);

            system("clear");
            m_score++;
            std::cout << "Score: " << m_score << std::endl;
        }
    }
}

glm::vec2 triangleIntersectionv2(Triangle u, Triangle v)
{
    float x1 = u.getA().getX();
    float x2 = u.getB().getX();
    float x3 = u.getC().getX();
    float x4 = v.getA().getX();
    float x5 = v.getB().getX();
    float x6 = v.getC().getX();
    float y1 = u.getA().getY();
    float y2 = u.getB().getY();
    float y3 = u.getC().getY();
    float y4 = v.getA().getY();
    float y5 = v.getB().getY();
    float y6 = v.getC().getY();

    if (doIntersect(LineSegment(x1, y1, x2, y2), LineSegment(x4, y4, x5, y5)) ||
        doIntersect(LineSegment(x1, y1, x3, y3), LineSegment(x4, y4, x5, y5)) ||
        doIntersect(LineSegment(x2, y2, x3, y3), LineSegment(x4, y4, x5, y5)))
    {
        m_triangle_middle = glm::vec2((x4 + x5) / 2.0f, (y4 + y5) / 2.0f);
        m_side_x1 = x4; 
        m_side_y1 = y4; 
        m_side_x2 = x5; 
        m_side_y2 = y5;
        return glm::vec2(x5 - x4, y5 - y4);
    }
    else if (doIntersect(LineSegment(x1, y1, x2, y2), LineSegment(x4, y4, x6, y6)) || 
             doIntersect(LineSegment(x1, y1, x3, y3), LineSegment(x4, y4, x6, y6)) || 
             doIntersect(LineSegment(x2, y2, x3, y3), LineSegment(x4, y4, x6, y6)))
    {
    	m_triangle_middle = glm::vec2((x4 + x6) / 2.0f, (y4 + y6) / 2.0f);
    	m_side_x1 = x4;
    	m_side_y1 = y4;
    	m_side_x2 = x6;
    	m_side_y2 = y6;
    	return glm::vec2(x6 - x4, x6 - y4);
    }
    else if (doIntersect(LineSegment(x1, y1, x2, y2), LineSegment(x5, y5, x6, y6)) || 
             doIntersect(LineSegment(x1, y1, x3, y3), LineSegment(x5, y5, x6, y6)) || 
             doIntersect(LineSegment(x2, y2, x3, y3), LineSegment(x5, y5, x6, y6)))
    {
    	m_triangle_middle = glm::vec2((x5 + x6) / 2.0f, (y5 + y6) / 2.0f);
    	m_side_x1 = x5;
    	m_side_y1 = y5;
    	m_side_x2 = x6;
    	m_side_y2 = y6;
    	return glm::vec2(x6 - x5, y6 - y5);
    }

    return glm::vec2(0.0f, 0.0f);
}


void restart(GLfloat *arr)
{
    // 4 rows of triangles: 1st upside down (8 pcs), 2nd normal (7 pcs), 3rd upside down (7 pcs), 4th normal (8 pcs)
    float r_col, g_col, b_col;

    for (int i = 0; i < 8; i++)
    {
        float x1 = i * m_brick_width + m_brick_start_x;
        float y1 = m_brick_start_y;
        float x2 = x1 + m_brick_width;
        float y2 = y1;
        float x3 = x1 + m_brick_width / 2;
        float y3 = y1 - m_brick_width * sqrt(3) / 2;
        r_col = 0.5f;
        g_col = 0.0f;
        b_col = 0.0f;
        m_triangles.push_back(Triangle(x1, y1, x2, y2, x3, y3, r_col, g_col, b_col));
        m_triangles_counter++;
    }

    for (int i = 0; i < 7; i++)
    {
        float x1 = i * m_brick_width + m_brick_start_x + m_brick_width / 2;
        float y1 = m_brick_start_y - m_brick_width * sqrt(3) / 2;
        float x2 = x1 + m_brick_width;
        float y2 = y1;
        float x3 = x1 + m_brick_width / 2;
        float y3 = y1 + m_brick_width * sqrt(3) / 2;
        r_col = 0.9f;
        g_col = 0.1f;
        b_col = 0.1f;
        m_triangles.push_back(Triangle(x1, y1, x2, y2, x3, y3, r_col, g_col, b_col));
        m_triangles_counter++;
    }

    for (int i = 0; i < 7; i++)
    {
        float x1 = i * m_brick_width + m_brick_start_x + m_brick_width / 2;
        float y1 = m_brick_start_y - m_brick_width * sqrt(3) / 2;
        float x2 = x1 + m_brick_width;
        float y2 = y1;
        float x3 = x1 + m_brick_width / 2;
        float y3 = y1 - m_brick_width * sqrt(3) / 2;
        r_col = 1.00f;
        g_col = 0.31f;
        b_col = 0.31f;
        m_triangles.push_back(Triangle(x1, y1, x2, y2, x3, y3, r_col, g_col, b_col));
        m_triangles_counter++;
    }

    for (int i = 0; i < 8; i++)
    {
        float x1 = i * m_brick_width + m_brick_start_x;
        float y1 = m_brick_start_y - m_brick_width * sqrt(3);
        float x2 = x1 + m_brick_width;
        float y2 = y1;
        float x3 = x1 + m_brick_width / 2;
        float y3 = y1 + m_brick_width * sqrt(3) / 2;
        r_col = 1.0f;
        g_col = 0.6f;
        b_col = 0.6f;
        m_triangles.push_back(Triangle(x1, y1, x2, y2, x3, y3, r_col, g_col, b_col));
        m_triangles_counter++;
    }
    fillBricks(arr);
}

void fillBricks(GLfloat *arr)
{
    for (int i = 0; i < m_triangles_counter; i++)
    {
        arr[0 + i * 15] = m_triangles.at(i).getA().getX();
        arr[1 + i * 15] = m_triangles.at(i).getA().getY();
        arr[2 + i * 15] = m_triangles.at(i).getRcol();
        arr[3 + i * 15] = m_triangles.at(i).getGcol();
        arr[4 + i * 15] = m_triangles.at(i).getBcol();

        arr[5 + i * 15] = m_triangles.at(i).getB().getX();
        arr[6 + i * 15] = m_triangles.at(i).getB().getY();
        arr[7 + i * 15] = m_triangles.at(i).getRcol();
        arr[8 + i * 15] = m_triangles.at(i).getGcol();
        arr[9 + i * 15] = m_triangles.at(i).getBcol();

        arr[10 + i * 15] = m_triangles.at(i).getC().getX();
        arr[11 + i * 15] = m_triangles.at(i).getC().getY();
        arr[12 + i * 15] = m_triangles.at(i).getRcol();
        arr[13 + i * 15] = m_triangles.at(i).getGcol();
        arr[14 + i * 15] = m_triangles.at(i).getBcol();
    }
}