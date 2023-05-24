cmake_minimum_required(VERSION 3.25)
project(Win32_multi_thread_programming)

set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 3.0)
project(MyProject)

# 计数器变量
set(PART_COUNTER 1)
set(EXAMPLE_COUNTER 1)

# 获取所有的part文件夹路径
file(GLOB PART_FOLDERS "src/part*")

foreach (PART_FOLDER ${PART_FOLDERS})
    # 遍历part文件夹下的所有chapter文件夹
    file(GLOB CHAPTER_FOLDERS "${PART_FOLDER}/chapter*")
    set(CHAPTER_COUNTER 1)

    foreach (CHAPTER_FOLDER ${CHAPTER_FOLDERS})
        # 获取chapter文件夹名称
        get_filename_component(CHAPTER_NAME ${CHAPTER_FOLDER} NAME)

        # 添加可执行文件
        file(GLOB CHAPTER_SOURCE_FILES "${CHAPTER_FOLDER}/*.c*")
        set(EXAMPLE_COUNTER 1)

        foreach (SOURCE_FILE ${CHAPTER_SOURCE_FILES})
            # 获取文件名（不包括路径和后缀）
            get_filename_component(FILE_NAME ${SOURCE_FILE} NAME_WE)

            # 生成目标名称
            set(TARGET_NAME "${PART_COUNTER}_${CHAPTER_COUNTER}_example${EXAMPLE_COUNTER}")

            # 添加可执行文件
            add_executable(${TARGET_NAME} ${SOURCE_FILE})

            math(EXPR EXAMPLE_COUNTER "${EXAMPLE_COUNTER} + 1")
        endforeach ()

        math(EXPR CHAPTER_COUNTER "${CHAPTER_COUNTER} + 1")
    endforeach ()

    math(EXPR PART_COUNTER "${PART_COUNTER} + 1")
endforeach ()
