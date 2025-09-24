#pragma once


#include "Utility.h"

namespace RE
{
	enum struct AdvanceAction
	{
		NormalUsage = 0,
		PowerAttack = 1,
		Bash = 2,
		LockpickSuccess = 3,
		LockpickBroken = 4,
	};
}
namespace PEPE
{
	namespace detail
	{
		struct FieldResult
		{
			enum Enum
			{
				Success = 1,
				Failure = 0,
				BadName = -1,
				BadArgs = -2,
				BadOrder = -3,
			};
		};
	}

	using FieldResult = detail::FieldResult::Enum;

	template <class CharType>
	constexpr CharType ConstToLower(const CharType c)
	{
		switch (c)
		{
		case 'A': return 'a';
		case 'B': return 'b';
		case 'C': return 'c';
		case 'D': return 'd';
		case 'E': return 'e';
		case 'F': return 'f';
		case 'G': return 'g';
		case 'H': return 'h';
		case 'I': return 'i';
		case 'J': return 'j';
		case 'K': return 'k';
		case 'L': return 'l';
		case 'M': return 'm';
		case 'N': return 'n';
		case 'O': return 'o';
		case 'P': return 'p';
		case 'Q': return 'q';
		case 'R': return 'r';
		case 'S': return 's';
		case 'T': return 't';
		case 'U': return 'u';
		case 'V': return 'v';
		case 'W': return 'w';
		case 'X': return 'x';
		case 'Y': return 'y';
		case 'Z': return 'z';
		default: return c;
		}
	}


	constexpr size_t Hash(const char* data, size_t const size = 0) noexcept
	{
		constexpr bool Insensitive = true;

		size_t hash = 5381;

		for (const char* c = data; size ? c < data + size : *c != '\0'; ++c) {
			unsigned char _c;

			if constexpr (Insensitive) _c = (unsigned char)ConstToLower(*c);
			else _c = (unsigned char)*c;

			hash = ((hash << 5) + hash) + _c;
		}

		return hash;
	}

	constexpr size_t Hash(std::string_view str) noexcept
	{
		return Hash(str.data(), str.length());
	}


	constexpr size_t operator"" _h(const char* str, size_t size) noexcept
	{
		//function and operator courtesy of Ershin's TDM
		return Hash(str, size);
	}

	//Handle is a struct that exists to add certain things that would be easy to add in papyrus when it comes to trying to emulate 
	// the results of a perk entry point function.
	struct Handle
	{
		//members marked with offset aren't reverse engineered yet. Should make a pull request later

		RE::EffectSetting* EPAlchemyEffectHasKeyword = nullptr;//PlayerCharacter::unkB90
		std::optional<bool> EPAlchemyGetMakingPoison = std::nullopt;//PlayerCharacter::offset->BDB (This is likely a flag rather than a bool I think, at 80h)
		RE::TESForm* EPModSkillUsage_AdvanceObjectHasKeyword = nullptr;//PlayerCharacter::9F0->advanceObject (Note, must be able to be made into a keyword form.
		std::optional<RE::AdvanceAction> EPModSkillUsage_IsAdvanceAction = std::nullopt;//PlayerCharacter::advanceAction_0AEC 
		std::optional<RE::ActorValue> EPModSkillUsage_IsAdvanceSkill = std::nullopt;//PlayerCharacter::advanceSkill_AE8
		//As these use InvetoryEntryData, there's not much of a way to set them from papyrus at the moment
		//Not gonna use right now.
		//std::optional<RE::InventoryEntryData> EPTemperingItem_ = std::nullopt;//PlayerCharacter::unkB98->temperingItem
		// Use EPTemperingItemHasKeyword to set template,
		// and EPTemperingItemIsEnchanted to set it as enchanted. Perhaps later it will double as a form, but for now it will do.
		
		//RE::InventoryEntryData* EPTemperingItemIsEnchanted = nullptr;//PlayerCharacter::unkB98->temperingItem


		FieldResult SetHandleItem(std::string_view name, RE::TESForm* form)
		{
			
			if (!form)
				return FieldResult::BadArgs;

			switch (Hash(name))
			{
				case "EPAlchemyEffectHasKeyword"_h:
					if (form->formType == RE::FormType::MagicEffect) 
					{
						EPAlchemyEffectHasKeyword = static_cast<RE::EffectSetting*>(form); 
						return FieldResult::Success;
					} 
					return FieldResult::BadArgs;

				case "EPModSkillUsage_AdvanceObjectHasKeyword"_h:
					switch (*form->formType)
					{
					case RE::FormType::Race:
					case RE::FormType::MagicEffect:
					case RE::FormType::Enchantment:
					case RE::FormType::Spell:
					case RE::FormType::Scroll:
					case RE::FormType::Ingredient:
					case RE::FormType::AlchemyItem:
					case RE::FormType::Activator:
					case RE::FormType::TalkingActivator:
					case RE::FormType::Flora:
					case RE::FormType::Furniture:
					case RE::FormType::Armor:
					case RE::FormType::Book:
					case RE::FormType::Ammo:
					case RE::FormType::Misc:
					case RE::FormType::Apparatus:
					case RE::FormType::KeyMaster:
					case RE::FormType::SoulGem:
					case RE::FormType::Weapon:
					case RE::FormType::NPC:
					case RE::FormType::Location:
						EPModSkillUsage_AdvanceObjectHasKeyword = form;
						return FieldResult::Success;
					default:
						return FieldResult::BadArgs;
					}
				/*
				case "EPTemperingItem"_h:
				case "EPTemperingItemHasKeyword"_h:
				case "EPTemperingItemIsEnchanted"_h:
					//All of these are technically viable here.
					if (form->IsBoundObject() == true)
					{
						EPTemperingItem_ = RE::InventoryEntryData{ static_cast<RE::TESBoundObject*>(form), 1 };

						RE::ExtraDataList test{};

						//EPTemperingItem_.value().Enchant
					}

				//*/
				default:
					return FieldResult::BadName;
			}
		}


