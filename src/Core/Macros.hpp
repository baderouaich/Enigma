#pragma once
#ifndef ENIGMA_MACROS_H
#define ENIGMA_MACROS_H


///Enable/Disable Assertions
#ifdef ENIGMA_DEBUG
	#define ENIGMA_ENABLE_ASSERTS true
#endif
///


/// Enigma NAME SPACE DEFINITION
#define NS_ENIGMA_BEGIN namespace Enigma {
#define NS_ENIGMA_END }
///


/// Enigma NAME SPACE DECLARATION
#define USING_NS_ENIGMA using namespace Enigma;
#define enigma Enigma
///


/// Bind Event function call back macro (alternative to std::bind for modern & better c++)
#define ENIGMA_BIND_FUN(fun) [this](auto&&... args) -> decltype(auto) { return this->fun(std::forward<decltype(args)>(args)...); }
//


///Asserts
#ifdef ENIGMA_ENABLE_ASSERTS
#define ENIGMA_ASSERT(x, ...) \
		do \
		{ \
			if(!(x)) \
			{ \
				ENIGMA_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
				ENIGMA_DEBUG_BREAK(); \
			} \
		} while(false) 
#else
	#define ENIGMA_ASSERT(x, ...)
	#define ENIGMA_ASSERT(x, ...)
#endif

#define ENIGMA_ASSERT_OR_THROW(x, msg) \
	do \
	{ \
		if(!(x)) \
		{ \
			ENIGMA_ASSERT(false, msg); \
			throw std::runtime_error(msg); \
		} \
	} while (false)

#define ENIGMA_ASSERT_OR_RETURN(x, msg, ret) \
	do \
	{ \
		if(!(x)) \
		{ \
			ENIGMA_ASSERT(false, msg); \
			return ret; \
		} \
	} while (false)
///






///Enigma Memory Safe Operations 
#define ENIGMA_SAFE_DELETE_PTR(ptr) \
do { \
	if ((ptr)){ \
		delete (ptr); \
		(ptr) = nullptr; \
	} \
} while(false)

#define ENIGMA_SAFE_DELETE_ARRAY_PTR(ptr) \
do { \
	if ((ptr)) { \
		delete[](ptr); \
		(ptr) = nullptr; \
	} \
} while(false)

#define ENIGMA_SAFE_DELETE_LIST_PTR(list) \
do { \
	if (((list)[0])){ \
		for (auto& ptr : list) \
			ENIGMA_SAFE_DELETE_PTR(ptr); \
	} \
} while (false);
///

/// Bit Shift
#define BIT(x) (1 << x)
///

/// Array Size
#define ENIGMA_ARRAY_SIZE(arr) static_cast<::Enigma::std::size_t>(sizeof(arr) / sizeof((arr)[0]))
///

/// Check if value is between or equal a range
#define ENIGMA_IS_BETWEEN(v, mi, ma) (v >= mi && v <= ma)
///

/// Resolve which function signature macro will be used. Note that this only
/// is resolved when the (pre)compiler starts, so the syntax highlighting
/// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
	#define ENIGMA_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
	#define ENIGMA_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
	#define ENIGMA_CURRENT_FUNCTION __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
	#define ENIGMA_CURRENT_FUNCTION __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
	#define ENIGMA_CURRENT_FUNCTION __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
	#define ENIGMA_CURRENT_FUNCTION __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
	#define ENIGMA_CURRENT_FUNCTION __func__
#elif defined(_MSC_VER)
	#define ENIGMA_CURRENT_FUNCTION __FUNCTION__
#else
	#define ENIGMA_CURRENT_FUNCTION "<unknown function>"
#endif
///



/// Prevent class copying or moving
#define ENIGMA_NON_COPYABLE(Class)                   \
    Class& operator=(const Class& obj) = delete;  \
    Class(const Class& obj) = delete;
#define ENIGMA_NON_MOVEABLE(Class)                       \
    Class& operator=(Class&& obj) = delete;        
///


/// Makes a class static, which will prevent creating instances from it and only use it as Class::Func()..
#define ENIGMA_STATIC_CLASS(Class) \
		public: \
			ENIGMA_NON_COPYABLE(Class) \
			ENIGMA_NON_MOVEABLE(Class) \
			Class() = delete; \
			~Class() = delete; \
		private:
///



/// Begin and end timer with a unit
#define ENIGMA_BEGIN_TIMER(var) const auto var = std::chrono::steady_clock::now()
#define ENIGMA_END_TIMER(var, type, unit) std::chrono::duration_cast<std::chrono::duration<type, unit>>(std::chrono::steady_clock::now() - var).count()
// example: double ms = ENIGMA_END_TIMER(t1, double, std::milli);
///




/// Logs current function for debugging
#if defined(ENIGMA_DEBUG)
//#define ENIGMA_TRACE_CURRENT_FUNCTION() ENIGMA_TRACE(::Enigma::StringUtils::Cleanup(::Enigma::StringUtils::Cleanup(::Enigma::StringUtils::Cleanup(ENIGMA_CURRENT_FUNCTION, " __cdecl"), "struct"), "class"))
#define ENIGMA_TRACE_CURRENT_FUNCTION() ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION)
#else
#define ENIGMA_TRACE_CURRENT_FUNCTION()
#endif
///

/// Allows looping over an enum by providing a BEGIN and END enum values NOTE: should be placed at the end of the enum, providing the first element of the enum
#define ENIGMA_ENUM_DECLARE_BEGIN_END(begin) \
	__HALT__, \
	BEGIN = begin, \
	END = __HALT__ - 1
///

/// Implements bitwise operators for enum classes
#define ENIGMA_ENUM_CLASS_BITWISE_OPERATORS(_enum, _type) \
	friend inline constexpr _enum operator~ (const _enum i) noexcept { return static_cast<_enum>(~static_cast<const _type>(i)); }  \
	friend inline constexpr _enum operator| (const _enum a, const _enum b) noexcept { return static_cast<_enum>(static_cast<const _type>(a) | static_cast<const _type>(b)); } \
	friend inline constexpr  bool operator& (const _enum a, const _enum b) noexcept { return static_cast<bool>(static_cast<const _type>(a) & static_cast<const _type>(b)); } \
	friend inline constexpr _enum operator^ (const _enum a, const _enum b) noexcept { return static_cast<_enum>(static_cast<const _type>(a) ^ static_cast<const _type>(b)); } \
	friend inline constexpr _enum operator|= (_enum & a, const _enum b) noexcept { return (_enum&)((_type&)(a) |= (const _type)(b)); } \
	friend inline constexpr _enum operator&= (_enum & a, const _enum b) noexcept { return (_enum&)((_type&)(a) &= (const _type)(b)); } \
	friend inline constexpr _enum operator^= (_enum & a, const _enum b) noexcept { return (_enum&)((_type&)(a) ^= (const _type)(b)); }
///


//static void TimedFunction(
//	const std::function<void()>& onStart,
//	const std::function<void()>& func,
//	const std::function<void(::Enigma::double)>& onFinish)
//{
//	onStart();
//	ENIGMA_BEGIN_TIMER(t1);
//		func();
//	::Enigma::double elapsed = ENIGMA_END_TIMER(t1, double, std::milli) / 1000.0;
//	onFinish(elapsed);
//}




#endif // !ENIGMA_MACROS_H
