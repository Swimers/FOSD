#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "vtksys" for configuration "Release"
set_property(TARGET vtksys APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtksys PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtksys-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtksys-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtksys )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtksys "${_IMPORT_PREFIX}/lib/libvtksys-8.2.so.1" )

# Import target "vtkCommonCore" for configuration "Release"
set_property(TARGET vtkCommonCore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCommonCore PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCommonCore-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkCommonCore-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkCommonCore )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkCommonCore "${_IMPORT_PREFIX}/lib/libvtkCommonCore-8.2.so.1" )

# Import target "vtkCommonMath" for configuration "Release"
set_property(TARGET vtkCommonMath APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCommonMath PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCommonMath-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkCommonMath-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkCommonMath )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkCommonMath "${_IMPORT_PREFIX}/lib/libvtkCommonMath-8.2.so.1" )

# Import target "vtkCommonMisc" for configuration "Release"
set_property(TARGET vtkCommonMisc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCommonMisc PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCommonMisc-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkCommonMisc-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkCommonMisc )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkCommonMisc "${_IMPORT_PREFIX}/lib/libvtkCommonMisc-8.2.so.1" )

# Import target "vtkCommonSystem" for configuration "Release"
set_property(TARGET vtkCommonSystem APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCommonSystem PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCommonSystem-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkCommonSystem-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkCommonSystem )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkCommonSystem "${_IMPORT_PREFIX}/lib/libvtkCommonSystem-8.2.so.1" )

# Import target "vtkCommonTransforms" for configuration "Release"
set_property(TARGET vtkCommonTransforms APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCommonTransforms PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCommonTransforms-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkCommonTransforms-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkCommonTransforms )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkCommonTransforms "${_IMPORT_PREFIX}/lib/libvtkCommonTransforms-8.2.so.1" )

# Import target "vtkCommonDataModel" for configuration "Release"
set_property(TARGET vtkCommonDataModel APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCommonDataModel PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonMisc;vtkCommonSystem;vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCommonDataModel-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkCommonDataModel-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkCommonDataModel )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkCommonDataModel "${_IMPORT_PREFIX}/lib/libvtkCommonDataModel-8.2.so.1" )

# Import target "vtkCommonColor" for configuration "Release"
set_property(TARGET vtkCommonColor APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCommonColor PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCommonColor-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkCommonColor-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkCommonColor )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkCommonColor "${_IMPORT_PREFIX}/lib/libvtkCommonColor-8.2.so.1" )

# Import target "vtkCommonComputationalGeometry" for configuration "Release"
set_property(TARGET vtkCommonComputationalGeometry APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCommonComputationalGeometry PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCommonComputationalGeometry-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkCommonComputationalGeometry-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkCommonComputationalGeometry )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkCommonComputationalGeometry "${_IMPORT_PREFIX}/lib/libvtkCommonComputationalGeometry-8.2.so.1" )

# Import target "vtkCommonExecutionModel" for configuration "Release"
set_property(TARGET vtkCommonExecutionModel APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCommonExecutionModel PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonMisc;vtkCommonSystem"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCommonExecutionModel-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkCommonExecutionModel-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkCommonExecutionModel )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkCommonExecutionModel "${_IMPORT_PREFIX}/lib/libvtkCommonExecutionModel-8.2.so.1" )

# Import target "vtkDICOMParser" for configuration "Release"
set_property(TARGET vtkDICOMParser APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkDICOMParser PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkDICOMParser-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkDICOMParser-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkDICOMParser )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkDICOMParser "${_IMPORT_PREFIX}/lib/libvtkDICOMParser-8.2.so.1" )

