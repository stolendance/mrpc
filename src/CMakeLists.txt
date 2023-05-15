# 当前文件夹所有文件的名字 放入SRC_LIST
#aux_source_directory(. SRC_LIST)
set(SRC_LIST rpc_header.pb.cc RpcProvider.cpp MrpcApplication.cpp MrpcConfig.cpp MrpcChannel.cpp)
# 生成动态库 头文件 在顶层的cmakelist中 已经找到
add_library(mrpc ${SRC_LIST})
target_link_libraries(mrpc muduo_net muduo_base pthread)