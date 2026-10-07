#include "Items.hpp"
#include "core/commands/Command.hpp"
#include "core/commands/Commands.hpp"
#include "core/commands/ListCommand.hpp"
#include "core/frontend/widgets/toggle/imgui_toggle.hpp"

namespace YimMenu
{
	ListCommandItem::ListCommandItem(joaat_t id, std::optional<std::string> label_override) :
	    m_Command(Commands::GetCommand<ListCommand>(id)),
	    m_LabelOverride(label_override)
	{
	}

	void ListCommandItem::Draw()
	{
		if (!m_Command)
		{
			ImGui::Text("Unknown list!");
			return;
		}

		int current_val = m_Command->GetState();
		auto& list = m_Command->GetList();
		const char* selected_label = "";
		const char* largest_string = "";
		std::size_t largest_string_len = 0;

		for (auto& item : list)
		{
			if (!item.second)
				continue;

			if (item.first == current_val)
				selected_label = item.second;

			const auto length = strlen(item.second);
			if (length > largest_string_len)
			{
				largest_string = item.second;
				largest_string_len = length;
			}
		}

		ImGui::SetNextItemWidth(ImGui::CalcTextSize(largest_string).x + 40.0f);

		char label_buf[128];
		const char* combo_label = label_buf;
		const unsigned int cmd_hash = static_cast<unsigned int>(m_Command->GetHash());

		if (m_LabelOverride.has_value())
		{
			// Caller-provided labels often include an explicit "##id" — keep them intact
			if (!m_LabelOverride->empty())
				combo_label = m_LabelOverride->c_str();
			else
				snprintf(label_buf, sizeof(label_buf), "Select Option##list_%08x", cmd_hash);
		}
		else if (!m_Command->GetLabel().empty())
		{
			snprintf(label_buf, sizeof(label_buf), "%s##list_%08x", m_Command->GetLabel().c_str(), cmd_hash);
		}
		else
		{
			snprintf(label_buf, sizeof(label_buf), "Select Option##list_%08x", cmd_hash);
		}

		std::optional<int> pending_state;

		if (ImGui::BeginCombo(combo_label, selected_label))
		{
			for (auto& el : list)
			{
				ImGui::PushID(el.first);

				// Never pass an empty selectable label (collides with window ID)
				const char* option_label = (el.second && el.second[0] != '\0') ? el.second : "(None)";
				if (ImGui::Selectable(option_label, el.first == current_val))
					pending_state = el.first;

				if (el.first == current_val)
					ImGui::SetItemDefaultFocus();

				ImGui::PopID();
			}
			ImGui::EndCombo();
		}

		// Apply after EndCombo so OnChange list swaps don't run mid-widget
		if (pending_state.has_value())
			m_Command->SetState(*pending_state);
	}
}
