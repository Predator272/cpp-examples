cmake_minimum_required(VERSION 3.22)

project("example")

add_executable(${PROJECT_NAME}
	"source/main.cpp"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
