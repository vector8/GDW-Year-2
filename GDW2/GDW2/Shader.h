#pragma once

#include <GL/glew.h>
#include <memory>

#define NUM_POINT_LIGHTS 1

namespace flopse
{
	struct PointLightLocs
	{
		GLint position;
		GLint ambient;
		GLint diffuse;
		GLint specular;
		GLint constantAttenuation;
		GLint linearAttenuation;
		GLint quadraticAttenuation;
	};

	struct DirectionalLightLocs
	{
		GLint direction;
		GLint ambient;
		GLint diffuse;
		GLint specular;
	};

	struct MaterialLocs
	{
		GLint diffuse;
		GLint specular;
		GLint specularExponent;
	};

	struct ShadowLocs
	{
		GLint worldToShadowMap;
		GLint shadowMapDepth;
		GLint drawShadow;
		GLint shadows;
	};

	enum class StandardShaders
	{
		Phong,
		PhongNoTexture,
		BloomHighPass,
		BloomComposite,
		BlurHorizontal,
		BlurVertical,
		Grayscale,
		ShadowMap,
		ShadowGenerator,
		ShadowComposite,
		Cel,
		Billboard
	};

	class Shader
	{
	private:
		GLuint createShader(const GLchar* sourcePath, GLenum shaderType);

	public:
		// Our program ID
		GLuint program;

		// Shader IDs
		GLuint vertexShader, fragmentShader, geometryShader;

		// Various uniform locations
		GLint modelLoc;
		GLint viewLoc;
		GLint projectionLoc;
		GLint objectColorLoc;
		GLint viewPosLoc;
		MaterialLocs materialLocs;
		PointLightLocs pointLightLocs[NUM_POINT_LIGHTS];
		DirectionalLightLocs directionalLightLocs;
		GLint thresholdLoc;
		GLint pixelSizeLoc;
		GLint sceneLoc;
		GLint bloomLoc;
		GLint fogFactorLoc;
		ShadowLocs shadowLocs;
		GLint lightPosLoc;

		// Constructor reads and builds our shader
		Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);
		Shader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath, const GLchar* geometrySourcePath);
		virtual ~Shader();

		void cacheUniformLocations();

		static std::shared_ptr<Shader> getStandardShader(StandardShaders type)
		{
			static std::shared_ptr<Shader> PHONG = std::make_shared<Shader>("shaders/StaticGeometry.vert", "shaders/Phong.frag");
			static std::shared_ptr<Shader> PHONG_NO_TEXTURE = std::make_shared<Shader>("shaders/StaticGeometry.vert", "shaders/PhongNoTexture.frag");
			static std::shared_ptr<Shader> BLOOM_HIGH_PASS = std::make_shared<Shader>("shaders/PosUVStraightPassThrough.vert", "shaders/bloom/BloomHighPass.frag");
			static std::shared_ptr<Shader> BLOOM_COMPOSITE = std::make_shared<Shader>("shaders/PosUVStraightPassThrough.vert", "shaders/bloom/BloomComposite.frag");
			static std::shared_ptr<Shader> BLUR_HORIZONTAL = std::make_shared<Shader>("shaders/PosUVStraightPassThrough.vert", "shaders/blur/BlurHorizontal.frag");
			static std::shared_ptr<Shader> BLUR_VERTICAL = std::make_shared<Shader>("shaders/PosUVStraightPassThrough.vert", "shaders/blur/BlurVertical.frag");
			static std::shared_ptr<Shader> GRAYSCALE = std::make_shared<Shader>("shaders/PosUVStraightPassThrough.vert", "shaders/GrayscalePost.frag");
			static std::shared_ptr<Shader> SHADOW_MAP = std::make_shared<Shader>("shaders/StaticGeometry.vert", "shaders/DoNothing.frag");
			static std::shared_ptr<Shader> SHADOW_GENERATOR = std::make_shared<Shader>("shaders/StaticGeometry.vert", "shaders/shadowmapping/GenerateShadows.frag");
			static std::shared_ptr<Shader> SHADOW_COMPOSITE = std::make_shared<Shader>("shaders/PosUVStraightPassThrough.vert", "shaders/shadowmapping/ShadowComposite.frag");
			static std::shared_ptr<Shader> CEL = std::make_shared<Shader>("shaders/StaticGeometry.vert", "shaders/CelShader.frag");
			static std::shared_ptr<Shader> BILLBOARD = std::make_shared<Shader>("shaders/particles/Billboard.vert", "shaders/particles/Billboard.frag", "shaders/particles/Billboard.geom");

			switch (type)
			{
			case flopse::StandardShaders::Phong:
				return PHONG;
				break;
			case flopse::StandardShaders::PhongNoTexture:
				return PHONG_NO_TEXTURE;
				break;
			case flopse::StandardShaders::BloomHighPass:
				return BLOOM_HIGH_PASS;
				break;
			case flopse::StandardShaders::BloomComposite:
				return BLOOM_COMPOSITE;
				break;
			case flopse::StandardShaders::BlurHorizontal:
				return BLUR_HORIZONTAL;
				break;
			case flopse::StandardShaders::BlurVertical:
				return BLUR_VERTICAL;
				break;
			case flopse::StandardShaders::Grayscale:
				return GRAYSCALE;
				break;
			case flopse::StandardShaders::ShadowMap:
				return SHADOW_MAP;
				break;
			case flopse::StandardShaders::ShadowGenerator:
				return SHADOW_GENERATOR;
				break;
			case flopse::StandardShaders::ShadowComposite:
				return SHADOW_COMPOSITE;
				break;
			case flopse::StandardShaders::Cel:
				return CEL;
				break;
			case flopse::StandardShaders::Billboard:
				return BILLBOARD;
				break;
			default:
				return nullptr;
				break;
			}
		}

		// Use our program
		void bind() const;

		void unbind() const;
	};
}