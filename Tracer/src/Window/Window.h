#pragma once

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Base.h"
#include "Shader.h"

namespace TC {

	class Window
	{
	public:
		Window(uint32_t width, uint32_t height, std::string title)
			: m_Width(width), m_Height(height), m_Title(title)
		{}

        ~Window()
		{
            // optional: de-allocate all resources once they've outlived their purpose:
			// ------------------------------------------------------------------------
            glDeleteVertexArrays(1, &m_VertexArray);
            glDeleteBuffers(1, &m_VertexBuffer);
            glDeleteBuffers(1, &m_IndexBuffer);

            // glfw: terminate, clearing all previously allocated GLFW resources.
            // ------------------------------------------------------------------
            glfwTerminate();
		}

		bool Init()
		{
			// glfw: initialize and configure
			// ------------------------------
			glfwInit();
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

			if (m_Width == NULL)
			{
				std::cerr << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				return false;
			}

			glfwMakeContextCurrent(m_Window);
			// glad: load all OpenGL function pointers
			// ---------------------------------------
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cerr << "Failed to initialize GLAD" << std::endl;
				return false;
			}

			m_Shader = CreateScope<Shader>("texture.vs", "texture.fs");

            // set up vertex data (and buffer(s)) and configure vertex attributes
			// ------------------------------------------------------------------
            float vertices[] = {
                // positions          // texture coords
                 1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
                 1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
                -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
                -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
            };
            unsigned int indices[] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
            };

            glGenVertexArrays(1, &m_VertexArray);
            glGenBuffers(1, &m_VertexBuffer);
            glGenBuffers(1, &m_IndexBuffer);

            glBindVertexArray(m_VertexArray);

            glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // color attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);


            // load and create a texture 
            // -------------------------

            glGenTextures(1, &m_Texture);

			glBindTexture(GL_TEXTURE_2D, m_Texture);
            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);

            // uint8_t* buffer = new uint8_t[3 * m_Width * m_Height];
            // std::memset(buffer, 1, 3 * m_Width * m_Height);

            // WriteToScreen(m_Width, m_Height, buffer);

			// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
            // -------------------------------------------------------------------------------------------
            m_Shader->Use(); // don't forget to activate/use the shader before setting uniforms!
            m_Shader->SetInt("texture1", 0);

            return true;
		}

        void Render()
		{
            // input
			// -----

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // bind textures on corresponding texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_Texture);

            // render container
            m_Shader->Use();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(m_Window);
            glfwPollEvents();
		}

        void WriteToScreen(uint32_t width, uint32_t height, void* data)
		{
            glBindTexture(GL_TEXTURE_2D, m_Texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glBindTexture(GL_TEXTURE_2D, 0);
		}

        bool ShouldWindowClose()
		{
            return glfwWindowShouldClose(m_Window);
		}
		
	private:
		std::string m_Title;
		GLFWwindow* m_Window;
		uint32_t m_Width, m_Height;
		Scope<Shader> m_Shader;

        GLuint m_VertexBuffer, m_VertexArray, m_IndexBuffer;
        GLuint m_Texture;
	};
}
