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

		glm::mat4 rotationMatrix, translationMatrix;

		float pitch, yaw;

		void setPosition(const glm::vec3 &pos);
		void translate(const glm::vec3 &t);
		void rotate(float degrees, const glm::vec3 &axis);

		glm::mat4 getTransformMatrix() const;

		glm::vec3 getFront() const;
		glm::vec3 getUp() const;
		glm::vec3 getPosition() const;

		static glm::mat4 getInverseTransform(glm::mat4 transform)
		{
			glm::mat4 inverse;

			glm::mat3 rotation(transform[0][0], transform[0][1], transform[0][2],
				transform[1][0], transform[1][1], transform[1][2],
				transform[2][0], transform[2][1], transform[2][2]);

			glm::vec3 translate(transform[3][0], transform[3][1], transform[3][2]);

			rotation = glm::transpose(rotation);

			translate = -1.f * (rotation * translate);

			inverse[0][0] = rotation[0][0];	inverse[1][0] = rotation[1][0];	inverse[2][0] = rotation[2][0];	inverse[3][0] = translate[0];
			inverse[0][1] = rotation[0][1];	inverse[1][1] = rotation[1][1];	inverse[2][1] = rotation[2][1];	inverse[3][1] = translate[1];
			inverse[0][2] = rotation[0][2];	inverse[1][2] = rotation[1][2];	inverse[2][2] = rotation[2][2];	inverse[3][2] = translate[2];
			inverse[0][3] = 0.f;			inverse[1][3] = 0.f;			inverse[2][3] = 0.f;			inverse[3][3] = 1.f;

			return inverse;
		}
	};
}