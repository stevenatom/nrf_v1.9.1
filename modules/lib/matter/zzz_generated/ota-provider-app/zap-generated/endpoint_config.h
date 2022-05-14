/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

// THIS FILE IS GENERATED BY ZAP

// Prevent multiple inclusion
#pragma once

// Default values for the attributes longer than a pointer,
// in a form of a binary blob
// Separate block is generated for big-endian and little-endian cases.
#if BIGENDIAN_CPU
#define GENERATED_DEFAULTS                                                                                                         \
    {                                                                                                                              \
                                                                                                                                   \
        /* Endpoint: 0, Cluster: Localization Configuration (server), big-endian */                                                \
                                                                                                                                   \
        /* 0 - ActiveLocale, */                                                                                                    \
        5, 'e', 'n', '-', 'U', 'S',                                                                                                \
                                                                                                                                   \
            /* Endpoint: 0, Cluster: General Commissioning (server), big-endian */                                                 \
                                                                                                                                   \
            /* 6 - Breadcrumb, */                                                                                                  \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                                                                        \
                                                                                                                                   \
            /* 14 - BasicCommissioningInfoList, */                                                                                 \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                                                                              \
                                                                                                                                   \
            /* 268 - FeatureMap, */                                                                                                \
            0x00, 0x00, 0x00, 0x00,                                                                                                \
                                                                                                                                   \
            /* Endpoint: 0, Cluster: Network Commissioning (server), big-endian */                                                 \
                                                                                                                                   \
            /* 272 - Networks, */                                                                                                  \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                                                \
                                                                                                                                   \
            /* 284 - LastConnectErrorValue, */                                                                                     \
            0x00, 0x00, 0x00, 0x00,                                                                                                \
                                                                                                                                   \
            /* 288 - FeatureMap, */                                                                                                \
            0x00, 0x00, 0x00, 0x01,                                                                                                \
    }

#else // !BIGENDIAN_CPU
#define GENERATED_DEFAULTS                                                                                                         \
    {                                                                                                                              \
                                                                                                                                   \
        /* Endpoint: 0, Cluster: Localization Configuration (server), little-endian */                                             \
                                                                                                                                   \
        /* 0 - ActiveLocale, */                                                                                                    \
        5, 'e', 'n', '-', 'U', 'S',                                                                                                \
                                                                                                                                   \
            /* Endpoint: 0, Cluster: General Commissioning (server), little-endian */                                              \
                                                                                                                                   \
            /* 6 - Breadcrumb, */                                                                                                  \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                                                                        \
                                                                                                                                   \
            /* 14 - BasicCommissioningInfoList, */                                                                                 \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,      \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                                                                              \
                                                                                                                                   \
            /* 268 - FeatureMap, */                                                                                                \
            0x00, 0x00, 0x00, 0x00,                                                                                                \
                                                                                                                                   \
            /* Endpoint: 0, Cluster: Network Commissioning (server), little-endian */                                              \
                                                                                                                                   \
            /* 272 - Networks, */                                                                                                  \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                                                \
                                                                                                                                   \
            /* 284 - LastConnectErrorValue, */                                                                                     \
            0x00, 0x00, 0x00, 0x00,                                                                                                \
                                                                                                                                   \
            /* 288 - FeatureMap, */                                                                                                \
            0x01, 0x00, 0x00, 0x00,                                                                                                \
    }

#endif // BIGENDIAN_CPU

#define GENERATED_DEFAULTS_COUNT (7)

#define ZAP_TYPE(type) ZCL_##type##_ATTRIBUTE_TYPE
#define ZAP_LONG_DEFAULTS_INDEX(index)                                                                                             \
    {                                                                                                                              \
        &generatedDefaults[index]                                                                                                  \
    }
#define ZAP_MIN_MAX_DEFAULTS_INDEX(index)                                                                                          \
    {                                                                                                                              \
        &minMaxDefaults[index]                                                                                                     \
    }
#define ZAP_EMPTY_DEFAULT()                                                                                                        \
    {                                                                                                                              \
        (uint16_t) 0                                                                                                               \
    }
#define ZAP_SIMPLE_DEFAULT(x)                                                                                                      \
    {                                                                                                                              \
        (uint16_t) x                                                                                                               \
    }

// This is an array of EmberAfAttributeMinMaxValue structures.
#define GENERATED_MIN_MAX_DEFAULT_COUNT 1
#define GENERATED_MIN_MAX_DEFAULTS                                                                                                 \
    {                                                                                                                              \
                                                                                                                                   \
        /* Endpoint: 0, Cluster: Time Format Localization (server) */                                                              \
        {                                                                                                                          \
            (uint16_t) 0x0, (uint16_t) 0x0, (uint16_t) 0x1                                                                         \
        } /* HourFormat */                                                                                                         \
    }

