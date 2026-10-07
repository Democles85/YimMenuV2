#pragma once
#include "HeistUI.hpp"

namespace YimMenu::Submenus::KortzCenterSetupUI
{
	struct Painting
	{
		int id;
		const char* name;
		long long n_weekly;
		long long n_fatigue;
		long long h_weekly;
		long long h_fatigue;
	};

	inline constexpr Painting kPaintings[] = {
	    {0, "La Dernière Débauche", 1'925'000, 481'250, 2'117'500, 529'375},
	    {1, "Hare Oneself Think", 0, 0, 0, 0},
	    {2, "The Downfall of Rome", 1'220'000, 305'000, 1'342'000, 335'500},
	    {3, "Brother Brother", 0, 0, 0, 0},
	    {4, "A Cast Characters", 0, 0, 0, 0},
	    {5, "Gone To Seed", 0, 0, 0, 0},
	    {6, "True Love", 0, 0, 0, 0},
	    {7, "Breathless", 0, 0, 0, 0},
	    {8, "Consumato", 1'232'000, 308'000, 1'355'200, 338'800},
	    {9, "I Hear Voices", 1'234'000, 308'500, 1'357'400, 339'350},
	    {10, "Winter, Nowhere in Particular", 0, 0, 0, 0},
	    {11, "The Girl With Pearl Necklace", 1'238'000, 309'500, 1'361'800, 340'450},
	    {12, "Chat on Fruit", 0, 0, 0, 0},
	    {13, "Pumpkin", 0, 0, 0, 0},
	    {14, "Twindifference", 0, 0, 0, 0},
	    {15, "Stacks Study V", 0, 0, 0, 0},
	    {16, "I, Fruit", 1'248'000, 312'000, 1'372'800, 343'200},
	    {17, "To Beat About Bush", 0, 0, 0, 0},
	    {18, "In Excess of Success", 0, 0, 0, 0},
	    {19, "Juiced", 0, 0, 0, 0},
	    {20, "A Winding Road Home", 0, 0, 0, 0},
	    {21, "Teckels", 0, 0, 0, 0},
	    {22, "Trust", 0, 0, 0, 0},
	    {23, "Until Death", 0, 0, 0, 0},
	    {24, "What Melons?", 0, 0, 0, 0},
	    {25, "The Outcome Endeavour", 0, 0, 0, 0},
	    {26, "Mi O Melee", 1'268'000, 317'000, 1'394'800, 348'700},
	};

	inline constexpr long long kPlanFee = 100'000;
	inline constexpr long long kBuyerN = 50'000;
	inline constexpr long long kBuyerH = 100'000;
	inline constexpr long long kEliteN = 50'000;
	inline constexpr long long kEliteH = 100'000;
	inline constexpr long long kSecondaryHintLo = 200'000;
	inline constexpr long long kSecondaryHintHi = 320'000;

	inline const Painting* FindPainting(int id)
	{
		for (const auto& p : kPaintings)
			if (p.id == id)
				return &p;
		return &kPaintings[0];
	}

	inline long long PrimaryValue(const Painting* p, bool hard, bool weekly)
	{
		if (!p)
			return 0;
		if (hard)
			return weekly ? p->h_weekly : p->h_fatigue;
		return weekly ? p->n_weekly : p->n_fatigue;
	}

