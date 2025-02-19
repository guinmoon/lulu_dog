
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
    static duk_ret_t jsLuLuCOMMAND_RIGHTHAND(duk_context *_ctx);
    static duk_ret_t jsLuLuCOMMAND_HALFLAYDOWN(duk_context *_ctx);
    static duk_ret_t jsLuLuCOMMAND_SET_TAIL_SPEED(duk_context *_ctx);
    static duk_ret_t jsLuLuCOMMAND_SIT(duk_context *_ctx);
    static duk_ret_t jsLuLuCOMMAND_STAND(duk_context *_ctx);
    static duk_ret_t jsLuLuCOMMAND_LAYDOWN(duk_context *_ctx);
    static duk_ret_t jsLuLuCOMMAND_HAPPY(duk_context *_ctx);
    static duk_ret_t jsLuLuCOMMAND_DANCE1(duk_context *_ctx);
    static duk_ret_t jsLuLuCOMMAND_TAILLEGSSTAND(duk_context *_ctx);
    static duk_ret_t jsLuLuCOMMAND_FULLLAYDOWN(duk_context *_ctx);
    void jsInit();
    void jsEval(char *code);
    bool jsEvalFile(char *fname);
};

#endif