#include "shaderPrograms.hpp"

#include <string>

namespace ShaderPrograms
{
	std::string path(const std::string& shaderName);

	std::unique_ptr<const ShaderProgram> topFace{};
	std::unique_ptr<const ShaderProgram> sideFace{};
	std::unique_ptr<const ShaderProgram> bottomFace{};

	void init()
	{
		topFace = std::make_unique<const ShaderProgram>(path("topFaceVS"), path("FS"));
		sideFace = std::make_unique<const ShaderProgram>(path("sideFaceVS"), path("FS"));
		bottomFace = std::make_unique<const ShaderProgram>(path("bottomFaceVS"), path("FS"));
	}

	std::string path(const std::string& shaderName)
	{
		return "src/shaders/" + shaderName + ".glsl";
	}
}
