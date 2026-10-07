#include "DiamondCasinoHeist.hpp"
#include "CasinoHeistSetupUI.hpp"
#include "HeistUI.hpp"

namespace YimMenu::Submenus
{
	namespace
	{
		using namespace HeistLayout;
		using namespace HeistUI;

		void DrawDuring()
		{
			static constexpr ActionButton kShortcuts[] = {
			    {ICON_LAPTOP, "Skip Hacking", "diamondcasinoheistskiphacking"_J},
			    {ICON_DRILL, "Skip Drilling", "diamondcasinoheistskipdrilling"_J},
			    {ICON_CREDIT_CARD, "Solo Mantrap", "diamondcasinoheistsolomantrap"_J},
			    {ICON_FLAG, "Instant Finish", "diamondcasinoheistinstantfinish"_J},
			};
			DrawDuringHeist(kShortcuts);
		}
	}

	std::shared_ptr<TabItem> RenderDiamondCasinoHeistMenu()
	{
		auto tab = std::make_shared<TabItem>("Diamond Casino Heist");
		auto sections = std::make_shared<TabBarItem>("DiamondCasinoHeistSections");

		auto setupTab = std::make_shared<TabItem>("Setup");
		setupTab->AddItem(std::make_shared<ImGuiItem>([] {
			CasinoHeistSetupUI::Draw();
		}));

		auto duringTab = std::make_shared<TabItem>("During Heist");
		duringTab->AddItem(std::make_shared<ImGuiItem>([] { DrawDuring(); }));

		sections->AddItem(std::move(setupTab));
		sections->AddItem(std::move(duringTab));
		tab->AddItem(std::move(sections));
		return tab;
	}
}
