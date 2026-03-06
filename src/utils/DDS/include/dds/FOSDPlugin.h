
/*
  WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

  This file was generated from FOSD.idl using "rtiddsgen".
  The rtiddsgen tool is part of the RTI Connext distribution.
  For more information, type 'rtiddsgen -help' at a command shell
  or consult the RTI Connext manual.
*/

#ifndef FOSDPlugin_1276479568_h
#define FOSDPlugin_1276479568_h

#include "FOSD.h"



struct RTICdrStream;

#ifndef pres_typePlugin_h
#include "pres/pres_typePlugin.h"
#endif


#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif


namespace DWR_FOSD{


#define NET_SEND_BUFF4Plugin_get_sample PRESTypePluginDefaultEndpointData_getSample  
#define NET_SEND_BUFF4Plugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define NET_SEND_BUFF4Plugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer 
 

#define NET_SEND_BUFF4Plugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define NET_SEND_BUFF4Plugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
    Support functions:
 * -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern NET_SEND_BUFF4*
NET_SEND_BUFF4PluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern NET_SEND_BUFF4*
NET_SEND_BUFF4PluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern NET_SEND_BUFF4*
NET_SEND_BUFF4PluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
NET_SEND_BUFF4PluginSupport_copy_data(
    NET_SEND_BUFF4 *out,
    const NET_SEND_BUFF4 *in);

NDDSUSERDllExport extern void 
NET_SEND_BUFF4PluginSupport_destroy_data_w_params(
    NET_SEND_BUFF4 *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
NET_SEND_BUFF4PluginSupport_destroy_data_ex(
    NET_SEND_BUFF4 *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
NET_SEND_BUFF4PluginSupport_destroy_data(
    NET_SEND_BUFF4 *sample);

NDDSUSERDllExport extern void 
NET_SEND_BUFF4PluginSupport_print_data(
    const NET_SEND_BUFF4 *sample,
    const char *desc,
    unsigned int indent);


/* ----------------------------------------------------------------------------
    Callback functions:
 * ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
NET_SEND_BUFF4Plugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
NET_SEND_BUFF4Plugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);
    
NDDSUSERDllExport extern PRESTypePluginEndpointData 
NET_SEND_BUFF4Plugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
NET_SEND_BUFF4Plugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);
    
NDDSUSERDllExport extern void    
NET_SEND_BUFF4Plugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    NET_SEND_BUFF4 *sample,
    void *handle);    
 

NDDSUSERDllExport extern RTIBool 
NET_SEND_BUFF4Plugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    NET_SEND_BUFF4 *out,
    const NET_SEND_BUFF4 *in);

/* --------------------------------------------------------------------------------------
    (De)Serialize functions:
 * -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool 
NET_SEND_BUFF4Plugin_serialize(
    PRESTypePluginEndpointData endpoint_data,
    const NET_SEND_BUFF4 *sample,
    struct RTICdrStream *stream, 
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
NET_SEND_BUFF4Plugin_deserialize_sample(
    PRESTypePluginEndpointData endpoint_data,
    NET_SEND_BUFF4 *sample, 
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

 
NDDSUSERDllExport extern RTIBool 
NET_SEND_BUFF4Plugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    NET_SEND_BUFF4 **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);




NDDSUSERDllExport extern RTIBool
NET_SEND_BUFF4Plugin_skip(
    PRESTypePluginEndpointData endpoint_data,
    struct RTICdrStream *stream, 
    RTIBool skip_encapsulation,  
    RTIBool skip_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern unsigned int 
NET_SEND_BUFF4Plugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
NET_SEND_BUFF4Plugin_get_serialized_sample_min_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int
NET_SEND_BUFF4Plugin_get_serialized_sample_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment,
    const NET_SEND_BUFF4 * sample);



/* --------------------------------------------------------------------------------------
    Key Management functions:
 * -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginKeyKind 
NET_SEND_BUFF4Plugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
NET_SEND_BUFF4Plugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
NET_SEND_BUFF4Plugin_serialize_key(
    PRESTypePluginEndpointData endpoint_data,
    const NET_SEND_BUFF4 *sample,
    struct RTICdrStream *stream,
    RTIBool serialize_encapsulation,
    RTIEncapsulationId encapsulation_id,
    RTIBool serialize_key,
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool 
NET_SEND_BUFF4Plugin_deserialize_key_sample(
    PRESTypePluginEndpointData endpoint_data,
    NET_SEND_BUFF4 * sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

 
NDDSUSERDllExport extern RTIBool 
NET_SEND_BUFF4Plugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    NET_SEND_BUFF4 ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);


NDDSUSERDllExport extern RTIBool
NET_SEND_BUFF4Plugin_serialized_sample_to_key(
    PRESTypePluginEndpointData endpoint_data,
    NET_SEND_BUFF4 *sample,
    struct RTICdrStream *stream, 
    RTIBool deserialize_encapsulation,  
    RTIBool deserialize_key, 
    void *endpoint_plugin_qos);

     
/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
NET_SEND_BUFF4Plugin_new(void);

NDDSUSERDllExport extern void
NET_SEND_BUFF4Plugin_delete(struct PRESTypePlugin *);

} /* namespace DWR_FOSD */

        
#if (defined(RTI_WIN32) || defined (RTI_WINCE)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif        

#endif /* FOSDPlugin_1276479568_h */
