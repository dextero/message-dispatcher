#pragma once

namespace pk
{
    class type
    {
    public:
        #pragma region lifetime
        type(void) = default;
        explicit type(const char* data) : _name{ data } {};
        #pragma endregion

        #pragma region operators
        bool operator==(const type& other) const
        {
            return _name == other._name;
        }
        bool operator!=(const type& other) const
        {
            return _name != other._name;
        }
        bool operator< (const type& other) const
        {
            return _name < other._name;
        }
        #pragma endregion

        #pragma region methods
        const char* name() const
        {
            return _name;
        }
        #pragma endregion

    private:
        #pragma region variables
        const char* _name;
        #pragma endregion
    };

    template <typename T> struct type_info
    {
        #pragma region variables
        static type type;
        #pragma endregion

        #pragma region methods
        static const char* name()
        {
#if defined(__FUNCSIG__)
            return __FUNCSIG__;
#elif defined(__GNUC__)
            return __PRETTY_FUNCTION__;
#else
#error
#endif
        };
        #pragma endregion
    };

    template <typename T> type type_info<T>::type(name());

    template <typename T> struct type_info<T&>               : type_info<T> {};
    template <typename T> struct type_info<T const>          : type_info<T> {};
    template <typename T> struct type_info<T volatile>       : type_info<T> {};
    template <typename T> struct type_info<T const volatile> : type_info<T> {};
}
