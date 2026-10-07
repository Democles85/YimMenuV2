#pragma once
#include "HeistUI.hpp"
#include "core/commands/Commands.hpp"
#include "core/commands/IntCommand.hpp"
#include "core/commands/ListCommand.hpp"

#include <climits>
#include <cstdio>
#include <cstring>
#include <optional>

namespace YimMenu::Submenus::CasinoHeistSetupUI
{
	struct VaultTake
	{
		const char* name;
		int id;
		long long normal;
		long long hard;
		long long weekly_n;
		long long weekly_h;
	};

	inline constexpr VaultTake kVaults[] = {
	    {"Cash", 0, 1'480'500, 1'628'550, 2'753'730, 3'029'103},
	    {"Artwork", 2, 1'645'000, 1'809'500, 3'059'700, 3'365'670},
	    {"Gold", 1, 1'809'500, 1'990'450, 3'365'670, 3'702'237},
	    {"Diamonds", 3, 2'303'000, 2'533'300, 4'283'580, 4'711'938},
	};

	inline constexpr int kLester = 5;
	inline constexpr int kHostFee = 25'000;
	inline constexpr long long kSafe = 2'550'000;

	struct Cmds
	{
		ListCommand* difficulty{};
		ListCommand* target{};
		ListCommand* approach{};
		ListCommand* gunman{};
		ListCommand* driver{};
		ListCommand* hacker{};
		ListCommand* weapon{};
		ListCommand* vehicle{};
		IntCommand* potential{};
		IntCommand* actual{};
		IntCommand* cuts[4]{};
	};

	inline Cmds Get()
	{
		return {
		    YimMenu::Commands::GetCommand<ListCommand>("diamondcasinoheistdifficulty"_J),
		    YimMenu::Commands::GetCommand<ListCommand>("diamondcasinoheistprimarytarget"_J),
		    YimMenu::Commands::GetCommand<ListCommand>("diamondcasinoheistapproach"_J),
		    YimMenu::Commands::GetCommand<ListCommand>("diamondcasinoheistgunman"_J),
		    YimMenu::Commands::GetCommand<ListCommand>("diamondcasinoheistdriver"_J),
		    YimMenu::Commands::GetCommand<ListCommand>("diamondcasinoheisthacker"_J),
		    YimMenu::Commands::GetCommand<ListCommand>("diamondcasinoheistweapon"_J),
		    YimMenu::Commands::GetCommand<ListCommand>("diamondcasinoheistvehicle"_J),
		    YimMenu::Commands::GetCommand<IntCommand>("diamondcasinoheistpotentialtake"_J),
		    YimMenu::Commands::GetCommand<IntCommand>("diamondcasinoheistactualtake"_J),
		    {
		        YimMenu::Commands::GetCommand<IntCommand>("diamondcasinoheistcut1"_J),
		        YimMenu::Commands::GetCommand<IntCommand>("diamondcasinoheistcut2"_J),
		        YimMenu::Commands::GetCommand<IntCommand>("diamondcasinoheistcut3"_J),
		        YimMenu::Commands::GetCommand<IntCommand>("diamondcasinoheistcut4"_J),
		    },
		};
	}

	inline bool Ok(const Cmds& c)
	{
		return c.difficulty && c.target && c.approach && c.gunman && c.driver && c.hacker && c.weapon && c.vehicle && c.potential && c.actual && c.cuts[0] && c.cuts[1] && c.cuts[2] && c.cuts[3];
	}

	inline int GunPct(int s)
	{
		switch (s)
		{
		case 0: return 10;
		case 1: return 8;
		case 2: return 8;
		case 3: return 7;
		case 4: return 5;
		default: return 0;
		}
	}
	inline int DrvPct(int s)
	{
		switch (s)
		{
		case 0: return 10;
		case 1: return 9;
		case 2: return 7;
		case 3: return 6;
		case 4: return 5;
		default: return 0;
		}
	}
	inline int HakPct(int s)
	{
		switch (s)
		{
		case 4: return 10;
		case 5: return 9;
		case 2: return 7;
		case 3: return 5;
		case 1: return 3;
		default: return 0;
		}
	}
	inline const char* HakTime(int s)
	{
		switch (s)
		{
		case 4: return "3:30";
		case 5: return "3:25";
		case 2: return "2:59";
		case 3: return "2:52";
		case 1: return "2:26";
		default: return "—";
		}
	}

