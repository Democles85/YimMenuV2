#pragma once
#include "core/commands/Command.hpp"
#include "game/backend/Self.hpp"
#include "types/rage/vector.hpp"

namespace YimMenu::Features
{
	class TeleportToPoint : public Command
	{
		rage::fvector3 m_Coords;
		float m_Heading;

	public:
		TeleportToPoint(std::string name, std::string label, std::string description, rage::fvector3 coords, float heading) :
		    Command(std::move(name), std::move(label), std::move(description)),
		    m_Coords(coords),
		    m_Heading(heading)
		{
		}

		virtual void OnCall() override
		{
			if (auto ped = Self::GetPed())
			{
				ped.TeleportTo(m_Coords);
				if (auto veh = ped.GetVehicle())
					veh.SetHeading(m_Heading);
				else
					ped.SetHeading(m_Heading);
			}
		}
	};
}
