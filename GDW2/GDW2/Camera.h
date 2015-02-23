#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include "SceneNode.h"

namespace flopse
{
	class Camera : public SceneNode
	{
	public:
		Camera();
		virtual ~Camera();

		glm::mat4 view;

		glm::vec3 getLocalPosition() const;
		glm::vec3 getGlobalPosition() const;

		void setPosition(const glm::vec3 &pos);
		void lookAt(const glm::vec3 &target);

		void recalculateView();
	};
}