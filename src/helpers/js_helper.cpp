#include <duktape.h>
#include <Arduino.h>
#include "js_helper.h"
#include "LittleFS.h"
#include "string.h"
#include "lulu_dog.h"

duk_context *JSRunner::ctx;
LuLuDog *JSRunner::luluDog;
JSRunner::JSRunner(LuLuDog *_luluDog)
{
    luluDog = _luluDog;
}
//

duk_ret_t JSRunner::jsLog(duk_context *_ctx)
{
    duk_push_string(_ctx, " ");
    duk_insert(_ctx, 0);
    duk_join(_ctx, duk_get_top(_ctx) - 1);
    log_d("%s", duk_to_string(_ctx, -1));

    return 0;
}

duk_ret_t JSRunner::jsDelay(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    delay(duk_to_int(_ctx, 0));

    return 0;
}

duk_ret_t JSRunner::jsRunLuLuCommand(duk_context *_ctx)
{
    duk_get_top(_ctx);
    int command = duk_to_int(_ctx, 0);
    int speed = duk_to_int(_ctx, 1);
    int tail_speed = duk_to_int(_ctx, 2);
    const char* eye = duk_to_string(_ctx, 3);
    const char* wav = duk_to_string(_ctx, 4);

    luluDog->luluCharacter->doReact(command, speed, tail_speed, const_cast<char*>(eye), const_cast<char*>(wav));
        
    return 0;
}

duk_ret_t JSRunner::jsLuLuCOMMAND_LEFTHAND(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    int speed = duk_to_int(_ctx, 0);
    int tail_speed = duk_to_int(_ctx, 1);    

    luluDog->luluCharacter->doReact(5, speed, tail_speed, "/imgs/eye3.gif", "/audio/woof3.wav");
    return 0;
}

void JSRunner::jsInit()
{
    ctx = duk_create_heap_default();

    duk_push_c_function(ctx, jsLog, DUK_VARARGS);
    duk_put_global_string(ctx, "log_d");
    duk_push_c_function(ctx, jsDelay, DUK_VARARGS);
    duk_put_global_string(ctx, "delay");
    duk_push_c_function(ctx, jsRunLuLuCommand, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_command");
    duk_push_c_function(ctx, jsLuLuCOMMAND_LEFTHAND, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_leftHand");
}

void JSRunner::jsEval(char *code)
{
    // Serial.printf("\n>>> Eval Code: %s\n", code);
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

    // Serial.printf("\n>>> Reslt: %s\n", res ? res : "null");

    duk_pop(ctx);
    //  duk_destroy_heap(ctx);
}

bool JSRunner::jsEvalFile(char *filename)
{
    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        log_d("Failed to open JS file");
        return false;
    }

    String fileData = file.readString();

    duk_push_string(ctx, fileData.c_str());
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

    duk_pop(ctx);
    return true;
    //  duk_destroy_heap(ctx);
}
