#include "Vehicle.hpp"
#include "game/frontend/items/Items.hpp"
#include "game/frontend/IconUI.hpp"
#include "game/frontend/submenus/Vehicle/SpawnVehicle.hpp"
#include "Vehicle/VehicleEditor.hpp"
#include "Vehicle/SavedVehicles.hpp"

namespace YimMenu::Submenus
{
	namespace
	{
		void DrawVehicleActions()
		{
			using namespace IconUI;

			static constexpr ActionButton kMaintenance[] = {
			    {ICON_WRENCH,   "Repair Vehicle", "repairvehicle"_J},
			    {ICON_BROOM,    "Clean Vehicle",  "cleanvehicle"_J},
			    {ICON_TOOLS,    "Fix All",        "fixallvehicles"_J},
			};

			static constexpr ActionButton kPersonal[] = {
			    {ICON_SIGN_IN, "Enter Last",   "enterlastvehicle"_J},
			    {ICON_PHONE,   "Call Mechanic", "callmechanic"_J},
			    {ICON_GARAGE,  "Request PV",   "requestpv"_J},
			    {ICON_CAR,     "Despawn PV",   "despawnpv"_J},
			    {ICON_SAVE,    "Save PV",      "savepersonalvehicle"_J},
			};

			DrawSectionHeader("Maintenance", ICON_WRENCH);
			DrawActionGrid(kMaintenance);

			ImGui::Spacing();
			ImGui::Spacing();

			DrawSectionHeader("Personal Vehicle", ICON_CAR);
			DrawActionGrid(kPersonal);
		}
	}

	Vehicle::Vehicle() :
		#define ICON_FA_CAR "\xef\x86\xb9"
	    Submenu::Submenu("Vehicle", ICON_FA_CAR)
	{
		auto main = std::make_shared<Category>("Main");
	
	// ── Quick Fix Actions ────────────────────────────────────────────────────//// 
	main->AddItem(std::make_shared<ImGuiItem>([] {
	    using namespace IconUI;

	    static constexpr ActionButton kQuickFix[] = {
	        {ICON_WRENCH,   "Repair Vehicle", "repairvehicle"_J},
	        {ICON_BROOM,    "Clean Vehicle",  "cleanvehicle"_J},
	        {ICON_TOOLS,    "Fix All",        "fixallvehicles"_J},
	    };

	    DrawSectionHeader("Quick Fix", ICON_TOOLS);
	    DrawActionGrid(kQuickFix);
	    ImGui::Spacing();
	    ImGui::Spacing();
	}));

		auto sections = std::make_shared<TabBarItem>("VehicleMainSections");

		// ── Options ──────────────────────────────────────────────────────────
		auto optionsTab = std::make_shared<TabItem>("Options");

		auto protection = std::make_shared<Group>("Protection", 2);
		protection->AddItem(std::make_shared<BoolCommandItem>("vehiclegodmode"_J, "Godmode"));
		protection->AddItem(std::make_shared<BoolCommandItem>("keepfixed"_J, "Keep Fixed"));
		protection->AddItem(std::make_shared<BoolCommandItem>("keepvehicleclean"_J, "Keep Clean"));
		protection->AddItem(std::make_shared<BoolCommandItem>("seatbelt"_J));

		auto performance = std::make_shared<Group>("Performance");
		performance->AddItem(std::make_shared<BoolCommandItem>("hornboost"_J));
		performance->AddItem(std::make_shared<BoolCommandItem>("modifyboostbehavior"_J));
		performance->AddItem(std::make_shared<ConditionalItem>("modifyboostbehavior"_J, std::make_shared<ListCommandItem>("boostbehavior"_J)));
		performance->AddItem(std::make_shared<BoolCommandItem>("speedometer"_J));
		performance->AddItem(std::make_shared<BoolCommandItem>("lowervehiclestance"_J, "Lower Stance"));

		auto misc = std::make_shared<Group>("Misc", 2);
		misc->AddItem(std::make_shared<BoolCommandItem>("allowhatsinvehicles"_J));
		misc->AddItem(std::make_shared<BoolCommandItem>("lsccustomsbypass"_J));
		misc->AddItem(std::make_shared<BoolCommandItem>("dlcvehicles"_J));

		optionsTab->AddItem(protection);
		optionsTab->AddItem(performance);
		optionsTab->AddItem(misc);

		// ── Actions ──────────────────────────────────────────────────────────
		auto actionsTab = std::make_shared<TabItem>("Actions");
		actionsTab->AddItem(std::make_shared<ImGuiItem>([] {
			DrawVehicleActions();
		}));

		sections->AddItem(std::move(optionsTab));
		sections->AddItem(std::move(actionsTab));
		main->AddItem(std::move(sections));

		AddCategory(std::move(main));
		AddCategory(BuildSpawnVehicleMenu());
		AddCategory(BuildVehicleEditorMenu());
		AddCategory(BuildSavedVehiclesMenu());
	}
}
