#include "gui.hpp"

#include "cutters/cutterType.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <algorithm>

GUI::GUI(GLFWwindow* window, Scene& scene, const glm::ivec2& windowSize) :
	m_scene{scene},
	m_windowSize{windowSize}
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::update()
{
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
	ImGui::SetNextWindowSize({width, static_cast<float>(m_windowSize.y)}, ImGuiCond_Always);
	ImGui::Begin("leftPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::PushItemWidth(100);

	updateSimulationSpeed();

	separator();

	updateMaterialSize();
	updateGridSize();

	separator();

	ImGui::Text("Cutter");
	updateCutterType();
	updateCutterDiameter();
	updateCutterMillingHeight();
	updateMaxMillingDepth();

	separator();

	updatePathsFilePath();
	updateButtons();

	ImGui::PopItemWidth();
	ImGui::End();
}

void GUI::render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::updateSimulationSpeed()
{
	static constexpr float stepPrecision = 0.1f;
	static const std::string format = "%.2f";

	float simulationSpeed = m_scene.getSimulationSpeed();
	float prevSimulationSpeed = simulationSpeed;

	ImGui::InputFloat("simulation speed", &simulationSpeed, stepPrecision, stepPrecision,
		format.c_str());

	simulationSpeed = std::max(simulationSpeed, 0.01f);

	if (simulationSpeed != prevSimulationSpeed)
	{
		m_scene.setSimulationSpeed(simulationSpeed);
	}
}

void GUI::updateMaterialSize()
{
	static constexpr float stepPrecision = 1.0f;
	static const std::string format = "%.1f";

	ImGui::Text("Material size");

	glm::vec3 materialSize = m_scene.getMaterialSize();
	glm::vec3 prevMaterialSize = materialSize;

	ImGui::InputFloat("x##materialSize", &materialSize.x, stepPrecision, stepPrecision,
		format.c_str());
	ImGui::InputFloat("y##materialSize", &materialSize.y, stepPrecision, stepPrecision,
		format.c_str());
	ImGui::InputFloat("z##materialSize", &materialSize.z, stepPrecision, stepPrecision,
		format.c_str());

	materialSize.x = std::max(materialSize.x, 1.0f);
	materialSize.y = std::max(materialSize.y, 1.0f);
	materialSize.z = std::max(materialSize.z, 1.0f);

	if (materialSize != prevMaterialSize)
	{
		m_scene.setMaterialSize(materialSize);
	}
}

void GUI::updateGridSize()
{
	static constexpr int stepPrecision = 1;

	ImGui::Text("Grid size");

	glm::ivec2 gridSize = m_scene.getGridSize();
	glm::ivec2 prevGridSize = gridSize;

	ImGui::InputInt("x##gridSize", &gridSize.x, stepPrecision, stepPrecision);
	ImGui::InputInt("y##gridSize", &gridSize.y, stepPrecision, stepPrecision);

	gridSize.x = std::max(gridSize.x, 1);
	gridSize.y = std::max(gridSize.y, 1);

	if (gridSize != prevGridSize)
	{
		m_scene.setGridSize(gridSize);
	}
}

void GUI::updateCutterType()
{
	CutterType cutterType = m_scene.getCutterType();
	CutterType prevCutterType = cutterType;

	if (ImGui::BeginCombo("##cutterType", cutterTypeLabels[static_cast<int>(cutterType)].c_str()))
	{
		for (int cutterTypeIndex = 0; cutterTypeIndex < cutterTypeCount; ++cutterTypeIndex)
		{
			bool isSelected = cutterTypeIndex == static_cast<int>(cutterType);
			if (ImGui::Selectable(cutterTypeLabels[cutterTypeIndex].c_str(), isSelected))
			{
				cutterType = static_cast<CutterType>(cutterTypeIndex);
			}
		}
		ImGui::EndCombo();
	}
	if (cutterType != prevCutterType)
	{
		m_scene.setCutterType(static_cast<CutterType>(cutterType));
	}
}

void GUI::updateCutterDiameter()
{
	static constexpr float stepPrecision = 0.1f;
	static const std::string format = "%.1f";

	float cutterDiameter = m_scene.getCutterDiameter();
	float prevCutterDiameter = cutterDiameter;

	ImGui::InputFloat("diameter##cutter", &cutterDiameter, stepPrecision, stepPrecision,
		format.c_str());

	cutterDiameter = std::max(cutterDiameter, 0.1f);

	if (cutterDiameter != prevCutterDiameter)
	{
		m_scene.setCutterDiameter(cutterDiameter);
	}
}

void GUI::updateCutterMillingHeight()
{
	static constexpr float stepPrecision = 0.1f;
	static const std::string format = "%.1f";

	float millingHeight = m_scene.getCutterMillingHeight();
	float prevMillingHeight = millingHeight;

	ImGui::InputFloat("milling height##cutter", &millingHeight, stepPrecision, stepPrecision,
		format.c_str());

	millingHeight = std::max(millingHeight, 0.1f);

	if (millingHeight != prevMillingHeight)
	{
		m_scene.setCutterMillingHeight(millingHeight);
	}
}

void GUI::updateMaxMillingDepth()
{
	static constexpr float stepPrecision = 0.1f;
	static const std::string format = "%.1f";

	float maxMillingDepth = m_scene.getMaxMillingDepth();
	float prevMaxMillingDepth = maxMillingDepth;

	ImGui::InputFloat("max milling depth##cutter", &maxMillingDepth, stepPrecision, stepPrecision,
		format.c_str());

	maxMillingDepth = std::max(maxMillingDepth, 0.1f);

	if (maxMillingDepth != prevMaxMillingDepth)
	{
		m_scene.setMaxMillingDepth(maxMillingDepth);
	}
}

void GUI::updatePathsFilePath()
{
	ImGui::InputText("##pathsFilePath", m_pathsFilePath.data(), m_pathsFilePath.size());
	ImGui::SameLine();
	if (ImGui::Button("Load paths"))
	{
		m_scene.loadPathsFile({m_pathsFilePath.data()});
		m_pathsFilePath[0] = '\0';
	}
}

void GUI::updateButtons()
{
	if (ImGui::Button("Mill"))
	{
		m_scene.mill();
	}
	if (ImGui::Button("Mill instantly"))
	{
		m_scene.millInstantly();
	}
	if (ImGui::Button("Reset"))
	{
		m_scene.reset();
	}
}

void GUI::separator()
{
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}
