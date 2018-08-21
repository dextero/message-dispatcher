cmake_minimum_required(VERSION 3.10)
project(message_dispatcher CXX)

add_library(message_dispatcher STATIC
            source/message_token.h
            source/message_dispatcher.h
            source/message_data.h
            source/type_info.h
            source/function_traits.h
            source/message_dispatcher.cpp
            source/message_token.cpp)
set_target_properties(message_dispatcher PROPERTIES CXX_STANDARD 17)

add_executable(sample
               samples/main.cpp)
target_link_libraries(sample PRIVATE message_dispatcher)
