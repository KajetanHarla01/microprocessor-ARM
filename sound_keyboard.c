#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"

#define GPIO_PINS_ALL GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7


void playSound(uint32_t frequency, uint32_t duration, float loudness)
{
uint32_t PWMclock = ROM_SysCtlClockGet() / 4 ; // get the current PWM clock value
uint32_t period = (PWMclock/frequency) - 1; // calculate the period for PWM signal
float numOfCyc = ROM_SysCtlClockGet()/1000.0; // calculate number of cycles per ms
uint32_t counter = duration * numOfCyc / 3; //calculate the counter for SysCtlDelay function;
ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, period); // set the period of the PWM signal
 // Set the pulse width of PWM1 for a 50% duty cycle:
ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, loudness * period);
ROM_PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true); // enable PWM1 output
ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_1); // enable the PWM signal generation
ROM_SysCtlDelay(counter); // add delay
ROM_PWMGenDisable(PWM1_BASE, PWM_GEN_1); // disable the PWM signal generation
}
int main(void)
{
float loudness_offset = 0;
int freq_offset = 0;
// Set the clocking to run directly from the crystal.
ROM_SysCtlClockSet (SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
// Set the PWM clock configuration.
ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_4);
ROM_SysCtlDelay(ROM_SysCtlClockGet() / 2);
// Enable GPIOA and PWM module 1
ROM_SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA);
ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
// Configure the PWM generator for count down mode with immediate updates
// to the parameters.
ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_1,PWM_GEN_MODE_DOWN);
//Configure the pin mux to select module 1 PWM generator 1 for GPIO pin PA6
ROM_GPIOPinConfigure(GPIO_PA6_M1PWM2);
// set the type of PA6 pin to PWM
ROM_GPIOPinTypePWM(GPIO_PORTA_BASE,GPIO_PIN_6);

SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOJ);
SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOH);

GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5);
GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PINS_ALL);

for (;;)
{
// play the melody
while(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_0) == GPIO_PIN_0)
playSound(523+freq_offset, 300, 0.5+loudness_offset);
while(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_1) == GPIO_PIN_1)
playSound(587+freq_offset, 300, 0.5+loudness_offset);
while(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_2) == GPIO_PIN_2)
playSound(659+freq_offset, 300, 0.5+loudness_offset);
while(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_3) == GPIO_PIN_3)
playSound(698+freq_offset, 300, 0.5+loudness_offset);
while(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_4) == GPIO_PIN_4)
playSound(784+freq_offset, 300, 0.5+loudness_offset);
while(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_5) == GPIO_PIN_5)
playSound(880+freq_offset, 300, 0.5+loudness_offset);
while(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_6) == GPIO_PIN_6)
playSound(988+freq_offset, 300, 0.5+loudness_offset);
while(GPIOPinRead(GPIO_PORTJ_BASE,GPIO_PIN_7) == GPIO_PIN_7)
playSound(1047+freq_offset, 300, 0.5+loudness_offset);

while(GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_0) == GPIO_PIN_0)
{
    if(loudness_offset < 0.4)
        loudness_offset+=0.1;
    SysCtlDelay(SysCtlClockGet() / 4);
}
while(GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_1) == GPIO_PIN_1)
{
    if(freq_offset < 100)
        freq_offset+=20;
    SysCtlDelay(SysCtlClockGet() / 4);
}
while(GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_4) == GPIO_PIN_4)
{
    if(loudness_offset > -0.4)
        loudness_offset-=0.1;
    SysCtlDelay(SysCtlClockGet() / 4);
}
while(GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_5) == GPIO_PIN_5)
{
    if(freq_offset > -100)
        freq_offset-=20;
    SysCtlDelay(SysCtlClockGet() / 4);
}

}
return 0;
}
