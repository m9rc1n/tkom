#include <iostream>
#include <string>
#include <cstdlib>

#include "file_name.hpp"

bool source (std::string source) {
	// sprawdź adres i port źródłowy
	if (checkAddressAndPort (source)) {
		return true;
	} else {
		return false;
	}
}
 
bool destination (std::string destination) {
	// sprawdź adres i port docelowy
	if (checkAddressAndPort (destination)) {
		return true;
	} else {
		return false;
	}
}

bool checkAddressAndPort (std::string text) {
	int i = 0;  
	// sprawdź poprawność adresu
	for (i; i < 4; i++) {
		// pierwszy segment adresu
		int address_segment = atoi(text.substr(i*4, i*4+3).c_str());
		// sprawdź czy mieści się w zakresie
		if (address_segment > 255 || address_segment < 0) {
			return false;
		}
		// sprawdź czy są wymagne kropki
		if (text [i*4+3] != '.') {
			return false;
		} 
	}
	// sprawdź poprawność portu
	int port = atoi(text.substr(i*4).c_str());
	// sprawdź zakres portu
	if (port > 65535 || port < 0) {
		return false;
	}
	// zwróć informacje o sukcesie
	return true;
}
