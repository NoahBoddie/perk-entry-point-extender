#include "PerkEntryPointExtenderAPI.h"

#include "EntryPoint.h"



namespace PerkEntryPointExtenderAPI
{
	struct PerkEntryPointExtenderInterface : public CurrentInterface
	{
		Version GetVersion() override { return Version::Current; }

		//Deprecated version. Cannot handle vectors properly.
		RequestResult ApplyPerkEntryPoint_Deprecated(RE::Actor* target, RE::PerkEntryPoint a_entryPoint, ABIContainer<RE::TESForm*> arg_list, void* out, const char* cat, uint8_t channel) override
		{
			std::vector<RE::TESForm*> args{ arg_list.data(), arg_list.data() +  arg_list.size() };
			//RE::BSFixedString category = cat;
			
			return PEPE::EntryPointHandler::ApplyPerkEntryPoint(a_entryPoint, target, args, out, cat, channel, EntryPointFlag::None);
		}

		RequestResult ApplyPerkEntryPoint(RE::Actor* target, RE::PerkEntryPoint a_entryPoint, ABIContainer<RE::TESForm*> arg_list, void* out, const char* cat, 
			uint8_t channel, EntryPointFlag flags) override
		{
			void* in = nullptr;
			std::vector<RE::TESForm*> buffer{ 1 };

			if (flags & EntryPointFlag::UsesCollection) {
				in = kernels_fix ? &buffer : (void*)buffer.data();
			}
			else if (kernels_fix) {
				switch (a_entryPoint)
				{
				case RE::PerkEntryPoint::kApplyBashingSpell:
				case RE::PerkEntryPoint::kApplyCombatHitSpell:
				case RE::PerkEntryPoint::kApplyReanimateSpell:
				case RE::PerkEntryPoint::kApplySneakingSpell:
				case RE::PerkEntryPoint::kApplyWeaponSwingSpell:
					in = &buffer;
					break;
				}
			}
			else {
				in = out;
			}

			std::vector<RE::TESForm*> args{ arg_list.data(), arg_list.data() + arg_list.size() };
			//RE::BSFixedString category = cat;

			auto result = PEPE::EntryPointHandler::ApplyPerkEntryPoint(a_entryPoint, target, args, out, cat, channel, flags);
			
			if (flags & EntryPointFlag::UsesCollection) {
				IFormCollection* collection = reinterpret_cast<IFormCollection*>(out);

				for (auto entry : buffer)
				{
					if (entry)
						collection->LoadForm(entry);

				}
			}
			else if (in != out) {
				*reinterpret_cast<RE::TESForm**>(out) = buffer.front();
			}

			return result;
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

		if (result && result->GetVersion() >= version)
			return result;
		//Unsure if I actually will get rid of this. If I ever do multiple inherit it will be useful.
		/*
		switch (version)
		{
		case Version::Version1:
			return dynamic_cast<InterfaceVersion1*>(result);
		default:
			//Should show an error or something like that.
			return nullptr;
		}
		//*/
		return nullptr;
	}
}