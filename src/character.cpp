#include <Wire.h>
#include "character.h"
#include "lulu_dog.h"
#include "eyes_drawer.h"



// LuLuCharacter luluCharacter;

LuLuCharacter::LuLuCharacter(LuLuDog *_luluDog)
{
    luluDog = _luluDog;
    NormalizeProbabilities();
}



int LuLuCharacter::generateRandomWithProbabilities(float probabilities[], int size)
{
    // Cуммируем вероятности, чтобы они представляли собой диапазоны
    float cumulativeProbabilities[size];
    cumulativeProbabilities[0] = probabilities[0];
    for (int i = 1; i < size; i++)
    {
        cumulativeProbabilities[i] = cumulativeProbabilities[i - 1] + probabilities[i];
    }

    // Генерируем случайное число от 0 до 1
    float randomValue = random(1000) / 1000.0;

    // Определяем, в какой диапазон попало случайное число
    for (int i = 0; i < size; i++)
    {
        if (randomValue < cumulativeProbabilities[i])
        {
            // Возвращаем i+1, чтобы числа были 1 до n, i чтобы от 0..n-1
            return i;
        }
    }

    // В случае ошибки возвращаем 0
    return 0;
}

void LuLuCharacter::NormalizeProbabilities()
{
    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += probabilities[i];
    }
    for (int i = 0; i < size; i++)
    {
        probabilities[i] /= sum;
    }
}


int LuLuCharacter::getAllowedRandomReact()
{
    bool allowed = false;
    int choice = generateRandomWithProbabilities(probabilities, size);
    // if (!isCharging())
    return choice;

    // Если заряжается то выбираем доступный в этом режиме вариант
    while (!allowed)
    {
        choice = random(maxChoise);
        for (int i = 0; i < 4; i++)
        {
            if (choice == allowedOnCharging[i])
            {
                allowed = true;
                break;
            }
        }
    }
    return choice;
}

int LuLuCharacter::GetAllowedSceneReact()
{
    int choice = random(1);
    return choice;
}



void LuLuCharacter::doReact(int command, int speed, int tail_speed, int eye, char *wav)
{

    // log_d("doReact: command: %i speed: %i tail_speed: %i eye: %s wav: %s",command,speed,tail_speed,eye,wav);
    luluDog->dogEvents->lastImpact = millis(); // Для вызовов не из этого класса
    luluDog->dogEvents->pingPaused = true;
    luluDog->displayHelper->StopGif();    
    

    if (tail_speed != -1 && luluDog->configHelper->EnableMove)
    {
        delay(200);
        luluDog->i2cSlaveHelper->SendCommand(COMMAND_SET_TAIL_SPEED, tail_speed);        
    }
    if (command != -1 && luluDog->configHelper->EnableMove)
    {
        luluDog->gyroHelper->PauseGyro();
        luluDog->i2cSlaveHelper->SendCommand(command, speed);
        luluDog->i2cSlaveHelper->ConfirmCommand(command, speed);
    }
    switch (eye)
    {
    case 0:
        luluDog->displayHelper->luluEyes->setMood(0);
        break;
    case 1:
        luluDog->displayHelper->luluEyes->setMood(1);
        break;
    case 2:
        luluDog->displayHelper->luluEyes->setMood(2);
        break;
    case 3:
        luluDog->displayHelper->luluEyes->setMood(3);
        break;

    default:
        break;
    }
    if (wav != nullptr)
        luluDog->audioHelper->PlayWav(wav);
    delay(200);
    luluDog->dogEvents->pingPaused = false;
}

void LuLuCharacter::doReactGif(int command, int speed, int tail_speed, char *eye, char *wav)
{
    // log_d("doReact: command: %i speed: %i tail_speed: %i eye: %s wav: %s",command,speed,tail_speed,eye,wav);
    luluDog->dogEvents->lastImpact = millis(); // Для вызовов не из этого класса
    luluDog->dogEvents->pingPaused = true;
    if (tail_speed != -1 && luluDog->configHelper->EnableMove)
    {
        delay(200);
        luluDog->i2cSlaveHelper->SendCommand(COMMAND_SET_TAIL_SPEED, tail_speed);        
    }
    if (command != -1 && luluDog->configHelper->EnableMove)
    {
        luluDog->gyroHelper->PauseGyro();
        luluDog->i2cSlaveHelper->SendCommand(command, speed);
        luluDog->i2cSlaveHelper->ConfirmCommand(command, speed);
    }
    if (eye != nullptr)
        luluDog->displayHelper->PlayGif(eye);
    if (wav != nullptr)
        luluDog->audioHelper->PlayWav(wav);
    delay(200);
    luluDog->dogEvents->pingPaused = false;
}

