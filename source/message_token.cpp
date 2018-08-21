#include "message_token.h"

namespace pk
{
    #pragma region lifetime
    message_token::~message_token()
    {
        if (_scope_exit) { unregister(); }
    }

    message_token::message_token(message_token&& other) noexcept
    {
        _scope_exit = other._scope_exit;
        _data = other._data;
        _filters = other._filters;

        other._scope_exit = false;
        other._data.clear();
        other._filters.clear();
    }
    message_token& message_token::operator=(message_token&& other) noexcept
    {
        if (_scope_exit) { unregister(); }

        _scope_exit = other._scope_exit;
        _data = other._data;
        _filters = other._filters;

        other._scope_exit = false;
        other._data.clear();
        other._filters.clear();

        return *this;
    }
    #pragma endregion

    #pragma region public methods
    void message_token::clear()
    {
        _data.clear();
    }
    void message_token::unregister()
    {
        for (const auto& entry : _data)
        {
            message_dispatcher::unbind(entry);
        }
        _data.clear();
    }
    void message_token::add_filter(void * sender)
    {
        _filters.push_back(sender);
    }
    void message_token::clear_filters()
    {
        _filters.clear();
    }
    #pragma endregion

    #pragma region operator overload
    message_token& message_token::operator+=(message_token&& other)
    {
        _scope_exit |= other._scope_exit;

        for (const auto& elem : other._data)
        {
            _data.emplace_back(elem);
        }

        other._scope_exit = false;
        other._filters.clear();
        other._data.clear();

        return *this;
    }
    #pragma endregion
}