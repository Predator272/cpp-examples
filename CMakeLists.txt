cmake_minimum_required(VERSION 3.25)

project("example")

add_executable(${PROJECT_NAME}
	"source/main.cpp"
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
