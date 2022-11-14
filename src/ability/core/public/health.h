#pragma once

class healthContainer {
public:
	healthContainer(const unsigned short inBase, const unsigned short inShield = 0)
		: base(inBase)
		, shield(inShield) {}

	// return shield + base amount
	const unsigned short takeDamage(unsigned short amount);

	const unsigned short getBase() const { return this->base; }
	const unsigned short getShield() const { return this->shield; }

	// consider the base and shield as health. Indeed shield as no expiration time or different rules as base amount.
	const unsigned short getAmount() const { return this->base + this->shield; }

protected:
	unsigned short base;
	unsigned short shield;
};