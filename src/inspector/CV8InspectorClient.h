#pragma once

#include "v8-inspector.h"
#include "../helpers/V8Class.h"

class CV8InspectorClient : public v8_inspector::V8InspectorClient
{
public:
    CV8InspectorClient(v8::Local<v8::Context> context, bool connect);

    static v8::Local<v8::Promise> SendInspectorMessage(v8::Isolate* isolate, alt::String method, v8::Local<v8::Object> params);

    static std::unordered_map<uint32_t, v8::Global<v8::Promise::Resolver>> promises;

private:
    static v8_inspector::V8InspectorSession* GetSession(v8::Local<v8::Context> context) 
    {
        CV8InspectorClient* inspector_client = static_cast<CV8InspectorClient*>(
            context->GetAlignedPointerFromEmbedderData(2));
        return inspector_client->_session.get();
    }

    v8::Local<v8::Context> ensureDefaultContextInGroup(int group_id) override 
    {
        auto isolate = _isolate;
        //V8_CHECK(isolate, "Invalid isolate");
        //V8_CHECK(group_id == 1, "Invalid context group id");
        return _context.Get(isolate);
    }

    std::unique_ptr<v8_inspector::V8Inspector> _inspector;
    std::unique_ptr<v8_inspector::V8InspectorSession> _session;
    std::unique_ptr<v8_inspector::V8Inspector::Channel> _channel;
    v8::Global<v8::Context> _context;
    v8::Isolate* _isolate;

    static uint32_t GetNextMessageId();
    static uint32_t lastMessageId;
};