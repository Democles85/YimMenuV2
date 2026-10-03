#include "CayoPericoHeist.hpp"
#include "HeistUI.hpp"

namespace YimMenu::Submenus
{
	std::shared_ptr<TabItem> RenderCayoPericoHeistMenu()
	{
		using namespace HeistUI;

		auto tab = std::make_shared<TabItem>("Cayo Perico Heist");
		auto sections = std::make_shared<TabBarItem>("CayoPericoHeistSections");

		// ── Setup ────────────────────────────────────────────────────────────
		auto setupTab = std::make_shared<TabItem>("Setup");

		auto cuts = std::make_shared<Group>("Heist Cuts", 2);
		cuts->AddItem(std::make_shared<IntCommandItem>("cayopericoheistcut1"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("cayopericoheistcut3"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("cayopericoheistcut2"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("cayopericoheistcut4"_J));
		cuts->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kCutActions[] = {
			    {ICON_PERCENT, "Set Cuts",    "cayopericoheistsetcuts"_J},
			    {ICON_USERS,   "Force Ready", "cayopericoheistforceready"_J},
			};
			DrawSetupActions(kCutActions);
		}));

		auto setup = std::make_shared<Group>("Heist Setup");
		setup->AddItem(std::make_shared<ListCommandItem>("cayopericoheistdifficulty"_J));
		setup->AddItem(std::make_shared<ListCommandItem>("cayopericoheistprimarytarget"_J));
		setup->AddItem(std::make_shared<ListCommandItem>("cayopericoheistweapon"_J));
		setup->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kSetupActions[] = {
			    {ICON_CHECK, "Apply Setup", "cayopericoheistsetup"_J},
			};
			DrawSetupActions(kSetupActions);
		}));

		auto loots = std::make_shared<Group>("Loots", 2);
		loots->AddItem(std::make_shared<IntCommandItem>("cayopericoheistprimarytargetvalue"_J));
		loots->AddItem(std::make_shared<IntCommandItem>("cayopericoheistsecondarytakevalue"_J));
		loots->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kLootActions[] = {
			    {ICON_GEM,   "Set Primary",   "cayopericoheistsetprimarytargetvalue"_J},
			    {ICON_COINS, "Set Secondary", "cayopericoheistsetsecondarytakevalue"_J},
			};
			DrawSetupActions(kLootActions);
		}));

		setupTab->AddItem(cuts);
		setupTab->AddItem(setup);
		setupTab->AddItem(loots);

		// ── During Heist ─────────────────────────────────────────────────────
		auto duringTab = std::make_shared<TabItem>("During Heist");
		duringTab->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kShortcuts[] = {
			    {ICON_LAPTOP, "Skip Hacking",  "cayopericoheistskiphacking"_J},
			    {ICON_WATER,  "Cut Sewer",     "cayopericoheistcutsewer"_J},
			    {ICON_CUT,    "Cut Glass",     "cayopericoheistcutglass"_J},
			    {ICON_GEM,    "Take Primary",  "cayopericoheisttakeprimarytarget"_J},
			    {ICON_FLAG,   "Instant Finish", "cayopericoheistinstantfinish"_J},
			};
			DrawDuringHeistSections(kShortcuts);
		}));

		sections->AddItem(std::move(setupTab));
		sections->AddItem(std::move(duringTab));
		tab->AddItem(std::move(sections));

		return tab;
	}
}
