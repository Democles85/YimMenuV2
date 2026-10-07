#pragma once
#include "game/frontend/IconUI.hpp"
#include "core/commands/BoolCommand.hpp"
#include "core/commands/Commands.hpp"
#include "core/commands/IntCommand.hpp"
#include "core/commands/ListCommand.hpp"
#include "game/frontend/Menu.hpp"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <optional>
#include <span>

namespace YimMenu::Submenus
{
	namespace HeistUI = IconUI;

	namespace HeistLayout
	{
		inline constexpr float kBtnH = 34.f;
		inline constexpr float kBtnMaxW = 200.f;

		inline void FieldLabel(const char* label)
		{
			ImGui::TextDisabled("%s", label);
		}

		inline float FieldWidth()
		{
			return ImGui::GetContentRegionAvail().x;
		}

		inline ImU32 Col(ImGuiCol idx, float a = 1.f)
		{
			ImVec4 c = ImGui::GetStyleColorVec4(idx);
			c.w *= a;
			return ImGui::ColorConvertFloat4ToU32(c);
		}

		inline void PushFieldStyle()
		{
			ImVec4 frame = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
			frame.x = std::min(frame.x + 0.06f, 1.f);
			frame.y = std::min(frame.y + 0.06f, 1.f);
			frame.z = std::min(frame.z + 0.06f, 1.f);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, frame);
			ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyleColorVec4(ImGuiCol_Border));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
		}

		inline void PopFieldStyle()
		{
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(2);
		}

		inline void BeginPanel(const char* id)
		{
			ImVec4 bg = ImGui::GetStyleColorVec4(ImGuiCol_ChildBg);
			if (bg.w < 0.05f)
				bg = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
			bg.w = 0.55f;
			ImGui::PushStyleColor(ImGuiCol_ChildBg, bg);
			ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyleColorVec4(ImGuiCol_Border));
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14, 12));
			ImGui::BeginChild(id, ImVec2(0, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding);
			ImGui::PushID(id);
		}

		inline void EndPanel()
		{
			ImGui::PopID();
			ImGui::EndChild();
			ImGui::PopStyleVar(3);
			ImGui::PopStyleColor(2);
			ImGui::Spacing();
		}

		inline void SectionLabel(const char* title)
		{
			if (Menu::Font::g_ChildTitleFont)
			{
				ImGui::PushFont(Menu::Font::g_ChildTitleFont);
				ImGui::TextUnformatted(title);
				ImGui::PopFont();
			}
			else
				ImGui::TextUnformatted(title);
			ImGui::Dummy(ImVec2(0, 4));
		}

		inline void Hint(const char* text)
		{
			ImGui::TextDisabled("%s", text);
		}

		inline void Stat(const char* label, const char* value)
		{
			ImGui::BeginGroup();
			ImGui::TextDisabled("%s", label);
			ImGui::TextUnformatted(value);
			ImGui::EndGroup();
		}

		inline bool Seg(const char* label, bool on, float w = 40.f)
		{
			const ImVec2 sz{w, 28.f};
			const ImVec2 p0 = ImGui::GetCursorScreenPos();
			ImGui::PushID(label);
			const bool click = ImGui::InvisibleButton("##seg", sz);
			const bool hov = ImGui::IsItemHovered();
			ImGui::PopID();
			ImDrawList* dl = ImGui::GetWindowDrawList();
			dl->AddRectFilled(p0, p0 + sz, Col(on ? ImGuiCol_ButtonActive : (hov ? ImGuiCol_ButtonHovered : ImGuiCol_Button)), 6.f);
			dl->AddRect(p0, p0 + sz, on ? Col(ImGuiCol_CheckMark) : Col(ImGuiCol_Border), 6.f, 0, on ? 1.5f : 1.0f);
			const ImVec2 ts = ImGui::CalcTextSize(label);
			dl->AddText(p0 + ImVec2((sz.x - ts.x) * 0.5f, (sz.y - ts.y) * 0.5f), Col(ImGuiCol_Text), label);
			return click;
		}

		inline bool PanelBtn(const char* id, const char* icon, const char* label)
		{
			const float w = std::min(FieldWidth(), kBtnMaxW);
			return HeistUI::DrawIconButton(id, icon, label, {w, kBtnH});
		}

		inline bool EqualBtn(const char* id, const char* icon, const char* label, int count, int index)
		{
			const float gap = 8.f;
			const float avail = std::min(FieldWidth(), kBtnMaxW * count + gap * (count - 1));
			const float w = (avail - gap * (count - 1)) / count;
			const bool clicked = HeistUI::DrawIconButton(id, icon, label, {w, kBtnH});
			if (index + 1 < count)
				ImGui::SameLine(0, gap);
			return clicked;
		}

		inline const char* ListLabel(ListCommand* cmd, int value)
		{
			if (!cmd)
				return "—";
			for (auto& el : cmd->GetList())
				if (el.first == value && el.second)
					return el.second;
			return "—";
		}

		inline void DrawListCombo(ListCommand* cmd, const char* id_suffix = "field")
		{
			if (!cmd)
				return;
			const int cur = cmd->GetState();
			const char* preview = ListLabel(cmd, cur);
			PushFieldStyle();
			ImGui::SetNextItemWidth(FieldWidth());
			char id[64];
			snprintf(id, sizeof(id), "##list_%s_%08x", id_suffix, (unsigned)cmd->GetHash());
			std::optional<int> next;
			if (ImGui::BeginCombo(id, preview))
			{
				for (auto& el : cmd->GetList())
				{
					ImGui::PushID(el.first);
					const char* name = el.second && el.second[0] ? el.second : "(None)";
					if (ImGui::Selectable(name, el.first == cur))
						next = el.first;
					if (el.first == cur)
						ImGui::SetItemDefaultFocus();
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}
			PopFieldStyle();
			if (next)
				cmd->SetState(*next);
		}

		inline void DrawLabeledListCombo(ListCommand* cmd, const char* label)
		{
			if (!cmd)
				return;
			FieldLabel(label);
			DrawListCombo(cmd, "field");
		}

		template<typename Fmt>
		inline void DrawListComboFmt(ListCommand* cmd, const char* id_suffix, Fmt&& fmt)
		{
			if (!cmd)
				return;
			const int cur = cmd->GetState();
			const char* name = ListLabel(cmd, cur);
			char extra[96]{};
			fmt(cur, extra, sizeof(extra));
			char preview[160];
			if (extra[0])
				snprintf(preview, sizeof(preview), "%s  ·  %s", name, extra);
			else
				snprintf(preview, sizeof(preview), "%s", name);

			PushFieldStyle();
			ImGui::SetNextItemWidth(FieldWidth());
			char id[64];
			snprintf(id, sizeof(id), "##listf_%s_%08x", id_suffix, (unsigned)cmd->GetHash());
			std::optional<int> next;
			if (ImGui::BeginCombo(id, preview))
			{
				for (auto& el : cmd->GetList())
				{
					ImGui::PushID(el.first);
					const char* n = el.second && el.second[0] ? el.second : "(None)";
					extra[0] = 0;
					fmt(el.first, extra, sizeof(extra));
					char row[192];
					if (extra[0])
						snprintf(row, sizeof(row), "%s    %s", n, extra);
					else
						snprintf(row, sizeof(row), "%s", n);
					if (ImGui::Selectable(row, el.first == cur))
						next = el.first;
					if (el.first == cur)
						ImGui::SetItemDefaultFocus();
					ImGui::PopID();
				}
				ImGui::EndCombo();
			}
			PopFieldStyle();
			if (next)
				cmd->SetState(*next);
		}

		template<typename Fmt>
		inline void DrawLabeledListComboFmt(ListCommand* cmd, const char* label, Fmt&& fmt)
		{
			if (!cmd)
				return;
			FieldLabel(label);
			DrawListComboFmt(cmd, "field", fmt);
		}

		inline bool DrawInt(IntCommand* cmd, const char* label)
		{
			if (!cmd)
				return false;
			int v = cmd->GetState();
			FieldLabel(label);
			PushFieldStyle();
			ImGui::SetNextItemWidth(FieldWidth());
			char id[64];
			snprintf(id, sizeof(id), "##int_%08x", (unsigned)cmd->GetHash());
			const bool changed = ImGui::InputInt(id, &v);
			if (changed)
				cmd->SetState(v);
			PopFieldStyle();
			return changed;
		}

		inline bool DrawBool(BoolCommand* cmd, const char* label = nullptr)
		{
			if (!cmd)
				return false;
			bool v = cmd->GetState();
			const char* text = label ? label : cmd->GetLabel().c_str();
			ImGui::PushID(static_cast<int>(cmd->GetHash()));
			const bool changed = ImGui::Checkbox(text, &v);
			ImGui::PopID();
			if (changed)
				cmd->SetState(v);
			return changed;
		}

		inline IntCommand* IntCmd(joaat_t hash)
		{
			return Commands::GetCommand<IntCommand>(hash);
		}

		inline ListCommand* ListCmd(joaat_t hash)
		{
			return Commands::GetCommand<ListCommand>(hash);
		}

		inline BoolCommand* BoolCmd(joaat_t hash)
		{
			return Commands::GetCommand<BoolCommand>(hash);
		}

		inline void Call(joaat_t hash)
		{
			HeistUI::CallCommand(hash);
		}

		inline void DrawCutsGrid(IntCommand* c1, IntCommand* c2, IntCommand* c3, IntCommand* c4)
		{
			if (ImGui::BeginTable("##cuts_grid", 2, ImGuiTableFlags_SizingStretchSame))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				DrawInt(c1, "P1 host");
				ImGui::TableNextColumn();
				DrawInt(c2, "P2");
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				DrawInt(c3, "P3");
				ImGui::TableNextColumn();
				DrawInt(c4, "P4");
				ImGui::EndTable();
			}
		}

		inline void DrawBoolGrid(std::span<BoolCommand*> items, int columns = 2)
		{
			if (items.empty())
				return;
			if (ImGui::BeginTable("##bool_grid", columns, ImGuiTableFlags_SizingStretchSame))
			{
				for (size_t i = 0; i < items.size(); ++i)
				{
					if (i % columns == 0)
						ImGui::TableNextRow();
					ImGui::TableNextColumn();
					DrawBool(items[i]);
				}
				ImGui::EndTable();
			}
		}

		inline void DrawActionButtons(std::span<const HeistUI::ActionButton> actions, int columns = 2)
		{
			const float gap = 8.f;
			const float avail = FieldWidth();
			const float max_row = kBtnMaxW * columns + gap * (columns - 1);
			const float row_w = std::min(avail, max_row);
			const float btn_w = (row_w - gap * (columns - 1)) / columns;

			for (size_t i = 0; i < actions.size(); ++i)
			{
				const auto& a = actions[i];
				char id_scope[32];
				snprintf(id_scope, sizeof(id_scope), "act_%08x", static_cast<unsigned>(a.hash));
				ImGui::PushID(id_scope);

				const char* tip = nullptr;
				if (auto* cmd = Commands::GetCommand<Command>(a.hash))
					tip = cmd->GetDescription().c_str();

				if (HeistUI::DrawIconButton("##action", a.icon, a.label, {btn_w, kBtnH}, tip))
					Call(a.hash);

				ImGui::PopID();

				if ((i + 1) % columns != 0 && i + 1 < actions.size())
					ImGui::SameLine(0, gap);
			}
		}

		inline void DrawActionPanel(const char* id, const char* title, std::span<const HeistUI::ActionButton> actions, const char* hint = nullptr, int columns = 2)
		{
			BeginPanel(id);
			SectionLabel(title);
			if (hint)
				Hint(hint);
			ImGui::Spacing();
			DrawActionButtons(actions, columns);
			EndPanel();
		}

		inline void DrawTwoColumns(float gap = 12.f)
		{
			const float col_w = (ImGui::GetContentRegionAvail().x - gap) * 0.5f;
			ImGui::BeginChild("##heist_left", ImVec2(col_w, 0), ImGuiChildFlags_AutoResizeY);
		}

		inline void NextColumn(float gap = 12.f)
		{
			ImGui::EndChild();
			ImGui::SameLine(0, gap);
			ImGui::BeginChild("##heist_right", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);
		}

		inline void EndColumns()
		{
			ImGui::EndChild();
		}

		inline void DrawDuringHeist(std::span<const HeistUI::ActionButton> shortcuts, std::span<const HeistUI::ActionButton> teleports = {})
		{
			DrawActionPanel("shortcuts", "Shortcuts", shortcuts);
			if (!teleports.empty())
				DrawActionPanel("teleports", "Teleports", teleports);
		}

		inline void FmtMoney(char* o, size_t n, long long v)
		{
			const bool neg = v < 0;
			unsigned long long a = neg ? static_cast<unsigned long long>(-v) : static_cast<unsigned long long>(v);
			char d[32];
			snprintf(d, sizeof(d), "%llu", a);
			char c[48];
			int len = (int)strlen(d), j = 0;
			for (int i = 0; i < len; ++i)
			{
				if (i && (len - i) % 3 == 0)
					c[j++] = ',';
				c[j++] = d[i];
			}
			c[j] = 0;
			snprintf(o, n, "%s$%s", neg ? "-" : "", c);
		}

		inline long long Pay(long long net, int cut)
		{
			return (net <= 0 || cut <= 0) ? 0 : net * (long long)cut / 100;
		}

		inline int CutFor(long long net, long long target)
		{
			if (net <= 0 || target <= 0)
				return 0;
			int cut = (int)std::min<long long>(target * 100 / net, 9999);
			while (cut > 0 && Pay(net, cut) > target)
				--cut;
			while (cut < 9999 && Pay(net, cut + 1) <= target)
				++cut;
			return cut;
		}

		inline void FairPlayerCuts(IntCommand* cuts[4], int players, long long net, long long safe, long long host_fee)
		{
			if (!cuts[0] || !cuts[1] || !cuts[2] || !cuts[3])
				return;
			const int other = CutFor(net, safe);
			const int host = CutFor(net, safe + host_fee);
			cuts[0]->SetState(host);
			cuts[1]->SetState(players >= 2 ? other : 0);
			cuts[2]->SetState(players >= 3 ? other : 0);
			cuts[3]->SetState(players >= 4 ? other : 0);
		}

		inline void DrawPayoutTable(IntCommand* cuts[4], int players, long long net, long long host_fee, long long safe)
		{
			char money[48];
			PushFieldStyle();
			if (ImGui::BeginTable("##payout", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp))
			{
				ImGui::TableSetupColumn("Player", ImGuiTableColumnFlags_WidthFixed, 72);
				ImGui::TableSetupColumn("Cut %", ImGuiTableColumnFlags_WidthFixed, 100);
				ImGui::TableSetupColumn("Payout");
				ImGui::TableSetupColumn("In pocket");
				ImGui::TableHeadersRow();

				static const char* kNames[] = {"P1 host", "P2", "P3", "P4"};
				for (int i = 0; i < players; ++i)
				{
					if (!cuts[i])
						continue;
					int cut = cuts[i]->GetState();
					const long long pay = Pay(net, cut);
					const long long pocket = i == 0 ? pay - host_fee : pay;

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted(kNames[i]);

					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-1);
					char cut_id[32];
					snprintf(cut_id, sizeof(cut_id), "##cut_in_%d", i);
					if (ImGui::InputInt(cut_id, &cut))
						cuts[i]->SetState(cut);

					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					if (cut > 0)
					{
						FmtMoney(money, sizeof(money), pay);
						if (safe > 0 && pay > safe)
							ImGui::TextColored(ImVec4(1.f, 0.45f, 0.4f, 1.f), "%s", money);
						else
							ImGui::TextUnformatted(money);
					}
					else
						ImGui::TextDisabled("—");

					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					if (cut > 0)
					{
						FmtMoney(money, sizeof(money), pocket);
						ImGui::TextUnformatted(money);
					}
					else
						ImGui::TextDisabled("—");
				}
				ImGui::EndTable();
			}
			PopFieldStyle();

			for (int i = players; i < 4; ++i)
			{
				if (cuts[i] && cuts[i]->GetState() != 0)
					cuts[i]->SetState(0);
			}
		}
	}
}
