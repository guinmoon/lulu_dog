void touchReadTask(void *params)
{
    while (true)
    {
        int val = digitalRead(2);
        if (val != 0)
        {
            log_d("loop %d", val);
            doRandomReact();
        }
    }
    vTaskDelete(NULL);
}