	inline const VaultTake* Vault(int id)
	{
		for (const auto& v : kVaults)
			if (v.id == id)
				return &v;
		return &kVaults[0];
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

	struct Snap
	{
		long long gross{}, net{};
		int hire{}, g{}, d{}, h{};
	};

	inline long long OfficialVault(const Cmds& c, bool weekly)
	{
		const bool hard = c.difficulty->GetState() == 1;
		const auto* v = Vault(c.target->GetState());
		if (weekly)
			return hard ? v->weekly_h : v->weekly_n;
		return hard ? v->hard : v->normal;
	}

	inline Snap MakeSnap(const Cmds& c, bool weekly = false, bool custom_pot = false)
	{
		Snap s{};
		if (custom_pot && c.potential->GetState() > 0)
			s.gross = c.potential->GetState();
		else
			s.gross = OfficialVault(c, weekly);
		s.g = GunPct(c.gunman->GetState());
		s.d = DrvPct(c.driver->GetState());
		s.h = HakPct(c.hacker->GetState());
		s.hire = s.g + s.d + s.h;
		s.net = s.gross - s.gross * s.hire / 100 - s.gross * kLester / 100;
		return s;
	}

	inline void FairCuts(const Cmds& c, int players, bool weekly = false, bool custom_pot = false)
	{
		const Snap s = MakeSnap(c, weekly, custom_pot);
		const int other = CutFor(s.net, kSafe);
		const int host = CutFor(s.net, kSafe + kHostFee);
		c.cuts[0]->SetState(host);
		c.cuts[1]->SetState(players >= 2 ? other : 0);
		c.cuts[2]->SetState(players >= 3 ? other : 0);
		c.cuts[3]->SetState(players >= 4 ? other : 0);
	}

	using namespace HeistLayout;

	inline void DrawCrewCombo(ListCommand* cmd, int (*pct_fn)(int), const char* (*extra_fn)(int) = nullptr)
	{
		if (!cmd)
			return;

		const int cur = cmd->GetState();
		const char* name = ListLabel(cmd, cur);
		char preview[96];
		if (extra_fn)
			snprintf(preview, sizeof(preview), "%s  (%s)", name, extra_fn(cur));
		else
			snprintf(preview, sizeof(preview), "%s", name);

		PushFieldStyle();
		ImGui::SetNextItemWidth(FieldWidth());
		char id[64];
		snprintf(id, sizeof(id), "##crew_%08x", (unsigned)cmd->GetHash());

		std::optional<int> next;
		if (ImGui::BeginCombo(id, preview))
		{
			for (auto& el : cmd->GetList())
			{
				ImGui::PushID(el.first);
				const int pct = pct_fn(el.first);
				const char* n = el.second && el.second[0] ? el.second : "(None)";
				char row[96];
				if (extra_fn)
					snprintf(row, sizeof(row), "%s    %d%%    %s", n, pct, extra_fn(el.first));
				else
					snprintf(row, sizeof(row), "%s    %d%%", n, pct);

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

	inline void DrawLabeledCrewCombo(ListCommand* cmd, const char* label, int (*pct_fn)(int), const char* (*extra_fn)(int) = nullptr)
	{
		FieldLabel(label);
		DrawCrewCombo(cmd, pct_fn, extra_fn);
	}

	inline void Draw()
	{
		const auto c = Get();
		if (!Ok(c))
		{
			Hint("Casino heist commands unavailable.");
			return;
		}

		static int players = 2;
		static bool weekly = false;
		static bool custom_pot = false;
		static int last_target = -1;
		static int last_diff = -1;
		static int last_hire = -1;

		const int cur_target = c.target->GetState();
		const int cur_diff = c.difficulty->GetState();
		bool recalc_cuts = false;

		if (!custom_pot && (cur_target != last_target || cur_diff != last_diff))
		{
			c.potential->SetState(static_cast<int>(OfficialVault(c, weekly)));
			last_target = cur_target;
			last_diff = cur_diff;
			recalc_cuts = true;
		}

		const auto* vault = Vault(cur_target);
		const bool hard = cur_diff == 1;

		char money[48], money2[48];

		BeginPanel("hero");
		{
			SectionLabel("Overview");
			ImGui::Text("%s  ·  %s  ·  %s",
			    ListLabel(c.difficulty, cur_diff),
			    vault->name,
			    ListLabel(c.approach, c.approach->GetState()));

			ImGui::Spacing();
			FieldLabel("Vault source");
			if (Seg("Repeat", !weekly && !custom_pot, 88.f))
			{
				weekly = false;
				custom_pot = false;
				c.potential->SetState(static_cast<int>(OfficialVault(c, false)));
				last_target = cur_target;
				last_diff = cur_diff;
				recalc_cuts = true;
			}
			ImGui::SameLine(0, 6);
			if (Seg("Weekly first", weekly && !custom_pot, 110.f))
			{
				weekly = true;
				custom_pot = false;
				c.potential->SetState(static_cast<int>(OfficialVault(c, true)));
				last_target = cur_target;
				last_diff = cur_diff;
				recalc_cuts = true;
			}
			if (custom_pot)
			{
				ImGui::SameLine(0, 10);
				ImGui::TextDisabled("Custom");
			}
			Hint("Weekly first if you haven't hosted a Casino finale since Thursday.");

			if (recalc_cuts)
				FairCuts(c, players, weekly, custom_pot);

			const Snap s_preview = MakeSnap(c, weekly, custom_pot);
			ImGui::Spacing();
			FmtMoney(money, sizeof(money), s_preview.gross);
			Stat("Vault", money);
			ImGui::SameLine(0, 24);
			char fees[40];
			snprintf(fees, sizeof(fees), "%d%% hires + %d%% Lester", s_preview.hire, kLester);
			Stat("Fees", fees);
			ImGui::SameLine(0, 24);
			FmtMoney(money, sizeof(money), s_preview.net);
			Stat("Net pot", money);
			ImGui::SameLine(0, 24);
			FmtMoney(money, sizeof(money), kHostFee);
			Stat("Host setup", money);

			ImGui::Spacing();
			FmtMoney(money, sizeof(money), hard ? vault->hard : vault->normal);
			FmtMoney(money2, sizeof(money2), hard ? vault->weekly_h : vault->weekly_n);
			ImGui::TextDisabled("Repeat %s  ·  Weekly %s%s", money, money2, vault->id == 3 ? "  ·  Diamonds are event-only" : "");
		}
		EndPanel();

		Snap s = MakeSnap(c, weekly, custom_pot);

		const float col_w = (ImGui::GetContentRegionAvail().x - 12.f) * 0.5f;

		ImGui::BeginChild("##left_col", ImVec2(col_w, 0), ImGuiChildFlags_AutoResizeY);
		{
			BeginPanel("heist");
			SectionLabel("Heist");
			DrawLabeledListCombo(c.difficulty, "Difficulty");
			DrawLabeledListComboFmt(c.target, "Primary Target", [hard = (c.difficulty->GetState() == 1)](int id, char* out, size_t n) {
				const auto* v = Vault(id);
				const long long rep = hard ? v->hard : v->normal;
				const long long week = hard ? v->weekly_h : v->weekly_n;
				char r[32], w[32];
				FmtMoney(r, sizeof(r), rep);
				FmtMoney(w, sizeof(w), week);
				snprintf(out, n, "R %s  ·  W %s%s", r, w, v->id == 3 ? "  ·  event" : "");
			});
			DrawLabeledListCombo(c.approach, "Approach");
			if (!custom_pot
			    && (c.target->GetState() != last_target || c.difficulty->GetState() != last_diff))
			{
				last_target = c.target->GetState();
				last_diff = c.difficulty->GetState();
				c.potential->SetState(static_cast<int>(OfficialVault(c, weekly)));
				FairCuts(c, players, weekly, custom_pot);
				s = MakeSnap(c, weekly, custom_pot);
			}
			ImGui::Spacing();
			if (PanelBtn("##apply_setup", HeistUI::ICON_CHECK, "Apply Setup"))
				Call("diamondcasinoheistsetup"_J);
			EndPanel();

			BeginPanel("loot");
			SectionLabel("Loot values");
			if (DrawInt(c.potential, "Potential Take"))
			{
				custom_pot = true;
				FairCuts(c, players, weekly, custom_pot);
				s = MakeSnap(c, weekly, custom_pot);
			}
			DrawInt(c.actual, "Actual Take");
			ImGui::Spacing();
			if (EqualBtn("##set_pot", HeistUI::ICON_COINS, "Set Potential", 2, 0))
				Call("diamondcasinoheistsetpotentialtake"_J);
			if (EqualBtn("##set_act", HeistUI::ICON_DOLLAR, "Set Actual", 2, 1))
				Call("diamondcasinoheistsetactualtake"_J);
			EndPanel();
		}
		ImGui::EndChild();

		ImGui::SameLine(0, 12);

		ImGui::BeginChild("##right_col", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);
		{
			BeginPanel("crew");
			SectionLabel("Crew");
			ImGui::Spacing();

			if (ImGui::BeginTable("##crew_grid", 2, ImGuiTableFlags_SizingStretchSame))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				FieldLabel("Gunman");
				ImGui::TableNextColumn();
				FieldLabel("Weapons");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				DrawCrewCombo(c.gunman, GunPct);
				ImGui::TableNextColumn();
				if (c.gunman->GetState() < 5)
					DrawListCombo(c.weapon, "weapons");
				else
				{
					PushFieldStyle();
					ImGui::SetNextItemWidth(FieldWidth());
					ImGui::BeginDisabled();
					ImGui::BeginCombo("##weapons_disabled", "N/A");
					ImGui::EndCombo();
					ImGui::EndDisabled();
					PopFieldStyle();
				}

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				FieldLabel("Driver");
				ImGui::TableNextColumn();
				FieldLabel("Vehicles");

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				DrawCrewCombo(c.driver, DrvPct);
				ImGui::TableNextColumn();
				if (c.driver->GetState() < 5)
					DrawListCombo(c.vehicle, "vehicles");
				else
				{
					PushFieldStyle();
					ImGui::SetNextItemWidth(FieldWidth());
					ImGui::BeginDisabled();
					ImGui::BeginCombo("##vehicles_disabled", "N/A");
					ImGui::EndCombo();
					ImGui::EndDisabled();
					PopFieldStyle();
				}

				ImGui::EndTable();
			}

			ImGui::Spacing();
			DrawLabeledCrewCombo(c.hacker, "Hacker", HakPct, HakTime);
			Hint("Time in parentheses is vault time.");

			s = MakeSnap(c, weekly, custom_pot);
			if (s.hire != last_hire)
			{
				last_hire = s.hire;
				FairCuts(c, players, weekly, custom_pot);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::Text("Hires %d%%   (G %d  ·  D %d  ·  H %d)   +   Lester %d%%", s.hire, s.g, s.d, s.h, kLester);
			EndPanel();
		}
		ImGui::EndChild();

		BeginPanel("cuts");
		{
			SectionLabel("Player cuts & payout");

			ImGui::TextDisabled("Finale size");
			ImGui::SameLine();
			for (int p = 2; p <= 4; ++p)
			{
				char lab[4];
				snprintf(lab, sizeof(lab), "%d", p);
				if (Seg(lab, players == p, 36.f))
				{
					players = p;
					FairCuts(c, players, weekly, custom_pot);
				}
				if (p < 4)
					ImGui::SameLine(0, 6);
			}

			s = MakeSnap(c, weekly, custom_pot);

			ImGui::Spacing();
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
					int cut = c.cuts[i]->GetState();
					const long long pay = Pay(s.net, cut);
					const long long pocket = i == 0 ? pay - kHostFee : pay;

					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted(kNames[i]);

					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-1);
					char cut_id[32];
					snprintf(cut_id, sizeof(cut_id), "##cut_in_%d", i);
					if (ImGui::InputInt(cut_id, &cut))
						c.cuts[i]->SetState(cut);

					ImGui::TableNextColumn();
					ImGui::AlignTextToFramePadding();
					if (cut > 0)
					{
						FmtMoney(money, sizeof(money), pay);
						if (pay > kSafe)
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
				if (c.cuts[i]->GetState() != 0)
					c.cuts[i]->SetState(0);
			}

			ImGui::Spacing();
			if (EqualBtn("##fair", HeistUI::ICON_PERCENT, "Fair cuts", 3, 0))
				FairCuts(c, players, weekly, custom_pot);
			if (EqualBtn("##setcuts", HeistUI::ICON_CHECK, "Set Cuts", 3, 1))
				Call("diamondcasinoheistsetcuts"_J);
			if (EqualBtn("##ready", HeistUI::ICON_USERS, "Force Ready", 3, 2))
				Call("diamondcasinoheistforceready"_J);

			ImGui::Spacing();
			Hint("In pocket subtracts the $25k setup from the host. Red means over ~$2.55M.");
		}
		EndPanel();
	}
}
