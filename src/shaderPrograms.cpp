#include "shaderPrograms.hpp"

#include <string>

namespace ShaderPrograms
{
	std::string path(const std::string& shaderName);

	std::unique_ptr<const ShaderProgram> topFace{};
	std::unique_ptr<const ShaderProgram> sideFace{};
	std::unique_ptr<const ShaderProgram> bottomFace{};
	std::unique_ptr<const ShaderProgram> cutter{};

	void init()
	{
		topFace = std::make_unique<const ShaderProgram>(path("topFaceVS"), path("faceFS"));
		sideFace = std::make_unique<const ShaderProgram>(path("sideFaceVS"), path("faceFS"));
		bottomFace = std::make_unique<const ShaderProgram>(path("bottomFaceVS"), path("faceFS"));
		cutter = std::make_unique<const ShaderProgram>(path("cutterVS"), path("cutterFS"));
	}

	std::string path(const std::string& shaderName)
	{
		return "src/shaders/" + shaderName + ".glsl";
	}
}
