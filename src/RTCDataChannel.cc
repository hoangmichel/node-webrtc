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

#include "RTCDataChannel.h"

using namespace v8;

Persistent<Function> RTCDataChannel::constructor;

RTCDataChannel::RTCDataChannel() {
}

RTCDataChannel::~RTCDataChannel() {
}

void RTCDataChannel::Init(Handle<Object> exports) {
  Isolate *isolate = Isolate::GetCurrent();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "RTCDataChannel"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "toJSON", toJSON);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "RTCDataChannel"),
               tpl->GetFunction());
}

void RTCDataChannel::New(const FunctionCallbackInfo<Value> &info) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (info.IsConstructCall()) {
    RTCDataChannel *obj = new RTCDataChannel();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    info.GetReturnValue().Set(cons->NewInstance());
  }
}

void RTCDataChannel::toJSON(const FunctionCallbackInfo<Value> &info) {
}