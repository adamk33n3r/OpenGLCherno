#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/imgui_internal.h"

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

const int WIDTH = 960;

void ReadLineFn(ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line)
{
	float r, g, b, a;
	if (sscanf(line, "ClearColor=%f,%f,%f,%f", &r, &g, &b, &a) == 4) {
		ImVec4* vec = ((ImVec4*)entry);
		vec->x = r;
		vec->y = g;
		vec->z = b;
		vec->w = a;
	}
}
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
void* ReadOpenFn(ImGuiContext* ctx, ImGuiSettingsHandler* handler, const char* name)
{
	std::cout << name << std::endl;
	return (void*)&clear_color;
}
int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowAspectRatio(window, 16, 9);
	glfwSetWindowSizeLimits(window, WIDTH, 540, GL_DONT_CARE, GL_DONT_CARE);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cerr << "Error!" << std::endl;
	}

	glfwSetKeyCallback(window, &KeyCallback);

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f, // 0
			 50.0f, -50.0f, 1.0f, 0.0f, // 1
			 50.0f,  50.0f, 1.0f, 1.0f, // 2
			-50.0f,  50.0f, 0.0f, 1.0f, // 3
		};

		unsigned int indicies[] = {
			0, 1, 2,
			2, 3, 0,
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indicies, 6);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::vec3 camera(0, 0, 0);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), camera);
		glm::mat4 model =
			glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))
			* glm::rotate(glm::mat4(1.0f), 0.5f, glm::vec3(0, 0, 1));
			//glm::rotate(glm::mat4(1.0f), 0.1f, glm::vec3(0, 0, 1))
			//* glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

		glm::mat4 mvp = proj * view * model;

		Shader shader("res/shaders/Basic.shader");
		Renderer renderer;
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture texture("res/textures/test.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();
		texture.Unbind();

		/**
		 * ImGui
		 */
		ImGuiContext* imguiContext = ImGui::CreateContext();
		ImGuiSettingsHandler handler;
		handler.TypeName = "Settings";
		handler.TypeHash = ImHash("Settings", 0, 0);
		handler.ReadOpenFn = ReadOpenFn;
		handler.ReadLineFn = ReadLineFn;
		handler.WriteAllFn = [](ImGuiContext* imgui_ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf) {
			buf->appendf("[%s][%s]\n", "Settings", "Settings");
			buf->appendf("ClearColor=%f,%f,%f,%f\n", clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			buf->appendf("\n");
		};
		imguiContext->SettingsHandlers.push_back(handler);
		ImGui_ImplGlfwGL3_Init(window, false);
		ImGui::StyleColorsDark();

		float r = 0.0f;
		float increment = 0.05f;
		bool show_demo_window = true;
		bool show_another_window = false;
		glm::vec3 translation(200, 200, 0);
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			renderer.Clear(clear_color.x, clear_color.y, clear_color.z, clear_color.z);

			ImGui_ImplGlfwGL3_NewFrame();

			// 1. Show a simple window.
			// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
			{
				static float f = 0.0f;
				static int counter = 0;
				ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
				ImGui::SliderFloat3("Translation", &translation.x, 0.0f, WIDTH);
				ImGui::SliderFloat3("Camera", &camera.x, 0.0f, WIDTH);
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
			}

			shader.Bind();
			texture.Bind();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
			glm::mat4 view = glm::translate(glm::mat4(1.0f), camera);
			glm::mat4 mvp = proj * view * model;
			shader.SetUniformMat4f("u_MVP", mvp);


			renderer.Draw(va, ib, shader);
			model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			mvp = proj * view * model;
			shader.SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(va, ib, shader);
			//texture.Unbind();
			//renderer.Draw(va, ib, shader);


			if (r > 1.0f) {
				increment = -0.05f;
			} else if (r < 0.0f) {
				increment = 0.05f;
			}

			r += increment;

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());
		}
	}

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}