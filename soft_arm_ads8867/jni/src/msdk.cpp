#include "sdk.h"
#include "msdk.h"

/* Impl for class Msdk */

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     Msdk
 * Method:    sdk_main_init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jscb_Msdk_sdk_1main_1init
  (JNIEnv *, jclass)
{
    return sdk_main_init();
}

/*
 * Class:     Msdk
 * Method:    sdk_main_start
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_jscb_Msdk_sdk_1main_1start
  (JNIEnv *, jclass)
{
    return sdk_main_start();
}

#ifdef __cplusplus
}
#endif

