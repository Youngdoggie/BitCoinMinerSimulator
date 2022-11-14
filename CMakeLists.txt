cmake_minimum_required(VERSION 3.11)
project(miner C)
set(CMAKE_C_STANDARD 99)
set(THREADS_PREFER_PTHREAD_FLAG ON)
add_executable(miner main.c main.h linked_list.c linked_list.h siggen.c siggen.h
        memopool.c memopool.h array_list.c array_list.h nonce.c nonce.h reader.c reader.h
        event.c event.h)

find_package(Threads REQUIRED)
target_link_libraries(miner PRIVATE Threads::Threads)