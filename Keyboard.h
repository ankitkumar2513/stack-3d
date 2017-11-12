enum Key {
	LEFT = 100,
	UP = 101,
	RIGHT = 102,
	DOWN = 103
};

class Keyboard {
private:
	bool leftPressed;
	bool rightPressed;
	bool upPressed;
	bool downPressed;

	bool downReleased;
public:
	Keyboard() {
		downReleased = false;
	}

	void keyboardDown(int);
	void keyboardUp(int);

	bool keyPressed(int);
	bool keyTapped(int);
};