
#ifndef JS_HELPER_H
#define JS_HELPER_H

#include <duktape.h>

class LuLuDog;

class JSRunner
{
private:
    static LuLuDog *luluDog;
    static duk_context *ctx;

public:
    JSRunner(LuLuDog *_luluDog);
    static duk_ret_t jsLog(duk_context *_ctx);
    static duk_ret_t jsDelay(duk_context *_ctx);
    static duk_ret_t jsRunLuLuCommand(duk_context *_ctx);
    static duk_ret_t jsLuLuCOMMAND_LEFTHAND(duk_context *_ctx);
    void jsInit();
    void jsEval(char *code);
    bool jsEvalFile(char *fname);
};

#endif