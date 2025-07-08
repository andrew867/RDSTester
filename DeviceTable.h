#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    struct deviceInfo {
        char ipAddr[16];
    };

    extern struct deviceInfo deviceTable[33];

#ifdef __cplusplus
}
#endif
