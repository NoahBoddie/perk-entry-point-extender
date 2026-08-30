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

	void NullifyInstructions(uintptr_t address, size_t num, size_t place_size = 5)
	{
		address += place_size;
		num -= place_size;

		while (num-- != 0) {
			// 0x90 is an int literal, and REL::safe_write's integral overload writes
			// sizeof(T) bytes -- passing it bare stamped 4 bytes (90 00 00 00) and wiped
			// three bytes of real code past the intended NOP. Cast to a single byte, and
			// advance the cursor so a run longer than one byte nops forward instead of
			// hammering the same address.
			REL::safe_write(address++, static_cast<std::uint8_t>(0x90));
		}
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

			//SE: 628C20, AE(640): 660A70, VR: 631AF0
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
			//SE: 2DDA40, AE: 2F3C80, VR: 2EEF50
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

			NullifyInstructions(hook_addr, 6);

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
			//SE: 338060, AE: 3500A0, VR: 347920
			auto hook_addr = REL::RelocationID(23346, 23815).address() + RELOCATION_OFFSET(0x109, 0x124);

			auto& trampoline = SKSE::GetTrampoline();

			auto placed_call = IsCallOrJump(hook_addr) > 0;

			auto place_query = trampoline.write_call<6>(hook_addr, (uintptr_t)thunk);

			if (placed_call)
				func = place_query;

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

	struct BGSPerkEntry_ConditionEvalHook
	{
		static void Install()
		{
			//SE: 337660, AE: 34F4B0, VR: 346F20
			auto hook_addr = REL::RelocationID(23332, 23800).address();

			struct Code : Xbyak::CodeGenerator
			{
				Code(uintptr_t func)
				{
					mov(r9d, ebp);
					mov(rax, func);
					jmp(rax);
				}
			} static code{ (uintptr_t)thunk };

			auto& trampoline = SKSE::GetTrampoline();

			//Need a pattern to ensure
			//REL::make_pattern<"EB">() == true;

			func = trampoline.write_call<5>(hook_addr + RELOCATION_OFFSET(0x17C, 0x1FF), (uintptr_t)code.getCode());


			logger::info("BGSPerkEntry_ConditionEvalHook complete...");
		}


		
		static bool thunk(RE::TESCondition& condition, RE::TESObjectREFR* subject, RE::TESObjectREFR* target, uint32_t index)
		{
			if (auto extra = EntryPointHandler::GetSecondaryArgument(index); extra.has_value()) {
				auto form = extra.value();
				if (!form)
					return !condition.head;
				
				target = ObtainConditionTarget(form);
				
				if (!target)
					return !condition.head;
			}

			struct ExtraDataListPtr
			{
				RE::ExtraDataList* ptr = nullptr;
				RE::ExtraDataList* other = nullptr;
				bool locked = false;
				constexpr ExtraDataListPtr() noexcept = default;
				constexpr ExtraDataListPtr(RE::ExtraDataList* list) noexcept : ptr{ list } {}
				constexpr operator bool() const noexcept { return ptr; }

				RE::BaseExtraList* GetBase(RE::ExtraDataList* tar = nullptr)
				{
					if (!tar)
						tar = ptr;

					return reinterpret_cast<RE::BaseExtraList*>(tar);
				}

				RE::BSReadWriteLock& GetLock(RE::ExtraDataList* tar = nullptr) const noexcept
				{
					if (!tar)
						tar = ptr;

					if SKYRIM_REL_CONSTEXPR(REL::Module::IsAE()) {
						return *reinterpret_cast<RE::BSReadWriteLock*>(reinterpret_cast<std::uintptr_t>(tar) +
							(REL::Module::get().version().compare(SKSE::RUNTIME_SSE_1_6_629) == std::strong_ordering::less ? 0x10 : 0x18));
					}
					else {
						return *reinterpret_cast<RE::BSReadWriteLock*>(reinterpret_cast<std::uintptr_t>(tar) + 0x10);
					}
				}


				void Lock(RE::ExtraDataList* list)
				{

					other = list;
					GetLock().LockForRead();
					locked = true;
				}

				void AssignToReference(RE::TESObjectREFR* ref)
				{
					if (!ref || !ptr)
						return;


					Lock(&ref->extraList);

					auto base = GetBase();
					auto other_base = GetBase(other);
					assert(!other_base->GetData());
					assert(!other_base->GetPresence());
					other_base->GetData() = base->GetData();
					other_base->GetPresence() = base->GetPresence();


				}

				~ExtraDataListPtr()
				{
					if (ptr && locked) {
						GetLock().UnlockForRead();
					}

					if (other) {
						auto other_base = GetBase(other);
						other_base->GetData() = nullptr;
						other_base->GetPresence() = nullptr;;

						auto data = RE::GetStaticTLSData();

						if (other = data->cachedExtraDataList) {
							data->cachedExtraDataList = nullptr;
							std::memset(data->cachedExtraData, 0, RE::TLSData::CACHED_EXTRA_DATA_SIZE + 8);
						}
					}
				}
			};

			ExtraDataListPtr subject_list;
			ExtraDataListPtr target_list;


			if (subject) {
				if (subject_list = EntryPointHandler::GetExtraArgument(true, index)) {
					subject_list.AssignToReference(subject);
				}
			}
		
			if (target) {
				if (target_list = EntryPointHandler::GetExtraArgument(false, index)) {
					subject_list.AssignToReference(target);
				}
			}

			return func(condition, subject, target);

		}

		inline static REL::Relocation<bool(RE::TESCondition&, RE::TESObjectREFR*, RE::TESObjectREFR*)> func;
	};

	//TODO: Move to PEE
	struct TEMP_RedoAttackDamageHook
	{
		static void  Patch()
		{
			auto& trampoline = SKSE::GetTrampoline();

			//SE: 694350, AE: NA, VR: ???
			auto hit_hook = REL::ID(39215);

			func = trampoline.write_call<5>(hit_hook.address() + 0x56, thunk);

			logger::info("ApplyAttackSpellsHook complete...");
		}

		//This hook is so ununique btw, that I think I can just write branch this shit. Straight up.
		static void thunk(RE::PerkEntryPoint ep, RE::Actor* owner, RE::TESObjectWEAP* weapon, RE::Actor* target, float* out)
		{
			if (auto hand = owner->GetMiddleHighProcess()->rightHand)
			{
				if constexpr (1)
				{
					std::vector<RE::TESForm*> args{ hand->object, target };
					std::vector<RE::ExtraDataList*> extras{ hand->extraLists && hand->extraLists->size() ? hand->extraLists->front() : nullptr, nullptr };

					auto result = RE::HandleEntryPoint(ep, owner, *out, hand, target);

					assert(result == RequestResult::Success);
				}
				else
				{
					std::vector<RE::TESForm*> args{ hand->object, target };
					std::vector<RE::ExtraDataList*> extras{ hand->extraLists && hand->extraLists->size() ? hand->extraLists->front() : nullptr, nullptr };

					auto result = EntryPointHandler::ApplyPerkEntryPoint(ep, owner, args, extras, out, "", 0, EntryPointFlag::None);

					assert(result == RequestResult::Success);
				}
			}
			else
			{
				return func(ep, owner, weapon, target, out);
			}
		}

		static inline REL::Relocation<decltype(thunk)> func;
	};



	struct Hooks
	{
		static void Install()
		{
			//Big write_call 6 right here for perks. That way I don't need help for legacy support
			//338060+109

			//BGSPerk::SetEditorID, we want to intercept this and then rerun the actual function, unless what was there was a 5 byte call


			SKSE::AllocTrampoline(14 * 5);
#ifndef NDEBUG
			//TEMP_RedoAttackDamageHook::Patch();
#endif
			//No need for allocation, YAAAAAY!
			ForEachPerkEntryHook::Install();
			Condition_HasKeywordHook::Patch();
			EntryPointPerkEntry__EvaluateConditionHook::Patch();
			ApplyAttackSpellsHook::Patch();

			BGSPerk_SetFormEditorIDHook::Install();
		}
	};
}