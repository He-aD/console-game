#pragma once

#include "health.h"
#include <json/json.h>
#include "delegate.h"

class gameWorld;

// container of all attributes mutable by game rules
// target's type for all gameMaster and abilities process functions
class abilityTargetCharacteristics final {
public: // construct
	struct data { // characteristics base data
		data()
			: healthBase(10)
			, healthShield(5)
			, attackPower(2) {}

		void hydrateFromJson(const Json::Value& json);

		unsigned short healthBase;
		unsigned short healthShield;
		unsigned short attackPower;
	};

	abilityTargetCharacteristics(const abilityTargetCharacteristics::data& data, gameWorld& world);

public: // mutable attributes type: define logic to mutate and then reset to default value
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

		// called every new turn to reset the value to default
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

public: // mutable attributes
	healthContainer health;

	variable<unsigned short> attackPower;

	variable<bool> stun;
};