#include "Makcu.cpp"
#include <chrono>
#include <string>

struct Makcu {
	static Makcu& Get() {
		static Makcu m_pInstance;
		return m_pInstance;
	}
public:
	_com myserial;

	bool MakcuLeft() { return LEFT; }
	bool MakcuRight() { return RIGHT; }
	bool MakcuMiddle() { return MIDDLE; }
	bool MakcuSide1() { return SIDE2; }
	bool MakcuSide2() { return SIDE1; }

	bool MakcuMouseMove(int x, int y) {
		char buff[256];
		std::string MoveCommand = "km.move(" + std::to_string(x) + "," + std::to_string(y) + ")\r\n";
		sprintf(buff, "%s", MoveCommand.c_str());
		myserial.write(buff);
		return true;
	}

	bool MakcuMouseMoveMock(int x, int y, int time) {
		char buff[256];
		std::string MoveCommand = "km.move(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(time) + ")\r\n";
		sprintf(buff, "%s", MoveCommand.c_str());
		myserial.write(buff);
		return true;
	}

	bool MakcuMouseMoveto(int x, int y) {
		char buff[256];
		std::string MoveCommand = "km.move(" + std::to_string(x) + "," + std::to_string(y) + ")\r\n";
		sprintf(buff, "%s", MoveCommand.c_str());
		return true;
	}

	bool MakcuMouseLeft(int type) {
		char buff[256];
		std::string MoveCommand = "km.left(" + std::to_string(type) + ")\r\n";
		sprintf(buff, "%s", MoveCommand.c_str());
		myserial.write(buff);
		return true;
	}
	bool MakcuMouseRight(int type) {
		char buff[256];
		std::string MoveCommand = "km.right(" + std::to_string(type) + ")\r\n";
		sprintf(buff, "%s", MoveCommand.c_str());
		myserial.write(buff);
		return true;
	}

	bool MakcuMouseState(int type) {
		const char* commands[] = {
			"km.left()", "km.right()", "km.middle()",
			"km.ms2()", "km.ms1()"
		};

		if (type < 0 || type > 4) return false;


		std::string cmd = std::string(commands[type]) + "\r\n";
		char buff[256];
		snprintf(buff, sizeof(buff), "%s", cmd.c_str());
		myserial.write(buff);

		char buffer[256] = { 0 };
		DWORD totalRead = 0;
		const DWORD timeout = 200;
		auto start = GetTickCount64();

		while ((GetTickCount64() - start) < timeout) {
			DWORD bytesRead = myserial.read(buffer + totalRead, sizeof(buffer) - 1 - totalRead);
			if (bytesRead > 0) {
				totalRead += bytesRead;
				buffer[totalRead] = '\0';

				std::string response(buffer);
				size_t cmdPos = response.find(commands[type]);
				if (cmdPos != std::string::npos) {
					size_t valuePos = response.find('\n', cmdPos);
					if (valuePos != std::string::npos && (valuePos + 1) < response.size()) {
						char valueChar = response[valuePos + 1];
						switch (type) {
						case 0: LEFT = (valueChar == '1') ? 1 : 0; break;
						case 1: RIGHT = (valueChar == '1') ? 1 : 0; break;
						case 2: MIDDLE = (valueChar == '1') ? 1 : 0; break;
						case 3: SIDE1 = (valueChar == '1') ? 1 : 0; break;
						case 4: SIDE2 = (valueChar == '1') ? 1 : 0; break;
						}
						return true;
					}
				}
			}
		}
		return false;
	}
private:
	int LEFT, RIGHT, MIDDLE, SIDE1, SIDE2;

};