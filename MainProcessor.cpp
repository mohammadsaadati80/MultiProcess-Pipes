#include "defs.hpp"

using namespace std;

struct Word
{
    string key;
    int value;
};

vector<string> find_testcase_dir(string testcase_folder_address);
void write_final_result_to_file(string final_result);

int main(int argc, char const *argv[])
{
    string testcase_folder_address = argv[1];
    vector<string> testcase_dir = find_testcase_dir(testcase_folder_address);
    for (int i = 0; i < testcase_dir.size(); i++)
    {
        unlink((MAP_PIPE_NAME + to_string(i + 1)).c_str());
        mkfifo((MAP_PIPE_NAME + to_string(i + 1)).c_str(), 0666);
    }
    string reduce_result_pipe = REDUCE_PIPE_NAME;
    unlink(reduce_result_pipe.c_str());
    mkfifo(reduce_result_pipe.c_str(), 0666);
    int map_pipe_s[2];
    for (int i = 0; i < testcase_dir.size(); i++)
    {
        pipe(map_pipe_s);
        int p = fork();
        if (p > 0)
        {
            string output =  testcase_folder_address + DOLLORSIGN + to_string(i + 1);
            close(map_pipe_s[0]);
            write(map_pipe_s[1], output.c_str(), output.length() + 1);
            close(map_pipe_s[1]);
        }
        else if (p == 0)
        {
            char arg_input[1000];
            close(map_pipe_s[1]);
            read(map_pipe_s[0], arg_input, 1000);
            close(map_pipe_s[0]);
            char *args[] = {"./MapProcessor.out", arg_input, NULL};
            execv(args[0], args);
        }
    }
    int reduce_pipe_s[2];
    pipe(reduce_pipe_s);
    int p = fork();
    if (p > 0)
    {
        close(reduce_pipe_s[1]);
    }
    else if (p == 0)
    {
        close(reduce_pipe_s[0]);
        char arg_input[100000];
        string output = to_string(testcase_dir.size()) + DOLLORSIGN + to_string(reduce_pipe_s[1]);
        strcpy(arg_input, output.c_str());
        char *args[] = {"./ReduceProcessor.out", arg_input, NULL};
        execv(args[0], args);
    }
    char final_result[1000000];
    read(reduce_pipe_s[0], final_result, 1000000);
    close(reduce_pipe_s[0]);
    string s = final_result;
    write_final_result_to_file(s);
    unlink(reduce_result_pipe.c_str());
    for (int i = 0; i < testcase_dir.size(); i++)
        unlink((MAP_PIPE_NAME + to_string(i + 1)).c_str());
    return 0;
}

vector<string> find_testcase_dir(string testcase_folder_address)
{
    vector<string> testcase_dir;
    struct dirent *entry;
    DIR *dir = opendir(testcase_folder_address.c_str());
    while (entry = readdir(dir))
        if (entry->d_type == DT_REG)
            testcase_dir.push_back(testcase_folder_address + SLASH + entry->d_name);
    closedir(dir);
    return testcase_dir;
}

void write_final_result_to_file(string final_result)
{
    // ofstream out("output.csv");
    // out << final_result;
    // out.close();
    ofstream out(OUTPUT_FILE);
    stringstream templine(final_result);
    string data;
    while(getline(templine, data, COMMA))
        out << data << COMMA << endl;  
    out.close();
}