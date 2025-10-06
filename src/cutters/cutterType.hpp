#pragma once

#include <array>
#include <string>

inline constexpr int cutterTypeCount = 2;

enum class CutterType
{
	flat,
	round
};

inline const std::array<const std::string, cutterTypeCount> cutterTypeLabels
{
	"Flat",
	"Round"
};
