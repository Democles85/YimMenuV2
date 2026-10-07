#pragma once
#include "core/backend/FiberPool.hpp"
#include "core/commands/Command.hpp"
#include "core/commands/Commands.hpp"
#include "core/util/Joaat.hpp"
#include "game/frontend/Menu.hpp"

#include <span>

namespace YimMenu::Submenus::IconUI
{
	// Font Awesome 5 solid glyphs (matches Menu::Font::g_AwesomeFont)
	constexpr const char* ICON_CHECK        = "\xef\x80\x8c";
	constexpr const char* ICON_CHECK_DOUBLE = "\xef\x95\xa0";
	constexpr const char* ICON_USERS        = "\xef\x83\x80";
	constexpr const char* ICON_PERCENT      = "\xef\x95\x81";
	constexpr const char* ICON_FLAG         = "\xef\x84\x9e";
	constexpr const char* ICON_LAPTOP       = "\xef\x97\xbc";
	constexpr const char* ICON_MICROCHIP    = "\xef\x8b\x9b";
	constexpr const char* ICON_FINGERPRINT  = "\xef\x95\xb7";
	constexpr const char* ICON_KEY          = "\xef\x82\x84";
	constexpr const char* ICON_CUT          = "\xef\x83\x84";
	constexpr const char* ICON_BOLT         = "\xef\x83\xa7";
	constexpr const char* ICON_IMAGE        = "\xef\x80\xbe";
	constexpr const char* ICON_BOX          = "\xef\x92\x9e";
	constexpr const char* ICON_SERVER       = "\xef\x88\xb3";
	constexpr const char* ICON_PLUG         = "\xef\x87\xa6";
	constexpr const char* ICON_USER_TIE     = "\xef\x94\x88";
	constexpr const char* ICON_DOLLAR       = "\xef\x85\x95";
	constexpr const char* ICON_COINS        = "\xef\x94\x9e";
	constexpr const char* ICON_DRILL        = "\xef\x95\xa9";
	constexpr const char* ICON_CREDIT_CARD  = "\xef\x82\x9d";
	constexpr const char* ICON_WATER        = "\xef\x9d\xb3";
	constexpr const char* ICON_GEM          = "\xef\x8e\xa5";
	constexpr const char* ICON_BOMB         = "\xef\x87\xa2";
	constexpr const char* ICON_SKULL        = "\xef\x95\x8c";
	constexpr const char* ICON_PLAY         = "\xef\x81\x8b";
	constexpr const char* ICON_SHIELD       = "\xef\x84\xb2";
	constexpr const char* ICON_WRENCH       = "\xef\x82\xad";
	constexpr const char* ICON_BROOM        = "\xef\x94\x9a";
	constexpr const char* ICON_SPARKLES     = "\xef\xa2\x90";
	constexpr const char* ICON_TOOLS        = "\xef\x9f\x99";
	constexpr const char* ICON_PHONE        = "\xef\x82\x95";
	constexpr const char* ICON_SAVE         = "\xef\x83\x87";
	constexpr const char* ICON_CAR          = "\xef\x86\xb9";
	constexpr const char* ICON_TACHOMETER   = "\xef\x8f\xbd";
	constexpr const char* ICON_ROCKET       = "\xef\x84\xb5";
	constexpr const char* ICON_GARAGE       = "\xef\x83\x91";
	constexpr const char* ICON_SIGN_IN      = "\xef\x8b\xb6";

	constexpr float kButtonWidth  = 180.0f;
	constexpr float kButtonHeight = 36.0f;
	constexpr int kColumns        = 2;

	struct ActionButton
	{
		const char* icon;
		const char* label;
		joaat_t hash;
	};

