#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>
#include "ShaderProgram.h"
#include "data.h"
#include "lodepng.h"

const char *ShaderProgram::VSFile = "../lab3_vs.glsl";
const char *ShaderProgram::FSFile = "../lab3_fs.glsl";

ShaderProgram *SP;

float viewFz = 2.0;
float viewFx = 0.0;
float viewFy = 0.0;

float gRot[2] = {0.0, 0.0};
float dAng = 0.1;
float Norms[180];

glm::vec4 lightCol[4] = {glm::vec4(1.0, 1.0, 1.0, 1.0),
                         glm::vec4(1.0, 1.0, 1.0, 1.0),
                         glm::vec4(1.0, 1.0, 1.0, 1.0),
                         glm::vec4(1.0, 1.0, 1.0, 1.0)};

glm::vec4 lightPos[4] = {glm::vec4(2.0, 0.0, 0.0, 0.0),
                         glm::vec4(-2.0, 0.0, 0.0, 0.0),
                         glm::vec4(0.0, 2.0, 0.0, 0.0),
                         glm::vec4(0.0, -2.0, 0.0, 0.0)};

glm::vec4 PolygonNorm(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    glm::vec3 u = p2 - p1;
    glm::vec3 v = p3 - p1;

    // std::cout << "p1: " << p1.x << " " << p1.y << " " << p1.z << '\n';
    // std::cout << "p2: " << p2.x << " " << p2.y << " " << p2.z << '\n';
    // std::cout << "p3: " << p3.x << " " << p3.y << " " << p3.z << '\n';

    // std::cout << "u: " << u.x << " " << u.y << " " << u.z << '\n';
    // std::cout << "v: " << v.x << " " << v.y << " " << v.z << '\n';
    glm::vec4 norm = glm::vec4(0, 0, 0, 0);

    norm.x = u.y * v.z - u.z * v.y;
    norm.y = u.z * v.x - u.x * v.z;
    norm.z = u.x * v.y - u.y * v.x;
    norm.w = 0.0;

    norm = glm::normalize(norm);
    // std::cout << "Norm length: " << glm::length(norm) << '\n';

    return norm;
}

unsigned char *image_data;
unsigned image_w;
unsigned image_h;
unsigned image_file;

unsigned readTextureFile(const char *filePath)
{
    unsigned tempFile = lodepng_decode32_file(&image_data, &image_w, &image_h, filePath);
    std::cout << "Loaded image [" << filePath << "] ("
              << image_w << "x" << image_h << ")\n";
    return tempFile;
}

