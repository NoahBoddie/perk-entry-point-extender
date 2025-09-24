#pragma once

#include "Handle.h"
#include "Condition.h"


inline std::unordered_map<RE::BGSPerk*, std::string> legacyEditorIDs;


namespace PEPE
{
	using Channel = uint16_t;




	struct EntryPointHandler
	{
		struct EntryPointFilter
		{
			std::string category;
			uint8_t channel;
		};



		inline static std::array<RE::BSFixedString, RE::PerkEntryPoint::kTotal> entryPointStringList
		{
			"Calculate Weapon Damage",				//0
			"Calculate My Critical Hit Chance",		//1
			"Calculate My Critical Hit Damage",		//2
			"Calculate Mine Explode Chance",		//3
			"Adjust Limb Damage",					//4
			"Adjust Book Skill Points",				//5
			"Modify Recovered Health",				//6
			"Get Should Attack",					
			"Modify Buy Prices",					
			"Add Leveled List On Death",
			"Get Max Carry Weight",
			"Modify Addiction Chance",
			"Modify Addiction Duration",
			"Modify Positive Chem Duration",
			"Activate",
			"Ignore Running During Detection",
			"Ignore Broken Lock",
			"Modify Enemy Critical Hit Chance",
			"Modify Sneak Attack Mult",
			"Modify Max Placeable Mines",
			"Modify Bow Zoom",
			"Modify Recover Arrow Chance",
			"Modify Skill Use",
			"Modify Telekinesis Damage",
			"Modify Telekinesis Damage Mult",
			"Modify Telekinesis Distance",
			"Mod Bashing Damage",
			"Mod Power Attack Stamina",
			"Mod Power Attack Damage",
			"Mod Spell Magnitude",
			"Mod Spell Duration",
			"Mod Secondary Value Weight",
			"Mod Armor Weight",
			"Mod Incoming Stagger",
			"Mod Target Stagger",
			"Mod Attack Damage",
			"Mod Incoming Damage",
			"Mod Target Damage Resistance",
			"Mod Spell Cost",
			"Mod Percent Blocked",
			"Mod Shield Deflect Arrows",
			"Mod Incoming Spell Magnitude",
			"Mod Incoming Spell Duration",
			"Mod Player Intimidation",
			"Mod Player Reputation",
			"Mod Favor Points",
			"Mod Bribe Amount",
			"Mod Detection Light",
			"Mod Detection Movement",
			"Mod Soul Gem Recharge",
			"Set Sweep Attack",
			"Apply Combat Hit Spell",
			"Apply Bashing Spell",
			"Apply Reanimate Spell",
			"Set Boolean Graph Variable",
			"Mod Spell Casting Sound Event",
			"Modify Max Pickpocket Chance",
			"Modify Detection Sneak Skill",
			"Modify Falling Damage",
			"Modify Lockpick Sweet Spot",
			"Modify Sell Prices",
			"Can Pickpocket Equipped Item",
			"Modify Lockpick Level Allowed",
			"Set Lockpick Starting Arc",
			"Set Progression Picking",
			"Make Lockpicks Unbreakable",
			"Modify Alchemy Effectiveness",
			"Apply Weapon Swing Spell",
			"Modify Commanded Actor Limit",
			"Apply Sneaking Spell",
			"Modify Player Magic Slowdown",
			"Modify Ward Magicka Absorption Pct",
			"Modify Initial Ingredient Effects Learned",
			"Purify Alchemy Ingredients",
			"Filter Activation",
			"Can Dual Cast Spell",
			"Modify Tempering Health",
			"Modify Enchantment Power",
			"Modify Soul Pct Captured To Weapon",
			"Mod Soul Gem Enchanting",
			"Mod # Applied Enchantments Allowed",
			"Set Activate Label",
			"Mod Shout OK",
			"Mod Poison Dose Count",
			"Should Apply Placed Item",
			"Modify Armor Rating",
			"Modify Lockpicking Crime Chance",
			"Modify Ingredients Harvested",
			"Modify Spell Range (Target Loc.)",
			"Modify Potions Created",
			"Modify Lockpicking Key Reward Chance",
			"Allow Mount Actor"
		};

		static Channel& GetChannel(RE::BGSPerkEntry* a_this)
		{
			return a_this->header.unk2;
		}

