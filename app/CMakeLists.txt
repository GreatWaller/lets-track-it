project("tracking")

include_directories("include")

add_executable(${PROJECT_NAME} 
                "main.cpp"
                "src/Application.cpp"
                "src/Config.cpp"
                )
target_link_libraries(${PROJECT_NAME} gryllidae_tracker ${THIRD_PARTY_LIBS})