void LuLuCharacter::doRandomReact(int direction, bool withMove)
{

    if (luluDog->displayHelper->showMatrixAnimation)
    {
        luluDog->displayHelper->StopMatrixAnimation();
    }

    int choice = getAllowedRandomReact();

    switch (choice)
    {
    case 0:
        doReact(-1, -1, 0, 0, nullptr);
        break;
    case 1:
        doReact(withMove ? COMMAND_SIT: -1, 4, 6, 1, "/audio/woof2.wav");
        break;
    case 2:
        doReact(withMove ? COMMAND_SIT: -1, 5, 4, 2, "/audio/woof2.wav");
        break;
    case 3:
        doReact(withMove ? COMMAND_STAND: -1, 2, 0, 3, "/audio/woof1.wav");
        break;
    case 4:
        doReact(withMove ? COMMAND_LAYDOWN: -1, 4, 4, 0, nullptr);
        break;
    case 5:
        doReact(-1, -1, 4, 1, nullptr);
        break;
    case 6:
        doReact(withMove ? COMMAND_LEFTHAND: -1, 4, 4, 2, "/audio/woof3.wav");
        break;
    case 7:
        doReact(withMove ? COMMAND_LAYDOWN: -1, 3, 6, 3, "/audio/woof2.wav");
        break;
    case 8:
        doReact(withMove ? COMMAND_LAYDOWN: -1, 3, 0, 0, "/audio/woof2.wav");
        break;
    case 9:
        doReact(withMove ? COMMAND_HALFLAYDOWN: -1, 2, 7, 1, "/audio/woof2.wav");
        break;
    case 10:
        doReact(-1, -1, 4, 2, "/woof1.wav");
        break;
    case 11:
        doReact(withMove ? COMMAND_RIGHTHAND: -1, 4, 4, 3, "/audio/woof3.wav");
        break;
    case 12:
        doReact(withMove ? COMMAND_FULLLAYDOWN: -1, 7, 0, 0, "/audio/woof1.wav");
        break;
    case 13:
        doReact(withMove ? COMMAND_TAILLEGSSTAND: -1, 4, 4, 1, "/audio/woof1.wav");
        break;
    case 14:
        doReact(withMove ? COMMAND_HALFLAYDOWNTAIL: -1, 4, 6, 3, "/audio/woof1.wav");
        break;
    default:
        doReact(-1, -1, 0, 0, nullptr);
        break;
    }
}

