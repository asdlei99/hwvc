/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/HwJavaNativeHelper.h"
#include "HwAudioProcessor.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

static JMethodDescription aPlayProgressDesc = {"Java_com_lmy_hwvcnative_processor_AudioProcessor",
                                               "onPlayProgress", "(JJ)V"};

static HwAudioProcessor *getHandler(jlong handler) {
    return reinterpret_cast<HwAudioProcessor *>(handler);
}

JNIEXPORT jlong JNICALL Java_com_lmy_hwvcnative_processor_AudioProcessor_create
        (JNIEnv *env, jobject thiz) {
    HwAudioProcessor *p = new HwAudioProcessor();
    p->post([] {
        HwJavaNativeHelper::getInstance()->attachThread();
    });
    jlong handler = reinterpret_cast<jlong>(p);
    HwJavaNativeHelper::getInstance()->registerAnObject(env, handler, thiz);
    return handler;
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AudioProcessor_setSource
        (JNIEnv *env, jobject thiz, jlong handler, jstring path) {
    if (handler) {
        const char *pPath = env->GetStringUTFChars(path, JNI_FALSE);
        std::string pathStr(pPath);
        getHandler(handler)->setSource(pathStr);
        env->ReleaseStringUTFChars(path, pPath);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AudioProcessor_prepare
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->prepare();
        getHandler(handler)->setPlayProgressListener([handler](int64_t us, int64_t duration) {
            jobject jObject = nullptr;
            JNIEnv *pEnv = nullptr;
            jmethodID methodID = nullptr;
            if (HwJavaNativeHelper::getInstance()->findEnv(&pEnv) &&
                HwJavaNativeHelper::getInstance()->findJObject(handler, &jObject) &&
                HwJavaNativeHelper::getInstance()->findMethod(handler,
                                                              aPlayProgressDesc,
                                                              &methodID)) {
                pEnv->CallVoidMethod(jObject, methodID, static_cast<jlong>(us),
                                     static_cast<jlong>(duration));
            }
        });
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AudioProcessor_start
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->start();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AudioProcessor_pause
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->pause();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AudioProcessor_stop
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        getHandler(handler)->stop();
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AudioProcessor_seek
        (JNIEnv *env, jobject thiz, jlong handler, jlong us) {
    if (handler) {
        getHandler(handler)->seek(us);
    }
}

JNIEXPORT void JNICALL Java_com_lmy_hwvcnative_processor_AudioProcessor_release
        (JNIEnv *env, jobject thiz, jlong handler) {
    if (handler) {
        HwAudioProcessor *p = getHandler(handler);
        p->post([] {
            HwJavaNativeHelper::getInstance()->detachThread();
        });
        p->stop();
        delete p;
    }
    HwJavaNativeHelper::getInstance()->unregisterAnObject(env, handler);
}

#ifdef __cplusplus
}
#endif
