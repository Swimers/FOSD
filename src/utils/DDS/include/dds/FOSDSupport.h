
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from FOSD.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Connext distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Connext manual.
*/

#ifndef FOSDSupport_1276479568_h
#define FOSDSupport_1276479568_h

/* Uses */
#include "FOSD.h"



#ifdef __cplusplus
#ifndef ndds_cpp_h
  #include "ndds/ndds_cpp.h"
#endif
#else
#ifndef ndds_c_h
  #include "ndds/ndds_c.h"
#endif
#endif


namespace DWR_FOSD{
        

/* ========================================================================= */
/**
   Uses:     T

   Defines:  TTypeSupport, TDataWriter, TDataReader

   Organized using the well-documented "Generics Pattern" for
   implementing generics in C and C++.
*/

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, start exporting symbols.
  */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport __declspec(dllexport)
#endif

#ifdef __cplusplus

DDS_TYPESUPPORT_CPP(NET_SEND_BUFF4TypeSupport, NET_SEND_BUFF4);

DDS_DATAWRITER_CPP(NET_SEND_BUFF4DataWriter, NET_SEND_BUFF4);
DDS_DATAREADER_CPP(NET_SEND_BUFF4DataReader, NET_SEND_BUFF4Seq, NET_SEND_BUFF4);


#else

DDS_TYPESUPPORT_C(NET_SEND_BUFF4TypeSupport, NET_SEND_BUFF4);
DDS_DATAWRITER_C(NET_SEND_BUFF4DataWriter, NET_SEND_BUFF4);
DDS_DATAREADER_C(NET_SEND_BUFF4DataReader, NET_SEND_BUFF4Seq, NET_SEND_BUFF4);

#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, stop exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport
#endif


} /* namespace DWR_FOSD */


#endif  /* FOSDSupport_1276479568_h */
