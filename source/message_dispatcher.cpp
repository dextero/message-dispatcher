#include "message_dispatcher.h"

#include <algorithm>

namespace pk
{
    #pragma region statics
    std::vector<message_dispatcher::message_data> message_dispatcher::bindings;
    #pragma endregion

    #pragma region helper methods
    void message_dispatcher::unbind_object(void* obj)
    {
        for(size_t i = 0; i < bindings.size();)
        {
            const auto& data = bindings[i];
            if(data.receiver == obj)
            {
                std::swap(bindings[i], bindings.back());
                bindings.pop_back();
                continue;
            }
            ++i;
        }
    }
    void message_dispatcher::unbind_function(void* obj)
    {
        for(size_t i = 0; i < bindings.size(); ++i)
        {
            const auto& data = bindings[i];
            if(data.functor_pointer == obj)
            {
                std::swap(bindings[i], bindings.back());
                bindings.pop_back();
                return;
            }
        }
    }
    void message_dispatcher::unbind(const message_data& data)
    {
        auto it = std::find(bindings.begin(), bindings.end(), data);
        if(it == bindings.end())
        {
            return;
        }

        std::swap(*it, bindings.back());
        bindings.pop_back();
    }

    message_dispatcher::message_data& message_dispatcher::find(const message_data& data)
    {
        auto element = std::find(bindings.begin(), bindings.end(), data);
        return *element;
    }
    #pragma endregion
}