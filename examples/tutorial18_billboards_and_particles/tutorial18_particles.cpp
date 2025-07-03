#include <cstdio>
#include <cstdlib>

#include <algorithm>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;


#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>

// CPU representation of a particle
struct Particle
{
    vec3 pos, speed;
    unsigned char r, g, b, a; // Color
    float size, angle, weight;
    float life; // Remaining life of the particle. If <0: dead and unused.
    float cameradistance; // *Squared* distance to the camera. if dead: -1.0f

    bool operator<(const Particle& that) const
    {
        // Sort in reverse order: far particles drawn first.
        return this->cameradistance > that.cameradistance;
    }
};

const auto TutorialSourcePath = "../tutorial18_billboards_and_particles/";
constexpr int MaxParticles = 100000;
Particle ParticlesContainer[MaxParticles];
int LastUsedParticle = 0;

// Finds a Particle in ParticlesContainer which isn't used yet.
// (i.e. life < 0);
int FindUnusedParticle()
{
    for (int i = LastUsedParticle; i < MaxParticles; i++)
    {
        if (ParticlesContainer[i].life < 0)
        {
            LastUsedParticle = i;
            return i;
        }
    }

    for (int i = 0; i < LastUsedParticle; i++)
    {
        if (ParticlesContainer[i].life < 0)
        {
            LastUsedParticle = i;
            return i;
        }
    }

    return 0; // All particles are taken, override the first one
}

