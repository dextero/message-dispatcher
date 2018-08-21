#pragma once

#include <type_traits>

#ifndef __cpp_lib_type_trait_variable_templates
namespace std
{
    template <typename T>
    constexpr bool is_function_v = std::is_function<T>::value;
}
#endif

namespace pk
{
    template <typename F>
    struct lambda_traits;

    template <typename F, typename Ret, typename... Args>
    struct lambda_traits<Ret(F::*)(Args...) const>
    {
        static const std::size_t parameter_count = sizeof...(Args);
        using return_type = Ret;

        template <std::size_t N>
        using parameter = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    // Query function traits
    template<typename F>
    struct function_traits : lambda_traits<decltype(&F::operator())> {};

    // Function without dereferencing
    template <typename Ret, typename ...Args>
    struct function_traits<Ret(Args...)> : function_traits<Ret(*)(Args...)>
    {};

    // Functions
    template<typename Ret, typename ...Args>
    struct function_traits<Ret(*)(Args...)>
    {
        static const std::size_t parameter_count = sizeof...(Args);
        using return_type = Ret;

        template <std::size_t N>
        using parameter = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };

    // Methods
    template <typename C, typename Ret, typename... Args>
    struct function_traits<Ret(C::*)(Args...)>
    {
        static const std::size_t parameter_count = sizeof...(Args);
        using return_type = Ret;

        template <std::size_t N>
        using parameter = typename std::tuple_element<N, std::tuple<Args...>>::type;
    };
}
