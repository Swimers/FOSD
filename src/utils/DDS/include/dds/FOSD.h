
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from FOSD.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Connext distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Connext manual.
*/

#ifndef FOSD_1276479568_h
#define FOSD_1276479568_h

#ifndef NDDS_STANDALONE_TYPE
    #ifdef __cplusplus
        #ifndef ndds_cpp_h
            #include "ndds/ndds_cpp.h"
        #endif
    #else
        #ifndef ndds_c_h
            #include "ndds/ndds_c.h"
        #endif
    #endif
#else
    #include "ndds_standalone_type.h"
#endif


namespace DWR_FOSD{

#define DWR_FOSD_NET_SEND_BUFF4_LAST_MEMBER_ID 4
        
extern const char *NET_SEND_BUFF4TYPENAME;
        


#ifdef __cplusplus
    struct NET_SEND_BUFF4Seq;

#ifndef NDDS_STANDALONE_TYPE
    class NET_SEND_BUFF4TypeSupport;
    class NET_SEND_BUFF4DataWriter;
    class NET_SEND_BUFF4DataReader;
#endif

#endif

            
    
class NET_SEND_BUFF4                                        
{
public:            
#ifdef __cplusplus
    typedef struct NET_SEND_BUFF4Seq Seq;

#ifndef NDDS_STANDALONE_TYPE
    typedef NET_SEND_BUFF4TypeSupport TypeSupport;
    typedef NET_SEND_BUFF4DataWriter DataWriter;
    typedef NET_SEND_BUFF4DataReader DataReader;
#endif

#endif
    
    DDS_Long  flag;

    DDS_Float  strian[12];

    DDS_Float  temper[12];

    DDS_Float  free[16];

    DDS_Octet  end_flag;

            
};                        
    
                            
#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, start exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport __declspec(dllexport)
#endif

    
NDDSUSERDllExport DDS_TypeCode* NET_SEND_BUFF4_get_typecode(void); /* Type code */
    

DDS_SEQUENCE(NET_SEND_BUFF4Seq, NET_SEND_BUFF4);
        
NDDSUSERDllExport
RTIBool NET_SEND_BUFF4_initialize(
        NET_SEND_BUFF4* self);
        
NDDSUSERDllExport
RTIBool NET_SEND_BUFF4_initialize_ex(
        NET_SEND_BUFF4* self,
        RTIBool allocatePointers,RTIBool allocateMemory);
        
NDDSUSERDllExport
RTIBool NET_SEND_BUFF4_initialize_w_params(
        NET_SEND_BUFF4* self,
        const struct DDS_TypeAllocationParams_t * allocParams);

NDDSUSERDllExport
void NET_SEND_BUFF4_finalize(
        NET_SEND_BUFF4* self);
                        
NDDSUSERDllExport
void NET_SEND_BUFF4_finalize_ex(
        NET_SEND_BUFF4* self,RTIBool deletePointers);
       
NDDSUSERDllExport
void NET_SEND_BUFF4_finalize_w_params(
        NET_SEND_BUFF4* self,
        const struct DDS_TypeDeallocationParams_t * deallocParams);
        
NDDSUSERDllExport
void NET_SEND_BUFF4_finalize_optional_members(
        NET_SEND_BUFF4* self, RTIBool deletePointers);        
        
NDDSUSERDllExport
RTIBool NET_SEND_BUFF4_copy(
        NET_SEND_BUFF4* dst,
        const NET_SEND_BUFF4* src);

#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
  /* If the code is building on Windows, stop exporting symbols.
   */
  #undef NDDSUSERDllExport
  #define NDDSUSERDllExport
#endif


} /* namespace DWR_FOSD */


#endif /* FOSD_1276479568_h */
