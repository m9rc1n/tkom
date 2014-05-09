#include "file.hpp"
#include <cstdio>
#include <fstream>
#include <string>

void help() {
    printf("HTTP Parser - Usage\n");
    printf("-s <stat_directory>\n");
    printf("-f <stat_file>\n");
    printf("-o <output_directory>\n");
    printf("-d <files_directory>\n");
}

int main(int argc, char** argv) {

    std::string stat_directory = "";
    std::string stat_file = "";
    std::string output_directory = "";
    std::string files_directory = "";

    std::ifstream settings_file("http_parser.conf");

    if (settings_file.is_open()) {
        for(std::string line; std::getline(settings_file, line);) {
            if (line.length() < 4) {
                continue;
            }
            if (line[0] != '-') {
                continue;
            }
            if (line[1] == 's') {
                stat_directory = line.substr(4);
            } else if (line[1] == 'd') {
                files_directory = line.substr(4);
            } else if (line[1] == 'f') {
                stat_file = line.substr(4);
            } else if (line[1] == 'o') {
                output_directory = line.substr(4);
            }
        }
    }

    // first parse file

    for (int i=1; i<argc; i++) {
        if (argv[i][0] == '-' && argv[i+1] != NULL) {
            switch(argv[i][1]) {
                case 's':
                    stat_directory = argv[i+1];
                    break;
                case 'f':
                    stat_file = argv[i+1];
                    break;
                case 'd':
                    files_directory = argv[i+1];
                    break;
                case 'o':
                    output_directory = argv[i+1];
                    break;
                case 'h':
                    help();
            }
        } else {
            help();
        }
    }

    if (output_directory.length() == 0) return -1;
    if (files_directory.length() == 0) return -1;
    if (stat_file.length() == 0) return -1;
    if (stat_directory.length() == 0) return -1;
    checkDirectory(stat_file, stat_directory, files_directory, output_directory);
	return 0;
}
