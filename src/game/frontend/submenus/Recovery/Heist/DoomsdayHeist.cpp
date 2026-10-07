#include "DoomsdayHeist.hpp"
#include "DoomsdaySetupUI.hpp"
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
			    {ICON_LAPTOP, "Skip Hacking", "doomsdayheistskiphacking"_J},
			    {ICON_FLAG, "Instant Finish", "doomsdayheistinstantfinish"_J},
			    {ICON_BOMB, "Instant Finish Act3", "doomsdayheistinstantfinishact3"_J},
			};
			DrawDuringHeist(kShortcuts);
		}
	}

	std::shared_ptr<TabItem> RenderDoomsdayHeistMenu()
	{
		auto tab = std::make_shared<TabItem>("Doomsday Heist");
		auto sections = std::make_shared<TabBarItem>("DoomsdayHeistSections");

		auto setupTab = std::make_shared<TabItem>("Setup");
		setupTab->AddItem(std::make_shared<ImGuiItem>([] { DoomsdaySetupUI::Draw(); }));

		auto duringTab = std::make_shared<TabItem>("During Heist");
		duringTab->AddItem(std::make_shared<ImGuiItem>([] { DrawDuring(); }));

		sections->AddItem(std::move(setupTab));
		sections->AddItem(std::move(duringTab));
		tab->AddItem(std::move(sections));
		return tab;
	}
}
