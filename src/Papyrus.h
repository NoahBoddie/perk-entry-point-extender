#pragma once

namespace PEPE
{

	using SkyrimVM = RE::BSScript::IVirtualMachine;


	class Papyrus
	{
	public:
		static bool Install(SkyrimVM* a_vm)
		{
			a_vm->RegisterFunction("ApplyPerkEntryPoint", "PerkEntryPointExtender", ApplyPerkEntryPoint);
			a_vm->RegisterFunction("ApplyPerkEntryPointForm", "PerkEntryPointExtender", ApplyPerkEntryPointForm);
			a_vm->RegisterFunction("ApplyPerkEntryPointSpell", "PerkEntryPointExtender", ApplyPerkEntryPointSpell);
			a_vm->RegisterFunction("ApplyPerkEntryPointFloat", "PerkEntryPointExtender", ApplyPerkEntryPointFloat);
			a_vm->RegisterFunction("ApplyPerkEntryPointString", "PerkEntryPointExtender", ApplyPerkEntryPointString);
			
			
			a_vm->RegisterFunction("GetVersion", "PerkEntryPointExtender", GetVersion);

			return true;
		}

		//a_vm->TraceStack("Actor is None", a_stackID);

		//registers active effects to be watched, they must have a special id to do so though.

		//This is effectively only used for add leveled list. Also of note, what the fuck is with that, is it 4 params or 3?
		// It only seems to use 3, but WHAT THE FUCK, the forth is NEVER used. Or shouldn't
		static inline void ApplyPerkEntryPoint(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			logger::info("hit");

			RE::HandleEntryPoint(RE::PerkEntryPoint::kAllowMountActor, target, std::nullopt);
			auto result = EntryPointHandler::ApplyPerkEntryPoint(target, entry_point, args, nullptr, category, channel);
			auto nult = nullptr;
			const void* test = std::addressof(std::nullopt);

			if (result == RequestResult::Success)
				return;

			logger::info("{}", (int)result);
		}
		
		
		static inline std::vector<RE::TESForm*> ApplyPerkEntryPointForm(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			constexpr bool kernels_fix = false;

			//By default has a capacity of 1 so if kernel's fix is not installed, I can just give data.
			std::vector<RE::TESForm*> out{1};

			RequestResult result;
			if (kernels_fix)
				result = EntryPointHandler::ApplyPerkEntryPoint(target, entry_point, args, &out, category, channel);
			else
				result = EntryPointHandler::ApplyPerkEntryPoint(target, entry_point, args, out.data(), category, channel);

			//Don't seem like there's anything that should be different. I will need to print later.
			//if (result == RequestResult::Success)
			//	...;

			return out;
		}

		//Same as the above.
		static inline std::vector<RE::SpellItem*> ApplyPerkEntryPointSpell(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			constexpr bool kernels_fix = false;

			//By default has a capacity of 1 so if kernel's fix is not installed, I can just give data.
			std::vector<RE::SpellItem*> out{ 1 };

			RequestResult result;
			if (kernels_fix)
				result = EntryPointHandler::ApplyPerkEntryPoint(target, entry_point, args, &out, category, channel);
			else
				result = EntryPointHandler::ApplyPerkEntryPoint(target, entry_point, args, out.data(), category, channel);

			//Don't seem like there's anything that should be different. I will need to print later.
			//if (result == RequestResult::Success)
			//	...;

			return out;
		}


		static inline float ApplyPerkEntryPointFloat(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, float base_value, RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			
			//size_t size = target->currentProcess->middleHigh->perkData->at(RE::PerkEntryPoint::kModAttackDamage).size();
			
			//logger::info("size of attack array {}", size);



			auto result = EntryPointHandler::ApplyPerkEntryPoint(target, entry_point, args, &base_value, category, channel);

			if (result == RequestResult::Success)
				return base_value;

			return -1;//temporary, want to make an error value.
		}
		
		
		static inline RE::BSFixedString ApplyPerkEntryPointString(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, RE::BSFixedString or_value, RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			const char* out = nullptr;

			auto result = EntryPointHandler::ApplyPerkEntryPoint(target, entry_point, args, &out, category, channel);

			if (result == RequestResult::Success)
				return out;

			return or_value;
		}

		static inline std::vector<int32_t> GetVersion(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*)
		{
			return std::vector<int32_t> {std::begin(version), std::end(version)};
		}


		/*function ApplyPerkEntryPoint(Actor target, string entry_point, Form[] args, string category = "", int channel = 1) native global


		
			string function ApplyPerkEntryPointString(Actor target, string entry_point, Form[] args, string out_value = "", string category = "", int channel = 1) native global

			
			string function ApplyPerkEntryPointFloat(Actor target, string entry_point, Form[] args, float base_value, string category = "", int channel = 1) native global

			Form[] function ApplyPerkEntryPointForm(Actor target, string entry_point, Form[] args, string category = "", int channel = 1) native global

		//*/
	};


}