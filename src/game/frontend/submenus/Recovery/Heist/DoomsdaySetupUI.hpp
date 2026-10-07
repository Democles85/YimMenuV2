#pragma once
#include "HeistUI.hpp"

namespace YimMenu::Submenus::DoomsdaySetupUI
{
	struct Act
	{
		int id;
		const char* name;
		long long setup;
		long long normal;
		long long hard;
		long long weekly_n;
		long long weekly_h;
	};

	inline constexpr Act kActs[] = {
	    {0, "The Data Breaches", 65'000, 877'500, 1'096'875, 1'072'305, 1'340'381},
	    {1, "The Bogdan Problem", 95'000, 1'282'500, 1'603'125, 1'567'215, 1'959'019},
	    {2, "The Doomsday Scenario", 120'000, 1'620'000, 2'025'000, 1'979'640, 2'474'550},
	};

	inline constexpr long long kSafe = 2'550'000;
	inline constexpr long long kElite = 50'000;

	inline const Act* FindAct(int id)
	{
		for (const auto& a : kActs)
			if (a.id == id)
				return &a;
		return &kActs[0];
	}

	inline void Draw()
	{
		using namespace HeistLayout;
		using namespace HeistUI;

		auto* category = ListCmd("doomsdayheistcategory"_J);
		IntCommand* cuts[4] = {
		    IntCmd("doomsdayheistcut1"_J),
		    IntCmd("doomsdayheistcut2"_J),
		    IntCmd("doomsdayheistcut3"_J),
		    IntCmd("doomsdayheistcut4"_J),
		};

		if (!category || !cuts[0])
		{
			Hint("Doomsday commands unavailable.");
			return;
		}

		static bool weekly = true;
		static bool hard = true;
		static int players = 2;
		static int last_act = -1;
		static bool last_weekly = true;
		static bool last_hard = true;
		static int last_players = -1;

		char money[48], money2[48];

		BeginPanel("overview");
		{
			SectionLabel("Overview");

			ImGui::Spacing();
			FieldLabel("Payout source");
			if (Seg("Weekly first", weekly, 110.f))
				weekly = true;
			ImGui::SameLine(0, 6);
			if (Seg("Repeat", !weekly, 88.f))
				weekly = false;
			Hint("Weekly first if you haven't hosted this act since Thursday.");

			ImGui::Spacing();
			FieldLabel("Difficulty");
			if (Seg("Normal", !hard, 88.f))
				hard = false;
			ImGui::SameLine(0, 6);
			if (Seg("Hard", hard, 88.f))
				hard = true;

			const auto* act = FindAct(category->GetState());
			ImGui::Spacing();
			ImGui::Text("%s  ·  %s  ·  %s",
			    hard ? "Hard" : "Normal",
			    act->name,
			    weekly ? "Weekly first" : "Repeat");

			const long long gross = hard
			    ? (weekly ? act->weekly_h : act->hard)
			    : (weekly ? act->weekly_n : act->normal);

			ImGui::Spacing();
			FmtMoney(money, sizeof(money), gross);
			Stat("Finale", money);
			ImGui::SameLine(0, 24);
			FmtMoney(money, sizeof(money), act->setup);
			Stat("Host setup", money);
			ImGui::SameLine(0, 24);
			FmtMoney(money, sizeof(money), kElite);
			Stat("Elite", money);

			ImGui::Spacing();
			FmtMoney(money, sizeof(money), hard ? act->hard : act->normal);
			FmtMoney(money2, sizeof(money2), hard ? act->weekly_h : act->weekly_n);
			ImGui::TextDisabled("Repeat %s  ·  Weekly %s", money, money2);
		}
		EndPanel();

		const Act* act = FindAct(category->GetState());
		const long long net = hard
		    ? (weekly ? act->weekly_h : act->hard)
		    : (weekly ? act->weekly_n : act->normal);

		if (category->GetState() != last_act || weekly != last_weekly || hard != last_hard || players != last_players)
		{
			last_act = category->GetState();
			last_weekly = weekly;
			last_hard = hard;
			last_players = players;
			FairPlayerCuts(cuts, players, net, kSafe, act->setup);
		}

		DrawTwoColumns();
		{
			BeginPanel("heist");
			SectionLabel("Heist");
			DrawLabeledListComboFmt(category, "Select Heist", [is_hard = hard](int id, char* out, size_t n) {
				const auto* a = FindAct(id);
				const long long rep = is_hard ? a->hard : a->normal;
				const long long week = is_hard ? a->weekly_h : a->weekly_n;
				char r[32], w[32], s[32];
				FmtMoney(r, sizeof(r), rep);
				FmtMoney(w, sizeof(w), week);
				FmtMoney(s, sizeof(s), a->setup);
				snprintf(out, n, "R %s  ·  W %s  ·  fee %s", r, w, s);
			});
			act = FindAct(category->GetState());
			const long long live_net = hard
			    ? (weekly ? act->weekly_h : act->hard)
			    : (weekly ? act->weekly_n : act->normal);
			if (category->GetState() != last_act)
			{
				last_act = category->GetState();
				FairPlayerCuts(cuts, players, live_net, kSafe, act->setup);
			}
			ImGui::Spacing();
			if (PanelBtn("##apply_setup", ICON_CHECK, "Apply Setup"))
				Call("doomsdayheistsetup"_J);
			EndPanel();
		}
		NextColumn();
		{
			act = FindAct(category->GetState());
			const long long cut_net = hard
			    ? (weekly ? act->weekly_h : act->hard)
			    : (weekly ? act->weekly_n : act->normal);

			BeginPanel("cuts");
			SectionLabel("Player cuts & payout");
			FieldLabel("Finale size");
			for (int p = 2; p <= 4; ++p)
			{
				char lab[4];
				snprintf(lab, sizeof(lab), "%d", p);
				if (Seg(lab, players == p, 36.f))
				{
					players = p;
					last_players = players;
					FairPlayerCuts(cuts, players, cut_net, kSafe, act->setup);
				}
				if (p < 4)
					ImGui::SameLine(0, 6);
			}

			ImGui::Spacing();
			DrawPayoutTable(cuts, players, cut_net, act->setup, kSafe);

			ImGui::Spacing();
			if (EqualBtn("##fair", ICON_PERCENT, "Fair cuts", 3, 0))
				FairPlayerCuts(cuts, players, cut_net, kSafe, act->setup);
			if (EqualBtn("##setcuts", ICON_CHECK, "Set Cuts", 3, 1))
				Call("doomsdayheistsetcuts"_J);
			if (EqualBtn("##ready", ICON_USERS, "Force Ready", 3, 2))
				Call("doomsdayheistforceready"_J);

			ImGui::Spacing();
			Hint("In pocket subtracts the act setup fee from the host. Red means over ~$2.55M.");
			EndPanel();
		}
		EndColumns();
	}
}
