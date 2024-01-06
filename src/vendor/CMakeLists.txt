target_sources(${PROJECT_NAME} PRIVATE
    xzDecompressor/xz/decode.cpp
    xzDecompressor/xz/encode.cpp
    )

add_subdirectory(11Zip)
