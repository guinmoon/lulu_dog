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

static void lulu_js_fatal(void *udata, const char *msg)
{
    log_d("*** FATAL ERROR: %s\n", (msg ? msg : "no message"));
    abort();
}

static void *lulu_js_alloc(void *udata, duk_size_t size)
{
    return ps_malloc(size);
}

static void *lulu_js_realloc(void *udata, void *ptr, duk_size_t size)
{
    return ps_realloc(ptr, size);
}

static void lulu_js_free(void *udata, void *ptr)
{
    free(ptr);
    return;
}

void JSRunner::jsInit()
{
    // ctx = duk_create_heap_default();
    ctx = duk_create_heap(lulu_js_alloc, lulu_js_realloc, lulu_js_free, NULL, lulu_js_fatal);

    duk_push_c_function(ctx, jsLog, DUK_VARARGS);
    duk_put_global_string(ctx, "log_d");
    duk_push_c_function(ctx, jsDelay, DUK_VARARGS);
    duk_put_global_string(ctx, "delay");
    duk_push_c_function(ctx, jsRunLuLuCommand, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_command");
    duk_push_c_function(ctx, jsLuLuDoRnadomCommand, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_random_command");    
    duk_push_c_function(ctx, jsLuLuDoRnadomCommandGif, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_random_command_gif");    
    duk_push_c_function(ctx, jsLuLuCOMMAND_LEFTHAND, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_leftHand");
    duk_push_c_function(ctx, jsLuLuCOMMAND_RIGHTHAND, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_rightHand");
    duk_push_c_function(ctx, jsLuLuCOMMAND_LEFTHAND_LONG, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_leftHand_long");
    duk_push_c_function(ctx, jsLuLuCOMMAND_RIGHTHAND_LONG, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_rightHand_long");
    duk_push_c_function(ctx, jsLuLuCOMMAND_HALFLAYDOWN, DUK_VARARGS); 
    duk_put_global_string(ctx, "lulu_halfLayDown");
    duk_push_c_function(ctx, jsLuLuCOMMAND_SET_TAIL_SPEED, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_setTailSpeed");
    duk_push_c_function(ctx, jsLuLuCOMMAND_SIT, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_sit");
    duk_push_c_function(ctx, jsLuLuCOMMAND_STAND, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_stand");
    duk_push_c_function(ctx, jsLuLuCOMMAND_LAYDOWN, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_layDown");
    duk_push_c_function(ctx, jsLuLuCOMMAND_HAPPY, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_happy");
    duk_push_c_function(ctx, jsLuLuCOMMAND_DANCE1, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_dance1");
    duk_push_c_function(ctx, jsLuLuCOMMAND_TAILLEGSSTAND, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_tailLegsStand");
    duk_push_c_function(ctx, jsLuLuCOMMAND_FULLLAYDOWN, DUK_VARARGS);
    duk_put_global_string(ctx, "lulu_fullLayDown");
    
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

bool JSRunner::_jsCallFunctionPre(char *filename, char *funcName){
    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        log_d("Failed to open JS file");
        return false;
    }

    String fileData = file.readString();

    duk_push_string(ctx, fileData.c_str());
    duk_int_t rc = duk_peval(ctx);

    auto res = duk_get_global_string(ctx, funcName);
    if (res == false)
    {
        log_d("Failed to get global function");
        return false;
    }
    return true;
}

bool JSRunner::_jsCallFunctionPost(int argc){
    bool  res = false;
    if (duk_pcall(ctx, argc /*nargs*/) != 0)
    {
        log_d("Error: %s\n", duk_safe_to_string(ctx, -1));
    }
    else
    {
        log_d("%s\n", duk_safe_to_string(ctx, -1));
        res = true;
    }
    duk_pop(ctx);  
    return res; 
}

void JSRunner::jsCallFunctionNIntArgs(char *filename, char *funcName,int argc, int* argv)
{
    if(!_jsCallFunctionPre(filename, funcName))
        return;

    for (int i =0 ;i<argc;i++)
    {
        duk_push_int(ctx, argv[i]);    
    }
    
    _jsCallFunctionPost(argc);
}


void JSRunner::jsCallFunctionNFloatArgs(char *filename, char *funcName,int argc, float* argv)
{
    if(!_jsCallFunctionPre(filename, funcName))
        return;

    for (int i =0 ;i<argc;i++)
    {
        duk_push_number(ctx, argv[i]);    
    }
    
    _jsCallFunctionPost(argc);
}


void JSRunner::jsCallFunctionNIntKFloatArgs(char *filename, char *funcName,int argcInt, int* argvInt,int argcFloat, int* argvFloat)
{
    if(!_jsCallFunctionPre(filename, funcName))
        return;

    for (int i =0 ;i<argcInt;i++)
    {
        duk_push_int(ctx, argvInt[i]);    
    }

    for (int i =0 ;i<argcFloat;i++)
    {
        duk_push_number(ctx, argvFloat[i]);    
    }
    
    _jsCallFunctionPost(argcInt+argcFloat);
}

void JSRunner::jsCallFunctionKFloatNIntArgs(char *filename, char *funcName,int argcFloat, int* argvFloat,int argcInt, int* argvInt)
{
    if(!_jsCallFunctionPre(filename, funcName))
        return;

    for (int i =0 ;i<argcFloat;i++)
    {
        duk_push_number(ctx, argvFloat[i]);    
    }

    for (int i =0 ;i<argcInt;i++)
    {
        duk_push_int(ctx, argvInt[i]);    
    }
    
    _jsCallFunctionPost(argcInt+argcFloat);
}