		inline static thread_local EntryPointFilter* filterPtr = nullptr;

		static RE::PerkEntryPoint GetPerkEntryPoint(RE::BSFixedString& name)
		{
			if (name != "") {
				for (int i = 0; i < RE::PerkEntryPoint::kTotal; i++) {
					if (entryPointStringList[i] == name)
						return static_cast<RE::PerkEntryPoint>(i);
				}
			}

			return RE::PerkEntryPoint::kTotal;
		}

		static constexpr int GetEntryPointTargetCount(RE::PerkEntryPoint entry_point)
		{
			//papyrus users should use 2 to comply with the look, but the function takes a form as its "out", so it's value is 1.
			if (entry_point == RE::PerkEntryPoint::kAddLeveledListOnDeath) return 1;

			switch (entry_point)
			{
			case RE::PerkEntryPoint::kModAttackDamage:
			case RE::PerkEntryPoint::kAdjustLimbDamage:
			case RE::PerkEntryPoint::kApplyCombatHitSpell:
			case RE::PerkEntryPoint::kApplyReanimateSpell:
			case RE::PerkEntryPoint::kApplyWeaponSwingSpell:
			case RE::PerkEntryPoint::kCalculateMyCriticalHitChance:
			case RE::PerkEntryPoint::kCalculateMyCriticalHitDamage:
			case RE::PerkEntryPoint::kCanPickpocketEquippedItem:
			case RE::PerkEntryPoint::kModIncomingDamage:
			case RE::PerkEntryPoint::kModPoisonDoseCount:
			case RE::PerkEntryPoint::kModPowerAttackDamage:
			case RE::PerkEntryPoint::kModSecondaryValueWeight:
			case RE::PerkEntryPoint::kModSoulGemEnchanting:
			case RE::PerkEntryPoint::kModSpellDuration:
			case RE::PerkEntryPoint::kModSpellMagnitude:
			case RE::PerkEntryPoint::kModTargetDamageResistance:
			case RE::PerkEntryPoint::kModEnchantmentPower:
			case RE::PerkEntryPoint::kModEnemyCriticalHitChance:
			case RE::PerkEntryPoint::kModSneakAttackMult:
			case RE::PerkEntryPoint::kModPickpocketChance:
			case RE::PerkEntryPoint::kModSoulPctCapturedToWeapon:
			case RE::PerkEntryPoint::kShouldApplyPlacedItem:
				return 2;

			case RE::PerkEntryPoint::kAllowMountActor:
			case RE::PerkEntryPoint::kModBashingDamage:
			case RE::PerkEntryPoint::kApplyBashingSpell:
			case RE::PerkEntryPoint::kCalculateMineExplodeChance:
			case RE::PerkEntryPoint::kCanDualCastSpell:
			case RE::PerkEntryPoint::kFilterActivation:
			case RE::PerkEntryPoint::kGetShouldAttack:
			case RE::PerkEntryPoint::kModArmorWeight:
			case RE::PerkEntryPoint::kModBribeAmount:
			case RE::PerkEntryPoint::kModDetectionLight:
			case RE::PerkEntryPoint::kModDetectionMovement:
			case RE::PerkEntryPoint::kModFavorPoints:
			case RE::PerkEntryPoint::kModIncomingSpellDuration:
			case RE::PerkEntryPoint::kModIncomingSpellMagnitude:
			case RE::PerkEntryPoint::kModIncomingStagger:
			case RE::PerkEntryPoint::kModPlayerIntimidation:
			case RE::PerkEntryPoint::kModPowerAttackStamina:
			case RE::PerkEntryPoint::kModSoulGemRecharge:
			case RE::PerkEntryPoint::kModSpellCastingSoundEvent:
			case RE::PerkEntryPoint::kModSpellCost:
			case RE::PerkEntryPoint::kModTargetStagger:
			case RE::PerkEntryPoint::kModArmorRating:
			case RE::PerkEntryPoint::kModBowZoom:
			case RE::PerkEntryPoint::kModBuyPrices:
			case RE::PerkEntryPoint::kModCommandedActorLimit:
			case RE::PerkEntryPoint::kModDetectionSneakSkill:
			case RE::PerkEntryPoint::kModIngredientsHarvested:
			case RE::PerkEntryPoint::kModInitialIngredientEffectsLearned:
			case RE::PerkEntryPoint::kModLockpickSweetSpot:
			case RE::PerkEntryPoint::kModLockpickingCrimeChance:
			case RE::PerkEntryPoint::kModLockpickingKeyRewardChance:
			case RE::PerkEntryPoint::kModPlayerMagicSlowdown:
			case RE::PerkEntryPoint::kModPotionsCreated:
			case RE::PerkEntryPoint::kModSellPrices:
			case RE::PerkEntryPoint::kModSpellRange_TargetLoc:
			case RE::PerkEntryPoint::kModTelekinesisDamage:
			case RE::PerkEntryPoint::kModTemperingHealth:
			case RE::PerkEntryPoint::kModWardMagickaAbsorptionPct:
			case RE::PerkEntryPoint::kSetActivateLabel:
			case RE::PerkEntryPoint::kSetSweepAttack:
			case RE::PerkEntryPoint::kActivate:
				return 1;

			case RE::PerkEntryPoint::kModPercentBlocked:
			case RE::PerkEntryPoint::kAdjustBookSkillPoints:
			case RE::PerkEntryPoint::kApplySneakingSpell:
			case RE::PerkEntryPoint::kGetMaxCarryWeight:
			case RE::PerkEntryPoint::kIgnoreBrokenLock:
			case RE::PerkEntryPoint::kMakeLockpicksUnbreakable:
			case RE::PerkEntryPoint::kIgnoreRunningDuringDetection:
			case RE::PerkEntryPoint::kModNumberAppliedEnchantmentsAllowed:
			case RE::PerkEntryPoint::kModPlayerReputation:
			case RE::PerkEntryPoint::kModShieldDeflectArrowChance:
			case RE::PerkEntryPoint::kModShoutOK:
			case RE::PerkEntryPoint::kModAddictionChance:
			case RE::PerkEntryPoint::kModAddictionDuration:
			case RE::PerkEntryPoint::kModTelekinesisDistance:
			case RE::PerkEntryPoint::kModTelekinesisDamageMult:
			case RE::PerkEntryPoint::kModSkillUse:

				return 0;
			}

			return -1;
		}

