add_library(fosd_calculation STATIC
    IFrameIFE.h
    CalculationControl.cpp
    CalculationControl.h
    StrainCalibration.cpp
    StrainCalibration.h
    HYYIFE.cpp
    HYYIFE.h
    PlateIFE.h PlateIFE.cpp
)

# Consumers include these headers as "calculation/CalculationControl.h", so the
# public include root is `src/`, not this directory.
target_include_directories(fosd_calculation
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

target_link_libraries(fosd_calculation
    PUBLIC
        Qt5::Core
        fosd_core
        fosd_storage
)

target_compile_features(fosd_calculation PUBLIC cxx_std_11)
