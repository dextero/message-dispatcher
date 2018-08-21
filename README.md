# message-dispatcher

C++ generic message dispatcher. 
Message receivers are determined by the type of the message sent.

## supports binding
- [x] functions
- [x] methods
- [x] static methods
- [x] lambdas
- [ ] const methods :<

## note
- type_info.h uses VS only \_\_FUNCSIG__ makro

## example

```cpp
void receive_value(int message)
{
    std::cout << "function received value_message: " << message << '\n';
}

int main() 
{ 
    pk::message_dispatcher::bind(receive_value);
    pk::message_dispatcher::signal(4);
    pk::message_dispatcher::unbind(receive_value);
}
```
\