		static constexpr bool IsOutRequired(RE::PerkEntryPoint entry_point)
		{
			switch (entry_point)
			{
			//case RE::PerkEntryPoint::kAddLeveledListOnDeath:
			case RE::PerkEntryPoint::kActivate:
				return false;

			default:
				return true;
			}
		}

		static constexpr bool IsSupported(RE::PerkEntryPoint entry_point)
		{
			switch (entry_point)
			{
			//case RE::PerkEntryPoint::kActivate:
			//	return false;

			default:
				return true;
			}
		}

		static bool IsInGroup(std::string_view editor_id, std::string_view group_id)
		{
			auto end = editor_id.end();
			
			auto it = std::search(editor_id.begin(), editor_id.end(),
				group_id.begin(), group_id.end(),
				[](const char ch1, const char ch2) { return std::toupper(ch1) == std::toupper(ch2); });

			return it != end;
		}

		static bool IsInGroup(RE::BGSPerk* perk, std::string_view group_id)
		{
			auto it = legacyEditorIDs.find(perk);


			if (legacyEditorIDs.end() == it){
				return false;
			}
			

			//std::string_view editor_id = perk->GetFormEditorID();
			std::string_view editor_id = it->second;

			if (editor_id.empty() == true)
				return false;

			auto end = editor_id.end();

			logger::debug("{} vs {} {} ?", editor_id, group_id, nullptr != filterPtr);

			return IsInGroup(editor_id, group_id);

			//auto it = std::search(editor_id.begin(), end,
			//	group_id.begin(), group_id.end(),
			//	[](const char ch1, const char ch2) { return std::toupper(ch1) == std::toupper(ch2); });
			//return it != end;
		}


		static bool IsCategoryValid_Legacy(RE::BGSEntryPointPerkEntry* entry)
		{
			//If there's no filter on the thread, it's a regular call.
			auto channel = GetChannel(entry);

			if (!filterPtr)
				//If there's no filter, the category is valid if there is no channel.
				return !channel;

			//Check for channel first, this is more extensive

			uint8_t filter = filterPtr->channel;

			//If the channel doesn't match the current filter OR the filter isn't set to any, It is invalid.
			if (channel != filter || filter == 255)
				return false;

			//If it's not within the group, don't run it.
			return IsInGroup(entry->perk, filterPtr->category);


		}

