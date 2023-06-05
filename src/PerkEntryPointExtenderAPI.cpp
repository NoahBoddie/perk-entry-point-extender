#include "PerkEntryPointExtenderAPI.h"

#include "EntryPoint.h"

namespace PerkEntryPointExtenderAPI
{
	struct PerkEntryPointExtenderInterface : public CurrentInterface
	{
		Version GetVersion() override { return Version::Current; }

		RequestResult ApplyPerkEntryPoint(RE::Actor* target, RE::PerkEntryPoint a_entryPoint, ABIContainer<RE::TESForm*> arg_list, void* out, const char* cat, uint8_t channel) override
		{
			std::vector<RE::TESForm*> args = arg_list;
			RE::BSFixedString category = cat;
			
			return PEPE::EntryPointHandler::ApplyPerkEntryPoint(a_entryPoint, target, args, out, category, channel);
		}

	};


	[[nodiscard]] CurrentInterface* InferfaceSingleton()
	{
		static PerkEntryPointExtenderInterface intfc{};
		return std::addressof(intfc);
	}
}