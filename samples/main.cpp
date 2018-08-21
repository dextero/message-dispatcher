#include <iostream>
#include "../source/message_token.h"
#include "../source/message_dispatcher.h"

/* messages */
int value_message{ 3 };
struct object_message{};
int function_message() { return 13; }

/* receivers */
namespace functions
{
    /* functions */
    void value(int message)
    {
        std::cout << "function received value_message: " << message << '\n';
    }

    void object(const object_message& message)
    {
        std::cout << "function received object_message\n";
    }

    void function(int(*message)(void))
    {
        std::cout << "function received function_message: " << message() << '\n';
    }
}

namespace methods
{
    struct receiver
    {
        void method(int message)
        {
            std::cout << "method received value_message\n";
        }
        static void static_method(const object_message& message)
        {
            std::cout << "static method received object_message\n";
        }
    };
}

int main() 
{
    /* simple */
    {
        /* bind functions */
        pk::message_dispatcher::bind(functions::value);
        pk::message_dispatcher::bind(functions::object);
        pk::message_dispatcher::bind(functions::function);

        /* bind methods */
        methods::receiver receiver{};
        pk::message_dispatcher::bind(&methods::receiver::method, receiver);
        pk::message_dispatcher::bind(&methods::receiver::static_method);

        /* bind lambdas */
        auto lambda = [](int message) { std::cout << "lambda received value_message\n"; };
        pk::message_dispatcher::bind(lambda);

        /* dispatch messages */
        std::cout << "\n== dispatch to all receivers ==\n\n";
        pk::message_dispatcher::signal(value_message);
        pk::message_dispatcher::signal(object_message{});
        pk::message_dispatcher::signal(&function_message);

        /* unbind and try to send again */
        std::cout << "\n== unbind receivers ==\n\n";
        pk::message_dispatcher::unbind(functions::value);
        pk::message_dispatcher::unbind(functions::object);
        pk::message_dispatcher::unbind(functions::function);

        pk::message_dispatcher::unbind(&methods::receiver::method, receiver);
        pk::message_dispatcher::unbind(&methods::receiver::static_method);
        pk::message_dispatcher::unbind(lambda);

        pk::message_dispatcher::signal(&value_message);
    }

    /* with tokens */
    {
        {
            auto token = pk::message_token{ true };
            token += pk::message_dispatcher::bind(functions::value);
            token += pk::message_dispatcher::bind(&methods::receiver::static_method);

            /* unregister token binds at scope exit */
        }
        std::cout << "\n== signal value receivers ==\n\n";
        pk::message_dispatcher::signal(value_message);
    }

    system("pause");
    return 0;
}