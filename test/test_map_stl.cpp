#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, std::string> map;
    for (int i = 0; i < 1000; i++) {
        std::string temp = "Key " + std::to_string(i);
        map[temp] = temp; // Corrected insertion
    }

    for (const auto& pair : map) {
		std::cout << "{ " << pair.first << " -> " << pair.second << " }\n";
	}
	return 0;
}
