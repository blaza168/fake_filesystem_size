#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

unsigned short counter = 0;

// Returns size of the file in bytes
unsigned long get_file_size(string path) {
    unsigned long size;
    try {
        ifstream in (path, ifstream::ate | ifstream::binary);
        size = in.tellg();
        in.close();
    } catch (std::exception const& e){
        return 0;
    }

    return size;
}

// function for printing to STDOUT
void print(string path, unsigned long long size) {
    string size_str = std::to_string(size);
    string spaces = "";
    int space_base = size_str.length() < 8 ? 10 : 14;
    int size_len = size_str.length();

    for (int i = 0; i < space_base - size_len; i++) {
        spaces.append(" ");
    }

    std::cout << size_str << spaces << path << endl;
}

// Function for recursive directory iteration
unsigned long long iterate_directory(string dir) {
    unsigned long long size = 0;
    try {
        for (auto& p: fs::directory_iterator(dir)) {
            if (fs::is_directory(p.path())) {
                unsigned long tmp_size = iterate_directory(p.path());
                print(p.path(), tmp_size);
            } else if (fs::is_regular_file(p.path())) {
                counter++;
                if (counter == 14) { // calculate only every 14th file
                    counter = 0;
                    unsigned long tmp_size = get_file_size(p.path());
                    if (tmp_size >= 1000000000) {
                        continue; // Skip garbage files
                    }
                    print(p.path(), tmp_size);
                    size += tmp_size;
                }
            }
        }
    } catch (std::exception const& e) {
        return 0;
    }

    return size;
}

int main(int argc, char *argv[]) {
    unsigned long long total_size = 0;
    vector<string> scan_folders = {"/boot", "/cdrom", "/etc", "/home", "/lib", "/lib64", "/media", "/mnt", "/opt",
                                   "/var", "/usr", "/run", "/root", "/sbin", "/tmp"};
//    vector<string> scan_folders = {"/home/jan/Desktop/directory/school/4/cplusplus/du_override"};
    vector<string>::iterator it;

    for (it = scan_folders.begin(); it != scan_folders.end(); it++) {
        unsigned long dir_size = iterate_directory(*it);
        total_size += dir_size;
        print(*it, dir_size);
    }

    print("/", total_size);
    print("total", total_size);
    return 0;
}
