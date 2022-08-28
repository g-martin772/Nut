#pragma once

#include "../vendor/glm/glm/glm.hpp"

namespace Nut {
	class Camera {
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : m_Projection(projection) {}
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }

		// TODO SetPerspective, SetOrtho Camera
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}