#pragma once

#include "Condition.h"

//-1 is not init, 0 is not installed, 1 is installed and ready
inline int g_EditorIDs = -1;

using GetFormEditorID__Func = const char* (*)(RE::FormID);


inline bool CheckExternFunc(GetFormEditorID__Func& func)
{
	if (!func)
	{
		HINSTANCE PLUGIN = GetModuleHandle(L"po3_Tweaks.dll");

		//TODO: Look into the other Editor ID caching system
		if (PLUGIN == nullptr) {
			if (static unsigned int once = 0; once++)
				logger::warn("'po3_Tweaks.dll' not found, categories cannot be established without it.");
			
			g_EditorIDs = -1;
			
			return false;
		}

		//TODO:Actually make sure po3_Tweaks exists first.
		func = (GetFormEditorID__Func)GetProcAddress(PLUGIN, "GetFormEditorID");

		if (!func) {
			if (static unsigned int once = 0; once++)
				logger::error("'GetFormEditorID' not found within po3_Tweaks.");
			
			g_EditorIDs = -1;
			
			return false;
		}
		
		g_EditorIDs = 1;
	}

	return func != nullptr;

}

inline const char* GetFormEditorID(RE::TESForm* form)
{
	

	static GetFormEditorID__Func extern_func = nullptr;

	if (!extern_func)
	{
		CheckExternFunc(extern_func);
	}

	

	return extern_func ? extern_func(form->formID) : nullptr;
}


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

		static constexpr std::string_view groupHeader = "GROUP__";


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
			//outsiders should use 1 only, but the function actually uses 2.
			if (entry_point == RE::PerkEntryPoint::kAddLeveledListOnDeath) return 2;

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
				return 0;
			}

			return -1;
		}

		static constexpr bool IsOutRequired(RE::PerkEntryPoint entry_point)
		{
			switch (entry_point)
			{
			case RE::PerkEntryPoint::kAddLeveledListOnDeath:
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

		static bool IsInGroup(RE::BGSPerk* perk, std::string_view group_id)
		{
			if (!g_EditorIDs){
				return false;
			}
			//std::string_view editor_id = perk->GetFormEditorID();
			std::string_view editor_id = GetFormEditorID(perk);

			//if (editor_id.empty())
			//	logger::info("empty");
			
			auto end = editor_id.end();

			auto it = std::search(editor_id.begin(), editor_id.end(),
				group_id.begin(), group_id.end(),
				[](const char ch1, const char ch2) { return std::toupper(ch1) == std::toupper(ch2); });

			return it != end;
		}

		static bool IsInGroup(RE::BGSPerk* perk, std::string& group_id)
		{
			return IsInGroup(perk, std::string_view(group_id.begin(), group_id.end()));
		}

		//Currently doesn't work, if there's no filter this shouldn't be handling like this
		static bool IsCategoryValid(RE::BGSEntryPointPerkEntry* entry)
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

		//These should likely use references in some of these, to reduce the creating and shit.
		static RequestResult ApplyPerkEntryPoint(RE::PerkEntryPoint entry_point, RE::Actor* target, std::vector<RE::TESForm*>& args, void* out, RE::BSFixedString& category, uint8_t channel)
		{
			if (!target) {
				logger::debug("no actor given");
				return RequestResult::NoActor;
			}

			if (entry_point >= RE::PerkEntryPoint::kTotal) {
				logger::debug("bad ep {}", entry_point);
				return RequestResult::BadEntryPoint;
			}



			if (IsSupported(entry_point) == false) {
				logger::debug("unsupported ep {}", entry_point);
				return RequestResult::Unsupported;
			}


			bool use_out = IsOutRequired(entry_point);

			if (use_out && !out) {
				logger::debug("entry point requires out, no out detected.");
				return RequestResult::InvalidOut;
			}


			if (entry_point == RE::PerkEntryPoint::kAddLeveledListOnDeath)
				args.push_back(args.back());//adjusting the size for this wacky ass perk entry

			


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
					
					refr->ActivateRef(target, 0, nullptr, count, false);
				}

				logger::info("success {}", res);
				
				
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
				logger::debug("unmatching args {} vs {} ({})", args.size(), param_count, entry_point);

			}

			filterPtr = nullptr;

			return result;

		}

		static RequestResult ApplyPerkEntryPoint(RE::Actor* target, RE::BSFixedString& point_name, std::vector<RE::TESForm*>& args, void* out, RE::BSFixedString& category, uint8_t channel)
		{
			RE::PerkEntryPoint entry_point = GetPerkEntryPoint(point_name);

			if (entry_point == RE::PerkEntryPoint::kTotal) {
				logger::debug("bad string {}", point_name.c_str());
				return RequestResult::BadString;
			}

			//removing making the channel zero based.
			return ApplyPerkEntryPoint(entry_point, target, args, out, category, channel - 1);
		}


		static int ProcessPerk(RE::BGSPerk* perk)
		{
			
			//It should ignore this perk if it has no ranks.

			bool is_group = IsInGroup(perk, groupHeader);
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
				logger::info("rank of {}, {}", perk->GetName(), perk->data.numRanks);

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

			for (RE::TESForm* form : form_array) {
				auto* true_form = form->As<RE::BGSPerk>();

				if (true_form) {
					auto count = ProcessPerk(true_form);

					form_count += count != 0;
					entry_count += count;
				}
			}
			
			logger::info("Perk Entries Categorized. Processed {} perk{} and {} entr{}.",
				form_count, _s[form_count != 1],
				entry_count, _y[entry_count != 1]);

		}
	};


}