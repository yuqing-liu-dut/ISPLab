#pragma once

#include <stdio.h>
#define ABS(X) ( (X) >= 0 ? (X) : (-(X)) )
#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))
#define TRACE_DEBUG_LOG_WARNING (printf("[WARNING] %s(%d)-<%s>: ",__FILE__, __LINE__, __FUNCTION__), printf)
#define TRACE_DEBUG_LOG_ERROR (printf("[ERROR] %s(%d)-<%s>: ",__FILE__, __LINE__, __FUNCTION__), printf)
