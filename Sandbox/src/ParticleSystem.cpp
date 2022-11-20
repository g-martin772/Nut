//#include "ParticleSystem.h"
//
//#include <random>
//
//class Random
//{
//public:
//	static void Init()
//	{
//		s_RandomEngine.seed(std::random_device()());
//	}
//
//	static float Float()
//	{
//		return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
//	}
//
//private:
//	static std::mt19937 s_RandomEngine;
//	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
//};
//
//std::mt19937 Random::s_RandomEngine;
//std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;
//
//ParticleSystem::ParticleSystem()
//{
//	m_ParticlePool.resize(100000);
//}
//
//void ParticleSystem::OnUpdate(Nut::Timestep ts)
//{
//	for (auto& particle : m_ParticlePool)
//	{
//		if (!particle.Active)
//			continue;
//
//		if (particle.LifeRemaining <= 0.0f)
//		{
//			particle.Active = false;
//			continue;
//		}
//
//		particle.LifeRemaining -= ts;
//		particle.Position += particle.Velocity * (float)ts;
//		particle.Rotation += 0.01f * ts;
//	}
//}
//
//void ParticleSystem::OnRender(Nut::OrthographicCamera& camera)
//{
//	if (!m_QuadVA)
//	{
//		float vertices[] = {
//			 -0.5f, -0.5f, 0.0f,
//			  0.5f, -0.5f, 0.0f,
//			  0.5f,  0.5f, 0.0f,
//			 -0.5f,  0.5f, 0.0f
//		};
//	}
//
//	Nut::Renderer2D::BeginScene(camera);
//	for (auto& particle : m_ParticlePool)
//	{
//		if (!particle.Active)
//			continue;
//
//		// Fade away particles
//		float life = particle.LifeRemaining / particle.LifeTime;
//		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
//		//color.a = color.a * life;
//
//		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
//
//		glm::mat4 transform = 
//			glm::translate(glm::mat4(), { particle.Position, 1 })
//			* glm::scale(glm::mat4(), { size, size, 1 })
//			* glm::rotate(glm::mat4(), glm::degrees(particle.Rotation), {0, 0, 1});
//
//		Nut::Renderer2D::DrawQuad(transform, color, -1);
//	}
//	Nut::Renderer2D::EndScene();
//}
//
//void ParticleSystem::Emit(const ParticleProps& particleProps)
//{
//	Particle& particle = m_ParticlePool[m_PoolIndex];
//	particle.Active = true;
//	particle.Position = particleProps.Position;
//	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();
//
//	// Velocity
//	particle.Velocity = particleProps.Velocity;
//	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
//	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);
//
//	// Color
//	particle.ColorBegin = particleProps.ColorBegin;
//	particle.ColorEnd = particleProps.ColorEnd;
//
//	particle.LifeTime = particleProps.LifeTime;
//	particle.LifeRemaining = particleProps.LifeTime;
//	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
//	particle.SizeEnd = particleProps.SizeEnd;
//
//	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
//}