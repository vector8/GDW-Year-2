#pragma once

#include <GL/glew.h>
#include "Shader.h"
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include "Colour.h"

namespace flopse
{
	class Model
	{
	private:
		GLuint VBO;

		GLfloat *vertexData;
		int numVertices;

		float width, height, depth;

		std::vector<GLfloat> objData;

		void loadOBJ(const std::string &fileName);

		sf::Texture* texture;

		bool useUVs;
		bool useNormals;
		bool useColour;

	public:
		Model(GLfloat *vertexData, int numVertices, Shader *s, bool useUVs = true, bool useNormals = true, bool useColour = false);
		Model(const std::string &objFileName, Shader *s);

		void initArrays(GLfloat *vertexData, int numVertices, bool useUVs = true, bool useNormals = true, bool useColour = false);
		void calculateDimensions(GLfloat *vertexData, int numVertices);

		Shader *shader;
		GLuint VAO;
		Colour overlayColour;

		void setTexture(const std::string &filename);
		void setTexture(sf::Texture *t);

		void setPointColour(const Colour &c);

		int getNumberOfVertices() const;

		sf::Texture* getTexture() const;

		float getWidth() const;
		float getHeight() const;
		float getDepth() const;
	};
}