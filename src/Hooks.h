

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
	

	struct Hooks
	{
		static void Install()
		{
			//No need for allocation, YAAAAAY!
			EntryPointPerkEntry__EvaluateConditionHook::Patch();
		}
	};
}