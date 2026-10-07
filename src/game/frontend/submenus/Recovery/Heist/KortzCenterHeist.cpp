#include "KortzCenterHeist.hpp"
#include "KortzCenterSetupUI.hpp"
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
			    {ICON_FINGERPRINT, "Skip Fingerprint", "kortzcenterheistskipfingerprint"_J},
			    {ICON_MICROCHIP, "Skip Signal Nodes", "kortzcenterheistskipsignalnodes"_J},
			    {ICON_LAPTOP, "Skip Data Crack", "kortzcenterheistskipdatacrack"_J},
			    {ICON_KEY, "Enter Access Code", "kortzcenterheistautoenterpcaccesscode"_J},
			    {ICON_CUT, "Cut Glass", "kortzcenterheistcutglass"_J},
			    {ICON_BOLT, "Disable Lasers", "kortzcenterheistdisablelaser"_J},
			    {ICON_IMAGE, "Take Primary", "kortzcenterheisttakeprimary"_J},
			    {ICON_BOX, "Take Secondary", "kortzcenterheisttakesecondary"_J},
			};

			static constexpr ActionButton kTeleports[] = {
			    {ICON_SERVER, "CCTV Server Room", "kortzcenterheisttpcctvserverroom"_J},
			    {ICON_PLUG, "Green Powerbox", "kortzcenterheisttpgreenpowerbox"_J},
			    {ICON_USER_TIE, "Staff Room", "kortzcenterheisttpstaffroom"_J},
			    {ICON_DOLLAR, "Sale Spot", "kortzcenterheisttpsalespot"_J},
			};

			DrawDuringHeist(kShortcuts, kTeleports);
		}
	}

	std::shared_ptr<TabItem> RenderKortzCenterHeistMenu()
	{
		auto tab = std::make_shared<TabItem>("Kortz Center Heist");
		auto sections = std::make_shared<TabBarItem>("KortzCenterSections");

		auto setupTab = std::make_shared<TabItem>("Setup");
		setupTab->AddItem(std::make_shared<ImGuiItem>([] { KortzCenterSetupUI::Draw(); }));

		auto duringTab = std::make_shared<TabItem>("During Heist");
		duringTab->AddItem(std::make_shared<ImGuiItem>([] { DrawDuring(); }));

		sections->AddItem(std::move(setupTab));
		sections->AddItem(std::move(duringTab));
		tab->AddItem(std::move(sections));
		return tab;
	}
}
