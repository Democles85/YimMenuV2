#pragma once
#include "HeistUI.hpp"

namespace YimMenu::Submenus::ApartmentSetupUI
{
	struct Job
	{
		const char* name;
		long long setup;
		long long easy;
		long long normal;
		long long hard;
		int min_players;
	};

	inline constexpr Job kJobs[] = {
	    {"The Fleeca Job", 11'500, 100'625, 201'250, 251'563, 2},
	    {"The Prison Break", 25'000, 350'000, 700'000, 875'000, 4},
	    {"The Humane Labs Raid", 25'000, 472'500, 945'000, 1'181'250, 4},
	    {"Series A Funding", 25'000, 353'500, 707'000, 883'750, 4},
	    {"The Pacific Standard Job", 25'000, 750'000, 1'500'000, 1'875'000, 4},
	};

	inline constexpr long long kSafe = 2'550'000;

	inline long long Gross(const Job& j, int diff, bool weekly)
	{
		long long base = j.normal;
		if (diff == 0)
			base = j.easy;
		else if (diff == 2)
			base = j.hard;
		return weekly ? base * 3 / 2 : base;
	}

	inline void Draw()
	{
		using namespace HeistLayout;
		using namespace HeistUI;

		IntCommand* cuts[4] = {
		    IntCmd("apartmentheistcut1"_J),
		    IntCmd("apartmentheistcut2"_J),
		    IntCmd("apartmentheistcut3"_J),
		    IntCmd("apartmentheistcut4"_J),
		};

		if (!cuts[0])
		{
			Hint("Apartment heist commands unavailable.");
			return;
		}

		static int job = 4;
		static int diff = 2;
		static bool weekly = true;
		static int players = 4;
		static int last_job = -1;
		static int last_diff = -1;
		static bool last_weekly = true;
		static int last_players = -1;

		if (job < 0 || job >= (int)(sizeof(kJobs) / sizeof(kJobs[0])))
			job = 0;

		char money[48], money2[48];

		BeginPanel("overview");
		{
			SectionLabel("Overview");

			FieldLabel("Heist");
			{
				char preview[160];
				char a[32], b[32];
				FmtMoney(a, sizeof(a), Gross(kJobs[job], diff, false));
				FmtMoney(b, sizeof(b), Gross(kJobs[job], diff, true));
				snprintf(preview, sizeof(preview), "%s  ·  R %s  ·  W %s", kJobs[job].name, a, b);
				PushFieldStyle();
				ImGui::SetNextItemWidth(FieldWidth());
				if (ImGui::BeginCombo("##apt_job", preview))
				{
					for (int i = 0; i < (int)(sizeof(kJobs) / sizeof(kJobs[0])); ++i)
					{
						FmtMoney(a, sizeof(a), Gross(kJobs[i], diff, false));
						FmtMoney(b, sizeof(b), Gross(kJobs[i], diff, true));
						char row[192];
						snprintf(row, sizeof(row), "%s    R %s  ·  W %s", kJobs[i].name, a, b);
						if (ImGui::Selectable(row, job == i))
							job = i;
						if (job == i)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				PopFieldStyle();
			}
			Hint("For estimates only — Apply Setup uses whatever is on your board.");

			ImGui::Spacing();
			FieldLabel("Payout source");
			if (Seg("Weekly first", weekly, 110.f))
				weekly = true;
			ImGui::SameLine(0, 6);
			if (Seg("Repeat", !weekly, 88.f))
				weekly = false;
			Hint("Weekly first if you haven't hosted this finale since Thursday (1.5×).");

			ImGui::Spacing();
			FieldLabel("Difficulty");
			if (Seg("Easy", diff == 0, 70.f))
				diff = 0;
			ImGui::SameLine(0, 6);
			if (Seg("Normal", diff == 1, 80.f))
				diff = 1;
			ImGui::SameLine(0, 6);
			if (Seg("Hard", diff == 2, 70.f))
				diff = 2;

			const auto& j = kJobs[job];
			if (players < j.min_players)
				players = j.min_players;
			const long long gross = Gross(j, diff, weekly);
			const char* diff_name = diff == 0 ? "Easy" : (diff == 2 ? "Hard" : "Normal");

			ImGui::Spacing();
			ImGui::Text("%s  ·  %s  ·  %s", diff_name, j.name, weekly ? "Weekly first" : "Repeat");

			ImGui::Spacing();
			FmtMoney(money, sizeof(money), gross);
			Stat("Finale", money);
			ImGui::SameLine(0, 24);
			FmtMoney(money, sizeof(money), j.setup);
			Stat("Host setup", money);
			ImGui::SameLine(0, 24);
			FmtMoney(money, sizeof(money), job == 0 ? 50'000 : 100'000);
			Stat("Elite", money);

			ImGui::Spacing();
			FmtMoney(money, sizeof(money), Gross(j, diff, false));
			FmtMoney(money2, sizeof(money2), Gross(j, diff, true));
			ImGui::TextDisabled("Repeat %s  ·  Weekly %s", money, money2);
		}
		EndPanel();

		const auto& j = kJobs[job];
		if (players < j.min_players)
			players = j.min_players;
		const long long net = Gross(j, diff, weekly);

		if (job != last_job || diff != last_diff || weekly != last_weekly || players != last_players)
		{
			last_job = job;
			last_diff = diff;
			last_weekly = weekly;
			last_players = players;
			FairPlayerCuts(cuts, players, net, kSafe, j.setup);
		}

		DrawTwoColumns();
		{
			BeginPanel("setup");
			SectionLabel("Heist");
			ImGui::Spacing();
			if (PanelBtn("##apply_setup", ICON_CHECK, "Apply Setup"))
				Call("apartmentheistsetup"_J);
			EndPanel();
		}
		NextColumn();
		{
			BeginPanel("cuts");
			SectionLabel("Player cuts & payout");
			FieldLabel("Finale size");
			const int pmin = j.min_players;
			for (int p = pmin; p <= 4; ++p)
			{
				char lab[4];
				snprintf(lab, sizeof(lab), "%d", p);
				if (Seg(lab, players == p, 36.f))
				{
					players = p;
					FairPlayerCuts(cuts, players, net, kSafe, j.setup);
				}
				if (p < 4)
					ImGui::SameLine(0, 6);
			}

			ImGui::Spacing();
			DrawPayoutTable(cuts, players, net, j.setup, kSafe);

			ImGui::Spacing();
			if (EqualBtn("##fair", ICON_PERCENT, "Fair cuts", 3, 0))
				FairPlayerCuts(cuts, players, net, kSafe, j.setup);
			if (EqualBtn("##setcuts", ICON_CHECK, "Set Cuts", 3, 1))
				Call("apartmentheistsetcuts"_J);
			if (EqualBtn("##ready", ICON_USERS, "Force Ready", 3, 2))
				Call("apartmentheistforceready"_J);

			ImGui::Spacing();
			Hint("In pocket subtracts the setup fee from the host. Red means over ~$2.55M.");
			EndPanel();
		}
		EndColumns();
	}
}
