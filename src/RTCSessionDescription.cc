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

#include "RTCSessionDescription.h"

using namespace v8;

Persistent<Function> RTCSessionDescription::constructor;

RTCSessionDescription::RTCSessionDescription
  (const FunctionCallbackInfo<Value>& args) : _sessionDescription(NULL) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 1 || !args[0]->IsObject()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "parameter 1 ('descriptionInitDict') is not an object.")));
    return;
  }

  Local<Object> object = args[0]->ToObject();

  v8::Local<v8::Value> sdpValue;
  sdpValue = object->Get(String::NewFromUtf8(isolate, "sdp"));

  if (sdpValue.IsEmpty() || sdpValue->IsUndefined()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "sdp is invalid")));
  }

  const std::string sdp = *String::Utf8Value(sdpValue->ToString());

  v8::Local<v8::Value> typeValue;
  typeValue = object->Get(String::NewFromUtf8(isolate, "type"));

  if (typeValue.IsEmpty() || typeValue->IsUndefined()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "type is invalid")));
  }

  const std::string type = *String::Utf8Value(typeValue->ToString());

  if (type != "offer" && type != "answer" && type != "pranswer") {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "type must be set to 'offer', 'answer' or 'pranswer'")));
  }

  webrtc::SdpParseError error;
  _sessionDescription = webrtc::CreateSessionDescription(type, sdp, &error);

  if (!_sessionDescription) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, error.description.c_str())));
  }
}

RTCSessionDescription::~RTCSessionDescription() {
  if (_sessionDescription) {
    delete _sessionDescription;
  }
}

void RTCSessionDescription::Init(Handle<Object> exports) {
  Isolate* isolate = Isolate::GetCurrent();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "RTCSessionDescription"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "toJSON", toJSON);

  tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "type"),
                                       GetType);
  tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "sdp"),
                                       GetSdp);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "RTCSessionDescription"),
               tpl->GetFunction());
}

void RTCSessionDescription::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if (args.IsConstructCall()) {
    RTCSessionDescription* obj = new RTCSessionDescription(args);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance());
  }
}

void RTCSessionDescription::GetType(Local<String> property,
                                    const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();
  RTCSessionDescription* w =
    node::ObjectWrap::Unwrap<RTCSessionDescription>(info.This());
  webrtc::SessionDescriptionInterface* s = w->GetWrapped();

  info.GetReturnValue().Set(String::NewFromUtf8(isolate, s->type().c_str()));
}

void RTCSessionDescription::GetSdp(Local<String> property,
                                   const PropertyCallbackInfo<Value>& info) {
  Isolate* isolate = info.GetIsolate();
  RTCSessionDescription* w =
    node::ObjectWrap::Unwrap<RTCSessionDescription>(info.This());
  webrtc::SessionDescriptionInterface* s = w->GetWrapped();
  std::string sdp;

  s->ToString(&sdp);
  info.GetReturnValue().Set(String::NewFromUtf8(isolate, sdp.c_str()));
}

void RTCSessionDescription::toJSON(const FunctionCallbackInfo<Value>& args)
{
  Isolate* isolate = args.GetIsolate();
  RTCSessionDescription* w =
    node::ObjectWrap::Unwrap<RTCSessionDescription>(args.This());
  webrtc::SessionDescriptionInterface* s = w->GetWrapped();
  std::string sdp;
  Local<Object> object = Object::New(isolate);

  s->ToString(&sdp);
  object->Set(String::NewFromUtf8(isolate, "type"), String::NewFromUtf8(isolate,  s->type().c_str()));
  object->Set(String::NewFromUtf8(isolate, "sdp"), String::NewFromUtf8(isolate, sdp.c_str()));
  args.GetReturnValue().Set(object);
}