		static std::optional<bool> IsCategoryValid_Revised(RE::BGSEntryPointPerkEntry* entry, bool& do_legacy)
		{
			
			auto& conditions = entry->conditions;

			logger::debug("{}", entry->perk->GetName());

			if (conditions.size() == 0)
				return !filterPtr;
			
			logger::debug("{}", __COUNTER__);
			
			RE::TESConditionItem* head = conditions[0].head;

			if (!head)
				return !filterPtr;
			logger::debug("{}", __COUNTER__);
			RE::CONDITION_ITEM_DATA& data = head->data;

			if (data.functionData.function.get() != RE::FUNCTION_DATA::FunctionID::kHasKeyword)
				return !filterPtr;
			logger::debug("{}", __COUNTER__);


			auto& func_data = data.functionData;

			auto keyword = reinterpret_cast<RE::BGSKeyword*>(func_data.params[0]);

			if (!keyword || keyword->formType != RE::FormType::Keyword)
				return !filterPtr;
			
			auto comp = strncmp(keyword->GetFormEditorID(), groupHeader.data(), groupHeader.size());

			logger::debug("{}", keyword->GetFormEditorID());

			if (comp != 0)
				return !filterPtr;//This has to differ based on if there's a filter going or not.

			do_legacy = false;

			logger::debug("{}", __COUNTER__);

			//If there's no filter, it passes if no match, fails if there was one.
			if (!filterPtr) {
				logger::debug("{}", comp);
				return comp;
			}

			logger::debug("{}", __COUNTER__);
			//This actually doesn't need to do partials anymore, I can do the full thing with keywords
			if (stricmp(keyword->GetFormEditorID(), filterPtr->category.c_str()) != 0)
				return false;

			logger::debug("final alter {}", __COUNTER__);
			//if (IsInGroup(keyword->GetFormEditorID(), filterPtr->category) == false)
			//	return false;


			//I'm going to use this section to make sure that no matter what the conditions are it passes UNLESS it's something that may result in zero.

			using OpCode = RE::CONDITION_ITEM_DATA::OpCode;

			//Instead of editing data else where to make it pass, I'll be forcibly ensuring that it'll pass here.
			data.flags.global = false;
			data.flags.isOR = false;
			data.flags.opCode = OpCode::kGreaterThan;
			data.comparisonValue.f = 0.0f;


			return true;
		}




		static std::optional<bool> IsCategoryValid_RevisedNEW1(RE::TESConditionItem* head, float new_comp)
		{
			if (!head)
				return std::nullopt;

			logger::debug("{}", __COUNTER__);
			RE::CONDITION_ITEM_DATA& data = head->data;

			if (data.functionData.function.get() != RE::FUNCTION_DATA::FunctionID::kHasKeyword)
				return std::nullopt;

			logger::debug("{}", __COUNTER__);


			auto& func_data = data.functionData;

			auto keyword = reinterpret_cast<RE::BGSKeyword*>(func_data.params[0]);

			if (!keyword || keyword->formType != RE::FormType::Keyword)
				return std::nullopt;

			auto comp = strncmp(keyword->GetFormEditorID(), groupHeader.data(), groupHeader.size());

			logger::debug("{}", keyword->GetFormEditorID());

			if (comp != 0)
				return std::nullopt;//This has to differ based on if there's a filter going or not.


			logger::debug("{}", __COUNTER__);

			//If there's no filter, it passes if no match, fails if there was one.
			if (!filterPtr) {
				logger::debug("{}", comp);
				return false;
			}

			logger::debug("{}", __COUNTER__);
			//This actually doesn't need to do partials anymore, I can do the full thing with keywords
			if (stricmp(keyword->GetFormEditorID(), filterPtr->category.c_str()) != 0)
				return false;

			logger::debug("final alter {}", __COUNTER__);
			//if (IsInGroup(keyword->GetFormEditorID(), filterPtr->category) == false)
			//	return false;


			//I'm going to use this section to make sure that no matter what the conditions are it passes UNLESS it's something that may result in zero.

			using OpCode = RE::CONDITION_ITEM_DATA::OpCode;

			//Instead of editing data else where to make it pass, I'll be forcibly ensuring that it'll pass here.
			data.flags.global = false;
			data.flags.isOR = false;
			data.flags.opCode = OpCode::kGreaterThan;
			data.comparisonValue.f = new_comp;


			return true;
		}


