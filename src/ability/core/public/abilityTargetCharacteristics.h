#pragma once

#include "health.h"
#include <json/json.h>
#include "delegate.h"

struct abilityTargetCharacteristicsData {
	abilityTargetCharacteristicsData()
		: healthBase(10)
		, healthShield(5)
		, attackPower(2) {}

	void hydrateFromJson(const Json::Value& json);

	unsigned short healthBase;
	unsigned short healthShield;
	unsigned short attackPower;
};

class gameWorld;

class abilityTargetCharacteristics final {
public:
	template<typename T>
	struct variable {
		variable(const T& inDefaultValue, delegate<void, const unsigned short>& newTurnDelegate)
			: value(inDefaultValue)
			, defaultValue(inDefaultValue) {
			newTurnDelegate.add(std::bind(
				&abilityTargetCharacteristics::variable<T>::onNewTurn,
				this,
				std::placeholders::_1));
		}

		void onNewTurn(const unsigned short turn) {
			this->value = this->defaultValue;
		}

		operator T () const {
			return this->value;
		}
		variable<T>& operator*= (T multiplier) {
			this->value *= multiplier;
			return *this;
		}
		variable<T>& operator= (const T& inValue) {
			this->value = inValue;
			return *this;
		}

		T value;
		const T defaultValue;
	};

	abilityTargetCharacteristics(const abilityTargetCharacteristicsData& data, gameWorld& world);

	healthContainer health;

	variable<unsigned short> attackPower;

	variable<bool> stun;
};