	inline void DrawSectionHeader(const char* title, const char* icon = nullptr)
	{
		ImGui::PushFont(Menu::Font::g_ChildTitleFont);
		if (icon)
		{
			ImGui::PushFont(Menu::Font::g_AwesomeFont);
			ImGui::TextUnformatted(icon);
			ImGui::PopFont();
			ImGui::SameLine();
			// Add a small space after the icon
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.0f);
		}
		ImGui::TextUnformatted(title);
		ImGui::PopFont();
		ImGui::Separator();
		ImGui::Spacing();
	}

	inline bool DrawIconButton(const char* id, const char* icon, const char* label, const ImVec2& size, const char* tooltip = nullptr)
	{
		const ImVec2 start = ImGui::GetCursorScreenPos();
		const bool clicked = ImGui::InvisibleButton(id, size);
		const bool hovered = ImGui::IsItemHovered();
		const bool held    = ImGui::IsItemActive();

		ImDrawList* draw = ImGui::GetWindowDrawList();
		const float rounding = 6.0f;
		const ImU32 bg = ImGui::GetColorU32(held ? ImGuiCol_ButtonActive : (hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button));
		const ImU32 border = ImGui::GetColorU32(ImGuiCol_Border);
		const ImU32 text = ImGui::GetColorU32(ImGuiCol_Text);

		draw->AddRectFilled(start, start + size, bg, rounding);
		draw->AddRect(start, start + size, border, rounding);

		if (Menu::Font::g_AwesomeFont)
		{
			ImGui::PushFont(Menu::Font::g_AwesomeFont);
			const ImVec2 iconSize = ImGui::CalcTextSize(icon);
			const float iconScale = 0.55f;
			const ImVec2 iconPos{
			    start.x + 12.0f,
			    start.y + (size.y - iconSize.y * iconScale) * 0.5f};
			draw->AddText(Menu::Font::g_AwesomeFont, Menu::Font::g_AwesomeFontSize * iconScale, iconPos, text, icon);
			ImGui::PopFont();
		}

		const ImVec2 labelSize = ImGui::CalcTextSize(label);
		const ImVec2 labelPos{
		    start.x + 42.0f,
		    start.y + (size.y - labelSize.y) * 0.5f};
		draw->AddText(labelPos, text, label);

		if (tooltip && hovered)
			ImGui::SetTooltip("%s", tooltip);

		return clicked;
	}

	inline void CallCommand(joaat_t hash)
	{
		if (auto* cmd = Commands::GetCommand<Command>(hash))
		{
			FiberPool::Push([cmd] {
				cmd->Call();
			});
		}
	}

	inline void DrawActionGrid(std::span<const ActionButton> actions, float buttonWidth = kButtonWidth, float buttonHeight = kButtonHeight, int columns = kColumns)
	{
		const float spacing = ImGui::GetStyle().ItemSpacing.x;

		for (size_t i = 0; i < actions.size(); ++i)
		{
			const auto& action = actions[i];
			ImGui::PushID(static_cast<int>(action.hash));

			const char* tooltip = nullptr;
			if (auto* cmd = Commands::GetCommand<Command>(action.hash))
				tooltip = cmd->GetDescription().c_str();

			if (DrawIconButton(std::to_string(action.hash).c_str(), action.icon, action.label, {buttonWidth, buttonHeight}, tooltip))
				CallCommand(action.hash);

			ImGui::PopID();

			if ((i + 1) % columns != 0 && i + 1 < actions.size())
				ImGui::SameLine(0.0f, spacing);
		}
	}

	inline void DrawSetupActions(std::span<const ActionButton> actions)
	{
		DrawActionGrid(actions, kButtonWidth, kButtonHeight, kColumns);
	}

	inline void DrawDuringHeistSections(std::span<const ActionButton> shortcuts, std::span<const ActionButton> teleports = {})
	{
		DrawSectionHeader("Shortcuts");
		DrawActionGrid(shortcuts);

		if (!teleports.empty())
		{
			ImGui::Spacing();
			ImGui::Spacing();
			DrawSectionHeader("Teleports");
			DrawActionGrid(teleports);
		}
	}
}
