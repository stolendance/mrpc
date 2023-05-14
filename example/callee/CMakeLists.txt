#set(SRC_LIST userservice.cpp  ../user.pb.cc)
add_executable(provider userservice.cpp  ../user.pb.cc) 
target_link_libraries(provider mrpc protobuf)