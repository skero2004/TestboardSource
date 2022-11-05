#include <avr/io.h>
#include "TestboardSource.h"

void setup(void) {
	DDRA = 0x01;
    PORTA = 0x01;

}
void action(void) {
	//send can signal here
}