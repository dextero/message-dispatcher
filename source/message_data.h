#pragma once

#include <functional>

#include "type_info.h"

namespace pk
{
    struct message_token;

    namespace detail
    {
        struct message_data
        {
            #pragma region Operator overloads
            bool operator==(const message_data& other) const noexcept
            {
                return message_type == other.message_type &&
                    functor_pointer == other.functor_pointer &&
                    receiver == other.receiver;
            }
            bool operator==(message_data&& other) const noexcept
            {
                return message_type == other.message_type &&
                    functor_pointer == other.functor_pointer &&
                    receiver == other.receiver;
            }
            #pragma endregion

            #pragma region variables
            std::function<void(void*)> method;
            pk::type message_type;
            void* functor_pointer;
            void* receiver;
            message_token* token = nullptr;
            #pragma endregion
        };
    }
}
