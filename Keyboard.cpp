#include "Keyboard.h"
#include <cstdio>
using namespace std;
void Keyboard::keyboardUp(int keyCode) {
	switch (keyCode) {
	case LEFT:
		leftPressed = false;
		break;
	case UP:
		upPressed = false;
		break;
	case RIGHT:
		rightPressed = false;
		break;
	case DOWN:
		downPressed = false;
		downReleased = true;
		break;
	}
}

void Keyboard::keyboardDown(int keyCode) {

	switch (keyCode) {
	case LEFT:
		leftPressed = true;
		break;
	case UP:
		upPressed = true;
		break;
	case RIGHT:
		rightPressed = true;
		break;
	case DOWN:
		downPressed = true;
		downReleased = false;
		break;
	}
}

bool Keyboard::keyPressed(int keyCode) {
	switch (keyCode)
	{
	case LEFT:
		return leftPressed;
	case UP:
		return upPressed;
	case RIGHT:
		return rightPressed;
	case DOWN:
		return downPressed;
	default:
		return false;
	}
}

bool Keyboard::keyTapped(int keyCode) {
	switch (keyCode) {
	case DOWN:
		if (keyPressed(DOWN) && !downReleased) {
			downReleased = true;
			return true;
		}
		break;
	}
	return false;
}
