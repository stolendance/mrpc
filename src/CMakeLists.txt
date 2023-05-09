# 当前文件夹所有文件的名字 放入SRC_LIST
aux_source_directory(. SRC_LIST)
# 生成动态库 头文件 在顶层的cmakelist中 已经找到
add_library(mrpc SHARED ${SRC_LIST})