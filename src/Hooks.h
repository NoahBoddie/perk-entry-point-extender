

#include "EntryPoint.h"

namespace PEPE
{
	struct EntryPointPerkEntry__EvaluateConditionHook
	{
		static void  Patch()
		{
			func = REL::Relocation<uintptr_t>{ RE::VTABLE_BGSEntryPointPerkEntry[0] }.write_vfunc(0x0, thunk);
			logger::info("EntryPointPerkEntry__EvaluateConditionHook complete...");
		}
		//is zero
		static bool thunk(RE::BGSEntryPointPerkEntry* a_this, std::uint32_t a2, void* a3)
		{
			bool category_check = EntryPointHandler::IsCategoryValid(a_this);

			//If it's an invalid category, we do not wish to handle this regardless.
			if (!category_check)
				return false;

			return func(a_this, a2, a3);
		}

		inline static REL::Relocation<decltype(thunk)> func;
	};
	

	struct ApplyAttackSpellsHook
	{
		static void  Patch()
		{
			SKSE::AllocTrampoline(14);

			auto& trampoline = SKSE::GetTrampoline();

			//SE: 628C20, AE: 660A70, VR: ???
			auto hit_hook = REL::RelocationID(37673, 38627);
			
			func = trampoline.write_call<5>(hit_hook.address() + RELOCATION_OFFSET(0x185, 0x194), thunk);
			
			logger::info("ApplyAttackSpellsHook complete...");
		}

		//This hook is so ununique btw, that I think I can just write branch this shit. Straight up.
		static void thunk(RE::Actor* a_this, RE::InventoryEntryData* entry_data, bool is_left, RE::Actor* target)
		{
			if (!entry_data)
			{
				//static auto* weap = ;
				//Used to use new
				static RE::InventoryEntryData temp_data{ RE::TESForm::LookupByID<RE::TESBoundObject>(0x1F4), 1 };
				
				func(a_this, &temp_data, is_left, target);
				
				//delete data;
			}
			else
				func(a_this, entry_data, is_left, target);
		}

		static inline REL::Relocation<decltype(thunk)> func;
	};


	struct Hooks
	{
		static void Install()
		{
			//No need for allocation, YAAAAAY!
			EntryPointPerkEntry__EvaluateConditionHook::Patch();
			ApplyAttackSpellsHook::Patch();
		}
	};
}