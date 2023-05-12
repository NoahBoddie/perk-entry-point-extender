#pragma once


namespace PEPE
{
	struct ConditionHandler
	{
		struct Data
		{
			RE::ActorValue advanceSkill = RE::ActorValue::kNone;
			uint32_t advanceAction = 0;//Perhaps 0 is normal usage?
			uint8_t unkFlags = 0;//But 0x80 is known to be poison
			//RE::InventoryEntryData* inventoryItem = nullptr;//Just straight up, not gonna do this right now.
			RE::EffectSetting* magicEffect = nullptr;
			RE::ActiveEffect* activeEffect = nullptr;

			RE::TESObjectREFR* advanceObject = nullptr;

			void Apply()
			{

			}


			void Remove()
			{

			}
		} static _data;

		//Remember to reinterpret_cast the handles so it doesn't fuck up when sending it back.
		// It's just a linear climb, there's no way 4294967295 ACTUALLY gets used up, especially when I'm not serializing
		uint32_t _nextHandle = 1;

		static std::unordered_map<uint32_t, Data> conditionHandles;
	};

}