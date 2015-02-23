#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace flopse
{
	class Transform
	{
	public:
		Transform();
		Transform(const glm::vec3 &position);
		~Transform();

		glm::mat4 rotationMatrix, translationMatrix;// , scaleMatrix;

		GLfloat pitch, yaw;

		void setPosition(const glm::vec3 &pos);
		void translate(const glm::vec3 &t);
		void rotate(float degrees, const glm::vec3 &axis);
		//void scale(const glm::vec3 &s);

		glm::mat4 getTransformMatrix() const;

		glm::vec3 getFront() const;
		glm::vec3 getUp() const;
		glm::vec3 getPosition() const;
	};
}