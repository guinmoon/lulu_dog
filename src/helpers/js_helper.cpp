#include <duktape.h>
#include <Arduino.h>
#include "js_helper.h"


JSRunner::JSRunner(LuLuDog *_luluDog){
    this->luluDog = _luluDog;
}
// 

static duk_ret_t jsPrint(duk_context *ctx)
{
    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);
    log_d("%s", duk_to_string(ctx, -1));

    return 0;
}

void JSRunner::jsInit()
{
    ctx = duk_create_heap_default();

    duk_push_c_function(ctx, jsPrint, DUK_VARARGS);
    duk_put_global_string(ctx, "log_d");
}

void JSRunner::jsEval(char *code)
{
    Serial.printf("\n>>> Eval Code: %s\n", code);
    duk_push_string(ctx, code);
    duk_int_t rc = duk_peval(ctx);

    if (rc != 0)
    {
        duk_safe_to_stacktrace(ctx, -1);
    }
    else
    {
        duk_safe_to_string(ctx, -1);
    }

    String res = duk_get_string(ctx, -1);

    Serial.printf("\n>>> Reslt: %s\n", res ? res : "null");

    duk_pop(ctx);
    //  duk_destroy_heap(ctx);
}

//    log_d("Loading JS Example...");

//     // put your setup code here, to run once:
//     js_setup();
//     js_eval("for (var i=0;i<5;i++) print('Hello, world!');");