#define ZAP_ATTRIBUTE_MASK(mask) ATTRIBUTE_MASK_##mask
// This is an array of EmberAfAttributeMetadata structures.
#define GENERATED_ATTRIBUTE_COUNT 35
#define GENERATED_ATTRIBUTES                                                                                                       \
    {                                                                                                                              \
                                                                                                                                   \
        /* Endpoint: 0, Cluster: OTA Software Update Provider (server) */                                                          \
        { 0x0000FFFD, ZAP_TYPE(INT16U), 2, 0, ZAP_SIMPLE_DEFAULT(0x0001) }, /* ClusterRevision */                                  \
                                                                                                                                   \
            /* Endpoint: 0, Cluster: Localization Configuration (server) */                                                        \
            { 0x00000001, ZAP_TYPE(CHAR_STRING), 36, ZAP_ATTRIBUTE_MASK(TOKENIZE) | ZAP_ATTRIBUTE_MASK(WRITABLE),                  \
              ZAP_LONG_DEFAULTS_INDEX(0) },                                                                /* ActiveLocale */      \
            { 0x00000002, ZAP_TYPE(ARRAY), 0, ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE), ZAP_EMPTY_DEFAULT() }, /* SupportedLocales */  \
            { 0x0000FFFD, ZAP_TYPE(INT16U), 2, 0, ZAP_SIMPLE_DEFAULT(1) },                                 /* ClusterRevision */   \
                                                                                                                                   \
            /* Endpoint: 0, Cluster: Time Format Localization (server) */                                                          \
            { 0x00000000, ZAP_TYPE(ENUM8), 1,                                                                                      \
              ZAP_ATTRIBUTE_MASK(MIN_MAX) | ZAP_ATTRIBUTE_MASK(TOKENIZE) | ZAP_ATTRIBUTE_MASK(WRITABLE),                           \
              ZAP_MIN_MAX_DEFAULTS_INDEX(0) }, /* HourFormat */                                                                    \
            { 0x00000001, ZAP_TYPE(ENUM8), 1, ZAP_ATTRIBUTE_MASK(TOKENIZE) | ZAP_ATTRIBUTE_MASK(WRITABLE),                         \
              ZAP_SIMPLE_DEFAULT(0) }, /* ActiveCalendarType */                                                                    \
            { 0x00000002, ZAP_TYPE(ARRAY), 0, ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE),                                                \
              ZAP_EMPTY_DEFAULT() },                                       /* SupportedCalendarTypes */                            \
            { 0x0000FFFD, ZAP_TYPE(INT16U), 2, 0, ZAP_SIMPLE_DEFAULT(1) }, /* ClusterRevision */                                   \
                                                                                                                                   \
            /* Endpoint: 0, Cluster: General Commissioning (server) */                                                             \
            { 0x00000000, ZAP_TYPE(INT64U), 8, ZAP_ATTRIBUTE_MASK(WRITABLE), ZAP_LONG_DEFAULTS_INDEX(6) }, /* Breadcrumb */        \
            { 0x00000001, ZAP_TYPE(ARRAY), 254, 0, ZAP_LONG_DEFAULTS_INDEX(14) },   /* BasicCommissioningInfoList */               \
            { 0x00000002, ZAP_TYPE(ENUM8), 1, 0, ZAP_SIMPLE_DEFAULT(0) },           /* RegulatoryConfig */                         \
            { 0x00000003, ZAP_TYPE(ENUM8), 1, 0, ZAP_SIMPLE_DEFAULT(0) },           /* LocationCapability */                       \
            { 0x0000FFFC, ZAP_TYPE(BITMAP32), 4, 0, ZAP_LONG_DEFAULTS_INDEX(268) }, /* FeatureMap */                               \
            { 0x0000FFFD, ZAP_TYPE(INT16U), 2, 0, ZAP_SIMPLE_DEFAULT(0x0001) },     /* ClusterRevision */                          \
                                                                                                                                   \
            /* Endpoint: 0, Cluster: Network Commissioning (server) */                                                             \
            { 0x00000000, ZAP_TYPE(INT8U), 1, 0, ZAP_EMPTY_DEFAULT() },                              /* MaxNetworks */             \
            { 0x00000001, ZAP_TYPE(ARRAY), 12, 0, ZAP_LONG_DEFAULTS_INDEX(272) },                    /* Networks */                \
            { 0x00000002, ZAP_TYPE(INT8U), 1, 0, ZAP_EMPTY_DEFAULT() },                              /* ScanMaxTimeSeconds */      \
            { 0x00000003, ZAP_TYPE(INT8U), 1, 0, ZAP_EMPTY_DEFAULT() },                              /* ConnectMaxTimeSeconds */   \
            { 0x00000004, ZAP_TYPE(BOOLEAN), 1, ZAP_ATTRIBUTE_MASK(WRITABLE), ZAP_EMPTY_DEFAULT() }, /* InterfaceEnabled */        \
            { 0x00000005, ZAP_TYPE(ENUM8), 1, 0, ZAP_EMPTY_DEFAULT() },                              /* LastNetworkingStatus */    \
            { 0x00000006, ZAP_TYPE(OCTET_STRING), 33, 0, ZAP_EMPTY_DEFAULT() },                      /* LastNetworkID */           \
            { 0x00000007, ZAP_TYPE(INT32U), 4, 0, ZAP_LONG_DEFAULTS_INDEX(284) },                    /* LastConnectErrorValue */   \
            { 0x0000FFFC, ZAP_TYPE(BITMAP32), 4, 0, ZAP_LONG_DEFAULTS_INDEX(288) },                  /* FeatureMap */              \
            { 0x0000FFFD, ZAP_TYPE(INT16U), 2, 0, ZAP_SIMPLE_DEFAULT(0x0001) },                      /* ClusterRevision */         \
                                                                                                                                   \
            /* Endpoint: 0, Cluster: Operational Credentials (server) */                                                           \
            { 0x00000000, ZAP_TYPE(ARRAY), 0, ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE), ZAP_EMPTY_DEFAULT() }, /* NOCs */              \
            { 0x00000001, ZAP_TYPE(ARRAY), 0, ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE), ZAP_EMPTY_DEFAULT() }, /* fabrics list */      \
            { 0x00000002, ZAP_TYPE(INT8U), 1, 0, ZAP_EMPTY_DEFAULT() },                                    /* SupportedFabrics */  \
            { 0x00000003, ZAP_TYPE(INT8U), 1, 0, ZAP_EMPTY_DEFAULT() }, /* CommissionedFabrics */                                  \
            { 0x00000004, ZAP_TYPE(ARRAY), 0, ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE),                                                \
              ZAP_EMPTY_DEFAULT() }, /* TrustedRootCertificates */                                                                 \
            { 0x00000005, ZAP_TYPE(FABRIC_IDX), 0, ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE),                                           \
              ZAP_EMPTY_DEFAULT() },                                            /* CurrentFabricIndex */                           \
            { 0x0000FFFD, ZAP_TYPE(INT16U), 2, 0, ZAP_SIMPLE_DEFAULT(0x0001) }, /* ClusterRevision */                              \
                                                                                                                                   \
            /* Endpoint: 0, Cluster: Fixed Label (server) */                                                                       \
            { 0x00000000, ZAP_TYPE(ARRAY), 0, ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE), ZAP_EMPTY_DEFAULT() }, /* label list */        \
            { 0x0000FFFD, ZAP_TYPE(INT16U), 2, 0, ZAP_SIMPLE_DEFAULT(1) },                                 /* ClusterRevision */   \
                                                                                                                                   \
            /* Endpoint: 0, Cluster: User Label (server) */                                                                        \
            { 0x00000000, ZAP_TYPE(ARRAY), 0, ZAP_ATTRIBUTE_MASK(EXTERNAL_STORAGE) | ZAP_ATTRIBUTE_MASK(WRITABLE),                 \
              ZAP_EMPTY_DEFAULT() },                                       /* label list */                                        \
            { 0x0000FFFD, ZAP_TYPE(INT16U), 2, 0, ZAP_SIMPLE_DEFAULT(1) }, /* ClusterRevision */                                   \
    }