		static std::optional<bool> IsCategoryValid_RevisedNEW2(RE::BGSEntryPointPerkEntry* entry)
		{

			auto& conditions = entry->conditions;

			logger::debug("{}", entry->perk->GetName());

			if (conditions.size() == 0)
				return !filterPtr;

			logger::debug("{}", __COUNTER__);

			RE::TESConditionItem* head = conditions[0].head;

			std::optional<bool> result = IsCategoryValid_RevisedNEW1(head, 0.f);

			
		

			if (result == std::nullopt) {
				//This is designed to always be true, even if someone doesn't have the perk.
				// granted, this would just mean

				head = entry->perk->perkConditions.head;
				result = IsCategoryValid_RevisedNEW1(head, -1.f);
				if (head && result == std::nullopt) {
					result = IsCategoryValid_RevisedNEW1(head->next, -1.f);
				}
			}

			return result;
		}



		static bool IsCategoryValidNEW(RE::BGSEntryPointPerkEntry* entry)
		{
			logger::debug("{}~ {}", entry->perk->GetName(), !!filterPtr);
			//This is ugly as shit.
			//bool do_legacy = true;

			std::optional<bool> result = IsCategoryValid_RevisedNEW2(entry);

			if (result.value_or(false) == true)
				return true;

			return !result.has_value() ? IsCategoryValid_Legacy(entry) : false;
		}


		//Currently doesn't work, if there's no filter this shouldn't be handling like this
		static bool IsCategoryValid(RE::BGSEntryPointPerkEntry* entry)
		{
			logger::debug("{}~ {}", entry->perk->GetName(), !!filterPtr);
			//This is ugly as shit.
			bool do_legacy = true;

			if (IsCategoryValid_Revised(entry, do_legacy) == true)
				return true;

			return do_legacy ? IsCategoryValid_Legacy(entry) : false;
		}


		static bool IsFilterActive()
		{
			return !filterPtr;
		}


