project("gryllidae_tracker")

aux_source_directory("include" INC_DIR)
aux_source_directory("include/video" INC_VIDEO_DIR)
aux_source_directory("include/detector" INC_DETECTOR_DIR)
aux_source_directory("include/tracker" INC_TRACKER_DIR)


aux_source_directory("src" SRC_DIR)
aux_source_directory("src/video" SRC_VIDEO_DIR)
aux_source_directory("src/detector" SRC_DETECTOR_DIR)
aux_source_directory("src/tracker" SRC_TRACKER_DIR)


# 将源代码添加到此项目的可执行文件。
add_library (${PROJECT_NAME} SHARED
        ${INC_DIR}
        ${INC_VIDEO_DIR}
        ${INC_DETECTOR_DIR}
        ${INC_TRACKER_DIR}

        ${SRC_DIR}
        ${SRC_VIDEO_DIR}
        ${SRC_DETECTOR_DIR}
        ${SRC_TRACKER_DIR}
	)

target_link_libraries(${PROJECT_NAME}
    ${THIRD_PARTY_LIBS})