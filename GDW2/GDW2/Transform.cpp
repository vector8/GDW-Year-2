#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

namespace flopse
{
	Transform::Transform() : pitch(0.f), yaw(0.f), translationMatrix(glm::mat4()), scaleMatrix(glm::mat4()), rotationMatrix(glm::mat4())
	{

	}

	Transform::Transform(const glm::vec3 &position) : pitch(0.f), yaw(0.f), translationMatrix(glm::mat4()), scaleMatrix(glm::mat4()), rotationMatrix(glm::mat4())
	{
		setPosition(position);
	}

	void Transform::setPosition(const glm::vec3 &pos)
	{
		glm::mat4 temp;
		temp = glm::translate(temp, pos);
		translationMatrix = temp;
	}

	void Transform::translate(const glm::vec3 &t)
	{
		glm::mat4 temp;
		temp = glm::translate(temp, t);
		translationMatrix = temp * translationMatrix;
	}

	void Transform::rotate(float degrees, const glm::vec3 &axis)
	{
		glm::mat4 temp;
		temp = glm::rotate(temp, degrees, axis);
		rotationMatrix = temp * rotationMatrix;
	}

	void Transform::scale(const glm::vec3 &s)
	{
		glm::mat4 temp;
		temp = glm::scale(temp, s);
		scaleMatrix = temp * scaleMatrix;
	}

	glm::mat4 Transform::getTransformMatrix() const
	{
		return (translationMatrix * rotationMatrix) * scaleMatrix;
	}

	glm::vec3 Transform::getFront() const
	{
		glm::vec4 front(0.f, 0.f, 1.f, 1.f);

		front = rotationMatrix * front;

		return glm::vec3(front.x, front.y, front.z);
	}

	glm::vec3 Transform::getUp() const
	{
		/*glm::vec4 up(0.f, 1.f, 0.f, 1.f);

		up = rotationMatrix * up;

		return glm::vec3(up.x, up.y, up.z);*/

		return glm::vec3(0.f, 1.f, 0.f);
	}

	glm::vec3 Transform::getPosition() const
	{
		return glm::vec3(translationMatrix[3][0], translationMatrix[3][1], translationMatrix[3][2]);
	}

	Transform operator*(const Transform &lhs, const Transform &rhs)
	{
		Transform result;

		result.translationMatrix = lhs.translationMatrix * rhs.translationMatrix;
		result.rotationMatrix = lhs.rotationMatrix * rhs.rotationMatrix;
		result.scaleMatrix = lhs.scaleMatrix * rhs.scaleMatrix;

		return result;
	}
}