#pragma once

#include "nlohmann/json.hpp"

namespace PEPE
{

    static constexpr std::string_view groupHeader = "GROUP__";

    using SkyrimVM = RE::BSScript::IVirtualMachine;


    //Please submit a pull request to handle this plz.
    static RE::ActorValue LookupActorValueByName(const char* av_name)
    {
        //SE: 0x3E1450, AE: 0x3FC5A0, VR: ---
        using func_t = decltype(&LookupActorValueByName);
        REL::Relocation<func_t> func{ RELOCATION_ID(26570, 27203) };
        return func(av_name);
    }

    inline static bool kernels_fix = false;

    static void ScrambugsPatch()
    {

        std::ifstream scramble{ "Data/SKSE/Plugins/ScrambledBugs.json" };

        if (!scramble)
            return;

        logger::info("Scrambled bugs detected, reading settings...");

        //prefers scrambugs version over this fix if that fix in enabled
        nlohmann::json scrambugs_json = nlohmann::json::parse(scramble, nullptr, true, true);

        try
        {
            bool kernels_fix = scrambugs_json["patches"]["perkEntryPoints"]["applyMultipleSpells"].get<bool>();

            if (kernels_fix) {
                logger::info("Scrambled bugs applyMultipleSpells patch detected.");
            }
            else {
                logger::info("Scrambled bugs applyMultipleSpells patch disabled.");
            }
        }
        catch (nlohmann::json::exception& error)
        {
            logger::warn("Scrambled bugs does not include 'patches/perkEntryPoints/applyMultipleSpells'. Using modified version.");
        }
    }


}