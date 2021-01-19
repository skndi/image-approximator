#include "Utility.h"
constexpr float PI = 3.14159;

float Utility::rad(int angle) {

	return (static_cast<float>(angle) * PI) / static_cast<float>(180);
}
