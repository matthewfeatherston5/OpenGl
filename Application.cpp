#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
  if (!glfwInit())
        return -1;

  if(glewInit()  != GLEW_OK)
       std::cout << "Error!" << std::endl;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
   window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    (glfwMakeContextCurrent(window));
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
           -0.5f,-0.5f, //0
            0.5f,-0.5f,  //1
            0.5f, 0.5f, //2
           -0.5f, 0.5f  //3

        };

        unsigned int indices[]{
            0, 1, 2,
            2, 3, 0

        };

        unsigned int vao; //creates vertex attribute obj
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        
        //VertexBuffer vb(positions, 4 * 2 * sizeof(float));

       // glEnableVertexAttribArray(0);
       // GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); //Gets bound to array vertex obj
        //GLCall(glEnableVertexAttribArray(0));

        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
       // ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
       // std::cout << "Vertex: " << std::endl;
      //  std::cout << source.VertexSource << std::endl;
       // std::cout << "Fragment:" << std::endl;
       // std::cout << source.FragmentSource << std::endl;

        //unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        //GLCall(glUseProgram(shader));

         //uniform vec4;


        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

        //std::string vertexShader =
          //   "#version 330 core\n"
            // "\n"
            //"layout(location = 0) in vec4 position;\n"
            // "\n"
             //"void main()\n"
             //"{\n"
             //"    gl_Position = position;\n"
             //"}\n";

       // std::string fragmentShader =
         //    "#version 330 core\n"
           //  "\n"
            // "layout(location = 0) out vec4 color;\n"
             //"\n"
             //"void main()\n"
             //"{\n"
             //"  color = vec4(0.2, 0.3, 0.8, 1.0);\n"
             //"}\n";

         //uniform vec4 u_Color;

       // unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);//vertexShader, fragmentShader);
       // GLCall(glUseProgram(shader));

       
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


        //Unbinds everything 
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

       
        float r = 0.0f;// red channel 
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            shader.Bind();//GLCall(glUseProgram(shader));//Bind shader
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
            

           // GLCall(glBindVertexArray(vao));
            va.Bind();
            // No longer b/c of vao GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
            // No longer b/c of vao GLCall(glEnableVertexAttribArray(0));//Selects layout of vertex buffer (Vertex Array subs this)
             //No longer b/c of vao GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));// (Vertex Array subs this) 


            ib.Bind();//binds index buffer

            GLClearError();
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));//Draws our obj
             //ASSERT(GLLogCall());

            if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < 0.0f)
            {
                increment = 0.05f;
                // r += increment;
            }

            r += increment;

            //glBegin(GL_TRIANGLES);
            //glVertex2f(-0.5f,-0.5f);
            //glVertex2f(0.0f,  0.5f);
            //glVertex2f(0.5f, -0.5f);
            //glEnd();

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

    }


    glfwTerminate();
    return 0;
}