#include "DiamondCasinoHeist.hpp"
#include "HeistUI.hpp"
#include "core/commands/Commands.hpp"
#include "core/commands/ListCommand.hpp"

namespace YimMenu::Submenus
{
    std::shared_ptr<TabItem> RenderDiamondCasinoHeistMenu()
    {
        using namespace HeistUI;

        auto tab = std::make_shared<TabItem>("Diamond Casino Heist");
        auto sections = std::make_shared<TabBarItem>("DiamondCasinoHeistSections");

        // ── Setup ────────────────────────────────────────────────────────────
        auto setupTab = std::make_shared<TabItem>("Setup");

        auto cuts = std::make_shared<Group>("Heist Cuts", 2);
        cuts->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistcut1"_J));
        cuts->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistcut3"_J));
        cuts->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistcut2"_J));
        cuts->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistcut4"_J));
        cuts->AddItem(std::make_shared<ImGuiItem>([] {
            static constexpr ActionButton kCutActions[] = {
                {ICON_PERCENT, "Set Cuts",    "diamondcasinoheistsetcuts"_J},
                {ICON_USERS,   "Force Ready", "diamondcasinoheistforceready"_J},
            };
            DrawSetupActions(kCutActions);
        }));

        auto setup = std::make_shared<Group>("Heist Setup");
        setup->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistdifficulty"_J));
        setup->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistprimarytarget"_J));
        setup->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistapproach"_J));

        auto crew = std::make_shared<Group>("Crew", 2);
        crew->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistgunman"_J));
        crew->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheistdriver"_J));
        crew->AddItem(std::make_shared<ConditionalItem>([] { return Commands::GetCommand<ListCommand>("diamondcasinoheistgunman"_J)->GetState() < 5; }, std::make_shared<ListCommandItem>("diamondcasinoheistweapon"_J)));
        crew->AddItem(std::make_shared<ConditionalItem>([] { return Commands::GetCommand<ListCommand>("diamondcasinoheistdriver"_J)->GetState() < 5; }, std::make_shared<ListCommandItem>("diamondcasinoheistvehicle"_J)));
        setup->AddItem(std::move(crew));
        setup->AddItem(std::make_shared<ListCommandItem>("diamondcasinoheisthacker"_J));
        setup->AddItem(std::make_shared<ImGuiItem>([] {
            static constexpr ActionButton kSetupActions[] = {
                {ICON_CHECK, "Apply Setup", "diamondcasinoheistsetup"_J},
            };
            DrawSetupActions(kSetupActions);
        }));

        auto loots = std::make_shared<Group>("Loots", 2);
        loots->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistpotentialtake"_J));
        loots->AddItem(std::make_shared<IntCommandItem>("diamondcasinoheistactualtake"_J));
        loots->AddItem(std::make_shared<ImGuiItem>([] {
            static constexpr ActionButton kLootActions[] = {
                {ICON_COINS,  "Set Potential", "diamondcasinoheistsetpotentialtake"_J},
                {ICON_DOLLAR, "Set Actual",    "diamondcasinoheistsetactualtake"_J},
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
                {ICON_LAPTOP,      "Skip Hacking",  "diamondcasinoheistskiphacking"_J},
                {ICON_DRILL,       "Skip Drilling", "diamondcasinoheistskipdrilling"_J},
                {ICON_CREDIT_CARD, "Solo Mantrap",  "diamondcasinoheistsolomantrap"_J},
                {ICON_FLAG,        "Instant Finish", "diamondcasinoheistinstantfinish"_J},
            };
            DrawDuringHeistSections(kShortcuts);
        }));

        sections->AddItem(std::move(setupTab));
        sections->AddItem(std::move(duringTab));
        tab->AddItem(std::move(sections));

        return tab;
    }
}