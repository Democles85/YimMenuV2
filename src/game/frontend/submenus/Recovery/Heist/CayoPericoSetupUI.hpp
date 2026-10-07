#pragma once
#include "HeistUI.hpp"

namespace YimMenu::Submenus::CayoPericoSetupUI
{
	struct Primary
	{
		int id;
		const char* name;
		long long normal;
		long long hard;
		bool event_only;
	};

	inline constexpr Primary kTargets[] = {
	    {0, "Sinsimito Tequila", 400'000, 440'000, false},
	    {1, "Ruby Necklace", 560'000, 616'000, false},
	    {2, "Bearer Bonds", 616'000, 677'600, false},
	    {3, "Pink Diamond", 910'000, 1'001'000, false},
	    {4, "Madrazo Files", 0, 0, false},
	    {5, "Panther Statue", 1'900'000, 2'090'000, true},
	};

	inline constexpr int kPavel = 2;
	inline constexpr int kFence = 10;
	inline constexpr long long kHostFee = 100'000;
	inline constexpr long long kSafe = 2'550'000;

	inline const Primary* FindTarget(int id)
	{
		for (const auto& t : kTargets)
			if (t.id == id)
				return &t;
		return &kTargets[0];
	}

	inline void Draw()
	{
		using namespace HeistLayout;
		using namespace HeistUI;

		auto* diff = ListCmd("cayopericoheistdifficulty"_J);
		auto* target = ListCmd("cayopericoheistprimarytarget"_J);
		auto* weapon = ListCmd("cayopericoheistweapon"_J);
		auto* primary_val = IntCmd("cayopericoheistprimarytargetvalue"_J);
		auto* secondary_val = IntCmd("cayopericoheistsecondarytakevalue"_J);
		IntCommand* cuts[4] = {
		    IntCmd("cayopericoheistcut1"_J),
		    IntCmd("cayopericoheistcut2"_J),
		    IntCmd("cayopericoheistcut3"_J),
		    IntCmd("cayopericoheistcut4"_J),
		};

		if (!diff || !target || !weapon || !primary_val || !secondary_val || !cuts[0])
		{
			Hint("Cayo Perico commands unavailable.");
			return;
		}

		static int players = 1;
		static bool sync_primary = true;
		static int last_target = -1;
		static int last_diff = -1;

		const bool hard = diff->GetState() != 126823;
		const auto* primary = FindTarget(target->GetState());
		const long long official = hard ? primary->hard : primary->normal;

		if (sync_primary && (target->GetState() != last_target || diff->GetState() != last_diff))
		{
			if (official > 0)
				primary_val->SetState(static_cast<int>(official));
			last_target = target->GetState();
			last_diff = diff->GetState();
		}

		const long long bag = (long long)primary_val->GetState() + secondary_val->GetState();
		const long long fees = bag * kPavel / 100 + bag * kFence / 100;
		const long long net = bag - fees;

		char money[48];

		BeginPanel("overview");
		{
			SectionLabel("Overview");
			ImGui::Text("%s  ·  %s  ·  %s",
			    ListLabel(diff, diff->GetState()),
			    primary->name,
			    ListLabel(weapon, weapon->GetState()));

			ImGui::Spacing();
			Hint("No weekly cash bonus — only loot odds refresh on Thursday.");
			if (primary->event_only)
				Hint("Panther Statue is event-only.");
			if (primary->id == 4)
				Hint("Madrazo Files are the first-run story target.");

			ImGui::Spacing();
			FmtMoney(money, sizeof(money), bag);
			Stat("Bag take", money);
			ImGui::SameLine(0, 24);
			char fee[40];
			snprintf(fee, sizeof(fee), "%d%% Pavel + %d%% fence", kPavel, kFence);
			Stat("Fees", fee);
			ImGui::SameLine(0, 24);
			FmtMoney(money, sizeof(money), net);
			Stat("After fees", money);
			ImGui::SameLine(0, 24);
			FmtMoney(money, sizeof(money), kHostFee);
			Stat("Host setup", money);
		}
		EndPanel();

		DrawTwoColumns();
		{
			BeginPanel("heist");
			SectionLabel("Heist");
			DrawLabeledListCombo(diff, "Difficulty");
			DrawLabeledListComboFmt(target, "Primary Target", [](int id, char* out, size_t n) {
				const auto* t = FindTarget(id);
				if (!t || t->normal <= 0)
				{
					if (t && t->id == 4)
						snprintf(out, n, "story only");
					else
						out[0] = 0;
					return;
				}
				char a[32], b[32];
				FmtMoney(a, sizeof(a), t->normal);
				FmtMoney(b, sizeof(b), t->hard);
				if (t->event_only)
					snprintf(out, n, "N %s  ·  H %s  ·  event", a, b);
				else
					snprintf(out, n, "N %s  ·  H %s", a, b);
			});
			DrawLabeledListCombo(weapon, "Weapon");
			ImGui::Spacing();
			if (PanelBtn("##apply_setup", ICON_CHECK, "Apply Setup"))
				Call("cayopericoheistsetup"_J);
			EndPanel();

			BeginPanel("loot");
			SectionLabel("Loot values");
			if (DrawInt(primary_val, "Primary Target Value"))
				sync_primary = false;
			DrawInt(secondary_val, "Secondary Take Value");
			ImGui::Spacing();
			if (EqualBtn("##set_pri", ICON_GEM, "Set Primary", 2, 0))
				Call("cayopericoheistsetprimarytargetvalue"_J);
			if (EqualBtn("##set_sec", ICON_COINS, "Set Secondary", 2, 1))
				Call("cayopericoheistsetsecondarytakevalue"_J);
			if (!sync_primary)
			{
				ImGui::Spacing();
				if (PanelBtn("##resync", ICON_SPARKLES, "Use official primary"))
				{
					sync_primary = true;
					if (official > 0)
						primary_val->SetState(static_cast<int>(official));
				}
			}
			EndPanel();
		}
		NextColumn();
		{
			BeginPanel("cuts");
			SectionLabel("Player cuts & payout");
			FieldLabel("Finale size");
			for (int p = 1; p <= 4; ++p)
			{
				char lab[4];
				snprintf(lab, sizeof(lab), "%d", p);
				if (Seg(lab, players == p, 36.f))
				{
					players = p;
					FairPlayerCuts(cuts, players, net, kSafe, kHostFee);
				}
				if (p < 4)
					ImGui::SameLine(0, 6);
			}

			ImGui::Spacing();
			DrawPayoutTable(cuts, players, net, kHostFee, kSafe);

			ImGui::Spacing();
			if (EqualBtn("##fair", ICON_PERCENT, "Fair cuts", 3, 0))
				FairPlayerCuts(cuts, players, net, kSafe, kHostFee);
			if (EqualBtn("##setcuts", ICON_CHECK, "Set Cuts", 3, 1))
				Call("cayopericoheistsetcuts"_J);
			if (EqualBtn("##ready", ICON_USERS, "Force Ready", 3, 2))
				Call("cayopericoheistforceready"_J);

			ImGui::Spacing();
			Hint("In pocket subtracts the $100k setup from the host. Red means over ~$2.55M.");
			EndPanel();
		}
		EndColumns();
	}
}
