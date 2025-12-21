#pragma once

#include "Handle.h"
#include "Utility.h"
#include "EntryPoint.h"
namespace PEPE
{


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

			a_vm->RegisterFunction("ApplyPerkEntryPointWithFlags", "PerkEntryPointExtender", ApplyPerkEntryPointWithFlags);
			a_vm->RegisterFunction("ApplyPerkEntryPointFormWithFlags", "PerkEntryPointExtender", ApplyPerkEntryPointFormWithFlags);
			a_vm->RegisterFunction("ApplyPerkEntryPointSpellWithFlags", "PerkEntryPointExtender", ApplyPerkEntryPointSpellWithFlags);
			a_vm->RegisterFunction("ApplyPerkEntryPointFloatWithFlags", "PerkEntryPointExtender", ApplyPerkEntryPointFloatWithFlags);
			a_vm->RegisterFunction("ApplyPerkEntryPointStringWithFlags", "PerkEntryPointExtender", ApplyPerkEntryPointStringWithFlags);
			
			
			a_vm->RegisterFunction("CloseHandle", "PerkEntryPointExtender", CloseHandle);
			a_vm->RegisterFunction("CreateHandle", "PerkEntryPointExtender", CreateHandle);
			a_vm->RegisterFunction("ValidateHandle", "PerkEntryPointExtender", ValidateHandle);
			a_vm->RegisterFunction("SetHandleItemForm", "PerkEntryPointExtender", SetHandleItemForm);
			a_vm->RegisterFunction("SetHandleItemBool", "PerkEntryPointExtender", SetHandleItemBool);
			a_vm->RegisterFunction("SetHandleItemString", "PerkEntryPointExtender", SetHandleItemString);

			
			a_vm->RegisterFunction("GetVersion", "PerkEntryPointExtender", GetVersion);

