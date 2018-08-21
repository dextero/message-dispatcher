#pragma once
#include <vector>
#include "message_data.h"

namespace pk
{
    class message_token
    {
    public:
        #pragma region lifetime
        message_token(void) = default;

        message_token(bool scope_exit) : _scope_exit{scope_exit} {};
        message_token(pk::detail::message_data entry, bool scope_exit)
            : _data{entry}, _scope_exit{scope_exit}
        {};
        ~message_token();

        message_token(const message_token&) = delete;
        message_token& operator=(const message_token&) = delete;

        message_token(message_token&&) noexcept;
        message_token& operator=(message_token&&) noexcept;
        #pragma endregion

        #pragma region variables
        std::vector<pk::detail::message_data> _data;
        std::vector<void*> _filters;
        #pragma endregion

        #pragma region methods
        void clear();
        void unregister();

        void add_filter(void* sender);
        void clear_filters();
        #pragma endregion

        #pragma region operators
        message_token& operator+=(message_token&& other);
        #pragma endregion

    private:
        #pragma region variables
        bool _scope_exit = true;
        #pragma endregion

        #pragma region friends
        friend class message_dispatcher;
        #pragma endregion
    };
}