		FieldResult SetHandleItem(std::string_view name, std::string_view string)
		{

			switch (Hash(name))
			{
			case "EPModSkillUsage_IsAdvanceSkill"_h:
				if (RE::ActorValue av = LookupActorValueByName(string.data()); av != RE::ActorValue::kNone)
				{
					EPModSkillUsage_IsAdvanceSkill = av;
					return FieldResult::Success;
				}
				return FieldResult::BadArgs;

			case "EPModSkillUsage_IsAdvanceAction"_h:
				switch (Hash(name))
				{
				case "NormalUsage"_h:
					EPModSkillUsage_IsAdvanceAction = RE::AdvanceAction::NormalUsage;
					return FieldResult::Success;

				case "PowerAttack"_h:
					EPModSkillUsage_IsAdvanceAction = RE::AdvanceAction::PowerAttack;
					return FieldResult::Success;

				case "Bash"_h:
					EPModSkillUsage_IsAdvanceAction = RE::AdvanceAction::Bash;
					return FieldResult::Success;

				case "LockpickSuccess"_h:
					EPModSkillUsage_IsAdvanceAction = RE::AdvanceAction::LockpickSuccess;
					return FieldResult::Success;

				case "LockpickBroken"_h:
					EPModSkillUsage_IsAdvanceAction = RE::AdvanceAction::LockpickBroken;
					return FieldResult::Success;
					
				default:
					return FieldResult::BadArgs;
				}
				
			default:
				return FieldResult::BadName;
			}
		}


		FieldResult SetHandleItem(std::string_view name, bool value)
		{

			switch (Hash(name))
			{
			case "EPAlchemyGetMakingPoison"_h:
				EPAlchemyGetMakingPoison = value;
				return FieldResult::Success;

			default:
				return FieldResult::BadName;
			}
		}


		void RunHandle(bool load)
		{
			if (!this)
				return;

			static RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();

			if (EPAlchemyEffectHasKeyword) {
				REL::RelocateMember<RE::EffectSetting*>(player, 0xB90, 0x1288) = load ? EPAlchemyEffectHasKeyword : nullptr;
			}

			if (EPAlchemyGetMakingPoison){//unk2_7 is MakingPoison
				REL::RelocateMember<RE::PlayerCharacter::PlayerFlags>(player, 0xBD8, 0x12D0).unk2_7 = load ? EPAlchemyGetMakingPoison.value() : false;
			}
			
			if (EPModSkillUsage_AdvanceObjectHasKeyword){
				REL::RelocateMember<RE::TESForm*>(player, 0x9F0, 0x10F0) = load ? EPModSkillUsage_AdvanceObjectHasKeyword : nullptr;
			}

			if (EPModSkillUsage_IsAdvanceAction){
				REL::RelocateMember<RE::AdvanceAction>(player, 0xAEC, 0x11EC) = load ? EPModSkillUsage_IsAdvanceAction.value() : RE::AdvanceAction::NormalUsage;
			}

			if (EPModSkillUsage_IsAdvanceSkill){
				REL::RelocateMember<RE::ActorValue>(player, 0xAE8, 0x11E8) = load ? EPModSkillUsage_IsAdvanceSkill.value() : RE::ActorValue{};
				
			}
		}
		
	};

	class HandleManager
	{
		//I may use a random generated number in order to get this, that way the validator is even more valid.
		inline static int32_t _nextID = 1;

		inline static std::unordered_map<int32_t, Handle> _handleMap;

		static int32_t PromoteID()
		{
			auto _new = _nextID++;
			
			if (_nextID)
				_nextID++;

			return _new;
		}

	public:

		static int32_t CreateHandle()
		{
			auto _new = PromoteID();

			_handleMap[_new];

			return _new;
		}


		static Handle* GetHandle(int32_t id)
		{
			if (!id)
				return nullptr;

			auto it = _handleMap.find(id);
			auto end = _handleMap.end();
			return it != end ? std::addressof(it->second) : nullptr;
		}


		static bool CloseHandle(int32_t id)
		{
			if (!id)
				return false;

			return _handleMap.erase(id);
		}
	};

}