			return true;
		}

		//a_vm->TraceStack("Actor is None", a_stackID);

		//registers active effects to be watched, they must have a special id to do so though.

		//This is effectively only used for add leveled list. Also of note, what the fuck is with that, is it 4 params or 3?
		// It only seems to use 3, but WHAT THE FUCK, the forth is NEVER used. Or shouldn't
		static inline void ApplyPerkEntryPoint(SkyrimVM* vm, RE::VMStackID stack_id, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			EntryPointHandler::ApplyPerkEntryPointPapyrus(vm, stack_id, target, entry_point, args, nullptr, category, channel, handle, {});
		}
		
		
		static inline std::vector<RE::TESForm*> ApplyPerkEntryPointForm(SkyrimVM* vm, RE::VMStackID stack_id, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			//By default has a capacity of 1 so if kernel's fix is not installed, I can just give data.
			std::vector<RE::TESForm*> out{1};
			
			auto result = EntryPointHandler::ApplyPerkEntryPointPapyrus(vm, stack_id, target, entry_point, args, kernels_fix ? &out : (void*)out.data(), category, channel, handle, {});

			return result ? out : std::vector<RE::TESForm*>{};
		}

		//Same as the above.
		static inline std::vector<RE::SpellItem*> ApplyPerkEntryPointSpell(SkyrimVM* vm, RE::VMStackID stack_id, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			//By default has a capacity of 1 so if kernel's fix is not installed, I can just give data.
			std::vector<RE::SpellItem*> out{ 1 };
			
			auto result = EntryPointHandler::ApplyPerkEntryPointPapyrus(vm, stack_id, target, entry_point, args, kernels_fix ? &out : (void*)out.data(), category, channel, handle, {});

			return result ? out : std::vector<RE::SpellItem*>{};
		}


		static inline float ApplyPerkEntryPointFloat(SkyrimVM* vm, RE::VMStackID stack_id, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, float base_value, RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			
			//size_t size = target->currentProcess->middleHigh->perkData->at(RE::PerkEntryPoint::kModAttackDamage).size();
			
			//logger::info("size of attack array {}", size);



			EntryPointHandler::ApplyPerkEntryPointPapyrus(vm, stack_id, target, entry_point, args, &base_value, category, channel, handle, {});

			return base_value;//temporary, want to make an error value.
		}
		
		
		static inline RE::BSFixedString ApplyPerkEntryPointString(SkyrimVM* vm, RE::VMStackID stack_id, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, RE::BSFixedString or_value, RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			RE::BSString out;

			auto result = EntryPointHandler::ApplyPerkEntryPointPapyrus(vm, stack_id, target, entry_point, args, &out, category, channel, handle, {});

			return result ? out.c_str() : or_value;
		}



		//////////////////////////////////////

		static inline void ApplyPerkEntryPointWithFlags(SkyrimVM* vm, RE::VMStackID stack_id, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, std::vector<std::string> flag_strings,
			RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			EntryPointHandler::ApplyPerkEntryPointPapyrus(vm, stack_id, target, entry_point, args, nullptr, category, channel, handle, {});
		}


		static inline std::vector<RE::TESForm*> ApplyPerkEntryPointFormWithFlags(SkyrimVM* vm, RE::VMStackID stack_id, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, std::vector<std::string> flag_strings,
			RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			//By default has a capacity of 1 so if kernel's fix is not installed, I can just give data.
			std::vector<RE::TESForm*> out{ 1 };

			auto result = EntryPointHandler::ApplyPerkEntryPointPapyrus(vm, stack_id, target, entry_point, args, kernels_fix ? &out : (void*)out.data(), category, channel, handle, {});

			return result ? out : std::vector<RE::TESForm*>{};
		}

		//Same as the above.
		static inline std::vector<RE::SpellItem*> ApplyPerkEntryPointSpellWithFlags(SkyrimVM* vm, RE::VMStackID stack_id, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, std::vector<std::string> flag_strings,
			RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			//By default has a capacity of 1 so if kernel's fix is not installed, I can just give data.
			std::vector<RE::SpellItem*> out{ 1 };

			auto result = EntryPointHandler::ApplyPerkEntryPointPapyrus(vm, stack_id, target, entry_point, args, kernels_fix ? &out : (void*)out.data(), category, channel, handle, {});

			return result ? out : std::vector<RE::SpellItem*>{};
		}


		static inline float ApplyPerkEntryPointFloatWithFlags(SkyrimVM* vm, RE::VMStackID stack_id, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, float base_value, std::vector<std::string> flag_strings,
			RE::BSFixedString category, int32_t channel, int32_t handle)
		{

			//size_t size = target->currentProcess->middleHigh->perkData->at(RE::PerkEntryPoint::kModAttackDamage).size();

			//logger::info("size of attack array {}", size);



			EntryPointHandler::ApplyPerkEntryPointPapyrus(vm, stack_id, target, entry_point, args, &base_value, category, channel, handle, {});

			return base_value;//temporary, want to make an error value.
		}


		static inline RE::BSFixedString ApplyPerkEntryPointStringWithFlags(SkyrimVM* vm, RE::VMStackID stack_id, RE::StaticFunctionTag*,
			RE::Actor* target, RE::BSFixedString entry_point, std::vector<RE::TESForm*> args, std::vector<std::string> flag_strings, RE::BSFixedString or_value,
			RE::BSFixedString category, int32_t channel, int32_t handle)
		{
			RE::BSString out;

			auto result = EntryPointHandler::ApplyPerkEntryPointPapyrus(vm, stack_id, target, entry_point, args, &out, category, channel, handle, {});

			return result ? out.c_str() : or_value;
		}






		static int32_t CreateHandle(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*)
		{
			return HandleManager::CreateHandle();
		}


		static bool CloseHandle(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*,
			int32_t id)
		{
			return HandleManager::CloseHandle(id);
		}


		static int32_t ValidateHandle(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*,
			int32_t id)
		{
			return HandleManager::GetHandle(id) ? id : HandleManager::CreateHandle();
		}

		static int32_t SetHandleItemForm(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*,
			int32_t id, RE::BSFixedString name, RE::TESForm* form)
		{
			//returns 1 on success, 0 if handle does not exist, -1 if the field name doesn't exist and -2 if the parameter is invalid.

			Handle* handle = HandleManager::GetHandle(id);

			if (handle) {
				return 0;
			}

			return handle->SetHandleItem(name, form);

		}


		static int32_t SetHandleItemString(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*,
			int32_t id, RE::BSFixedString name, RE::BSFixedString string)
		{
			//returns 1 on success, 0 if handle does not exist, -1 if the field name doesn't exist and -2 if the parameter is invalid.

			Handle* handle = HandleManager::GetHandle(id);

			if (handle) {
				return 0;
			}

			return handle->SetHandleItem(name, string);
		}


		static int32_t SetHandleItemBool(SkyrimVM* a_vm, RE::VMStackID a_stackID, RE::StaticFunctionTag*,
			int32_t id, RE::BSFixedString name, bool value)
		{
			//returns 1 on success, 0 if handle does not exist, -1 if the field name doesn't exist and -2 if the parameter is invalid.

			Handle* handle = HandleManager::GetHandle(id);

			if (handle) {
				return 0;
			}

			return handle->SetHandleItem(name, value);
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