#include <duktape.h>

class LuLuDog;

class JSRunner
{
private:
    LuLuDog *luluDog;
    duk_context *ctx;

public:
    JSRunner(LuLuDog *_luluDog);
    static duk_ret_t jsPrint(duk_context *ctx);
    void jsInit();
    void jsEval(char *code);
};