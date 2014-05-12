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
		result.push_back(entry->d_name);
	}

	closedir(dp);

	return result;
}

void checkFile (std::string input, std::string &file) {

    std::ifstream f((input+file).c_str());
    long length;
    f.seekg(0, f.end);
    length = f.tellg();
    f.seekg(0, f.beg);
    char ch;
	int i = 0;

	do {
		ch = f.get();
		i++;
	} while (ch != 10);

	f.seekg(i);
    f.close();
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

void checkDirectory(std::string stat_file, std::string stat_directory, std::string input_directory, std::string output_directory) {
	// raportuj rozpoczecie skanowania
	// report (SCAN_RESULT, STARTED);
	// lista plikow w zadanym katalogu
	std::vector<std::string> files = getFilesFromDirectory(input_directory);

    // petla skanujaca dany katalog

    report(OUTPUT, output_directory);
    report(STAT, stat_file);

	for(std::vector<std::string>::iterator file = files.begin(); file != files.end(); file++) {
		// jesli plik nie ma poprawnej nazwy
		// przejdz do kolejnego
		if (isRequestFileNameIncorrect(*file)) {
			continue;
		}
		// rozpocznij sesje z danym plikiem
		// sprawdz, czy zadany plik zawiera zadanie HTTP
        report (START, *file);
		int request_state = request(input_directory, *file);
        if (request_state == SUCCESS) {
            int response_state = response(input_directory, getResponseFileFromRequestFile(*file));
		    report (FINISH, stat_file);
        } else {
            report (FINISH, stat_file);
        }
    }
}

