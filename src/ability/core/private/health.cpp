#include "health.h"

const unsigned short healthContainer::takeDamage(unsigned short amount) {
	// remove shield first
	if (this->shield >= amount) {
		this->shield -= amount;
		amount = 0;
	}
	else if (this->shield > 0) {
		amount -= this->shield;
		this->shield = 0;
	}

	// then remove base if shield didn't tank all the damage
	if (this->base <= amount) {
		this->base = 0;
	}
	else {
		this->base -= amount;
	}

	return this->getAmount();
}