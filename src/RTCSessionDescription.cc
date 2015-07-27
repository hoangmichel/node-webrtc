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

#include <sstream>
#include "RTCSessionDescription.h"

using namespace v8;

Local<String> RTCSessionDescription::kType;
Local<String> RTCSessionDescription::kSdp;

Persistent<Function> RTCSessionDescription::constructor;

RTCSessionDescription::RTCSessionDescription
  (const FunctionCallbackInfo<Value> &info) : _sessionDescription(NULL) {
  Isolate *isolate = info.GetIsolate();

  if (info.Length() < 1 || !info[0]->IsObject()) {
    Local<String> message = String::NewFromUtf8(
      isolate, "argument must be an object");
    isolate->ThrowException(Exception::TypeError(message));
    return;
  }

  Handle<Object> object = Handle<Object>::Cast(info[0]);

  if (!object->Has(kType)) {
    Local<String> message = String::NewFromUtf8(
      isolate, "'type' property must be set");
    isolate->ThrowException(Exception::TypeError(message));
    return;
  }

  Local<Value> typeValue = object->Get(kType);

  if (!typeValue->IsString()) {
    Local<String> message = String::NewFromUtf8(
      isolate, "'type' property must be a string");
    isolate->ThrowException(Exception::TypeError(message));
    return;
  }

  const std::string type = *String::Utf8Value(typeValue->ToString());

  if (type != "offer" && type != "answer" && type != "pranswer") {
    Local<String> message = String::NewFromUtf8(
      isolate, "'type' must be set to 'offer', 'answer' or 'pranswer'");
    isolate->ThrowException(Exception::TypeError(message));
    return;
  }

  if (!object->Has(kSdp)) {
    Local<String> message = String::NewFromUtf8(
      isolate, "'sdp' property must be set");
    isolate->ThrowException(Exception::TypeError(message));
    return;
  }

  Local<Value> sdpValue = object->Get(kSdp);

  if (!sdpValue->IsString()) {
    Local<String> message = String::NewFromUtf8(
      isolate, "'sdp' property must be a string");
    isolate->ThrowException(Exception::TypeError(message));
    return;
  }

  const std::string sdp = *String::Utf8Value(sdpValue->ToString());

  webrtc::SdpParseError error;
  _sessionDescription = webrtc::CreateSessionDescription(type, sdp, &error);

  if (!_sessionDescription) {
    std::stringstream strm;

    strm << "Failed to parse sdp at line " << error.line << ": " <<
    error.description;

    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, strm.str().c_str())));
  }
}

RTCSessionDescription::~RTCSessionDescription() {
  if (_sessionDescription) {
    delete _sessionDescription;
  }
}

void RTCSessionDescription::Init(Handle<Object> exports) {
  Isolate *isolate = Isolate::GetCurrent();

  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "RTCSessionDescription"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "toJSON", toJSON);

  kType = String::NewFromUtf8(isolate, "type");
  kSdp = String::NewFromUtf8(isolate, "sdp");

  tpl->InstanceTemplate()->SetAccessor(kType, GetType);
  tpl->InstanceTemplate()->SetAccessor(kSdp, GetSdp);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "RTCSessionDescription"),
               tpl->GetFunction());
}

void RTCSessionDescription::New(const FunctionCallbackInfo<Value> &info) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (info.IsConstructCall()) {
    RTCSessionDescription *obj = new RTCSessionDescription(info);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    info.GetReturnValue().Set(cons->NewInstance());
  }
}

void RTCSessionDescription::GetType(Local<String> property,
                                    const PropertyCallbackInfo<Value> &info) {
  Isolate *isolate = info.GetIsolate();
  RTCSessionDescription *w =
    node::ObjectWrap::Unwrap<RTCSessionDescription>(info.This());
  webrtc::SessionDescriptionInterface *s = w->GetWrapped();

  info.GetReturnValue().Set(String::NewFromUtf8(isolate, s->type().c_str()));
}

void RTCSessionDescription::GetSdp(Local<String> property,
                                   const PropertyCallbackInfo<Value> &info) {
  Isolate *isolate = info.GetIsolate();
  RTCSessionDescription *w =
    node::ObjectWrap::Unwrap<RTCSessionDescription>(info.This());
  webrtc::SessionDescriptionInterface *s = w->GetWrapped();
  std::string sdp;

  s->ToString(&sdp);
  info.GetReturnValue().Set(String::NewFromUtf8(isolate, sdp.c_str()));
}

void RTCSessionDescription::toJSON(const FunctionCallbackInfo<Value> &info) {
  Isolate *isolate = info.GetIsolate();
  RTCSessionDescription *w =
    node::ObjectWrap::Unwrap<RTCSessionDescription>(info.This());
  webrtc::SessionDescriptionInterface *s = w->GetWrapped();
  std::string sdp;
  Local<Object> object = Object::New(isolate);

  s->ToString(&sdp);
  object->Set(kType, String::NewFromUtf8(isolate, s->type().c_str()));
  object->Set(kSdp, String::NewFromUtf8(isolate, sdp.c_str()));
  info.GetReturnValue().Set(object);
}