# Import target "vtkFiltersCore" for configuration "Release"
set_property(TARGET vtkFiltersCore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkFiltersCore PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonMath;vtkCommonSystem;vtkCommonTransforms"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkFiltersCore-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkFiltersCore-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkFiltersCore )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkFiltersCore "${_IMPORT_PREFIX}/lib/libvtkFiltersCore-8.2.so.1" )

# Import target "vtkFiltersGeneral" for configuration "Release"
set_property(TARGET vtkFiltersGeneral APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkFiltersGeneral PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonComputationalGeometry;vtkCommonMath;vtkCommonSystem;vtkCommonTransforms"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkFiltersGeneral-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkFiltersGeneral-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkFiltersGeneral )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkFiltersGeneral "${_IMPORT_PREFIX}/lib/libvtkFiltersGeneral-8.2.so.1" )

# Import target "vtkImagingCore" for configuration "Release"
set_property(TARGET vtkImagingCore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkImagingCore PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonMath;vtkCommonTransforms"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkImagingCore-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkImagingCore-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkImagingCore )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkImagingCore "${_IMPORT_PREFIX}/lib/libvtkImagingCore-8.2.so.1" )

# Import target "vtkImagingFourier" for configuration "Release"
set_property(TARGET vtkImagingFourier APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkImagingFourier PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonDataModel;vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkImagingFourier-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkImagingFourier-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkImagingFourier )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkImagingFourier "${_IMPORT_PREFIX}/lib/libvtkImagingFourier-8.2.so.1" )

# Import target "vtkFiltersStatistics" for configuration "Release"
set_property(TARGET vtkFiltersStatistics APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkFiltersStatistics PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonDataModel;vtkCommonMisc;vtkImagingFourier"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkFiltersStatistics-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkFiltersStatistics-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkFiltersStatistics )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkFiltersStatistics "${_IMPORT_PREFIX}/lib/libvtkFiltersStatistics-8.2.so.1" )

# Import target "vtkFiltersExtraction" for configuration "Release"
set_property(TARGET vtkFiltersExtraction APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkFiltersExtraction PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonCore;vtkCommonDataModel;vtkFiltersCore;vtkFiltersStatistics"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkFiltersExtraction-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkFiltersExtraction-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkFiltersExtraction )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkFiltersExtraction "${_IMPORT_PREFIX}/lib/libvtkFiltersExtraction-8.2.so.1" )

# Import target "vtkFiltersGeometry" for configuration "Release"
set_property(TARGET vtkFiltersGeometry APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkFiltersGeometry PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkFiltersCore"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkFiltersGeometry-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkFiltersGeometry-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkFiltersGeometry )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkFiltersGeometry "${_IMPORT_PREFIX}/lib/libvtkFiltersGeometry-8.2.so.1" )

# Import target "vtkImagingSources" for configuration "Release"
set_property(TARGET vtkImagingSources APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkImagingSources PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonCore;vtkCommonDataModel;vtkImagingCore"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkImagingSources-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkImagingSources-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkImagingSources )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkImagingSources "${_IMPORT_PREFIX}/lib/libvtkImagingSources-8.2.so.1" )

# Import target "vtkFiltersSources" for configuration "Release"
set_property(TARGET vtkFiltersSources APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkFiltersSources PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonComputationalGeometry;vtkCommonCore;vtkCommonTransforms;vtkFiltersCore;vtkFiltersGeneral"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkFiltersSources-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkFiltersSources-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkFiltersSources )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkFiltersSources "${_IMPORT_PREFIX}/lib/libvtkFiltersSources-8.2.so.1" )

# Import target "vtkRenderingCore" for configuration "Release"
set_property(TARGET vtkRenderingCore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkRenderingCore PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonColor;vtkCommonComputationalGeometry;vtkCommonSystem;vtkCommonTransforms;vtkFiltersGeneral;vtkFiltersGeometry;vtkFiltersSources;vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkRenderingCore-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkRenderingCore-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkRenderingCore )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkRenderingCore "${_IMPORT_PREFIX}/lib/libvtkRenderingCore-8.2.so.1" )

# Import target "vtkFiltersHybrid" for configuration "Release"
set_property(TARGET vtkFiltersHybrid APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkFiltersHybrid PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonMath;vtkCommonMisc;vtkFiltersCore;vtkFiltersGeneral;vtkFiltersGeometry;vtkImagingCore;vtkImagingSources;vtkRenderingCore;vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkFiltersHybrid-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkFiltersHybrid-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkFiltersHybrid )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkFiltersHybrid "${_IMPORT_PREFIX}/lib/libvtkFiltersHybrid-8.2.so.1" )

# Import target "vtkFiltersModeling" for configuration "Release"
set_property(TARGET vtkFiltersModeling APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkFiltersModeling PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonCore;vtkCommonDataModel;vtkCommonTransforms;vtkFiltersCore;vtkFiltersSources"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkFiltersModeling-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkFiltersModeling-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkFiltersModeling )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkFiltersModeling "${_IMPORT_PREFIX}/lib/libvtkFiltersModeling-8.2.so.1" )

# Import target "vtkFiltersTexture" for configuration "Release"
set_property(TARGET vtkFiltersTexture APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkFiltersTexture PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonCore;vtkCommonDataModel;vtkCommonTransforms;vtkFiltersGeneral"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkFiltersTexture-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkFiltersTexture-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkFiltersTexture )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkFiltersTexture "${_IMPORT_PREFIX}/lib/libvtkFiltersTexture-8.2.so.1" )

# Import target "vtkInteractionStyle" for configuration "Release"
set_property(TARGET vtkInteractionStyle APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkInteractionStyle PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonCore;vtkCommonMath;vtkCommonTransforms;vtkFiltersExtraction;vtkFiltersSources"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkInteractionStyle-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkInteractionStyle-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkInteractionStyle )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkInteractionStyle "${_IMPORT_PREFIX}/lib/libvtkInteractionStyle-8.2.so.1" )

# Import target "vtkglew" for configuration "Release"
set_property(TARGET vtkglew APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkglew PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkglew-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkglew-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkglew )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkglew "${_IMPORT_PREFIX}/lib/libvtkglew-8.2.so.1" )

# Import target "vtkRenderingOpenGL2" for configuration "Release"
set_property(TARGET vtkRenderingOpenGL2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkRenderingOpenGL2 PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonExecutionModel;vtkCommonMath;vtkCommonSystem;vtkCommonTransforms;vtkglew;vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkRenderingOpenGL2-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkRenderingOpenGL2-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkRenderingOpenGL2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkRenderingOpenGL2 "${_IMPORT_PREFIX}/lib/libvtkRenderingOpenGL2-8.2.so.1" )

# Import target "vtkGUISupportQt" for configuration "Release"
set_property(TARGET vtkGUISupportQt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkGUISupportQt PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonDataModel;vtkFiltersExtraction;vtkInteractionStyle"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkGUISupportQt-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkGUISupportQt-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkGUISupportQt )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkGUISupportQt "${_IMPORT_PREFIX}/lib/libvtkGUISupportQt-8.2.so.1" )

# Import target "vtkdoubleconversion" for configuration "Release"
set_property(TARGET vtkdoubleconversion APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkdoubleconversion PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkdoubleconversion-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkdoubleconversion-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkdoubleconversion )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkdoubleconversion "${_IMPORT_PREFIX}/lib/libvtkdoubleconversion-8.2.so.1" )

# Import target "vtklz4" for configuration "Release"
set_property(TARGET vtklz4 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtklz4 PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtklz4-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtklz4-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtklz4 )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtklz4 "${_IMPORT_PREFIX}/lib/libvtklz4-8.2.so.1" )

# Import target "vtklzma" for configuration "Release"
set_property(TARGET vtklzma APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtklzma PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtklzma-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtklzma-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtklzma )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtklzma "${_IMPORT_PREFIX}/lib/libvtklzma-8.2.so.1" )

# Import target "vtkzlib" for configuration "Release"
set_property(TARGET vtkzlib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkzlib PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkzlib-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkzlib-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkzlib )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkzlib "${_IMPORT_PREFIX}/lib/libvtkzlib-8.2.so.1" )

# Import target "vtkIOCore" for configuration "Release"
set_property(TARGET vtkIOCore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkIOCore PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonDataModel;vtkCommonMisc;vtklz4;vtklzma;vtksys;vtkzlib;vtkdoubleconversion"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkIOCore-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkIOCore-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkIOCore )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkIOCore "${_IMPORT_PREFIX}/lib/libvtkIOCore-8.2.so.1" )

# Import target "vtkmetaio" for configuration "Release"
set_property(TARGET vtkmetaio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkmetaio PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkmetaio-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkmetaio-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkmetaio )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkmetaio "${_IMPORT_PREFIX}/lib/libvtkmetaio-8.2.so.1" )

# Import target "vtkjpeg" for configuration "Release"
set_property(TARGET vtkjpeg APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkjpeg PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkjpeg-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkjpeg-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkjpeg )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkjpeg "${_IMPORT_PREFIX}/lib/libvtkjpeg-8.2.so.1" )

# Import target "vtkpng" for configuration "Release"
set_property(TARGET vtkpng APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkpng PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkzlib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkpng-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkpng-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkpng )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkpng "${_IMPORT_PREFIX}/lib/libvtkpng-8.2.so.1" )

# Import target "vtktiff" for configuration "Release"
set_property(TARGET vtktiff APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtktiff PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkjpeg;vtkzlib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtktiff-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtktiff-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtktiff )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtktiff "${_IMPORT_PREFIX}/lib/libvtktiff-8.2.so.1" )

# Import target "vtkIOImage" for configuration "Release"
set_property(TARGET vtkIOImage APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkIOImage PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonDataModel;vtkCommonMath;vtkCommonMisc;vtkCommonSystem;vtkCommonTransforms;vtkDICOMParser;vtkmetaio;vtkjpeg;vtkpng;vtksys;vtktiff;vtkzlib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkIOImage-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkIOImage-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkIOImage )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkIOImage "${_IMPORT_PREFIX}/lib/libvtkIOImage-8.2.so.1" )