	inline void Draw()
	{
		using namespace HeistLayout;
		using namespace HeistUI;

		auto* target = ListCmd("kortzcenterheistprimarytarget"_J);
		if (!target)
		{
			Hint("Kortz Center commands unavailable.");
			return;
		}

		static bool weekly = true;
		static bool hard = false;
		static bool first_setup = true;
		static bool buyer = true;
		static bool elite = true;
		static int secondary = 250'000;
		static int players = 1;

		const auto* paint = FindPainting(target->GetState());
		const long long primary = PrimaryValue(paint, hard, weekly);
		const long long buyer_pay = buyer ? (hard ? kBuyerH : kBuyerN) : 0;
		const long long elite_pay = elite ? (hard ? kEliteH : kEliteN) : 0;
		const long long plan_fee = first_setup ? 0 : kPlanFee;
		const long long gross = primary + secondary + buyer_pay + elite_pay;
		const long long host_net = gross - plan_fee;

		char money[48], money2[48];

		BeginPanel("overview");
		{
			SectionLabel("Overview");
			ImGui::Text("%s  ·  %s  ·  %s",
			    hard ? "Hard" : "Normal",
			    paint->name,
			    weekly ? "First sale this week" : "Buyer fatigue");

			ImGui::Spacing();
			FieldLabel("Sale source");
			if (Seg("Weekly first", weekly, 110.f))
				weekly = true;
			ImGui::SameLine(0, 6);
			if (Seg("Buyer fatigue", !weekly, 110.f))
				weekly = false;
			Hint("First painting sold after Thursday pays more. Later sales use buyer fatigue.");

			ImGui::Spacing();
			FieldLabel("Difficulty");
			if (Seg("Normal", !hard, 88.f))
				hard = false;
			ImGui::SameLine(0, 6);
			if (Seg("Hard", hard, 88.f))
				hard = true;

			ImGui::Spacing();
			FmtMoney(money, sizeof(money), primary);
			Stat("Primary", money);
			ImGui::SameLine(0, 24);
			FmtMoney(money, sizeof(money), secondary);
			Stat("Secondary", money);
			ImGui::SameLine(0, 24);
			FmtMoney(money, sizeof(money), gross);
			Stat("Result take", money);
			ImGui::SameLine(0, 24);
			FmtMoney(money, sizeof(money), host_net);
			Stat("Host after fee", money);

			ImGui::Spacing();
			const long long alt = PrimaryValue(paint, hard, !weekly);
			FmtMoney(money, sizeof(money), weekly ? primary : alt);
			FmtMoney(money2, sizeof(money2), weekly ? alt : primary);
			if (primary > 0)
				ImGui::TextDisabled("Weekly %s  ·  Fatigue %s", weekly ? money : money2, weekly ? money2 : money);
			else
				Hint("No confirmed value for this painting yet.");
			if (paint->id == 0)
				Hint("Story target — must be sold.");
		}
		EndPanel();

		DrawTwoColumns();
		{
			BeginPanel("heist");
			SectionLabel("Heist");
			DrawLabeledListComboFmt(target, "Primary Target", [hard](int id, char* out, size_t n) {
				const auto* p = FindPainting(id);
				const long long w = PrimaryValue(p, hard, true);
				const long long f = PrimaryValue(p, hard, false);
				if (w <= 0 && f <= 0)
				{
					snprintf(out, n, "unconfirmed");
					return;
				}
				char a[32], b[32];
				FmtMoney(a, sizeof(a), w);
				FmtMoney(b, sizeof(b), f);
				snprintf(out, n, "W %s  ·  F %s", a, b);
			});
			ImGui::Spacing();
			if (PanelBtn("##apply_setup", ICON_CHECK, "Apply Setup"))
				Call("kortzcenterheistsetup"_J);
			EndPanel();

			BeginPanel("run");
			SectionLabel("This run");
			FieldLabel("Planning fee");
			if (Seg("First (free)", first_setup, 110.f))
				first_setup = true;
			ImGui::SameLine(0, 6);
			if (Seg("Repeat $100k", !first_setup, 120.f))
				first_setup = false;

			ImGui::Spacing();
			FieldLabel("Finale size");
			for (int p = 1; p <= 4; ++p)
			{
				char lab[4];
				snprintf(lab, sizeof(lab), "%d", p);
				if (Seg(lab, players == p, 36.f))
					players = p;
				if (p < 4)
					ImGui::SameLine(0, 6);
			}
			Hint("Buyer items in Crisp Gallery need 2 players.");

			ImGui::Spacing();
			FieldLabel("Secondary take");
			PushFieldStyle();
			ImGui::SetNextItemWidth(FieldWidth());
			ImGui::InputInt("##secondary", &secondary);
			PopFieldStyle();
			ImGui::TextDisabled("Solo carry is usually around $%lld–$%lld.", kSecondaryHintLo, kSecondaryHintHi);

			ImGui::Spacing();
			bool b = buyer, e = elite;
			if (ImGui::Checkbox("Buyer's Request", &b))
				buyer = b;
			if (ImGui::Checkbox("Elite Challenge", &e))
				elite = e;
			FmtMoney(money, sizeof(money), hard ? kBuyerH : kBuyerN);
			FmtMoney(money2, sizeof(money2), hard ? kEliteH : kEliteN);
			ImGui::TextDisabled("Buyer %s  ·  Elite %s", money, money2);
			EndPanel();
		}
		NextColumn();
		{
			BeginPanel("estimate");
			SectionLabel("Payout estimate");
			FmtMoney(money, sizeof(money), primary);
			Stat("Primary", money);
			ImGui::SameLine(0, 20);
			FmtMoney(money, sizeof(money), secondary);
			Stat("Secondary", money);
			ImGui::Spacing();
			FmtMoney(money, sizeof(money), buyer_pay);
			Stat("Buyer", money);
			ImGui::SameLine(0, 20);
			FmtMoney(money, sizeof(money), elite_pay);
			Stat("Elite", money);
			ImGui::Spacing();
			FmtMoney(money, sizeof(money), plan_fee);
			Stat("Plan fee", money);
			ImGui::SameLine(0, 20);
			FmtMoney(money, sizeof(money), host_net);
			Stat("Host net", money);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			if (players <= 1)
			{
				FmtMoney(money, sizeof(money), host_net);
				ImGui::Text("Solo in pocket: %s", money);
			}
			else
			{
				const long long each = gross / players;
				const long long host_each = each - plan_fee;
				FmtMoney(money, sizeof(money), each);
				FmtMoney(money2, sizeof(money2), host_each);
				ImGui::Text("Even split ~%s each", money);
				ImGui::TextDisabled("Host after fee ~%s", money2);
			}
			EndPanel();

			BeginPanel("purchases");
			SectionLabel("Board purchases");
			BoolCommand* purchases[] = {
			    BoolCmd("kortzcenterheistguardroutes"_J),
			    BoolCmd("kortzcenterheistglasscutter"_J),
			    BoolCmd("kortzcenterheistpowerdrills"_J),
			    BoolCmd("kortzcenterheistempcharges"_J),
			};
			DrawBoolGrid(purchases);
			EndPanel();

			BeginPanel("intel");
			SectionLabel("Intel");
			BoolCommand* intel[] = {
			    BoolCmd("kortzcenterheistscopeout"_J),
			    BoolCmd("kortzcenterheistalphamail"_J),
			    BoolCmd("kortzcenterheistscopesecondary"_J),
			    BoolCmd("kortzcenterheistscopepoi"_J),
			};
			DrawBoolGrid(intel);
			EndPanel();
		}
		EndColumns();

		DrawTwoColumns();
		{
			BeginPanel("equipment");
			SectionLabel("Equipment");
			BoolCommand* equipment[] = {
			    BoolCmd("kortzcenterheisthazmat"_J),
			    BoolCmd("kortzcenterheiststaffkeycard"_J),
			    BoolCmd("kortzcenterheisttacticalequip"_J),
			    BoolCmd("kortzcenterheisthackingdevice"_J),
			    BoolCmd("kortzcenterheistaccesscode"_J),
			    BoolCmd("kortzcenterheistunmarkedweapons"_J),
			    BoolCmd("kortzcenterheistguardshipments"_J),
			    BoolCmd("kortzcenterheistprepemp"_J),
			    BoolCmd("kortzcenterheistguardroutesprep"_J),
			    BoolCmd("kortzcenterheistglasscutterprep"_J),
			    BoolCmd("kortzcenterheistpowerdrillsprep"_J),
			    BoolCmd("kortzcenterheistempchargesprep"_J),
			};
			DrawBoolGrid(equipment);
			EndPanel();
		}
		NextColumn();
		{
			BeginPanel("vehicles");
			SectionLabel("Vehicles");
			BoolCommand* vehicles[] = {
			    BoolCmd("kortzcenterheistcaracara"_J),
			    BoolCmd("kortzcenterheistannihilator"_J),
			    BoolCmd("kortzcenterheistmanchez"_J),
			    BoolCmd("kortzcenterheistcaracaraprep"_J),
			    BoolCmd("kortzcenterheistannihilatorprep"_J),
			    BoolCmd("kortzcenterheistmanchezprep"_J),
			};
			DrawBoolGrid(vehicles);
			EndPanel();
		}
		EndColumns();
	}
}
