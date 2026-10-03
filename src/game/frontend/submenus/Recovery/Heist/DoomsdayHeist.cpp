#include "DoomsdayHeist.hpp"
#include "HeistUI.hpp"

namespace YimMenu::Submenus
{
	std::shared_ptr<TabItem> RenderDoomsdayHeistMenu()
	{
		using namespace HeistUI;

		auto tab = std::make_shared<TabItem>("Doomsday Heist");
		auto sections = std::make_shared<TabBarItem>("DoomsdayHeistSections");

		// ── Setup ────────────────────────────────────────────────────────────
		auto setupTab = std::make_shared<TabItem>("Setup");

		auto cuts = std::make_shared<Group>("Heist Cuts", 2);
		cuts->AddItem(std::make_shared<IntCommandItem>("doomsdayheistcut1"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("doomsdayheistcut3"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("doomsdayheistcut2"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("doomsdayheistcut4"_J));
		cuts->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kCutActions[] = {
			    {ICON_PERCENT, "Set Cuts",    "doomsdayheistsetcuts"_J},
			    {ICON_USERS,   "Force Ready", "doomsdayheistforceready"_J},
			};
			DrawSetupActions(kCutActions);
		}));

		auto setup = std::make_shared<Group>("Heist Setup");
		setup->AddItem(std::make_shared<ListCommandItem>("doomsdayheistcategory"_J));
		setup->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kSetupActions[] = {
			    {ICON_CHECK, "Apply Setup", "doomsdayheistsetup"_J},
			};
			DrawSetupActions(kSetupActions);
		}));

		setupTab->AddItem(cuts);
		setupTab->AddItem(setup);

		// ── During Heist ─────────────────────────────────────────────────────
		auto duringTab = std::make_shared<TabItem>("During Heist");
		duringTab->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kShortcuts[] = {
			    {ICON_LAPTOP, "Skip Hacking",        "doomsdayheistskiphacking"_J},
			    {ICON_FLAG,   "Instant Finish",      "doomsdayheistinstantfinish"_J},
			    {ICON_BOMB,   "Instant Finish Act3", "doomsdayheistinstantfinishact3"_J},
			};
			DrawDuringHeistSections(kShortcuts);
		}));

		sections->AddItem(std::move(setupTab));
		sections->AddItem(std::move(duringTab));
		tab->AddItem(std::move(sections));

		return tab;
	}
}
