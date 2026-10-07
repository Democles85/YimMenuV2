#include "ApartmentHeist.hpp"
#include "ApartmentSetupUI.hpp"
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
			    {ICON_LAPTOP, "Skip Hacking", "apartmentheistskiphacking"_J},
			    {ICON_DRILL, "Skip Drilling", "apartmentheistskipdrilling"_J},
			    {ICON_CREDIT_CARD, "Skip Swiping", "apartmentheistskipswiping"_J},
			    {ICON_FLAG, "Instant Finish", "apartmentheistinstantfinish"_J},
			    {ICON_SKULL, "Instant Finish PSJ", "apartmentheistinstantfinishpacific"_J},
			};
			DrawDuringHeist(kShortcuts);
		}
	}

	std::shared_ptr<TabItem> RenderApartmentHeistMenu()
	{
		auto tab = std::make_shared<TabItem>("Apartment Heist");
		auto sections = std::make_shared<TabBarItem>("ApartmentHeistSections");

		auto setupTab = std::make_shared<TabItem>("Setup");
		setupTab->AddItem(std::make_shared<ImGuiItem>([] { ApartmentSetupUI::Draw(); }));

		auto duringTab = std::make_shared<TabItem>("During Heist");
		duringTab->AddItem(std::make_shared<ImGuiItem>([] { DrawDuring(); }));

		sections->AddItem(std::move(setupTab));
		sections->AddItem(std::move(duringTab));
		tab->AddItem(std::move(sections));
		return tab;
	}
}