		//These should likely use references in some of these, to reduce the creating and shit.
		static RequestResult ApplyPerkEntryPoint(RE::PerkEntryPoint entry_point, RE::Actor* target, std::vector<RE::TESForm*>& args, void* out, RE::BSFixedString& category, uint8_t channel)
		{
			if (!target) {
				logger::error("no actor given");
				return RequestResult::NoActor;
			}

			if (entry_point >= RE::PerkEntryPoint::kTotal) {
				logger::error("bad ep {}", entry_point);
				return RequestResult::BadEntryPoint;
			}



			if (IsSupported(entry_point) == false) {
				logger::error("unsupported ep {}", entry_point);
				return RequestResult::Unsupported;
			}


			bool use_out = IsOutRequired(entry_point);

			if (use_out && !out) {
				logger::error("entry point requires out, no out detected.");
				return RequestResult::InvalidOut;
			}


			

			


			//TODO:Add a header to the category for filters, I don't want to see PEPEGROUP__ at the start of all of these.
			EntryPointFilter filter{ std::string{groupHeader}, channel };

			

			//I want to clean this with some kind of defered call, maybe something with temporal flags when I get RGL in here.
			if (!category.empty() && channel) 
			{
				filter.category += category;
				filterPtr = &filter;
			}

			//I would like to make a special function for this, which has the option of stopping right here, or resuming.
			// I can use InvalidAPI (A thing that's never used here) to tell what happens.

			if (entry_point == RE::PerkEntryPoint::kActivate) {
				
				bool res = false;
				
				//here's something I didn't think about with these, I never thought to myself if blocking was possibly wanted.
				// Trying for regular activation might not be what this exactly wanted, so I think I'll actually move that
				// to the papyrus part of this function. I'll need to devise how to do that later. 
				//THEN bring the idea of using it to JaySerpa

				//But chief among these changes is that activation CAN actually be lumped in with it's regular versions.
				// With that being said the longer the name, the eariler it can appear.
				// Second to this, I'll want to make it so keyword versions of groups can have longer groups.
				//Lastly, anything that that uses something like this should have an arranged order.
				//The longer the name of it's category the more it's priority will be increased. Just for being an activate/setlabel should do it.
				//Alternatively, just call it twice. 
				//But this bit of arrangement maybe a good idea specifically for folder like categories
							

				RE::TESObjectREFR* refr = args[0]->As<RE::TESObjectREFR>();

				//If the target source is the player
				if (target->IsPlayerRef() == true)
				{
					using func_t = bool(*)(RE::ObjectRefHandle&);

					//I believe I have to delete this handle when I'm done
					RE::ObjectRefHandle handle = refr->CreateRefHandle();


					//SE: 0x6C2320 AE: 0x6FDBF0
					REL::Relocation<func_t> func{ RELOCATION_ID(39934, 41003) };

					res = func(handle);
				}

				if (!res) {
					auto count = refr->extraList.GetCount();
					
					res = refr->ActivateRef(target, 0, nullptr, count, false);
				}

				logger::debug("success {}", res);
				
				filterPtr = nullptr;
				return RequestResult::Success;
			}

			RequestResult result = RequestResult::UnmatchedArgs;

			if (auto param_count = GetEntryPointTargetCount(entry_point); args.size() == param_count)
			{



				switch (param_count)
				{
				case 0:
					if (!use_out)
						RE::BGSEntryPoint::HandleEntryPoint(entry_point, target);
					else
						RE::BGSEntryPoint::HandleEntryPoint(entry_point, target, out);
					break;
				case 1:
					if (!use_out)
						RE::BGSEntryPoint::HandleEntryPoint(entry_point, target, args[0]);
					else
						RE::BGSEntryPoint::HandleEntryPoint(entry_point, target, args[0], out);
					break;
				case 2:
					if (!use_out)
						RE::BGSEntryPoint::HandleEntryPoint(entry_point, target, args[0], args[1]);
					else
						RE::BGSEntryPoint::HandleEntryPoint(entry_point, target, args[0], args[1], out);
					break;
				}

				result = RequestResult::Success;
			}
			else
			{
				logger::debug("unmatching args {} vs {} (Entry point No. {})", args.size(), param_count, entry_point);

			}

			filterPtr = nullptr;

			return result;

		}

		static bool ApplyPerkEntryPointPapyrus(SkyrimVM* vm, RE::VMStackID stack_id, RE::Actor* target, RE::BSFixedString& point_name, std::vector<RE::TESForm*>& args, void* out, RE::BSFixedString& category, uint8_t channel, int32_t h_id)
		{
			//I'm considering having a safety check some where around here that will make sure that the out isn't bad or something.

			RE::PerkEntryPoint entry_point = GetPerkEntryPoint(point_name);
			
			constexpr auto kError = SkyrimVM::Severity::kError;


			Handle* handle = h_id ? HandleManager::GetHandle(h_id) : nullptr;

			if (!handle && h_id) {
				vm->VTraceStack(stack_id, kError, "EP condition handle %i isn't valid", h_id);
			}
			
			bool fire = true;


			if (entry_point == RE::PerkEntryPoint::kTotal) {
				vm->VTraceStack(stack_id, kError, "Invalid entry point name %s", point_name.c_str());
				fire = false;
			}

			if (fire)
			{
				if (entry_point == RE::PerkEntryPoint::kAddLeveledListOnDeath) {
					//This one is an interesting one, because the actual argument takes 3 forms, so the out form is the TESObjectREFR that the items go to.
					// In keeping with it's look, and to not have to make another form, I'm going to have users use 3 arguments, with the third
					//args.push_back(args.back());
					if (args.size() < 2) {
						vm->TraceStack("AddLeveledListOnDeath requires 2 arguments. (First for conditions, second for distribution)", stack_id);
						fire = false;
					}

					RE::TESForm* form = args.back();
					args.pop_back();

					if (!form) {
						vm->TraceStack("Last argument of AddLeveledListOnDeath is null", stack_id);
						fire = false;
					}

					switch (*form->formType)
					{
					case RE::FormType::Reference:
					case RE::FormType::ActorCharacter:
						break;

					default:
						vm->TraceStack("Last argument of AddLeveledListOnDeath is not an ObjectReference or Actor", stack_id);
						fire = false;
						break;
					}
					out = args.back();
					args.pop_back();

				}
			}


			if (fire) 
			{
				RequestResult result = ApplyPerkEntryPoint(entry_point, target, args, out, category, channel - 1);
				fire = false;
				switch (result)
				{
					case RequestResult::Success:
						fire = true;
						break;

					case RequestResult::Unsupported:	//entry point is not/no longer supported.
						vm->VTraceStack(stack_id, kError, "Entry Point %s isn't supported", magic_enum::enum_name(entry_point).data());
						break;

					case RequestResult::UnmatchedArgs:	//entry arg number doesn't match no. args given
						vm->VTraceStack(stack_id, kError, "Expected argument count doesn't match given argument count %i", (int32_t)args.size());
						break;

					case RequestResult::BadFormArg:		//Invalid forms given as args
						vm->TraceStack("Invalid forms given for entry point", stack_id);
						break;

					case RequestResult::NoActor:		//No perk owner
						vm->TraceStack("Target for entry point is null", stack_id);
						break;

					case RequestResult::InvalidActor:	//Perk entry invalid on target
						vm->VTraceStack(stack_id, kError, "Invalid entry point to use on target %s(%.8X)", target->GetDisplayFullName(), target->formID);
						break;

					case RequestResult::InvalidOut:
						vm->TraceStack("Entry Point returns but no return type designated", stack_id);
						break;

					default:
						vm->VTraceStack(stack_id, kError, "Unexpected error experienced (%s)", magic_enum::enum_name(result).data());
						break;
				}

			}
			
			
			
			if (handle) {
				handle->RunHandle(false);
				HandleManager::CloseHandle(h_id);
			}

			return fire;
		}


