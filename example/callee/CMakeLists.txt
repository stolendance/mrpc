#set(SRC_LIST userservice.cpp  ../user.pb.cc)
add_executable(provider userservice.cpp  ../user.pb.cc) 
find_package(Threads)
target_link_libraries(provider mrpc protobuf ${CMAKE_THREAD_LIBS_INIT})