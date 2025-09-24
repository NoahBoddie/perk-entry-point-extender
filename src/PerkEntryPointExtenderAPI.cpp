#include "PerkEntryPointExtenderAPI.h"

#include "EntryPoint.h"



namespace PerkEntryPointExtenderAPI
{
	struct PerkEntryPointExtenderInterface : public CurrentInterface
	{
		Version GetVersion() override { return Version::Current; }

		RequestResult ApplyPerkEntryPoint(RE::Actor* target, RE::PerkEntryPoint a_entryPoint, ABIContainer<RE::TESForm*> arg_list, void* out, const char* cat, uint8_t channel) override
		{
			std::vector<RE::TESForm*> args{ arg_list.data(), arg_list.data() +  arg_list.size() };
			RE::BSFixedString category = cat;
			
			return PEPE::EntryPointHandler::ApplyPerkEntryPoint(a_entryPoint, target, args, out, category, channel);
		}

	};


	[[nodiscard]] CurrentInterface* InferfaceSingleton()
	{
		static PerkEntryPointExtenderInterface intfc{};
		return std::addressof(intfc);
	}


	extern "C" __declspec(dllexport) void* PEPE_RequestInterfaceImpl(Version version)
	{
		CurrentInterface* result = InferfaceSingleton();

		switch (version)
		{
		case Version::Version1:
			return dynamic_cast<InterfaceVersion1*>(result);
		default:
			//Should show an error or something like that.
			return nullptr;
		}

		return nullptr;
	}
}