void LuLuCharacter::doRandomReactGif(int direction, bool withMove)
{

    if (luluDog->displayHelper->showMatrixAnimation)
    {
        luluDog->displayHelper->StopMatrixAnimation();
    }

    int choice = getAllowedRandomReact();

    switch (choice)
    {
    case 0:
        doReactGif(-1, -1, 0, "/imgs/eye1.gif", nullptr);
        break;
    case 1:
        doReactGif(withMove ? COMMAND_SIT : -1, 4, 6, "/imgs/eye1.gif", "/audio/woof2.wav");
        break;
    case 2:
        doReactGif(withMove ? COMMAND_SIT : -1, 5, 4, "/imgs/eye1.gif", "/audio/woof2.wav");
        break;
    case 3:
        doReactGif(withMove ? COMMAND_STAND : -1, 2, 0, "/imgs/eye2.gif", "/audio/woof1.wav");
        break;
    case 4:
        doReactGif(withMove ? COMMAND_LAYDOWN : -1, 4, 4, "/imgs/eye2.gif", nullptr);
        break;
    case 5:
        doReactGif(-1, -1, 4, "/imgs/eye4.gif", nullptr);
        break;
    case 6:
        doReactGif(withMove ? COMMAND_LEFTHAND : -1, 4, 4, "/imgs/eye4.gif", "/audio/woof3.wav");
        break;
    case 7:
        doReactGif(withMove ? COMMAND_LAYDOWN : -1, 3, 6, "/imgs/eye3.gif", "/audio/woof2.wav");
        break;
    case 8:
        doReactGif(withMove ? COMMAND_LAYDOWN : -1, 3, 0, "/imgs/eye3.gif", "/audio/woof2.wav");
        break;
    case 9:
        doReactGif(withMove ? COMMAND_HALFLAYDOWN : -1, 2, 7, "/imgs/eye3.gif", "/audio/woof2.wav");
        break;
    case 10:
        doReactGif(-1, -1, 4, "/imgs/eye5.gif", "/woof1.wav");
        break;
    case 11:
        doReactGif(withMove ? COMMAND_RIGHTHAND : -1, 4, 4, "/imgs/eye4.gif", "/audio/woof3.wav");
        break;
    case 12:
        doReactGif(withMove ? COMMAND_FULLLAYDOWN : -1, 7, 0, "/imgs/eye3.gif", "/audio/woof1.wav");
        break;
    case 13:
        doReactGif(withMove ? COMMAND_TAILLEGSSTAND : -1, 4, 4, "/imgs/eye3.gif", "/audio/woof1.wav");
        break;
    case 14:
        doReactGif(withMove ? COMMAND_HALFLAYDOWNTAIL : -1, 4, 6, "/imgs/eye3.gif", "/audio/woof1.wav");
        break;
    default:
        doReactGif(-1, -1, 0, "/imgs/eye5.gif", nullptr);
        break;
    }
}

void LuLuCharacter::DoSceneReact(int x, int y)
{
    int current_time = millis();
    // if (current_time - lastImpact < LAST_IMPACT_MIN_PERIOD)
    // {
    //     lastImpact = current_time;
    //     return;
    // }
    luluDog->dogEvents->lastImpact = current_time;
    // _wake();

    int choice = GetAllowedSceneReact();
    switch (choice)
    {
    // case 0:
    //     SendCommand(COMMAND_LEFTHAND, 4);
    //     delay(200);
    //     SendCommand(COMMAND_SET_TAIL_SPEED, 4);
    //     PlayWav("/woof3.wav");
    //     PlayGif("/eye4.gif");
    //     break;
    case 1:
        doReactGif(COMMAND_DANCE1, 4, 7, "/eye5.gif", "/woof1.wav");
        break;
    default:
        doReactGif(COMMAND_DANCE1, 4, 7, "/eye5.gif", "/woof1.wav");
        break;
    }
}

void LuLuCharacter::LeftHandLong()
{
    // doReactGif(COMMAND_LEFTHAND, 4, 6, "/imgs/eye4.gif", "/audio/woof3.wav");
    luluDog->displayHelper->StopGif();
    luluDog->displayHelper->setIdleMode(false);
    luluDog->displayHelper->resumeEyes();
    luluDog->displayHelper->luluEyes->setPosition(SW);
    doReact(COMMAND_LEFTHAND_LONG, 4, 6, HAPPY, "/audio/woof3.wav");
    luluDog->displayHelper->luluEyes->setPosition(SW);
}

void LuLuCharacter::RightHandLong()
{
    // doReactGif(COMMAND_RIGHTHAND, 4, 6, "/imgs/eye5.gif", "/audio/woof3.wav");
    luluDog->displayHelper->StopGif();
    luluDog->displayHelper->setIdleMode(false);
    luluDog->displayHelper->resumeEyes();
    luluDog->displayHelper->luluEyes->setPosition(SE);
    doReact(COMMAND_RIGHTHAND_LONG, 4, 6, HAPPY, "/audio/woof3.wav");
    luluDog->displayHelper->luluEyes->setPosition(SE);
}

void LuLuCharacter::SitDown()
{
    // doReactGif(COMMAND_RIGHTHAND, 4, 6, "/imgs/eye5.gif", "/audio/woof3.wav");
    
    luluDog->displayHelper->setIdleMode(true);
    luluDog->displayHelper->resumeEyes();
    doReact(COMMAND_SIT, 4, 6, -1, nullptr);
    
}