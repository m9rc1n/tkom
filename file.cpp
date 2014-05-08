#include <cstdio>
#include <string>
#include <iostream>
#include <vector>

#include <dirent.h>

#include "file.hpp"
#include "const.hpp"
#include "file_name.hpp"
#include "log.hpp"
#include "request.hpp"
#include "response.hpp"

std::vector<std::string> getFilesFromDirectory(std::string path) {
	struct dirent *entry;
	DIR *dp;
	std::vector<std::string> result;

	dp = opendir(path.c_str());
	if (dp == NULL) {
		perror("opendir");
	}

	while(entry = readdir(dp)) {
	//	printf("%s\n", entry->d_name);
		result.push_back(entry->d_name);
		// puts(entry->d_name);
	}

	closedir(dp);

	return result;
}

bool isRequestFileNameIncorrect (std::string filename) {
	// indeks znaku rozdzielajacego
	std::size_t index_of_separator = filename.find('-');
	// jesli nie znaleziono znaku rozdzielajacego
	if (index_of_separator == NOT_FOUND) {
		return true;
	}
	// sprawdzaj port i adres zrodlowy
	if (source (filename.substr(0, index_of_separator))) {
		// sprawdz port i adres docelowy
		if (destination (filename.substr(index_of_separator+1))) {
			// zwroc informacje o poprawnej nazwie pliku
			return false;
		}
	}
	// plik niepoprawny
	return true;
}

void checkDirectory(std::string path) {
	// raportuj rozpoczecie skanowania
	// report (SCAN_RESULT, STARTED);
	// lista plikow w zadanym katalogu
	std::vector<std::string> files = getFilesFromDirectory(path);
	// petla skanujaca dany katalog

	for(std::vector<std::string>::iterator file = files.begin(); file != files.end(); file++) {
		// jesli plik nie ma poprawnej nazwy
		// przejdz do kolejnego
		if (isRequestFileNameIncorrect(*file)) {
			continue;
		}

		// rozpocznij sesje z danym plikiem
		// sprawdz, czy zadany plik zawiera zadanie HTTP
		report (START, *file);
		long request_state = request("files/", *file);
		report (REQUEST_RESULT, request_state);
		long response_state = response("files/", getResponseFileFromRequestFile(*file));
		report (RESPONSE_RESULT, response_state);
		report (FINISH, "log.txt");
	}

	// raportuj stan skanowania
	//report (SCAN_RESULT, FINISHED);
}
