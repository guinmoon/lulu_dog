onHeadIntensiveTouch = function(count){
    if (count >=5 && count <7){
        lulu_command(-1, 2, 5, "/imgs/eye5.gif", "");
    }
    if (count>=7 && count<10){
        lulu_command(-1, 2, 7, "/imgs/eye3.gif", "/audio/woof2.wav");
    }
    if (count>=10){
        lulu_command(5, 4, 8, "/imgs/eye3.gif", "/audio/woof2.wav");
    }
}