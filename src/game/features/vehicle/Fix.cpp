#include "core/commands/Command.hpp"
#include "game/backend/Self.hpp"

namespace YimMenu::Features
{
	class RepairVehicle : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			if (auto veh = Self::GetVehicle())
				veh.Fix();
		}
	};

	class CleanVehicle : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			if (auto veh = Self::GetVehicle())
				veh.Clean();
		}
	};

	static RepairVehicle _RepairVehicle{"repairvehicle", "Repair Vehicle", "Fixes any damage to your current vehicle"};
	static CleanVehicle _CleanVehicle{"cleanvehicle", "Clean Vehicle", "Removes dirt and decals from your current vehicle"};
}
