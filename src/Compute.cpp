#include "Compute.h"

void Compute::createSSBOs(unsigned int numParticles, const glm::vec2& startPos, const std::vector<glm::vec2>& randomVelocities)
{
	std::vector<glm::vec2> positions(numParticles, startPos);
	std::vector<float> transparency(numParticles, 1.f);

	m_velocites = randomVelocities;

	m_positionSSBO = ShaderStorageBuffer(numParticles * sizeof(glm::vec2), 0, positions.data());
	m_velocitySSBO = ShaderStorageBuffer(numParticles * sizeof(glm::vec2), 1, m_velocites.data());
	m_alphaSSBO = ShaderStorageBuffer(numParticles * sizeof(float), 2, transparency.data());
	m_numParticles = numParticles;
}

Compute::Compute(unsigned int numParticles, const glm::vec2& startPos, const std::vector<glm::vec2>& randomVertices)
{
	createSSBOs(numParticles, startPos, randomVertices);
}

Compute::Compute(Compute&& other) noexcept
{
	m_numParticles = other.m_numParticles;
	m_positionSSBO = std::move(other.m_positionSSBO);
	m_velocitySSBO = std::move(other.m_velocitySSBO);
	m_alphaSSBO = std::move(other.m_alphaSSBO);

	other.m_numParticles = 0;
}

Compute& Compute::operator=(Compute&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}

	m_numParticles = other.m_numParticles;
	m_positionSSBO = std::move(other.m_positionSSBO);
	m_velocitySSBO = std::move(other.m_velocitySSBO);
	m_alphaSSBO = std::move(other.m_alphaSSBO);
	other.m_numParticles = 0;

	return *this;
}

void Compute::bind() const
{
	m_positionSSBO.bind();
	m_velocitySSBO.bind();
	m_alphaSSBO.bind();
}

void Compute::bindPosition() const
{
	m_positionSSBO.bind();
}

void Compute::dispatch(unsigned int localSize) const
{
	glDispatchCompute((m_numParticles + localSize - 1) / localSize, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Compute::bindAlpha() const
{
	m_alphaSSBO.bind();
}

void Compute::resetParticles()
{
	if (m_numParticles >= 100000) {
		m_numParticles = 100;
	}
	else {
		m_numParticles *= 10;
	}

    std::vector<glm::vec2> newPositions(m_numParticles, glm::vec2(0.f));
    std::vector<float> newAlpha(m_numParticles, 1.f);
    std::vector<glm::vec2> newVelocities(m_numParticles);

    for (size_t i = 0; i < m_numParticles; ++i)
    {
        newVelocities[i] = glm::vec2(generateRandom(-0.2f, 0.2f), generateRandom(-0.2f, 0.2f));
    }

    m_velocites = newVelocities;

    m_positionSSBO = ShaderStorageBuffer(m_numParticles * sizeof(glm::vec2), 0, newPositions.data());

    m_velocitySSBO = ShaderStorageBuffer(m_numParticles * sizeof(glm::vec2), 1, newVelocities.data());

    m_alphaSSBO = ShaderStorageBuffer(m_numParticles * sizeof(float), 2, newAlpha.data());
}


