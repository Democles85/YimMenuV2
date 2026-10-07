#include "CayoPericoHeist.hpp"
#include "CayoPericoSetupUI.hpp"
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
			    {ICON_LAPTOP, "Skip Hacking", "cayopericoheistskiphacking"_J},
			    {ICON_WATER, "Cut Sewer", "cayopericoheistcutsewer"_J},
			    {ICON_CUT, "Cut Glass", "cayopericoheistcutglass"_J},
			    {ICON_GEM, "Take Primary", "cayopericoheisttakeprimarytarget"_J},
			    {ICON_FLAG, "Instant Finish", "cayopericoheistinstantfinish"_J},
			};
			DrawDuringHeist(kShortcuts);
		}
	}

	std::shared_ptr<TabItem> RenderCayoPericoHeistMenu()
	{
		auto tab = std::make_shared<TabItem>("Cayo Perico Heist");
		auto sections = std::make_shared<TabBarItem>("CayoPericoHeistSections");

		auto setupTab = std::make_shared<TabItem>("Setup");
		setupTab->AddItem(std::make_shared<ImGuiItem>([] { CayoPericoSetupUI::Draw(); }));

		auto duringTab = std::make_shared<TabItem>("During Heist");
		duringTab->AddItem(std::make_shared<ImGuiItem>([] { DrawDuring(); }));

		sections->AddItem(std::move(setupTab));
		sections->AddItem(std::move(duringTab));
		tab->AddItem(std::move(sections));
		return tab;
	}
}
