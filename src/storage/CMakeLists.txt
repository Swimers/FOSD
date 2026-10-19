add_library(fosd_storage STATIC
    StorageTypes.h StorageTypes.cpp
    StorageRecords.h StorageRecords.cpp
    IStorageWriter.h IStorageWriter.cpp
    CsvWriter.h CsvWriter.cpp
    SQLiteWriter.h SQLiteWriter.cpp
    StorageControl.h StorageControl.cpp
)
target_include_directories(fosd_storage PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(fosd_storage PUBLIC fosd_core Qt5::Core Qt5::Sql)
target_compile_features(fosd_storage PUBLIC cxx_std_11)