// This is an array of EmberAfCluster structures.
#define ZAP_ATTRIBUTE_INDEX(index) ((EmberAfAttributeMetadata *) (&generatedAttributes[index]))

// Cluster function static arrays
#define GENERATED_FUNCTION_ARRAYS                                                                                                  \
    const EmberAfGenericClusterFunction chipFuncArrayLocalizationConfigurationServer[] = {                                         \
        (EmberAfGenericClusterFunction) emberAfLocalizationConfigurationClusterServerInitCallback,                                 \
        (EmberAfGenericClusterFunction) MatterLocalizationConfigurationClusterServerPreAttributeChangedCallback,                   \
    };                                                                                                                             \
    const EmberAfGenericClusterFunction chipFuncArrayTimeFormatLocalizationServer[] = {                                            \
        (EmberAfGenericClusterFunction) emberAfTimeFormatLocalizationClusterServerInitCallback,                                    \
        (EmberAfGenericClusterFunction) MatterTimeFormatLocalizationClusterServerPreAttributeChangedCallback,                      \
    };

#define ZAP_CLUSTER_MASK(mask) CLUSTER_MASK_##mask
#define GENERATED_CLUSTER_COUNT 8
#define GENERATED_CLUSTERS                                                                                                         \
    {                                                                                                                              \
        {                                                                                                                          \
            0x00000029, ZAP_ATTRIBUTE_INDEX(0), 1, 2, ZAP_CLUSTER_MASK(SERVER), NULL                                               \
        }, /* Endpoint: 0, Cluster: OTA Software Update Provider (server) */                                                       \
            { 0x0000002B,                                                                                                          \
              ZAP_ATTRIBUTE_INDEX(1),                                                                                              \
              3,                                                                                                                   \
              38,                                                                                                                  \
              ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(PRE_ATTRIBUTE_CHANGED_FUNCTION),       \
              chipFuncArrayLocalizationConfigurationServer }, /* Endpoint: 0, Cluster: Localization Configuration (server) */      \
            { 0x0000002C,                                                                                                          \
              ZAP_ATTRIBUTE_INDEX(4),                                                                                              \
              4,                                                                                                                   \
              4,                                                                                                                   \
              ZAP_CLUSTER_MASK(SERVER) | ZAP_CLUSTER_MASK(INIT_FUNCTION) | ZAP_CLUSTER_MASK(PRE_ATTRIBUTE_CHANGED_FUNCTION),       \
              chipFuncArrayTimeFormatLocalizationServer }, /* Endpoint: 0, Cluster: Time Format Localization (server) */           \
            {                                                                                                                      \
                0x00000030, ZAP_ATTRIBUTE_INDEX(8), 6, 270, ZAP_CLUSTER_MASK(SERVER), NULL                                         \
            }, /* Endpoint: 0, Cluster: General Commissioning (server) */                                                          \
            {                                                                                                                      \
                0x00000031, ZAP_ATTRIBUTE_INDEX(14), 10, 60, ZAP_CLUSTER_MASK(SERVER), NULL                                        \
            }, /* Endpoint: 0, Cluster: Network Commissioning (server) */                                                          \
            {                                                                                                                      \
                0x0000003E, ZAP_ATTRIBUTE_INDEX(24), 7, 4, ZAP_CLUSTER_MASK(SERVER), NULL                                          \
            }, /* Endpoint: 0, Cluster: Operational Credentials (server) */                                                        \
            {                                                                                                                      \
                0x00000040, ZAP_ATTRIBUTE_INDEX(31), 2, 2, ZAP_CLUSTER_MASK(SERVER), NULL                                          \
            }, /* Endpoint: 0, Cluster: Fixed Label (server) */                                                                    \
            {                                                                                                                      \
                0x00000041, ZAP_ATTRIBUTE_INDEX(33), 2, 2, ZAP_CLUSTER_MASK(SERVER), NULL                                          \
            }, /* Endpoint: 0, Cluster: User Label (server) */                                                                     \
    }

