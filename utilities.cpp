#include "utilities.h"

void clamp(int min, int max, int& value) {
	if (value > max) { value = max; }
	else if (value < min) { value = min; }
}