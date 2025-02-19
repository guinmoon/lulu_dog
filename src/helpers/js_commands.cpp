#include "js_helper.h"
#include "lulu_dog.h"



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

    luluDog->luluCharacter->doReact(COMMAND_LEFTHAND, speed, tail_speed, "/imgs/eye3.gif", "/audio/woof3.wav");
    return 0;
}


duk_ret_t JSRunner::jsLuLuCOMMAND_RIGHTHAND(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    int speed = duk_to_int(_ctx, 0);
    int tail_speed = duk_to_int(_ctx, 1);    

    luluDog->luluCharacter->doReact(COMMAND_RIGHTHAND, speed, tail_speed, "/imgs/eye3.gif", "/audio/woof3.wav");
    return 0;
}

duk_ret_t JSRunner::jsLuLuCOMMAND_HALFLAYDOWN(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    int speed = duk_to_int(_ctx, 0);
    int tail_speed = duk_to_int(_ctx, 1);    

    luluDog->luluCharacter->doReact(COMMAND_HALFLAYDOWN, speed, tail_speed, "/imgs/eye2.gif", "/audio/woof1.wav");
    return 0;
}


duk_ret_t JSRunner::jsLuLuCOMMAND_SET_TAIL_SPEED(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    int tail_speed = duk_to_int(_ctx, 0);    

    luluDog->luluCharacter->doReact(COMMAND_SET_TAIL_SPEED, -1, tail_speed, nullptr, nullptr);
    return 0;
}

duk_ret_t JSRunner::jsLuLuCOMMAND_SIT(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    int speed = duk_to_int(_ctx, 0);
    int tail_speed = duk_to_int(_ctx, 1);    

    luluDog->luluCharacter->doReact(COMMAND_SIT, speed, tail_speed, "/imgs/eye1.gif", "/audio/woof2.wav");
    return 0;
}

duk_ret_t JSRunner::jsLuLuCOMMAND_LAYDOWN(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    int speed = duk_to_int(_ctx, 0);
    int tail_speed = duk_to_int(_ctx, 1);    

    luluDog->luluCharacter->doReact(COMMAND_LAYDOWN, speed, tail_speed, "/imgs/eye2.gif", "/audio/woof1.wav");
    return 0;
}

duk_ret_t JSRunner::jsLuLuCOMMAND_HAPPY(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    int speed = duk_to_int(_ctx, 0);
    int tail_speed = duk_to_int(_ctx, 1);    

    luluDog->luluCharacter->doReact(COMMAND_HAPPY, speed, tail_speed, "/imgs/eye3.gif", "/audio/woof3.wav");
    return 0;
}

duk_ret_t JSRunner::jsLuLuCOMMAND_DANCE1(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    int speed = duk_to_int(_ctx, 0);
    int tail_speed = duk_to_int(_ctx, 1);    

    luluDog->luluCharacter->doReact(COMMAND_DANCE1, speed, tail_speed, "/imgs/eye3.gif", "/audio/woof2.wav");
    return 0;
}

duk_ret_t JSRunner::jsLuLuCOMMAND_STAND(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    int speed = duk_to_int(_ctx, 0);
    int tail_speed = duk_to_int(_ctx, 1);    

    luluDog->luluCharacter->doReact(COMMAND_STAND, speed, tail_speed, "/imgs/eye3.gif", nullptr);
    return 0;
}

duk_ret_t JSRunner::jsLuLuCOMMAND_FULLLAYDOWN(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    int speed = duk_to_int(_ctx, 0);
    int tail_speed = duk_to_int(_ctx, 1);    

    luluDog->luluCharacter->doReact(COMMAND_FULLLAYDOWN, speed, tail_speed, "/imgs/eye6.gif", "/audio/woof3.wav");
    return 0;
}

duk_ret_t JSRunner::jsLuLuCOMMAND_TAILLEGSSTAND(duk_context *_ctx)
{
    duk_get_top(_ctx);    
    int speed = duk_to_int(_ctx, 0);
    int tail_speed = duk_to_int(_ctx, 1);    

    luluDog->luluCharacter->doReact(COMMAND_TAILLEGSSTAND, speed, tail_speed, "/imgs/eye6.gif", nullptr);
    return 0;
}
