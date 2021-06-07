#pragma once
#include "stdafx.h"

namespace leeder
{
	inline std::string GetRandomString(size_t length = 4) {

		const std::string NAMEARRAY = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

		std::random_device random_device;
		std::mt19937 generator(random_device());
		std::uniform_int_distribution<> distribution(0, NAMEARRAY.size() - 1);

		std::string random_string;

		for (std::size_t i = 0; i < length; ++i)
		{
			random_string += NAMEARRAY[distribution(generator)];
		}

		return random_string;

	}

	inline int GetRandomInt() {

		std::random_device random_device;
		std::mt19937 generator(random_device());
		std::uniform_int_distribution<> distribution(0, 255);


		return distribution(generator);

	}


	inline float GetRandomFloat() {

		std::random_device random_device;
		std::mt19937 generator(random_device());
		std::uniform_real_distribution<> distribution(0, 1);

		float random = distribution(generator);

		return random;

	}

};