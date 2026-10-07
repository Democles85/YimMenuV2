#include "Items.hpp"
#include "core/commands/Command.hpp"
#include "core/commands/Commands.hpp"
#include "core/backend/FiberPool.hpp"
#include "game/frontend/Menu.hpp"

namespace YimMenu
{
	Group::Group(const std::string& name, int items_per_column, const std::string& icon) :
	    m_Name(name),
	    m_Icon(icon),
	    m_ItemsPerColumn(items_per_column)
	{
	}

	void Group::Draw()
	{
		if (!m_Name.empty())
		{
			ImGui::PushFont(Menu::Font::g_ChildTitleFont);
			if (!m_Icon.empty())
			{
				ImGui::PushFont(Menu::Font::g_AwesomeFont);
				ImGui::TextUnformatted(m_Icon.c_str());
				ImGui::PopFont();
				ImGui::SameLine();
				// Add a small space after the icon
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.0f);
			}
			ImGui::Text("%s", m_Name.c_str());
			ImGui::PopFont();
			ImGui::Separator();
			ImGui::Spacing();
		}

		int item_count = 0;

		ImGui::BeginGroup();
		for (auto& item : m_Items)
		{
			if (!item->CanDraw())
				continue;

			item->Draw();
			item_count++;

			if (m_ItemsPerColumn != -1 && item_count % m_ItemsPerColumn == 0)
			{
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::BeginGroup();
			}
		}
		ImGui::EndGroup();
	}
}