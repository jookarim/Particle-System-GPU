#pragma once

#include "ShaderStorageBuffer.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Random.h"
#include <vector>
#include <glm/gtc/random.hpp>

class Compute
{
private:
	ShaderStorageBuffer m_positionSSBO;
	ShaderStorageBuffer m_velocitySSBO;
	ShaderStorageBuffer m_alphaSSBO;
	std::vector<glm::vec2> m_velocites;

	unsigned int m_numParticles;

	void createSSBOs(unsigned int numParticles, const glm::vec2& startPos, const std::vector<glm::vec2>& randomVelocities);
public:
	Compute(unsigned int numParticles, const glm::vec2& startPos, const std::vector<glm::vec2>& randomVelocities);
	~Compute() noexcept = default;

	Compute(Compute&& other) noexcept;
	Compute& operator=(Compute&& other) noexcept;

	Compute(const Compute&) = delete;
	Compute& operator=(const Compute&) = delete;

	void bind() const;
	void bindPosition() const;
	void bindAlpha() const;

	void resetParticles();

	void dispatch(unsigned int localSize) const;

	unsigned int getNumParticles() const { return m_numParticles; }
	void setNumParticles(int numParticles) { m_numParticles = numParticles; }
};