#define ZAP_CLUSTER_INDEX(index) ((EmberAfCluster *) (&generatedClusters[index]))

// This is an array of EmberAfEndpointType structures.
#define GENERATED_ENDPOINT_TYPES                                                                                                   \
    {                                                                                                                              \
        { ZAP_CLUSTER_INDEX(0), 8, 382 },                                                                                          \
    }

// Largest attribute size is needed for various buffers
#define ATTRIBUTE_LARGEST (401)

// Total size of singleton attributes
#define ATTRIBUTE_SINGLETONS_SIZE (0)

// Total size of attribute storage
#define ATTRIBUTE_MAX_SIZE (382)

// Number of fixed endpoints
#define FIXED_ENDPOINT_COUNT (1)

// Array of endpoints that are supported, the data inside
// the array is the endpoint number.
#define FIXED_ENDPOINT_ARRAY                                                                                                       \
    {                                                                                                                              \
        0x0000                                                                                                                     \
    }

// Array of profile ids
#define FIXED_PROFILE_IDS                                                                                                          \
    {                                                                                                                              \
        0x0103                                                                                                                     \
    }

// Array of device ids
#define FIXED_DEVICE_IDS                                                                                                           \
    {                                                                                                                              \
        22                                                                                                                         \
    }

// Array of device versions
#define FIXED_DEVICE_VERSIONS                                                                                                      \
    {                                                                                                                              \
        1                                                                                                                          \
    }

// Array of endpoint types supported on each endpoint
#define FIXED_ENDPOINT_TYPES                                                                                                       \
    {                                                                                                                              \
        0                                                                                                                          \
    }

// Array of networks supported on each endpoint
#define FIXED_NETWORKS                                                                                                             \
    {                                                                                                                              \
        0                                                                                                                          \
    }