void create2DTexture(int texIndex, GLuint texHandle, const char *filePath)
{
    glGenTextures(1, &texHandle);
    glActiveTexture(GL_TEXTURE0 + texIndex);
    glBindTexture(GL_TEXTURE_2D, texHandle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    // Load texture image
    image_file = readTextureFile(filePath);

    glTexImage2D(GL_TEXTURE_2D, 0 /* mipmap-level */,
                 GL_RGBA, image_w, image_h, 0 /* border */,
                 GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    glGenerateMipmap(GL_TEXTURE_2D);
}

static void error_callback(int error, const char *description)
{
    std::cerr << description;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if ((key == GLFW_KEY_R) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
        viewFz += 0.1;
    if ((key == GLFW_KEY_T) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
        viewFz -= 0.1;
    if ((key == GLFW_KEY_W) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
        SP->ReloadShaders();

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    // Update rotation angle here, for example
    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//

    if ((key == GLFW_KEY_UP) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
        gRot[0] -= dAng;
    if ((key == GLFW_KEY_DOWN) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
        gRot[0] += dAng;
    if ((key == GLFW_KEY_RIGHT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
        gRot[1] += dAng;
    if ((key == GLFW_KEY_LEFT) && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
        gRot[1] -= dAng;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char const *argv[])
{
    // start GL context and O/S window using the GLFW helper library

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    int w_height = 600;
    int w_width = 800;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow *window = glfwCreateWindow(w_width, w_height, "Hello Icosahedron", NULL, NULL);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"

    // Shaders
    SP = new ShaderProgram();
    SP->AddVertexShader();
    SP->AddFragmentShader();
    SP->Use();

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------//
    // Set up geometry, VBO, EBO, VAO
    // ----------Geometry----------
    // Vertices and indices in data.h

    for (int i = 0; i < sizeof(indices) / sizeof(GLushort); i += 3)
    {
        int v[3];

        for (int j = 0; j < 3; ++j)
        {
            v[j] = indices[i + j] * 3;
            // std::cout << " index: " << v[j] << " ";
        }
        // std::cout << '\n';

        glm::vec3 p1 = glm::vec3(positions[v[0] + 0], positions[v[0] + 1], positions[v[0] + 2]);
        glm::vec3 p2 = glm::vec3(positions[v[1] + 0], positions[v[1] + 1], positions[v[1] + 2]);
        glm::vec3 p3 = glm::vec3(positions[v[2] + 0], positions[v[2] + 1], positions[v[2] + 2]);

        glm::vec4 n = PolygonNorm(p1, p2, p3);

        for (int j = 0; j < 3; ++j)
        {
            Norms[v[j] + 0] = n.x;
            Norms[v[j] + 1] = n.y;
            Norms[v[j] + 2] = n.z;
        }
    }

    GLuint vao, vbo[3], ebo;

    // ------ VAO -----
    glGenVertexArrays(1, &vao);
    // Binded VAO will store connections between VBOs and attributes
    glBindVertexArray(vao);

    // ------ VBO ------
    glGenBuffers(3, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // ------ VBO Normals ------
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Norms), Norms, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // ------ VBO uv_coords ------
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv_coords), uv_coords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    // ------- EBO -------
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // reset bindings for VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // ------- Textures ------
    // Creates texture objects, with no content
    GLuint textureMap, environmentMap, bumpMap;

    // Texture mapping
    std::cout << "------Texture mapping------\n";
    create2DTexture(0, textureMap, "../../common/data/numberline_hires.png");

    // Draw
    glBindVertexArray(vao);
    // bind index buffer if you want to render indexed data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    std::cout << "Initializing rendering-loop\n";

    while (!glfwWindowShouldClose(window))
    {
        //you might need this for correcting the aspect ratio
        glfwGetFramebufferSize(window, &w_width, &w_height);

        // Define the projection matrix, rotation matrices, model matrix, etc.

        // 1. Set up model matrix
        glm::mat4 rotxM = glm::rotate(glm::mat4(1.0f), gRot[0], glm::vec3(1.0f, 0, 0));
        glm::mat4 modelMatrix = glm::rotate(rotxM, gRot[1], glm::vec3(0, 1.0f, 0));

        // 2. Set up view matrix
        glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, viewFz));
        glm::mat4 invViewMatrix = glm::inverse(viewMatrix);

        // 2.5 Set up modelview matrix
        glm::mat4 modelViewMatrix = invViewMatrix * modelMatrix;

        // 3. Set up projection matrix
        glm::mat4 projMatrix = glm::perspective(1.0f, 4.0f / 3.0f, 1.0f, 100.0f);

        // 4. Combine them
        glm::mat4 MVP = projMatrix * modelViewMatrix;

        // Set up normal matrix
        glm::mat4 NM = transpose(inverse(modelViewMatrix));

        // Unifoms
        GLint uniLoc = glGetUniformLocation(SP->program, "mv");
        glUniformMatrix4fv(uniLoc, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

        uniLoc = glGetUniformLocation(SP->program, "nm");
        glUniformMatrix4fv(uniLoc, 1, GL_FALSE, glm::value_ptr(NM));

        uniLoc = glGetUniformLocation(SP->program, "mvp");
        glUniformMatrix4fv(uniLoc, 1, GL_FALSE, glm::value_ptr(MVP));

        uniLoc = glGetUniformLocation(SP->program, "light_position");
        glUniform4fv(uniLoc, 4, glm::value_ptr(lightPos[0]));

        uniLoc = glGetUniformLocation(SP->program, "light_colour");
        glUniform4fv(uniLoc, 4, glm::value_ptr(lightCol[0]));

        GLuint texLoc = glGetUniformLocation(SP->program, "tex_sampler");
        /*  glUniform1i:
        *   The uniform value for a sampler refers to the texture unit,
        *   not the texture id.
        */
        glUniform1i(texLoc, 0); /* send texture in position 0 in texture array */

        // update other events like input handling
        glfwPollEvents();

        // clear the drawing surface
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // indexed draw call
        // glDrawRangeElements(GL_TRIANGLES, 0, 19,
        //                     sizeof(indices) / sizeof(GLushort),
        //                     GL_UNSIGNED_SHORT, 0);

        glDrawElements(GL_TRIANGLES,
                       sizeof(indices) / sizeof(GLushort),
                       GL_UNSIGNED_SHORT, 0);

        glfwSwapBuffers(window);
    }

    // close GL context and any other GLFW resources
    glfwTerminate();
    exit(EXIT_SUCCESS);
}