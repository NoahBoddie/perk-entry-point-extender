#include "Hooks.h"
#include "EntryPoint.h"
#include "Papyrus.h"

using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

//using namespace SOS;
//using namespace RGL;

void InitializeLogging() 
{
    auto path = log_directory();
    if (!path) {
        report_and_fail("Unable to lookup SKSE logs directory.");
    }
    *path /= PluginDeclaration::GetSingleton()->GetName();
    *path += L".log";

    std::shared_ptr<spdlog::logger> log;
    if (IsDebuggerPresent()) {
        log = std::make_shared<spdlog::logger>(
            "Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
    }
    else {
        log = std::make_shared<spdlog::logger>(
            "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
    }


#ifdef _DEBUG
    const auto level = spdlog::level::trace;
#else
    //Use right alt for just debug logging, control to allow debugger to attach.
    const auto level = GetKeyState(VK_RCONTROL) & 0x800 || GetKeyState(VK_RMENU) & 0x800 ?
        spdlog::level::debug : spdlog::level::info;
#endif


    log->set_level(level);
    log->flush_on(level);

    spdlog::set_default_logger(std::move(log));
    //spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
    spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);


#ifndef _DEBUG
    if (spdlog::level::debug == level) {
        logger::debug("debug logger in release enabled.");
    }
#endif
}



void InitializeMessaging() {
    if (!GetMessagingInterface()->RegisterListener([](MessagingInterface::Message* message) {
        switch (message->type) {
        case MessagingInterface::kPostLoad:
            
            break;
            // It is now safe to do multithreaded operations, or operations against other plugins.

        case MessagingInterface::kPostPostLoad: // Called after all kPostLoad message handlers have run.
            
            break;

        case MessagingInterface::kDataLoaded:
            PEPE::EntryPointHandler::CategorizePerkEntries();
            break;
        }
        })) {
        stl::report_and_fail("Unable to register message listener.");
    }
}


SKSEPluginLoad(const LoadInterface* skse) {
#ifdef _DEBUG
    
    InitializeLogging();
    
    if (GetKeyState(VK_RCONTROL) & 0x800) {
        constexpr auto text1 = L"Request for debugger detected. If you wish to attach one and press Ok, do so now if not please press Cancel.";
        constexpr auto text2 = L"Debugger still not detected. If you wish to continue without one please press Cancel.";
        constexpr auto caption = L"Debugger Required";

        int input = 0;

        do
        {
            input = MessageBox(NULL, !input ? text1 : text2, caption, MB_OKCANCEL);
        } while (!IsDebuggerPresent() && input != IDCANCEL);
    }
#endif

    const auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is loading...", plugin->GetName(), version);
    Init(skse);
   

    //TODO:Use message system to tell if po3 tweaks is present and the editor ID patch as been enabled
    InitializeMessaging();

    //May need to move.
    PEPE::Hooks::Install();

    auto papyrus = SKSE::GetPapyrusInterface();
    
    
    if (papyrus && papyrus->Register(PEPE::Papyrus::Install) == true) {
        log::debug("Papyrus functions bound.");
    }
    else {
        stl::report_and_fail("Failure to register Papyrus bindings.");
    }
    
    log::info("{} has finished loading.", plugin->GetName());
    
    return true;
}
