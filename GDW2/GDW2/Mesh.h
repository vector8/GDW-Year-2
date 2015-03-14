#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include <memory>
#include "Shader.h"
#include "Colour.h"

namespace flopse
{
	class Mesh
	{
	private:
		GLfloat *vertexData;
		int numVertices;

		float width, height, depth;

		bool loadFromFile(const std::string &fileName);

		sf::Texture* diffuseMap = NULL;
		sf::Texture* specularMap = NULL;

	public:
		Mesh(GLfloat *vertexData, int numVertices, std::shared_ptr<Shader> s, const std::string &diffuseMapFilename = "", const std::string &specularMapFilename = "", bool useUVs = true, bool useNormals = true, bool useColour = false);
		Mesh(const std::string &objFileName, std::shared_ptr<Shader> s, const std::string &diffuseMapFilename = "", const std::string &specularMapFilename = "");
		virtual ~Mesh();

		void refreshArrays();
		void initArrays(GLfloat *vertexData, int numVertices, bool useUVs = true, bool useNormals = true, bool useColour = false);
		void calculateDimensions(GLfloat *vertexData, int numVertices);

		std::shared_ptr<Shader> shader;
		GLuint VAO;
		GLuint VBO;
		Colour overlayColour;
		std::vector<GLfloat> objData;
		bool acceptShadow = false;
		bool useUVs;
		bool useNormals;
		bool useColour;
		float animationBlend = 0.f;

		void setDiffuseMap(const std::string &filename);
		void setDiffuseMap(sf::Texture *t);

		void setSpecularMap(const std::string &filename);
		void setSpecularMap(sf::Texture *t);

		void setPointColour(const Colour &c);

		int getNumberOfVertices() const;

		sf::Texture* getDiffuseMap() const;
		sf::Texture* getSpecularMap() const;

		float getWidth() const;
		float getHeight() const;
		float getDepth() const;
	};
}