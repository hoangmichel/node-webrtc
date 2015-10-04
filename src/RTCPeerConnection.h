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

#ifndef NWEBRTC_RTCPEERCONNECTION_H_
#define NWEBRTC_RTCPEERCONNECTION_H_

#include <node.h>
#include <node_object_wrap.h>

class RTCPeerConnection
  : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  RTCPeerConnection();
  RTCPeerConnection(const v8::FunctionCallbackInfo<v8::Value> &info);
  ~RTCPeerConnection();

  static void New(const v8::FunctionCallbackInfo<v8::Value> &info);
  static void toJSON(const v8::FunctionCallbackInfo<v8::Value> &info);
  static void createOffer(const v8::FunctionCallbackInfo<v8::Value> &info);
  static void createAnswer(const v8::FunctionCallbackInfo<v8::Value> &info);
  static void setLocalDescription(const v8::FunctionCallbackInfo<v8::Value> &info);
  static void setRemoteDescription(const v8::FunctionCallbackInfo<v8::Value> &info);

  static v8::Persistent<v8::Function> constructor;
};

#endif  // NWEBRTC_RTCPEERCONNECTION_H_