		static int ProcessPerk(RE::BGSPerk* perk)
		{
			
			//It should ignore this perk if it has no ranks.

			//bool is_group = IsInGroup(perk, groupHeader);
			bool is_group = true;
			auto result = 0;

			int max_ranks = perk->data.numRanks;


			for (auto& entry : perk->perkEntries)
			{
				//If if grouped, move rank into tag
				// if not grouped, make tag 0

				if (!result)
					max_ranks = 1;


				auto& channel = GetChannel(entry);


				if (entry->GetType() != RE::PERK_ENTRY_TYPE::kEntryPoint) {
					
					goto next;
				}
				

				if (is_group && entry->header.rank > 0) {
					//The rank represented the channel, so set it to zero with the rest of the ranks.
					channel = entry->header.rank;
					entry->header.rank = 0;

					if (!result)
						max_ranks = 1;

					result++;
					
				}
				else {
					//channel of zero means it acts like a regular perk entry.
					channel = 0;

				next:
					int new_ranks = entry->header.rank + 1;
					max_ranks = new_ranks > max_ranks ? new_ranks : max_ranks;
					continue;
				}



				//edit the num of ranks here\

				//Addition idea right here, use some of the padding on perk data (and confirm it's padding) to store what index to look for groups.
			}

			if (result) {//The ranks were a dream, they never existed
				perk->data.numRanks = max_ranks;
				logger::debug("rank of {}, {}", perk->GetName(), perk->data.numRanks);

			}

			return result;
		}



		static void CategorizePerkEntries()
		{
			RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();

			RE::BSTArray<RE::TESForm*>& form_array = handler->GetFormArray(RE::FormType::Perk);

			
			constexpr std::array<std::string_view, 2> _y{ "y", "ies" };
			constexpr std::array<std::string_view, 2> _s{ "", "s" };

			size_t form_count = 0;
			size_t entry_count = 0;

			//*
			for (RE::TESForm* form : form_array) {
				auto* perk = form->As<RE::BGSPerk>();

				if (perk) {
					//Dumb shit but it will probably be what I need to shake loose this terrible awfulness
					for (auto& entry : perk->perkEntries)
					{
						if (entry->GetType() == RE::PERK_ENTRY_TYPE::kEntryPoint) {
							EntryPointHandler::GetChannel(entry) = 0;
						}

					}
				}
			}
			//*/
			for (auto& [perk, str] : legacyEditorIDs)
			{
				auto count = ProcessPerk(perk);
				form_count += count != 0;
				entry_count += count;
			}

			logger::info("Perk Entries Categorized. Processed {} perk{} and {} entr{}.",
				form_count, _s[form_count != 1],
				entry_count, _y[entry_count != 1]);

		}
	};


}