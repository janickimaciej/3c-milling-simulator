#pragma once

#include "shaderProgram.hpp"

#include <memory>

namespace ShaderPrograms
{
	void init();

	extern std::unique_ptr<const ShaderProgram> topFace;
	extern std::unique_ptr<const ShaderProgram> sideFace;
	extern std::unique_ptr<const ShaderProgram> bottomFace;
	extern std::unique_ptr<const ShaderProgram> cutter;
}
