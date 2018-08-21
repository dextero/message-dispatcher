#pragma once
#include <type_traits>
#include <vector>
#include <functional>

#include "function_traits.h"
#include "type_info.h"
#include "message_token.h"
#include "message_data.h"

namespace pk
{
    class message_dispatcher
    {
        using message_data = ::pk::detail::message_data;

    public:
        #pragma region methods
        template <typename F, typename T>
        static message_token bind(const F& method, T* obj);

        template <typename F, typename T>
        static message_token bind(const F& method, T& obj);

        template <typename F>
        static message_token bind(F& function);

        template <typename F>
        static message_token bind(const F& function);

        template <typename F, typename T>
        static void unbind(const F& method, T* obj);

        template <typename F, typename T>
        static void unbind(const F& method, T& obj);

        template <typename T>
        static void unbind(T* obj);

        template <typename T>
        static void unbind(T& obj);

        static void unbind(const message_data& data);

        template <typename T>
        static void signal(T&& message, void* sender = nullptr);
        #pragma endregion

    private:
        #pragma region variables
        static std::vector<message_data> bindings;
        #pragma endregion

        #pragma region helper methods
        static void unbind_function(void* obj);
        static void unbind_object(void* obj);

        static message_data& find(const message_data& data);
        #pragma endregion

        #pragma region friends
        friend class message_token;
        #pragma endregion
    };

    #pragma region template implementation

    #pragma region bind
    template <typename F, typename T>
    message_token message_dispatcher::bind(const F& method, T* obj)
    {
        using message_type = std::decay_t<typename pk::function_traits<F>::template parameter<0>>;

        const auto lambda = [=](void* object){
            const auto& message = *reinterpret_cast<const message_type*>(object);
            (obj->*method)(message); };

        message_data data;
        data.method = lambda;
        data.message_type = pk::type_info<message_type>::type;
        data.receiver = obj;
        data.functor_pointer = (void*&)method;

        bindings.emplace_back(std::move(data));
        return message_token{bindings.back(), false};
    }

    template <typename F, typename T>
    message_token message_dispatcher::bind(const F& method, T& obj)
    {
        return bind(method, &obj);
    }

    template <typename F>
    message_token message_dispatcher::bind(F& function)
    {
        using message_type = std::decay_t<typename pk::function_traits<F>::template parameter<0>>;

        const auto lambda = [=](const void* object){
            const auto& message = *reinterpret_cast<const message_type*>(object);
            function(message); };

        message_data data;
        data.method = lambda;
        data.message_type = pk::type_info<message_type>::type;
        data.receiver = &function;
        data.functor_pointer = (void*&)function;

        bindings.emplace_back(std::move(data));
        return message_token{bindings.back(), false};
    }

    template <typename F>
    message_token message_dispatcher::bind(const F& function)
    {
        using message_type = std::decay_t<typename pk::function_traits<F>::template parameter<0>>;

        const auto lambda = [=](const void* object){
            const auto& message = *reinterpret_cast<const message_type*>(object);
            function(message); };

        message_data data;
        data.method = lambda;
        data.message_type = pk::type_info<message_type>::type;
        data.receiver = nullptr;
        data.functor_pointer = (void*&)function;

        bindings.emplace_back(std::move(data));
        return message_token{bindings.back(), false};
    }
    #pragma endregion

    #pragma region unbind
    template <typename F, typename T>
    void message_dispatcher::unbind(const F& method, T* obj)
    {
        for(size_t i = 0; i < bindings.size();)
        {
            const auto& data = bindings[i];
            if(data.receiver == obj && data.functor_pointer == (void*&)(method))
            {
                std::swap(bindings[i], bindings.back());
                bindings.pop_back();
                continue;
            }
            ++i;
        }
    }

    template <typename F, typename T>
    void message_dispatcher::unbind(const F& method, T& obj)
    {
        unbind(method, &obj);
    }

    template <typename T>
    void message_dispatcher::unbind(T* obj)
    {
        if constexpr (std::is_function_v<T>)
        {
            static_assert(sizeof(T*) <= sizeof(uintptr_t),
                          "unsupported function pointer size");
            unbind_function(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(obj)));
        }
        else
        {
            unbind_object(obj);
        }
       
    }

    template <typename T>
    void message_dispatcher::unbind(T& obj)
    {
        unbind_object(&obj);
    }
    #pragma endregion

    #pragma region signal
    template <typename T>
    void message_dispatcher::signal(T&& message, void* sender /* = nullptr */)
    {
        for(size_t i = 0; i < bindings.size(); ++i)
        {
            const auto& bind = bindings[i];
            const auto& message_type = pk::type_info<T>::type;

            if(bind.message_type != message_type)
            {
                continue;
            }

            bind.method(static_cast<void*>(&message));
        }
    }
    #pragma endregion

    #pragma endregion
}
