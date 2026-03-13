#include "Window.h"
#include <iostream>
#include "Shader.h"
#include "Mesh.h"
#include "ShaderStorageBuffer.h"
#include "Compute.h"
#include "Random.h"
#include "Texture.h"

int main()
{
	try
	{
		Window window(1270, 720, "Window");
		
		Shader computeShader("assets/shaders/compute.comp");
		Shader appShader("assets/shaders/vertex.vert", "assets/shaders/fragment.frag");
		Shader counterShader("assets/shaders/counter.comp");

		std::vector<glm::vec2> velocities(100);

		for (size_t i = 0; i < velocities.size(); ++i)
		{
			velocities[i] = glm::vec2(
				generateRandom(-0.2f, 0.2f),
				generateRandom(-0.2f, 0.2f)
			);
		}
		 
		Mesh mesh(100);
		Compute compute(100, { 0.f, 0.f }, velocities);
		Texture texture("assets/images/bricks2.jpg");

		float lastTime = glfwGetTime();
		float deltaTime = 0.f;

		ShaderStorageBuffer counterSSBO(1 * sizeof(unsigned int), 3, 0);

		bool finished = false;

		while (!window.shouldClose())
		{
			unsigned int zero = 0;
			counterSSBO.uploadData(0, &zero);

			float currTime = glfwGetTime();
			deltaTime = currTime - lastTime;
			lastTime = currTime;

			window.pollEvents();

			computeShader.bind();
			computeShader.setUniformFloat("dt", deltaTime);
			computeShader.setUniformInt("numParticles", static_cast<int>(compute.getNumParticles()));
			computeShader.setUniformFloat("speed", 10.f);

			compute.bind();
			compute.dispatch(256);

			counterShader.bind();

			compute.bindAlpha();

			glDispatchCompute((compute.getNumParticles() + 255) / 256, 1, 1);

			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			unsigned int counter = counterSSBO.readUInt();

			if (counter == compute.getNumParticles())
			{
				unsigned int current = compute.getNumParticles();

				if (current < 1000000)
				{
					compute.resetParticles();
					mesh = Mesh(compute.getNumParticles());
				}
			}

			window.clear(1.f, 0.f, 0.f, 1.f);

			appShader.bind();
			texture.bind(0);
			appShader.setUniformInt("diffuse", 0);
			compute.bindAlpha();

			mesh.draw();

			window.swapBuffers();
		}
	}

	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
}