#include "ApartmentHeist.hpp"
#include "HeistUI.hpp"

namespace YimMenu::Submenus
{
	std::shared_ptr<TabItem> RenderApartmentHeistMenu()
	{
		using namespace HeistUI;

		auto tab = std::make_shared<TabItem>("Apartment Heist");
		auto sections = std::make_shared<TabBarItem>("ApartmentHeistSections");

		// ── Setup ────────────────────────────────────────────────────────────
		auto setupTab = std::make_shared<TabItem>("Setup");

		auto cuts = std::make_shared<Group>("Heist Cuts", 2);
		cuts->AddItem(std::make_shared<IntCommandItem>("apartmentheistcut1"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("apartmentheistcut3"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("apartmentheistcut2"_J));
		cuts->AddItem(std::make_shared<IntCommandItem>("apartmentheistcut4"_J));
		cuts->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kCutActions[] = {
			    {ICON_PERCENT, "Set Cuts",    "apartmentheistsetcuts"_J},
			    {ICON_USERS,   "Force Ready", "apartmentheistforceready"_J},
			};
			DrawSetupActions(kCutActions);
		}));

		auto setup = std::make_shared<Group>("Heist Setup");
		setup->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kSetupActions[] = {
			    {ICON_CHECK, "Apply Setup", "apartmentheistsetup"_J},
			};
			DrawSetupActions(kSetupActions);
		}));

		setupTab->AddItem(cuts);
		setupTab->AddItem(setup);

		// ── During Heist ─────────────────────────────────────────────────────
		auto duringTab = std::make_shared<TabItem>("During Heist");
		duringTab->AddItem(std::make_shared<ImGuiItem>([] {
			static constexpr ActionButton kShortcuts[] = {
			    {ICON_LAPTOP,      "Skip Hacking",       "apartmentheistskiphacking"_J},
			    {ICON_DRILL,       "Skip Drilling",      "apartmentheistskipdrilling"_J},
			    {ICON_CREDIT_CARD, "Skip Swiping",       "apartmentheistskipswiping"_J},
			    {ICON_FLAG,        "Instant Finish",     "apartmentheistinstantfinish"_J},
			    {ICON_SKULL,       "Instant Finish PSJ", "apartmentheistinstantfinishpacific"_J},
			};
			DrawDuringHeistSections(kShortcuts);
		}));

		sections->AddItem(std::move(setupTab));
		sections->AddItem(std::move(duringTab));
		tab->AddItem(std::move(sections));

		return tab;
	}
}
