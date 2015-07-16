/*
 * Copyright (c) 2015 Axel Isouard <ax@eml.cc>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef NWEBRTC_RTCSESSIONDESCRIPTION_H_
#define NWEBRTC_RTCSESSIONDESCRIPTION_H_

#include <node.h>
#include <node_object_wrap.h>
#include "../third_party/webrtc/talk/app/webrtc/jsep.h"

using namespace v8;

class RTCSessionDescription
  : public node::ObjectWrap {
 public:
  static void Init(Handle<Object> exports);

  webrtc::SessionDescriptionInterface *GetWrapped() const {
    return _sessionDescription;
  }

  void SetWrapped(webrtc::SessionDescriptionInterface *sessionDescription) {
    _sessionDescription = sessionDescription;
  }

 private:
  RTCSessionDescription(const FunctionCallbackInfo<Value> &);

  ~RTCSessionDescription();

  static void New(const FunctionCallbackInfo<Value> &info);
  static void toJSON(const FunctionCallbackInfo<Value> &info);
  static void GetType(Local<String> property,
                      const PropertyCallbackInfo<Value> &info);
  static void GetSdp(Local<String> property,
                     const PropertyCallbackInfo<Value> &info);

  static Persistent<Function> constructor;

  static Local<String> kType;
  static Local<String> kSdp;

  webrtc::SessionDescriptionInterface *_sessionDescription;
};

#endif  // NWEBRTC_RTCSESSIONDESCRIPTION_H_