# Import target "vtkIOLegacy" for configuration "Release"
set_property(TARGET vtkIOLegacy APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkIOLegacy PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonMisc;vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkIOLegacy-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkIOLegacy-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkIOLegacy )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkIOLegacy "${_IMPORT_PREFIX}/lib/libvtkIOLegacy-8.2.so.1" )

# Import target "vtkIOGeometry" for configuration "Release"
set_property(TARGET vtkIOGeometry APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkIOGeometry PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonCore;vtkCommonMisc;vtkCommonSystem;vtkCommonTransforms;vtkIOImage;vtksys;vtkzlib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkIOGeometry-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkIOGeometry-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkIOGeometry )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkIOGeometry "${_IMPORT_PREFIX}/lib/libvtkIOGeometry-8.2.so.1" )

# Import target "vtkexpat" for configuration "Release"
set_property(TARGET vtkexpat APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkexpat PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkexpat-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkexpat-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkexpat )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkexpat "${_IMPORT_PREFIX}/lib/libvtkexpat-8.2.so.1" )

# Import target "vtkIOXMLParser" for configuration "Release"
set_property(TARGET vtkIOXMLParser APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkIOXMLParser PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkIOCore;vtkexpat;vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkIOXMLParser-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkIOXMLParser-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkIOXMLParser )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkIOXMLParser "${_IMPORT_PREFIX}/lib/libvtkIOXMLParser-8.2.so.1" )

