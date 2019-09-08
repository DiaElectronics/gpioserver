#ifndef _DIA_GPIO_H
#define _DIA_GPIO_H

// This is CPP header file

#include <stdio.h>
#include <wiringPi.h>

#include <atomic>

class DiaGPIO {
private:
    std::atomic<int> lastPressedKey;
    
public:
    DiaGPIO() {
        lastPressedKey.store(0);
    }
    
    ~DiaGPIO() {
    }
    
    // No copy constructor
    DiaGPIO(const DiaGPIO &) = delete;
    
    int LastKey() {
        if(lastPressedKey>0) {
            int res = lastPressedKey;
            int success = lastPressedKey.compare_exchange_weak(res, 0, 
                std::memory_order_release, 
                std::memory_order_relaxed);
            res = lastPressedKey;
            if (success) {
                return res;
            }
            return 0;
        }
        return 0;
    }
};

/*
int DiaGpio_GetLastKey(DiaGpio * gpio)
{
    if(gpio->LastPressedKey>=0) {
        int res = gpio->LastPressedKey;
        gpio->LastPressedKey = 0;
        return res;
    }
    return 0;
}

void DiaGpio_TurnLED(DiaGpio * gpio, int lightNumber, int value)
{
    if(value<=0) {
        value = 0;
    } else {
        value = 1;
    }
    if(gpio->InitializedOk)
    {
        if(lightNumber>=0 && lightNumber<PIN_COUNT && gpio->ButtonLightPin[lightNumber]>=0)
        {
            digitalWrite(gpio->ButtonLightPin[lightNumber], value);
            gpio->ButtonLightPinStatus[lightNumber] = value;
            gpio->ButtonLightTimeInCurrentPosition[lightNumber] = 0;
        }
    }
}

void DiaGpio_WriteLight(DiaGpio * gpio, int lightNumber, int value)
{
    if(lightNumber>=0 && lightNumber<PIN_COUNT)
    {
        gpio->ButtonLightMoveTo[lightNumber] = value;
    }
}

void * DiaGpio_LedSwitcher(void * gpio)
{
    DiaGpio *Gpio = (DiaGpio *)gpio;
    int step = 0;
    int MagicNumber = DEFAULT_LIGHTING_TIME_MS / 12; //10 brightness levels are existing
    while(Gpio->NeedWorking)
    {
        for(int i=0;i<PIN_COUNT;i++)
        {
            if(Gpio->ButtonPin[i]>=0)
            {
                if(step==MagicNumber)
                {
                    int abs = DiaGpio_Abs(Gpio->ButtonLightCurrentPosition[i], Gpio->ButtonLightMoveTo[i]);
                    Gpio->ButtonLightCurrentPosition[i] = Gpio->ButtonLightCurrentPosition[i] + abs;
                    step = 0;
                }
                int OnTime = Gpio->ButtonLightCurrentPosition[i];
                int OffTime = 10-OnTime;
                if(Gpio->ButtonLightPinStatus[i])
                {
                    if(Gpio->ButtonLightTimeInCurrentPosition[i]>=OnTime && OffTime)
                    {
                        DiaGpio_TurnLED(Gpio, i, 0);
                    }
                } else
                {
                    if(Gpio->ButtonLightTimeInCurrentPosition[i]>=OffTime && OnTime)
                    {
                        DiaGpio_TurnLED(Gpio, i, 1);
                    }
                }
                Gpio->ButtonLightTimeInCurrentPosition[i] = Gpio->ButtonLightTimeInCurrentPosition[i]+=1;
                step++;
            }
        }
        delay(1);
    }
    pthread_exit(NULL);
    return NULL;
}


void DiaGpio_WriteRelay(DiaGpio * gpio, int relayNumber, int value)
{
    //if(value) { printf("+%d\n", relayNumber); } else { printf("-%d\n", relayNumber);}
    if(value<=0) {
        value = 0;
    }
    else {
        value = 1;
    }
    if(gpio->InitializedOk) {
        if(relayNumber>=0 && relayNumber<PIN_COUNT && gpio->RelayPin[relayNumber]>=0)
        {
            if (value == 1) {
                gpio->RelayOnTime[relayNumber] = gpio->curTime;
            } else {
                if (gpio->RelayOnTime[relayNumber]>0) {
                    long time_elapsed = gpio->curTime - gpio->RelayOnTime[relayNumber];
                    gpio->Stat.relay_time[relayNumber] = gpio->Stat.relay_time[relayNumber] + time_elapsed;
                    gpio->Stat.relay_switch[relayNumber] = gpio->Stat.relay_switch[relayNumber] + 1;
                    gpio->RelayOnTime[relayNumber] = 0;
                }
            }
            digitalWrite(gpio->RelayPin[relayNumber], value);
            gpio->RelayPinStatus[relayNumber] = value;
        }
    }
}

DiaGpio::DiaGpio()
{
    InitializedOk = 0;
    NeedWorking = 1;
    CoinMoney = 0;
    ButtonsNormallyOn = DiaDefaultsReader::ButtonsNormallyOn();
    CurrentProgram = -1;
    AllTurnedOff = 1;
    AnimationCode = 0;
    AnimationSubCode = 0;
    if (wiringPiSetup () == -1) return ;
    if(ButtonsNormallyOn) {
        printf("all ok With buttons config\n");
    }
    
    ButtonPin[0] = -1;
    ButtonPin[1] = 13;
    ButtonPin[2] = 14;
    ButtonPin[3] = 21;
    ButtonPin[4] = 22;
    ButtonPin[5] = 23;
    ButtonPin[6] = 24;
    ButtonPin[7] = 25;
    ButtonPin[8] = -1;

    ButtonLightPin[0] = -1;
    ButtonLightPin[1] = 8;
    ButtonLightPin[2] = 9;
    ButtonLightPin[3] = 7;
    ButtonLightPin[4] = 0;
    ButtonLightPin[5] = 2;
    ButtonLightPin[6] = 3;
    ButtonLightPin[7] = 12;
    ButtonLightPin[8] = -1;

    RelayPin[0] = -1;
    RelayPin[1] = 15; // HighPreasurePump
    RelayPin[2] = 16; // Not USED
    RelayPin[3] = 1; // Soap
    RelayPin[4] = 4; // Wax
    RelayPin[5] = 5; // Pump Station
    RelayPin[6] = 6; // Light
    RelayPin[7] = -1;
    RelayPin[8] = -1;

    CoinPin = 29;
    DoorPin = 28;

    DiaGpio * gpio = this;

    if(DiaDefaultsReader::NewStyleButtonPositions()) {
        printf("new buttons config\n");
        DiaGpio_Setup(this);
    } else {
        printf("old buttons config\n");
        DiaGpio_SetupOld(this);
    }

    pinMode(CoinPin, INPUT);

    for(int i=0;i<PIN_COUNT;i++) {
        if(RelayPin[i] >= 0) {
            pinMode(RelayPin[i], OUTPUT);
            digitalWrite(RelayPin[i], 0);
        }
        if(ButtonPin[i]>=0) {
            pinMode(ButtonPin[i], INPUT);
        }
        if(ButtonLightPin[i]>=0) {
            pinMode(ButtonLightPin[i], OUTPUT);
            digitalWrite(ButtonLightPin[i],0);
        }
        RelayPinStatus[i] = 0;
        ButtonLightPinStatus[i] =0;
        ButtonLightMoveTo[i]=0;
        ButtonLightCurrentPosition[i]=0;
        ButtonLightTimeInCurrentPosition[i] = 0;
        RelayOnTime[i] = 0;
        Stat.relay_switch[i] = 0;
        Stat.relay_time[i] = 0;
    }

    pthread_create(&WorkingThread, NULL, DiaGpio_WorkingThread, this);
    pthread_setschedprio(WorkingThread, SCHED_FIFO);
    pthread_create(&LedSwitchingThread, NULL, DiaGpio_LedSwitcher, this);
    InitializedOk = 1;
    printf("Initialized OK, GPIO\n");
}



void DiaGpio_StopRelays(DiaGpio * gpio)
{
    for(int i=0;i<9;i++)
    {
        DiaGpio_WriteRelay(gpio, i, 0);
    }
}

void DiaGpio_SetProgram(DiaGpio * gpio, int programNumber, int relayNumber,  int onTime, int offTime)
{
    gpio->Programs[programNumber].RelayNum[relayNumber] = relayNumber;
    gpio->Programs[programNumber].OnTime[relayNumber] = onTime;
    gpio->Programs[programNumber].OffTime[relayNumber] = offTime;
}

void DiaGpio_SetProgram_Percent(DiaGpio * gpio, int programNumber, int relayNumber,  int percent)
{
    double totalMs = 500;
    double onMs = percent/100.0;
    onMs *= totalMs;
    double offMs = totalMs - onMs;
    if (onMs<0) {
        onMs = 0;
        offMs = 500;
    } else if (offMs<0) {
        offMs = 0;
        onMs = 500;
    }

    gpio->Programs[programNumber].RelayNum[relayNumber] = relayNumber;
    gpio->Programs[programNumber].OnTime[relayNumber] = (int)onMs;
    gpio->Programs[programNumber].OffTime[relayNumber] = (int)offMs;
    printf("pr:%d %d:%d\n", programNumber, (int)onMs, (int)offMs);
}

void DiaGpio_CheckRelays(DiaGpio * gpio, int curTime)
{
    if(gpio->CurrentProgram>8) {
        gpio->CurrentProgram = -1;
    }
    if(gpio->CurrentProgram<0)
    {
        if(!gpio->AllTurnedOff)
        {
            gpio->AllTurnedOff = 1;
            DiaGpio_StopRelays(gpio);
        } 
    } else {
        gpio->AllTurnedOff = 0;
        assert(gpio->CurrentProgram>=0 && gpio->CurrentProgram<PIN_COUNT);
        DiaRelayConfig * config = &gpio->Programs[gpio->CurrentProgram];
        for(int i=0;i<PIN_COUNT;i++)
        {
            if(gpio->RelayPin[i]<0) {
                continue;
            }
            if(config->OnTime[i]<=0) {
                //printf("pr: %d: turning permanent off: %d, cur stat:%d\n", gpio->CurrentProgram,
                //i, gpio->RelayPinStatus[i]);
                if(gpio->RelayPinStatus[i]) {
                    DiaGpio_WriteRelay(gpio, i, 0);
                }
            } else if(config->OffTime[i]<=0) {
                //printf("pr: %d: turning permanent on: %d, cur stat:%d\n", gpio->CurrentProgram,
                //i, gpio->RelayPinStatus[i]);
                if(!gpio->RelayPinStatus[i]) {
                    DiaGpio_WriteRelay(gpio, i, 1);
                }
            } else {
                if(curTime>=config->NextSwitchTime[i])
                {
                    //printf("pr: %d: switching: %d, cur stat:%d\n", gpio->CurrentProgram,
                    //i, gpio->RelayPinStatus[i]);
                    if(gpio->RelayPinStatus[i]) {
                        DiaGpio_WriteRelay(gpio, i, 0);
                        config->NextSwitchTime[i] = curTime + config->OffTime[i];
                    } else {
                        DiaGpio_WriteRelay(gpio, i, 1);
                        config->NextSwitchTime[i] = curTime + config->OnTime[i];
                    }
                }
            }
        }
        
    }
}

int DiaGpio_ReadButton(DiaGpio * gpio, int ButtonNumber)
{
    if(ButtonNumber>=0 && ButtonNumber<PIN_COUNT)
    {
        if(gpio->ButtonPin[ButtonNumber]>=0)
        {
            int val = digitalRead( gpio->ButtonPin[ButtonNumber] );
            if (gpio->ButtonsNormallyOn==2) {
                return 0;
            }
            if (gpio->ButtonsNormallyOn) {
                return !val;
            } else {
                return val;
            }
        }
    }
    return 0;
}

void DiaGpio_CheckCoin(DiaGpio * gpio)
{
    int curState = digitalRead(gpio->CoinPin);
    gpio->CoinStatus[gpio->CoinLoop] = curState;
    gpio->CoinLoop = gpio->CoinLoop + 1;
    //printf("%d",curState);
    if(gpio->CoinLoop>=COIN_TOTAL)
    {
         gpio->CoinLoop=0;
    }
    int curSwitchedOnPins = 0;
    for(int i=0;i < COIN_TOTAL;i++)
    {
         if( gpio->CoinStatus[i])
         {
              curSwitchedOnPins++;
         }
    }
    if(gpio->CoinStatus_)
    {
        if(curSwitchedOnPins<(COIN_TOTAL-COIN_SWITCH))
        {
            gpio->CoinStatus_ = 0;
            printf("%d\n", gpio->CoinMoney);
        }
    } else
    {
        if(curSwitchedOnPins>COIN_SWITCH)
        {
            gpio->CoinMoney = gpio->CoinMoney + 1;
            printf("%d\n", gpio->CoinMoney);
            //printf("k");
            gpio->CoinStatus_ = 1;
        }
    }
}



void DiaGpio_ButtonAnimation(DiaGpio * gpio, long curTime)
{
	if(gpio->AnimationCode == ONE_BUTTON_ANIMATION)
	{
		int curStep = curTime/DEFAULT_LIGHTING_TIME_MS;
		int value = 10;
		curStep=curStep%3;
		if(!curStep)
		{
            value = 0;
		}
		for(int i=0;i<PIN_COUNT;i++)
		{
			if (i==gpio->AnimationSubCode)
			{
				DiaGpio_WriteLight(gpio, i, value);
			}
			else
			{
				DiaGpio_WriteLight(gpio, i, 0);
			}
		}
	}
	else if(gpio->AnimationCode == IDLE_ANIMATION)
	{
		int curStep = curTime / DEFAULT_LIGHTING_TIME_MS;
		curStep=curStep % 12;
		int iShouldBeOn = curStep + 1;
		if(iShouldBeOn>7) iShouldBeOn = 14 - iShouldBeOn;
		for(int i=0;i<PIN_COUNT;i++)
		{
            if(i==iShouldBeOn)
            {
                DiaGpio_WriteLight(gpio, i, 10);
			} else
			{
                DiaGpio_WriteLight(gpio, i, 0);
			}
		}
	}
	else if(gpio->AnimationCode == BALANCE_ANIMATION)
	{
		int curStep = curTime/DEFAULT_LIGHTING_TIME_MS;
		curStep=curStep%14;
		int curHalfStep = curStep - 7;
		for(int i=0;i<PIN_COUNT;i++)
		{
            int iShouldBeOn = 0;
            if(curStep>=i)
            {
                iShouldBeOn = 10;
            }
            if(curHalfStep>=i)
            {
                iShouldBeOn = 0;
            }
            DiaGpio_WriteLight(gpio, i, iShouldBeOn);
		}
	}
}

inline int DiaGpio_Abs(int from, int to)
{
    if(to>from) return 1;
    if(to<from) return -1;
    return 0;
}

void * DiaGpio_WorkingThread(void * gpio)
{
	int buttonPinLastActivated = -1;
    DiaGpio *Gpio = (DiaGpio *)gpio;
    for(int i=0;i<COIN_TOTAL;i++)
    {
        Gpio->CoinStatus[i] = digitalRead(Gpio->CoinPin);
        Gpio->CoinStatus_ = digitalRead(Gpio->CoinPin);
    }
    Gpio->CoinLoop = 0;

    Gpio->curTime = 0;

    while(Gpio->NeedWorking)
    {
        delay(1);//This code will run once per ms
        Gpio->curTime=Gpio->curTime + 1;
        DiaGpio_CheckRelays(Gpio, Gpio->curTime);
        DiaGpio_CheckCoin(Gpio);
        DiaGpio_ButtonAnimation(Gpio, Gpio->curTime);
        if(Gpio->curTime % 50 == 0)
        {
            for(int i=0;i<PIN_COUNT;i++)
            {
                if(DiaGpio_ReadButton(Gpio, i))
                {
					if(buttonPinLastActivated!=i)
					{
						buttonPinLastActivated = i;
						Gpio->LastPressedKey = i;
						break;
					}
                }
                else
                {
					if(buttonPinLastActivated == i)
					{
						buttonPinLastActivated = -1;
					}
				}
            }
        }
    }
    pthread_exit(0);
    return 0;
}
*/

#endif
