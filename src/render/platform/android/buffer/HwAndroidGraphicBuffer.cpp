/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "HwAndroidGraphicBuffer.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

HwAndroidGraphicBuffer::HwAndroidGraphicBuffer(int w, int h) : HwAbsGraphicBuffer(w, h) {
    handler = new uint8_t[GRAPHIC_BUFFER_SIZE];
//    static_cast<uint32_t>(Usage::GRALLOC_USAGE_SW_WRITE_NEVER)
    uint32_t usage = static_cast<uint32_t>(Usage::GRALLOC_USAGE_SW_READ_OFTEN) |
                     static_cast<uint32_t>(Usage::GRALLOC_USAGE_HW_TEXTURE);
    HwUILoader::getInstance()->fGraphicBufferCtor(handler, width, height,
                                                  static_cast<uint32_t>(Format::HAL_PIXEL_FORMAT_RGBX_8888),
                                                  usage);
    void *nativeBuffer = HwUILoader::getInstance()->fGraphicBufferGetNativeBuffer(handler);
    dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint eglImageAttributes[] = {EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE};
    pImageKHR = eglCreateImageKHR(dpy, EGL_NO_CONTEXT, EGL_NATIVE_BUFFER_ANDROID,
                                  (EGLClientBuffer) nativeBuffer, eglImageAttributes);
}

HwAndroidGraphicBuffer::~HwAndroidGraphicBuffer() {
    if (handler) {
        HwUILoader::getInstance()->fGraphicBufferDtor(handler);
        delete[] handler;
        handler = nullptr;
    }
    if (pImageKHR && dpy) {
        eglDestroyImageKHR(dpy, pImageKHR);
        pImageKHR = nullptr;
    }
    dpy = EGL_NO_DISPLAY;
}

bool HwAndroidGraphicBuffer::bind() {
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES) pImageKHR);
    return true;
}

bool HwAndroidGraphicBuffer::read(uint8_t *pixels) {
    if (!pixels) {
        return false;
    }
    uint8_t *pBuf = nullptr;
    HwUILoader::getInstance()->fGraphicBufferLock(handler,
                                                  static_cast<uint32_t>(Usage::GRALLOC_USAGE_SW_READ_OFTEN),
                                                  &pBuf);
    memcpy(pixels, pBuf, width * height * 4);
    HwUILoader::getInstance()->fGraphicBufferUnlock(handler);
    return true;
}
