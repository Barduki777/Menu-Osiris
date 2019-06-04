#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <Windows.h>
#include "imgui/imgui.h"
#include "GUI.h"
#include "Config.h"
#include "Hacks/Misc.h"
#include "Hacks/Visuals.h"
#include "Hooks.h"

constexpr auto windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

void GUI::render() noexcept
{
    renderMenuBar();
	MainMenu();
}

extern void draw_gui();

void GUI::checkboxedColorPicker(const std::string& name, bool* enable, float* color) noexcept
{
    ImGui::Checkbox(("##" + name).c_str(), enable);
    ImGui::SameLine(0.0f, 5.0f);
    ImGui::PushID(0);
    bool openPopup = ImGui::ColorButton(("##" + name).c_str(), ImColor{ color[0], color[1], color[2] }, ImGuiColorEditFlags_NoTooltip);
    ImGui::PopID();
    ImGui::SameLine(0.0f, 5.0f);
    ImGui::Text(name.c_str());
    ImGui::PushID(1);
    if (openPopup)
        ImGui::OpenPopup(("##" + name).c_str());
    if (ImGui::BeginPopup(("##" + name).c_str())) {
        ImGui::PushID(2);
        ImGui::ColorPicker3(("##" + name).c_str(), color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
        ImGui::PopID();
        ImGui::EndPopup();
    }
    ImGui::PopID();
}

void GUI::hotkey(int& key) noexcept
{
    key ? ImGui::Text("[ 0x%x ]", key) : ImGui::Text("[ key ]");
    if (ImGui::IsItemHovered()) {
        ImGuiIO& io = ImGui::GetIO();
        for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
            if (ImGui::IsKeyPressed(i) && i != config.misc.menuKey)
                key = i != VK_ESCAPE ? i : 0;

        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
            if (ImGui::IsMouseDown(i) && i + (i > 1 ? 2 : 1) != config.misc.menuKey)
                key = i + (i > 1 ? 2 : 1);
    }
}

void GUI::renderMenuBar() noexcept
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(0, 0);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.0f;
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.ChildRounding = 0.0f;
	style.FramePadding = ImVec2(4, 3);
	style.FrameRounding = 0.0f;
	style.ItemSpacing = ImVec2(8, 8);
	style.ItemInnerSpacing = ImVec2(8, 8);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 0.0f;
	style.ScrollbarSize = 6.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 0.0f;
	style.ButtonTextAlign = ImVec2(0.0f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.f;

	ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
	ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
	ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(33 / 255.f, 35 / 255.f, 47 / 255.f, 1.0f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(30 / 255.f, 30 / 255.f, 41 / 255.f, 1.0f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(33 / 255.f, 35 / 255.f, 47 / 255.f, 1.0f);
	colors[ImGuiCol_FrameBg] = ImVec4(33 / 255.f, 35 / 255.f, 47 / 255.f, 1.0f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
	colors[ImGuiCol_TitleBg] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(33 / 255.f, 35 / 255.f, 47 / 255.f, 1.0f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f); //
	colors[ImGuiCol_ButtonActive] = ImVec4(135 / 255.f, 135 / 255.f, 135 / 255.f, 1.0f); //
	colors[ImGuiCol_Header] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f); //multicombo, combo selected item color.
	colors[ImGuiCol_HeaderHovered] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
	colors[ImGuiCol_HeaderActive] = ImVec4(35 / 255.f, 35 / 255.f, 35 / 255.f, 1.0f);
	colors[ImGuiCol_Separator] = ImVec4(0, 0, 0, 1);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0, 0, 0, 1);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0, 0, 0, 1);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

	window.MainMenu = true;
}

void GUI::MainMenu() noexcept {
	static int page = 0;
	if (window.MainMenu) {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));

		ImGui::SetNextWindowSize(ImVec2(620, 660));
		ImGui::Begin("Cheat", &window.MainMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
		ImVec2 p = ImGui::GetCursorScreenPos();                                       //a bit lower then WindowWeight (605)
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 30), ImVec2(p.x + 605, p.y - 3), ImColor(30, 30, 39));
		//a bit lower then WindowWeight (605)
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y + 32), ImVec2(p.x + 605, p.y + +30), ImColor(255, 200, 0, 255), ImColor(255, 100, 0, 255), ImColor(255, 100, 0, 255), ImColor(255, 200, 0, 255));
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
		ImGui::Text("");
		ImGui::SameLine(150, 0);
		ImGui::Text("     ");// xdxdxdddddddddddddddddddddd
		ImGui::SameLine(0, 9);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9);
		if (ImGui::ButtonT("Legit", ImVec2(45, 30), page, 0, false, false)) page = 0; ImGui::SameLine(0, 1);
		if (ImGui::ButtonT("Visuals", ImVec2(45, 30), page, 1, false, false)) page = 1; ImGui::SameLine(0, 0);
		if (ImGui::ButtonT("Misc", ImVec2(45, 30), page, 2, false, false)) page = 2; ImGui::SameLine(0, 0);
		if (ImGui::ButtonT("Skins", ImVec2(45, 30), page, 3, false, false)) page = 3; ImGui::SameLine(0, 0);
		if (ImGui::ButtonT("Config", ImVec2(45, 30), page, 4, false, false)) page = 4; ImGui::SameLine(0, 0);

		ImGui::Spacing();
		switch (page) {
		case 0:
			ImGui::Columns(2, NULL, false);
			ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

			//push window color for child
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
			//push border color for child
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

			ImGui::BeginChild("Aimbot", ImVec2(280, 460), true); {
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
				ImGui::Text("  Aimbot");
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				static int currentCategory{ 0 };
				ImGui::PushItemWidth(110.0f);
				ImGui::PushID(0);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Combo("", &currentCategory, "All\0Pistols\0Heavy\0SMG\0Rifles\0");
				ImGui::PopID();
				ImGui::SameLine();
				static int currentWeapon{ 0 };
				ImGui::PushID(1);

				switch (currentCategory) {
				case 0:
					currentWeapon = 0;
					ImGui::NewLine();
					break;
				case 1: {
					static int currentPistol{ 0 };
					ImGui::Combo("", &currentPistol, "Glock-18\0P2000\0USP-S\0Dual Berettas\0P250\0Tec-9\0Five-Seven\0CZ-75\0Desert Eagle\0Revolver\0");
					currentWeapon = currentPistol + 1;
					break;
				}
				case 2: {
					static int currentHeavy{ 0 };
					ImGui::Combo("", &currentHeavy, "Nova\0XM1014\0Sawed-off\0MAG-7\0M249\0Negev\0");
					currentWeapon = currentHeavy + 11;
					break;
				}
				case 3: {
					static int currentSmg{ 0 };
					ImGui::Combo("", &currentSmg, "Mac-10\0MP9\0MP7\0MP5-SD\0UMP-45\0P90\0PP-Bizon\0");
					currentWeapon = currentSmg + 17;
					break;
				}
				case 4: {
					static int currentRifle{ 0 };
					ImGui::Combo("", &currentRifle, "Galil AR\0Famas\0AK-47\0M4A4\0M4A1-S\0SSG-08\0SG-553\0AUG\0AWP\0G3SG1\0SCAR-20\0");
					currentWeapon = currentRifle + 24;
					break;
				}
				}
				ImGui::PopID();
				ImGui::Checkbox("Enabled", &config.aimbot[currentWeapon].enabled);
				//	ImGui::SameLine();
					//ImGui::Checkbox("On key", &config.aimbot[currentWeapon].onKey);
					//				hotkey(config.aimbot[currentWeapon].key);
				ImGui::Checkbox("Silent", &config.aimbot[currentWeapon].silent);
				ImGui::Checkbox("Friendly fire", &config.aimbot[currentWeapon].friendlyFire);
				ImGui::Checkbox("Visible only", &config.aimbot[currentWeapon].visibleOnly);
				ImGui::Checkbox("Scoped only", &config.aimbot[currentWeapon].scopedOnly);
				ImGui::Checkbox("Ignore flash", &config.aimbot[currentWeapon].ignoreFlash);
				ImGui::Checkbox("Ignore smoke", &config.aimbot[currentWeapon].ignoreSmoke);
				ImGui::Checkbox("Auto shot", &config.aimbot[currentWeapon].autoShot);
				ImGui::Checkbox("Recoil-based fov", &config.aimbot[currentWeapon].recoilbasedFov);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Combo("Bone", &config.aimbot[currentWeapon].bone, "Nearest\0Head\0Neck\0Sternum\0Chest\0Stomach\0Pelvis\0");
				ImGui::PushItemWidth(240.0f);
				ImGui::PushID(5);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderFloat("", &config.aimbot[currentWeapon].fov, 0.0f, 255.0f, "Fov: %.2f");
				ImGui::PopID();
				ImGui::PushID(6);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderFloat("", &config.aimbot[currentWeapon].maxAngleDelta, 0.0f, 255.0f, "Max angle delta: %.2f");
				ImGui::PopID();
				ImGui::PushID(7);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderFloat("", &config.aimbot[currentWeapon].smooth, 1.0f, 100.0f, "Smooth: %.2f");
				ImGui::PopID();
				ImGui::PushID(8);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderFloat("", &config.aimbot[currentWeapon].recoilControlX, 0.0f, 1.0f, "Recoil control x: %.2f");
				ImGui::PopID();
				ImGui::PushID(9);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderFloat("", &config.aimbot[currentWeapon].recoilControlY, 0.0f, 1.0f, "Recoil control y: %.2f");
				ImGui::PopID();
			}ImGui::EndChild();

			ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

			ImGui::BeginChild("Backtrack", ImVec2(280, 95), true); {
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
				ImGui::Text("  Backtrack");
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				ImGui::Checkbox("Enabled", &config.backtrack.enabled);
				ImGui::Checkbox("Ignore smoke", &config.backtrack.ignoreSmoke);
				ImGui::PushItemWidth(220.0f);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderInt("", &config.backtrack.timeLimit, 1, 200, "%d ms");
				ImGui::PopItemWidth();
			}ImGui::EndChild();

			ImGui::NextColumn();

			ImGui::BeginChild("Triggerbot", ImVec2(280, 460), true); {

				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);

				ImGui::Text("  Triggerbot");

				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

				static int currentCategory{ 0 };
				ImGui::PushItemWidth(110.0f);
				ImGui::PushID(0);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Combo("", &currentCategory, "All\0Pistols\0Heavy\0SMG\0Rifles\0");
				ImGui::PopID();
				ImGui::SameLine();
				static int currentWeapon{ 0 };
				ImGui::PushID(1);
				switch (currentCategory) {
				case 0:
					currentWeapon = 0;
					ImGui::NewLine();
					break;
				case 1: {
					static int currentPistol{ 0 };
					ImGui::Combo("", &currentPistol, "Glock-18\0P2000\0USP-S\0Dual Berettas\0P250\0Tec-9\0Five-Seven\0CZ-75\0Desert Eagle\0Revolver\0");
					currentWeapon = currentPistol + 1;
					break;
				}
				case 2: {
					static int currentHeavy{ 0 };
					ImGui::Combo("", &currentHeavy, "Nova\0XM1014\0Sawed-off\0MAG-7\0M249\0Negev\0");
					currentWeapon = currentHeavy + 11;
					break;
				}
				case 3: {
					static int currentSmg{ 0 };
					ImGui::Combo("", &currentSmg, "Mac-10\0MP9\0MP7\0MP5-SD\0UMP-45\0P90\0PP-Bizon\0");
					currentWeapon = currentSmg + 17;
					break;
				}
				case 4: {
					static int currentRifle{ 0 };
					ImGui::Combo("", &currentRifle, "Galil AR\0Famas\0AK-47\0M4A4\0M4A1-S\0SSG-08\0SG-553\0AUG\0AWP\0G3SG1\0SCAR-20\0");
					currentWeapon = currentRifle + 24;
					break;
				}
				}
				ImGui::PopID();
				ImGui::Checkbox("Enabled", &config.triggerbot[currentWeapon].enabled);
				ImGui::Checkbox("On key", &config.triggerbot[currentWeapon].onKey);
				ImGui::SameLine();
				hotkey(config.triggerbot[currentWeapon].key);
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Set the key while hovering and pressing the key.");
				}
				ImGui::Checkbox("Friendly fire", &config.triggerbot[currentWeapon].friendlyFire);
				ImGui::Checkbox("Scoped only", &config.triggerbot[currentWeapon].scopedOnly);
				ImGui::Checkbox("Ignore flash", &config.triggerbot[currentWeapon].ignoreFlash);
				ImGui::Checkbox("Ignore smoke", &config.triggerbot[currentWeapon].ignoreSmoke);
				ImGui::PushItemWidth(85.0f);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Combo("Hitgroup", &config.triggerbot[currentWeapon].hitgroup, "All\0Head\0Chest\0Stomach\0Left arm\0Right arm\0Left leg\0Right leg\0");
				ImGui::PushItemWidth(220.0f);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderInt("", &config.triggerbot[currentWeapon].shotDelay, 0, 250, "Shot delay: %d ms");

			}ImGui::EndChild();
			break;
		case 1:
			ImGui::Columns(2, NULL, false);
			ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

			//push window color for child
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
			//push border color for child
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
			ImGui::BeginChild("Visuals", ImVec2(280, 165), true); {
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
				ImGui::Text("  Esp");
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				static int currentCategory{ 0 };
				ImGui::PushItemWidth(110.0f);
				ImGui::PushID(0);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Combo("", &currentCategory, "Allies\0Enemies\0");
				ImGui::PopID();
				ImGui::SameLine();
				static int currentType{ 0 };
				ImGui::PushID(1);
				ImGui::Combo("", &currentType, "Visible\0Occluded\0");
				ImGui::PopID();
				int currentItem = currentCategory * 2 + currentType;
				ImGui::Checkbox("Enabled", &config.esp[currentItem].enabled);/*
				checkboxedColorPicker("Snaplines", &config.esp[currentItem].snaplines, config.esp[currentItem].snaplinesColor);
				checkboxedColorPicker("Box", &config.esp[currentItem].box, config.esp[currentItem].boxColor);
				checkboxedColorPicker("Name", &config.esp[currentItem].name, config.esp[currentItem].nameColor);
				checkboxedColorPicker("Head dot", &config.esp[currentItem].headDot, config.esp[currentItem].headDotColor);*/
				ImGui::Checkbox("Box", &config.esp[currentItem].box);
				ImGui::Checkbox("Name", &config.esp[currentItem].name);
				ImGui::Checkbox("Snaplines", &config.esp[currentItem].snaplines);
				ImGui::Checkbox("Head Dote", &config.esp[currentItem].headDot);
			}ImGui::EndChild();

			ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

			ImGui::BeginChild("Glow", ImVec2(280, 240), true); {
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
				ImGui::Text("  Glow");
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				static int currentCategory{ 0 };
				ImGui::PushItemWidth(110.0f);
				ImGui::PushID(0);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Combo("", &currentCategory, "Allies\0Enemies\0Planting\0Defusing\0Local player\0Weapons\0C4\0Planted C4\0Chickens\0");
				ImGui::PopID();
				static int currentItem{ 0 };
				if (currentCategory <= 3) {
					ImGui::SameLine();
					static int currentType{ 0 };
					ImGui::PushID(1);
					ImGui::Combo("", &currentType, "Visible\0Occluded\0");
					ImGui::PopID();
					currentItem = currentCategory * 2 + currentType;
				}
				else {
					currentItem = currentCategory + 4;
				}

				ImGui::Checkbox("Enabled", &config.glow[currentItem].enabled);
				ImGui::Checkbox("Health based", &config.glow[currentItem].healthBased);
				ImGui::Checkbox("Rainbow", &config.glow[currentItem].rainbow);
				bool openPopup = ImGui::ColorButton("Color", ImVec4{ config.glow[currentItem].color }, ImGuiColorEditFlags_NoTooltip);
				ImGui::SameLine(0.0f, 5.0f);
				ImGui::Text("Color");
				ImGui::PushID(2);
				if (openPopup)
					ImGui::OpenPopup("");
				if (ImGui::BeginPopup("")) {
					ImGui::PushID(3);
					ImGui::ColorPicker3("", config.glow[currentItem].color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
					ImGui::PopID();
					ImGui::EndPopup();
				}
				ImGui::PopID();
				ImGui::PushItemWidth(220.0f);
				ImGui::PushID(4);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderFloat("", &config.glow[currentItem].thickness, 0.0f, 1.0f, "Thickness: %.2f");
				ImGui::PopID();
				ImGui::PushID(5);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderFloat("", &config.glow[currentItem].alpha, 0.0f, 1.0f, "Alpha: %.2f");
				ImGui::PopID();
				ImGui::PushID(6);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderInt("", &config.glow[currentItem].style, 0, 3, "Style: %d");
				ImGui::PopID();
			}
			ImGui::EndChild();

			ImGui::NextColumn();

			ImGui::BeginChild("Chams", ImVec2(280, 260), true); {
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
				ImGui::Text("  Chams");
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				static int currentCategory{ 0 };
				ImGui::PushItemWidth(110.0f);
				ImGui::PushID(0);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Combo("", &currentCategory, "Allies\0Enemies\0Planting\0Defusing\0Local player\0Weapons\0Hands\0Backtrack\0");
				ImGui::PopID();
				static int currentItem{ 0 };

				if (currentCategory <= 3) {
					ImGui::SameLine();
					static int currentType{ 0 };
					ImGui::PushID(1);
					ImGui::Combo("", &currentType, "Visible\0Occluded\0");
					ImGui::PopID();
					currentItem = currentCategory * 2 + currentType;
				}
				else {
					currentItem = currentCategory + 4;
				}

				ImGui::Checkbox("Enabled", &config.chams[currentItem].enabled);
				ImGui::Checkbox("Health based", &config.chams[currentItem].healthBased);
				ImGui::Checkbox("Rainbow", &config.chams[currentItem].rainbow);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Combo("Material", &config.chams[currentItem].material, "Normal\0Flat\0Palm\0Platinum\0Glass\0Chrome\0Crystal\0Gold\0Plastic\0Blinking\0");
				ImGui::Checkbox("Wireframe", &config.chams[currentItem].wireframe);

				bool openPopup = ImGui::ColorButton("Color", ImVec4{ config.chams[currentItem].color }, ImGuiColorEditFlags_NoTooltip);
				ImGui::SameLine(0.0f, 5.0f);
				ImGui::Text("Color");
				ImGui::PushID(2);
				if (openPopup)
					ImGui::OpenPopup("");
				if (ImGui::BeginPopup("")) {
					ImGui::PushID(3);
					ImGui::ColorPicker3("", config.chams[currentItem].color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
					ImGui::PopID();
					ImGui::EndPopup();
				}
				ImGui::PopID();
				ImGui::PushItemWidth(220.0f);
				ImGui::PushID(4);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderFloat("", &config.chams[currentItem].alpha, 0.0f, 1.0f, "Alpha: %.2f");
				ImGui::PopID();
			}ImGui::EndChild();
			ImGui::BeginChild("", ImVec2(280, 270), true); {
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
				ImGui::Text("  Other Visuals");
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				ImGui::Columns(2, nullptr, false);
				ImGui::SetColumnOffset(1, 210.0f);
				ImGui::Checkbox("Disable post-processing", &config.visuals.disablePostProcessing);
				ImGui::Checkbox("Inverse ragdoll gravity", &config.visuals.inverseRagdollGravity);
				ImGui::Checkbox("No fog", &config.visuals.noFog);
				ImGui::Checkbox("No 3d sky", &config.visuals.no3dSky);
				ImGui::Checkbox("No visual recoil", &config.visuals.noVisualRecoil);
				ImGui::Checkbox("No hands", &config.visuals.noHands);
				ImGui::Checkbox("No sleeves", &config.visuals.noSleeves);
				ImGui::Checkbox("No weapons", &config.visuals.noWeapons);
				ImGui::Checkbox("No smoke", &config.visuals.noSmoke);
				ImGui::Checkbox("No blur", &config.visuals.noBlur);
				ImGui::Checkbox("No scope overlay", &config.visuals.noScopeOverlay);
				ImGui::Checkbox("No grass", &config.visuals.noGrass);
				ImGui::Checkbox("Wireframe smoke", &config.visuals.wireframeSmoke);
			}ImGui::EndChild();
			break;
		case 2:
			ImGui::Columns(2, NULL, false);
			ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

			//push window color for child
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
			//push border color for child
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
			ImGui::BeginChild("Misc", ImVec2(280, 460), true); {
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
				ImGui::Text("  Misc");
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				ImGui::Checkbox("Auto strafe", &config.misc.autoStrafe);
				ImGui::Checkbox("Bunny hop", &config.misc.bunnyHop);
				static char buffer[16];
				ImGui::PushItemWidth(120.0f);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer));
				ImGui::SameLine();
				if (ImGui::Button("Set clantag")) {
					Misc::setClanTag(buffer);
					Misc::animateClanTag(buffer);
				}
				ImGui::Checkbox("Animated clan tag", &config.misc.animatedClanTag);
				ImGui::Checkbox("Fast duck", &config.misc.fastDuck);
				ImGui::Checkbox("Sniper crosshair", &config.misc.sniperCrosshair);
				ImGui::Checkbox("Recoil crosshair", &config.misc.recoilCrosshair);
				ImGui::Checkbox("SlideWalk", &config.misc.SlideWalk);
				ImGui::Checkbox("Auto pistol", &config.misc.autoPistol);
				ImGui::Checkbox("Auto reload", &config.misc.autoReload);
				ImGui::Checkbox("Auto accept", &config.misc.autoAccept);
				ImGui::Checkbox("Radar hack", &config.misc.radarHack);
				ImGui::Checkbox("Reveal ranks", &config.misc.revealRanks);
				ImGui::Checkbox("Spectator list", &config.misc.spectatorList);
				ImGui::Checkbox("Watermark", &config.misc.watermark);
				ImGui::Checkbox("Fix animation LOD", &config.misc.fixAnimationLOD);
				ImGui::Checkbox("Prepare revolver", &config.misc.prepareRevolver);
				ImGui::Checkbox("Hit sound", &config.misc.hitSound);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Combo("Hitsound Type", &config.misc.iHitSound, "Off\0Default\0Pew\0Roblox\0Nya\0German\0Rust Headshot\0FadeCSGO.tk\0");
				//COMBOBOX HITSOUND
				ImGui::PushItemWidth(90.0f);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::InputInt("Fake Lag", &config.misc.chokedPackets, 1, 5);
				config.misc.chokedPackets = std::clamp(config.misc.chokedPackets, 0, 12);

				ImGui::Text("");
				ImGui::SameLine();
				if (ImGui::Button("Unload"))
					hooks.restore();
			}
			ImGui::EndChild();

			ImGui::NextColumn();

			ImGui::BeginChild("", ImVec2(280, 230), true); {
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
				ImGui::Text("  Other Misc");
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				ImGui::Checkbox("Thirdperson", &config.visuals.thirdperson);
				ImGui::SameLine();
				hotkey(config.visuals.thirdpersonKey);
				if (ImGui::IsItemHovered())
				{
					ImGui::SetTooltip("Set the key while hovering and pressing the key.");
				}
				ImGui::PushItemWidth(220.0f);
				ImGui::PushID(0);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderInt("", &config.visuals.thirdpersonDistance, 0, 1000, "Thirdperson distance: %d");
				ImGui::PopID();
				ImGui::PushID(1);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderInt("", &config.visuals.viewmodelFov, -60, 60, "Viewmodel FOV: %d");
				ImGui::PopID();
				ImGui::PushID(2);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderInt("", &config.visuals.flashReduction, 0, 100, "Flash reduction: %d%%");
				ImGui::PopID();
				ImGui::PushID(3);
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::SliderFloat("", &config.visuals.brightness, 0.0f, 1.0f, "Brightness: %.2f");
				ImGui::PopID();
				ImGui::PopItemWidth();
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Combo("Skybox", &config.visuals.skybox, "cs_baggage_skybox_\0cs_tibet\0embassy\0italy\0jungle\0nukeblank\0office\0sky_cs15_daylight01_hdr\0sky_cs15_daylight02_hdr\0sky_cs15_daylight03_hdr\0sky_cs15_daylight04_hdr\0sky_csgo_cloudy01\0sky_csgo_night_flat\0sky_csgo_night02\0sky_day02_05_hdr\0sky_day02_05\0sky_dust\0sky_l4d_rural02_ldr\0sky_venice\0vertigo_hdr\0vertigo\0vertigoblue_hdr\0vietnam\0");
				ImGui::ColorEdit3("World color", config.visuals.worldColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
			}
			ImGui::EndChild();
			break;
		case 3:
			ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

			//push window color for child
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
			//push border color for child
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
			draw_gui();
			break;
		case 4:
			ImGui::Columns(2, NULL, false);
			ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
			ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

			//push window color for child
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
			//push border color for child
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
			ImGui::BeginChild("Config", ImVec2(280, 230), true); {
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
				ImGui::Text("  Config");
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				ImGui::Columns(2, nullptr, false);
				ImGui::SetColumnOffset(1, 170.0f);

				ImGui::PushItemWidth(160.0f);

				constexpr auto& configItems = config.getConfigs();
				static int currentConfig = -1;

				if (static_cast<size_t>(currentConfig) >= configItems.size())
					currentConfig = -1;

				static char buffer[16];

				ImGui::Spacing();
				ImGui::Text("");
				ImGui::SameLine();
				if (ImGui::ListBox("", &currentConfig, [](void* data, int idx, const char** out_text) {
					auto& vector = *static_cast<std::vector<std::string>*>(data);
					*out_text = vector[idx].c_str();
					return true;
					}, &configItems, configItems.size(), 5) && currentConfig != -1)
					strcpy(buffer, configItems[currentConfig].c_str());

					ImGui::PushID(0);
					ImGui::Text("");
					ImGui::SameLine();
					if (ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
						if (currentConfig != -1)
							config.rename(currentConfig, buffer);
					}
					ImGui::PopID();
					ImGui::NextColumn();

					ImGui::PushItemWidth(100.0f);
					ImGui::Spacing();
					if (ImGui::Button("Create config", { 85.0f, 25.0f }))
						config.add(buffer);

					if (ImGui::Button("Reset config", { 85.0f, 25.0f }))
						config.reset();

					if (currentConfig != -1) {
						if (ImGui::Button("Load selected", { 85.0f, 25.0f }))
							config.load(currentConfig);
						if (ImGui::Button("Save selected", { 85.0f, 25.0f }))
							config.save(currentConfig);
						if (ImGui::Button("Delete selected", { 85.0f, 25.0f }))
							config.remove(currentConfig);
					}
			}ImGui::EndChild();
			ImGui::NextColumn();

			ImGui::BeginChild("Info", ImVec2(280, 230), true); {
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(255 / 255.f, 200 / 255.f, 0 / 255.f, 1.f);
				ImGui::Text("  Info");
				ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
				ImGui::Spacing();

				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Text("Build: " __DATE__ " / " __TIME__);
				if (std::strstr(GetCommandLineA(), "-insecure")) {
					ImGui::Text("");
					ImGui::SameLine();
					ImGui::Text("Insecure mode!");
				}
				ImGui::Text("Osiris with different menu.");

			}ImGui::EndChild();

			break;
		}
		ImGui::End();
	}
}
