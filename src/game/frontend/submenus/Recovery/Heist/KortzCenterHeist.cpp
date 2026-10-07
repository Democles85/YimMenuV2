#include "KortzCenterHeist.hpp"
#include "HeistUI.hpp"

namespace YimMenu::Submenus
{
	std::shared_ptr<TabItem> RenderKortzCenterHeistMenu()
	{
		using namespace HeistUI;

		auto tab = std::make_shared<TabItem>("Kortz Center Heist");
		auto sections = std::make_shared<TabBarItem>("KortzCenterSections");

		// ── Setup ────────────────────────────────────────────────────────────
		auto setupTab = std::make_shared<TabItem>("Setup");

		auto setup = std::make_shared<Group>("Heist Setup", 7, ICON_CHECK);
		setup->AddItem(std::make_shared<ListCommandItem>("kortzcenterheistprimarytarget"_J));
		setup->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kSetupActions[] = {
			    {ICON_CHECK, "Apply Setup", "kortzcenterheistsetup"_J},
			};
			DrawSetupActions(kSetupActions);
		}));

		auto purchases = std::make_shared<Group>("Board Purchases", 2, ICON_DOLLAR);
		purchases->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistguardroutes"_J));
		purchases->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistglasscutter"_J));
		purchases->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistpowerdrills"_J));
		purchases->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistempcharges"_J));

		auto intel = std::make_shared<Group>("Intel", 2, ICON_LAPTOP);
		intel->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistscopeout"_J));
		intel->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistalphamail"_J));
		intel->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistscopesecondary"_J));
		intel->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistscopepoi"_J));

		auto equipment = std::make_shared<Group>("Equipment", 2, ICON_TOOLS);
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheisthazmat"_J));
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheiststaffkeycard"_J));
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheisttacticalequip"_J));
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheisthackingdevice"_J));
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistaccesscode"_J));
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistunmarkedweapons"_J));
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistguardshipments"_J));
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistprepemp"_J));
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistguardroutesprep"_J));
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistglasscutterprep"_J));
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistpowerdrillsprep"_J));
		equipment->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistempchargesprep"_J));

		auto vehicles = std::make_shared<Group>("Vehicles", 2, ICON_CAR);
		vehicles->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistcaracara"_J));
		vehicles->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistannihilator"_J));
		vehicles->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistmanchez"_J));
		vehicles->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistcaracaraprep"_J));
		vehicles->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistannihilatorprep"_J));
		vehicles->AddItem(std::make_shared<BoolCommandItem>("kortzcenterheistmanchezprep"_J));

		setupTab->AddItem(setup);
		setupTab->AddItem(purchases);
		setupTab->AddItem(intel);
		setupTab->AddItem(equipment);
		setupTab->AddItem(vehicles);

		// ── During Heist ─────────────────────────────────────────────────────
		auto duringTab = std::make_shared<TabItem>("During Heist");
		duringTab->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kShortcuts[] = {
			    {ICON_FINGERPRINT, "Skip Fingerprint", "kortzcenterheistskipfingerprint"_J},
			    {ICON_MICROCHIP,   "Skip Signal Nodes", "kortzcenterheistskipsignalnodes"_J},
			    {ICON_LAPTOP,      "Skip Data Crack",   "kortzcenterheistskipdatacrack"_J},
			    {ICON_KEY,         "Enter Access Code", "kortzcenterheistautoenterpcaccesscode"_J},
			    {ICON_CUT,         "Cut Glass",         "kortzcenterheistcutglass"_J},
			    {ICON_BOLT,        "Disable Lasers",    "kortzcenterheistdisablelaser"_J},
			    {ICON_IMAGE,       "Take Primary",      "kortzcenterheisttakeprimary"_J},
			    {ICON_BOX,         "Take Secondary",    "kortzcenterheisttakesecondary"_J},
			};

			static constexpr ActionButton kTeleports[] = {
			    {ICON_SERVER,   "CCTV Server Room", "kortzcenterheisttpcctvserverroom"_J},
			    {ICON_PLUG,     "Green Powerbox",   "kortzcenterheisttpgreenpowerbox"_J},
			    {ICON_USER_TIE, "Staff Room",       "kortzcenterheisttpstaffroom"_J},
			    {ICON_DOLLAR,   "Sale Spot",        "kortzcenterheisttpsalespot"_J},
			};

			DrawDuringHeistSections(kShortcuts, kTeleports);
		}));

		sections->AddItem(std::move(setupTab));
		sections->AddItem(std::move(duringTab));
		tab->AddItem(std::move(sections));

		return tab;
	}
}
