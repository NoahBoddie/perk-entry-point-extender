#pragma once

namespace RE
{
	using PerkEntryPoint = BGSPerkEntry::EntryPoint;
}

namespace PEPE
{
	//TODO:Want something to print the errors for RequestResult
	//TODO:Move RequestResult to API
	enum struct RequestResult
	{
		Success = -1,	//Success entry point usage.
		InvalidAPI,		//API failure, interface is not detected.
		Unsupported,	//entry point is not/no longer supported.
		UnmatchedArgs,	//entry arg number doesn't match no. args given
		BadFormArg,		//Invalid forms given as args
		BadEntryPoint,	//Invalid entry point value
		BadString,		//Invalid entry point name
		NoActor,		//No perk owner
		InvalidActor,	//Perk entry invalid on target
		InvalidOut,		//Doesn't have an out when it should
		
		
		
		
		UnknownError//The last entry, if this is seen the API has an issue that's out of date to handle.

	};
}


//I'm literally gambling that this is backwards compatible but the sizes and data types are the same
/*
template <class T>
struct ABIContainer
{
	using pointer = T*;

	T* _data = nullptr;
	std::uint64_t _size{};


	T& operator[] (std::uint64_t index) const
	{
		assert(index < _size);

		return _data[index];
	}

	uint64_t size() const
	{
		return _size;
	}
	ABIContainer() = default;
	ABIContainer(T* d, uint64_t s) : _data{ d }, _size{ s } {}
	ABIContainer(std::vector<T>& v) : _data{ v.data() }, _size{ v.size() } {}

	operator std::vector<T>() { return _data && _size ? std::vector<T>(_data, _data + _size) : std::vector<T>{}; }
};
//*/


namespace PerkEntryPointExtenderAPI
{
	template<typename T>
	using ABIContainer = std::span<T>;

	using namespace PEPE;

	enum Version
	{
		Version1,


		Current = Version1
	};

	struct InterfaceVersion1
	{
		inline static constexpr auto VERSION = Version::Version1;

		virtual ~InterfaceVersion1() = default;

		/// <summary>
		/// Gets the current version of the interface.
		/// </summary>
		/// <returns></returns>
		virtual Version GetVersion() = 0;


		virtual RequestResult ApplyPerkEntryPoint(RE::Actor* target, RE::PerkEntryPoint a_entryPoint, ABIContainer<RE::TESForm*> args, void* out,
			const char* category, uint8_t channel) = 0;

	};

	using CurrentInterface = InterfaceVersion1;

	inline CurrentInterface* Interface = nullptr;




	/// <summary>
	/// Accesses the Arithmetic Interface, safe to call PostLoad
	/// </summary>
	/// <param name="version"> to request.</param>
	/// <returns>Returns void* of the interface, cast to the respective version.</returns>
	inline void* RequestInterface(Version version)
	{
		typedef void* (__stdcall* RequestFunction)(Version);

		constexpr std::string_view plugin_name = "PerkEntryPointExtender.dll";

		static RequestFunction request_interface = nullptr;

		HINSTANCE API = GetModuleHandle(L"PerkEntryPointExtender.dll");

		if (API == nullptr) {
			logger::critical("PerkEntryPointExtender.dll not found, API will remain non functional.");
			return nullptr;
		}

		request_interface = (RequestFunction)GetProcAddress(API, "PEPE_RequestInterfaceImpl");

		if (request_interface) {
			if (static unsigned int once = 0; once++)
				logger::info("Successful module and request, PEPE");

		}
		else {
			logger::critical("Unsuccessful module and request, PEPE");
			return nullptr;
		}

		auto intfc = (CurrentInterface*)request_interface(version);

		return intfc;
	}

	/// <summary>
	/// Accesses the ActorValueGenerator Interface, safe to call PostLoad
	/// </summary>
	/// <typeparam name="InterfaceClass">is the class derived from the interface to use.</typeparam>
	/// <returns>Casts to and returns a specific version of the interface.</returns>
	template <class InterfaceClass = CurrentInterface>
	inline  InterfaceClass* RequestInterface()
	{
		static InterfaceClass* intfc = nullptr;

		if (!intfc) {
			intfc = reinterpret_cast<InterfaceClass*>(RequestInterface(InterfaceClass::VERSION));

			if constexpr (std::is_same_v<InterfaceClass, CurrentInterface>)
				Interface = intfc;
		}
		
		return intfc;
	}

}


namespace RE
{
	//&& strings will no longer count. Probably only string references, and string_views.

	//Within the api function, I require the ability to know if the out parameter is a vector or not.
	
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner,
		void* out, const std::string_view& category, uint8_t channel, std::vector<RE::TESForm*> arg_list)
	{
		auto* intfc = PerkEntryPointExtenderAPI::RequestInterface();

		//No interface. Bail.
		if (!intfc)
			return PEPE::RequestResult::InvalidAPI;

		return intfc->ApplyPerkEntryPoint(a_perkOwner, a_entryPoint, arg_list, out, category.data(), channel);
	}


	//Combinations
	//Stuff with no category or channel
	//stuff with no channel
	//stuff with string
	//stuff with string_view

	
	template <class O, std::derived_from<RE::TESForm>... Args>
	[[deprecated("Use of channels are a legacy feature and are deprecated. Use of keywords are prefered over ranks.")]]
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, O& out,
		const std::string_view& category, uint8_t channel, Args*... a_args)
	{
		constexpr bool no_out = std::is_same_v<std::nullopt_t, O>;

		if constexpr (no_out) {
			//If no out is desired it will send it with a nullptr so the proper error can show
			return HandleEntryPoint(a_entryPoint, a_perkOwner, nullptr, category, channel, { a_args... });
		}
		else {
			void* o = const_cast<std::remove_const_t<O>*>(std::addressof(out));
			return HandleEntryPoint(a_entryPoint, a_perkOwner, o, category, channel, { a_args... });
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="O"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="a_entryPoint">The given entry point function to alias.</param>
	/// <param name="a_perkOwner">The actor who the perk runs on.</param>
	/// <param name="out">The output value of the perk entry. Set to std::nullopt if there is no out value for the function.</param>
	/// <param name="category">The category to check for the entry point, preventing anything without that group from firing.</param>
	/// <param name="...a_args">The condition targets for the entry point conditions tab of the perk.</param>
	/// <returns>Returns the result of the function, and whether the call failed due to a parameter or API issue.</returns>
	template <class O, std::derived_from<RE::TESForm>... Args>
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, O& out, const std::string_view& category, Args*... a_args)
	{

		constexpr bool no_out = std::is_same_v<std::nullopt_t, O>;

		if constexpr (no_out) {
			//If no out is desired it will send it with a nullptr so the proper error can show
			return HandleEntryPoint(a_entryPoint, a_perkOwner, nullptr, category, 0, { a_args... });
		}
		else {
			void* o = const_cast<std::remove_const_t<O>*>(std::addressof(out));
			return HandleEntryPoint(a_entryPoint, a_perkOwner, o, category, 0, { a_args... });
		}
	}

	//no cat nor cha
	template <class O, std::derived_from<RE::TESForm>... Args>
	inline static PEPE::RequestResult HandleEntryPoint(RE::PerkEntryPoint a_entryPoint, RE::Actor* a_perkOwner, O& out, Args*... a_args)
	{
		return HandleEntryPoint(a_entryPoint, a_perkOwner, out, "", a_args...);
	}

	//cat only
	


}