#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"

namespace YimMenu::Features
{
	class KeepFixed : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (auto veh = Self::GetVehicle())
				veh.Fix();
		}
	};

	class KeepVehicleClean : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (auto veh = Self::GetVehicle())
				veh.Clean();
		}
	};

	static KeepFixed _KeepFixed{"keepfixed", "Keep Vehicle Fixed", "Keeps your vehicle repaired"};
	static KeepVehicleClean _KeepVehicleClean{"keepvehicleclean", "Keep Vehicle Clean", "Keeps your vehicle free from dirt and decals"};
}
