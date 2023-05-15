#set(SRC_LIST userservice.cpp  ../user.pb.cc)
add_executable(comsumer callerservice.cpp  ../user.pb.cc) 
target_link_libraries(comsumer mrpc protobuf)