#pragma once

#include "Utility.h"
#include "EntryPoint.h"
#include "xbyak/xbyak.h"


namespace PEPE
{
	int IsCallOrJump(uintptr_t addr)
	{
		//0x15 0xE8//These are calls, represented by negative numbers
		//0x25 0xE9//These are jumps, represented by positive numbers.
		//And zero represent it being neither.

		if (addr)
		{
			auto first_byte = reinterpret_cast<uint8_t*>(addr);

			switch (*first_byte)
			{
			case 0x15:
			case 0xE8:
				return -1;

			case 0x25:
			case 0xE9:
				return 1;

			}
		}

		return 0;
	}



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
			//I will redesign this. The idea will basically be if the perk owner has the conditions of
			// if a keyword starting with "GROUP__" it will use that keyword to select it's group.
			// When doing it like this, channel will no longer matter. Legacy style will still care however.
			//At a later point I'll try to remove Po3Tweaks as a requirement.


			bool category_check = EntryPointHandler::IsCategoryValidNEW(a_this);

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

	struct Condition_HasKeywordHook
	{
		static void Patch()
		{
			//SE: (0x2DDA40), AE: (0x2F3C80), VR: ???
			auto hook_addr = REL::RelocationID(21187, 21644).address();
			auto return_addr = hook_addr + 0x6;
			//*
			struct Code : Xbyak::CodeGenerator
			{
				Code(uintptr_t ret_addr)
				{
					//AE/SE versions remain the same
					push(rbx);
					sub(rsp, 0x20);

					mov(rax, ret_addr);
					jmp(rax);
				}
			} static code{ return_addr };

			auto& trampoline = SKSE::GetTrampoline();

			auto placed_call = IsCallOrJump(hook_addr) > 0;

			auto place_query = trampoline.write_branch<5>(hook_addr, (uintptr_t)thunk);

			if (!placed_call)
				func = (uintptr_t)code.getCode();
			else
				func = place_query;

			logger::info("Condition_HasKeywordHook complete...");
			//*/
		}

		static bool thunk(RE::TESObjectREFR* a_this, RE::BGSKeyword* a2, void* a3, double* a4)
		{
			//This sort of thing always passes, it's the first thing that matters however.
			if (a2 && a2->formEditorID.c_str() && strncmp(a2->formEditorID.c_str(), groupHeader.data(), groupHeader.size()) == 0)
			{
				if (a4)
					*a4 = 1.0;

				return true;
			}
			else
			{
				return func(a_this, a2, a3, a4);
			}
		}

		static inline REL::Relocation<decltype(thunk)> func;
	};



	struct BGSPerk_SetFormEditorIDHook
	{
		static void Install()
		{
			//SE: (0x338060), AE: (0x3500A0), VR: ???
			auto hook_addr = REL::RelocationID(23346, 23815).address() + RELOCATION_OFFSET(0x109, 0x124);
			//*

			auto& trampoline = SKSE::GetTrampoline();

			auto placed_call = IsCallOrJump(hook_addr) > 0;

			auto place_query = trampoline.write_call<6>(hook_addr, (uintptr_t)thunk);

			

			if (placed_call)
				func = place_query;

			//logger::info("Condition_HasKeywordHook complete...");
			//*/
		}

		static bool thunk(RE::BGSPerk* a_this, const char* a_str)
		{
			if (a_str && EntryPointHandler::IsInGroup(a_str, groupHeader) == true) {
				legacyEditorIDs[a_this] = a_str;
				logger::info("Group found in {}", a_str);
			}
			else {
				auto res = legacyEditorIDs.erase(a_this);
				
				if (res)
					logger::info("Group erased in {}", a_str);
			}
			
			
			if (func.address())
				return func(a_this, a_str);
			else
				return a_this->SetFormEditorID(a_str);
		}

		static inline REL::Relocation<decltype(thunk)> func;
	};
	
	struct ForEachPerkEntryHook
	{

		static void Install()
		{
			auto Character_VTable = REL::Relocation{ RE::Character::VTABLE[0] };
			auto PlayerCharacter_VTable = REL::Relocation{ RE::PlayerCharacter::VTABLE[0] };
			func[0] = Character_VTable.write_vfunc(0x100, thunk<0>);
			func[1] = PlayerCharacter_VTable.write_vfunc(0x100, thunk<1>);

		}


		static void thunkImpl(RE::Actor* a_this, RE::PerkEntryPoint type, RE::PerkEntryVisitor& visitor, int I)
		{
			if (EntryPointHandler::ForEachPerkEntry(a_this, type, visitor) == true)
				return func[I](a_this, type, visitor);
		}

		template<int I = 0>
		static void thunk(RE::Actor* a_this, RE::PerkEntryPoint type, RE::PerkEntryVisitor& visitor)
		{
			return thunkImpl(a_this, type, visitor, I);
		}


		inline static REL::Relocation<decltype(thunk<>)> func[2];
	};


	struct Hooks
	{
		static void Install()
		{
			//Big write_call 6 right here for perks. That way I don't need help for legacy support
			//338060+109

			//BGSPerk::SetEditorID, we want to intercept this and then rerun the actual function, unless what was there was a 5 byte call


			SKSE::AllocTrampoline(14 * 3);


			//No need for allocation, YAAAAAY!
			ForEachPerkEntryHook::Install();
			Condition_HasKeywordHook::Patch();
			EntryPointPerkEntry__EvaluateConditionHook::Patch();
			ApplyAttackSpellsHook::Patch();

			BGSPerk_SetFormEditorIDHook::Install();
		}
	};
}