void SortParticles()
{
    std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Tutorial 18 - Particles", nullptr, nullptr);
    if (window == nullptr)
    {
        fprintf(
            stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it is closer to the camera than the former one
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    // Create and compile our GLSL program from the shaders
    char vertexShaderPath[256];
    char fragmentShaderPath[256];

    sprintf(vertexShaderPath, "%s%s", TutorialSourcePath, "Billboard.vert");
    sprintf(fragmentShaderPath, "%s%s", TutorialSourcePath, "Billboard.frag");

    const GLuint programID = LoadShaders(vertexShaderPath, fragmentShaderPath);

    // Vertex shader
    const GLuint CameraRight_worldspace_ID = glGetUniformLocation(programID, "CameraRight_worldspace");
    const GLuint CameraUp_worldspace_ID = glGetUniformLocation(programID, "CameraUp_worldspace");
    const GLuint ViewProjMatrixID = glGetUniformLocation(programID, "VP");

    // fragment shader
    const GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");


    static auto g_particule_position_size_data = new GLfloat[MaxParticles * 4];
    static auto g_particule_color_data = new GLubyte[MaxParticles * 4];

    for (int i = 0; i < MaxParticles; i++)
    {
        ParticlesContainer[i].life = -1.0f;
        ParticlesContainer[i].cameradistance = -1.0f;
    }

    char particleDDSPath[256];

    sprintf(particleDDSPath, "%s%s", TutorialSourcePath, "particle.DDS");

    const GLuint Texture = loadDDS(particleDDSPath);

    // The VBO containing the 4 vertices of the particles.
    // Thanks to instancing, they will be shared by all particles.
    static constexpr GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
    };
    GLuint billboard_vertex_buffer;
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
    GLuint particles_position_buffer;
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), nullptr, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    GLuint particles_color_buffer;
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), nullptr, GL_STREAM_DRAW);


    double lastTime = glfwGetTime();
    do
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const double currentTime = glfwGetTime();
        const double delta = currentTime - lastTime;
        lastTime = currentTime;


        computeMatricesFromInputs();
        mat4 ProjectionMatrix = getProjectionMatrix();
        mat4 ViewMatrix = getViewMatrix();

        // We will need the camera's position to sort the particles
        // w.r.t the camera's distance.
        // There should be a getCameraPosition() function in common/controls.cpp,
        // but this works too.
        vec3 CameraPosition(inverse(ViewMatrix)[3]);

        mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;


        // Generate 10 new particles each millisecond,
        // but limit this to 16 ms (60 fps), or if you have 1 long frame (1 sec),
        // new particles will be huge and the next frame even longer.
        int newparticles = static_cast<int>(delta * 10000.0);
        if (newparticles > static_cast<int>(0.016f * 10000.0))
            newparticles = static_cast<int>(0.016f * 10000.0);

        for (int i = 0; i < newparticles; i++)
        {
            const int particleIndex = FindUnusedParticle();
            ParticlesContainer[particleIndex].life = 5.0f; // This particle will live 5 seconds.
            ParticlesContainer[particleIndex].pos = vec3(0, 0, -20.0f);

            float spread = 1.5f;
            auto maindir = vec3(0.0f, 10.0f, 0.0f);
            // Very bad way to generate a random direction;
            // See, for instance, http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
            // combined with some user-controlled parameters (main direction, spread, etc.)
            auto randomdir = vec3(
                (rand() % 2000 - 1000.0f) / 1000.0f,
                (rand() % 2000 - 1000.0f) / 1000.0f,
                (rand() % 2000 - 1000.0f) / 1000.0f
            );

            ParticlesContainer[particleIndex].speed = maindir + randomdir * spread;


            // Very bad way to generate a random color
            ParticlesContainer[particleIndex].r = rand() % 256;
            ParticlesContainer[particleIndex].g = rand() % 256;
            ParticlesContainer[particleIndex].b = rand() % 256;
            ParticlesContainer[particleIndex].a = rand() % 256 / 3;

            ParticlesContainer[particleIndex].size = rand() % 1000 / 2000.0f + 0.1f;
        }


        // Simulate all particles
        int ParticlesCount = 0;
        for (int i = 0; i < MaxParticles; i++)
        {
            if (Particle& p = ParticlesContainer[i]; p.life > 0.0f)
            {
                // Decrease life
                p.life -= delta;
                if (p.life > 0.0f)
                {
                    // Simulate simple physics : gravity only, no collisions
                    p.speed += vec3(0.0f, -9.81f, 0.0f) * static_cast<float>(delta) * 0.5f;
                    p.pos += p.speed * static_cast<float>(delta);
                    p.cameradistance = length2(p.pos - CameraPosition);
                    //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                    // Fill the GPU buffer
                    g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
                    g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
                    g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

                    g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

                    g_particule_color_data[4 * ParticlesCount + 0] = p.r;
                    g_particule_color_data[4 * ParticlesCount + 1] = p.g;
                    g_particule_color_data[4 * ParticlesCount + 2] = p.b;
                    g_particule_color_data[4 * ParticlesCount + 3] = p.a;
                }
                else
                {
                    // Particles that just died will be put at the end of the buffer in SortParticles();
                    p.cameradistance = -1.0f;
                }

                ParticlesCount++;
            }
        }

        SortParticles();

        //printf("%d ",ParticlesCount);

        // Update the buffers that OpenGL uses for rendering.
        // There are much more sophisticated means to stream data from the CPU to the GPU,
        // but this is outside the scope of this tutorial.
        // http://www.opengl.org/wiki/Buffer_Object_Streaming

        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), nullptr, GL_STREAM_DRAW);
        // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), nullptr, GL_STREAM_DRAW);
        // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Use our shader
        glUseProgram(programID);

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);

        // Same as the billboards tutorial
        glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
        glUniform3f(CameraUp_worldspace_ID, ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

        glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glVertexAttribPointer(
            0, // Attribute. No particular reason for 0, but must match the layout in the shader.
            3, // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            static_cast<void*>(nullptr // array buffer offset
            ) // array buffer offset
        );

        // 2nd attribute buffer: positions of particles' centers
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glVertexAttribPointer(
            1, // Attribute. No particular reason for 1, but must match the layout in the shader.
            4, // size : x + y + z + size => 4
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0, // stride
            static_cast<void*>(nullptr // array buffer offset
            ) // array buffer offset
        );

        // 3rd attribute buffer : particles' colors
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glVertexAttribPointer(
            2, // Attribute. No particular reason for 1, but must match the layout in the shader.
            4, // size : r + g + b + a => 4
            GL_UNSIGNED_BYTE, // type
            GL_TRUE,
            // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
            0, // stride
            static_cast<void*>(nullptr // array buffer offset
            ) // array buffer offset
        );

        // These functions are specific to glDrawArrays*Instanced*.
        // The first parameter is the attribute buffer we're talking about.
        // The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
        // http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
        glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
        glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
        glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

        // Draw the particles !
        // This draws many times a small triangle_strip (which looks like a quad).
        // This is equivalent to :
        // for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
        // but faster.
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);


    delete[] g_particule_position_size_data;

    // Cleanup VBO and shader
    glDeleteBuffers(1, &particles_color_buffer);
    glDeleteBuffers(1, &particles_position_buffer);
    glDeleteBuffers(1, &billboard_vertex_buffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);


    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