# Import target "vtkIOXML" for configuration "Release"
set_property(TARGET vtkIOXML APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkIOXML PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonDataModel;vtkCommonMisc;vtkCommonSystem;vtkIOCore;vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkIOXML-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkIOXML-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkIOXML )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkIOXML "${_IMPORT_PREFIX}/lib/libvtkIOXML-8.2.so.1" )

# Import target "vtkImagingColor" for configuration "Release"
set_property(TARGET vtkImagingColor APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkImagingColor PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonCore;vtkCommonDataModel;vtkCommonSystem"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkImagingColor-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkImagingColor-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkImagingColor )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkImagingColor "${_IMPORT_PREFIX}/lib/libvtkImagingColor-8.2.so.1" )

# Import target "vtkImagingGeneral" for configuration "Release"
set_property(TARGET vtkImagingGeneral APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkImagingGeneral PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonCore;vtkCommonDataModel;vtkImagingSources"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkImagingGeneral-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkImagingGeneral-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkImagingGeneral )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkImagingGeneral "${_IMPORT_PREFIX}/lib/libvtkImagingGeneral-8.2.so.1" )

# Import target "vtkImagingHybrid" for configuration "Release"
set_property(TARGET vtkImagingHybrid APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkImagingHybrid PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonDataModel;vtkIOImage;vtkImagingCore"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkImagingHybrid-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkImagingHybrid-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkImagingHybrid )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkImagingHybrid "${_IMPORT_PREFIX}/lib/libvtkImagingHybrid-8.2.so.1" )

# Import target "vtkImagingMath" for configuration "Release"
set_property(TARGET vtkImagingMath APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkImagingMath PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonCore;vtkCommonDataModel"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkImagingMath-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkImagingMath-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkImagingMath )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkImagingMath "${_IMPORT_PREFIX}/lib/libvtkImagingMath-8.2.so.1" )

# Import target "vtkfreetype" for configuration "Release"
set_property(TARGET vtkfreetype APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkfreetype PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkfreetype-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkfreetype-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkfreetype )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkfreetype "${_IMPORT_PREFIX}/lib/libvtkfreetype-8.2.so.1" )

# Import target "vtkRenderingFreeType" for configuration "Release"
set_property(TARGET vtkRenderingFreeType APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkRenderingFreeType PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonDataModel;vtkFiltersGeneral"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkRenderingFreeType-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkRenderingFreeType-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkRenderingFreeType )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkRenderingFreeType "${_IMPORT_PREFIX}/lib/libvtkRenderingFreeType-8.2.so.1" )

# Import target "vtkRenderingAnnotation" for configuration "Release"
set_property(TARGET vtkRenderingAnnotation APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkRenderingAnnotation PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonMath;vtkCommonTransforms;vtkFiltersCore;vtkFiltersGeneral;vtkFiltersSources;vtkImagingColor;vtkRenderingFreeType"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkRenderingAnnotation-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkRenderingAnnotation-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkRenderingAnnotation )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkRenderingAnnotation "${_IMPORT_PREFIX}/lib/libvtkRenderingAnnotation-8.2.so.1" )

# Import target "vtkRenderingVolume" for configuration "Release"
set_property(TARGET vtkRenderingVolume APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkRenderingVolume PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonDataModel;vtkCommonMath;vtkCommonMisc;vtkCommonSystem;vtkCommonTransforms;vtkIOXML;vtkImagingCore"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkRenderingVolume-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkRenderingVolume-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkRenderingVolume )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkRenderingVolume "${_IMPORT_PREFIX}/lib/libvtkRenderingVolume-8.2.so.1" )

# Import target "vtkInteractionWidgets" for configuration "Release"
set_property(TARGET vtkInteractionWidgets APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkInteractionWidgets PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonComputationalGeometry;vtkCommonDataModel;vtkCommonMath;vtkCommonSystem;vtkCommonTransforms;vtkFiltersCore;vtkFiltersHybrid;vtkFiltersModeling;vtkImagingColor;vtkImagingCore;vtkImagingGeneral;vtkImagingHybrid;vtkInteractionStyle;vtkRenderingAnnotation;vtkRenderingFreeType;vtkRenderingVolume"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkInteractionWidgets-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkInteractionWidgets-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkInteractionWidgets )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkInteractionWidgets "${_IMPORT_PREFIX}/lib/libvtkInteractionWidgets-8.2.so.1" )

# Import target "vtkRenderingLabel" for configuration "Release"
set_property(TARGET vtkRenderingLabel APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkRenderingLabel PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonMath;vtkCommonSystem;vtkCommonTransforms;vtkFiltersGeneral"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkRenderingLabel-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkRenderingLabel-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkRenderingLabel )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkRenderingLabel "${_IMPORT_PREFIX}/lib/libvtkRenderingLabel-8.2.so.1" )

# Import target "vtkRenderingQt" for configuration "Release"
set_property(TARGET vtkRenderingQt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkRenderingQt PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonDataModel;vtkCommonSystem;vtkFiltersSources;vtkFiltersTexture;vtkGUISupportQt;Qt5::Widgets"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkRenderingQt-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkRenderingQt-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkRenderingQt )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkRenderingQt "${_IMPORT_PREFIX}/lib/libvtkRenderingQt-8.2.so.1" )

# Import target "vtkRenderingVolumeOpenGL2" for configuration "Release"
set_property(TARGET vtkRenderingVolumeOpenGL2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkRenderingVolumeOpenGL2 PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "vtkCommonMath;vtkCommonSystem;vtkCommonTransforms;vtkFiltersCore;vtkFiltersGeneral;vtkFiltersSources;vtksys"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libvtkRenderingVolumeOpenGL2-8.2.so.1"
  IMPORTED_SONAME_RELEASE "libvtkRenderingVolumeOpenGL2-8.2.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS vtkRenderingVolumeOpenGL2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_vtkRenderingVolumeOpenGL2 "${_IMPORT_PREFIX}/lib/libvtkRenderingVolumeOpenGL2-8.2.so.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
