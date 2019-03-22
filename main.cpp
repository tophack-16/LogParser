#include <iostream>
#include <fstream>
#include <string>
#include <argp.h>
#include "stdbool.h"
#include <assert.h>
#include "SplendorBoardParser.cpp"

using namespace std;
using namespace Json;

// 抄的处理命令行的代码
const char *argp_program_version = "Splendor player beta";
const char *argp_program_bug_address = "zhanghuimeng1997@gmail.com";
static char doc[] = "Just a player AI.";
static char args_doc[] = "[FILENAME]...";
static struct argp_option options[] = {
        { "input", 'i', 0, 0, "Input json file (and print result to cmd)" },
        { 0 }
};

struct arguments {
    char* filename;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    arguments* arguments = static_cast<struct arguments*>(state->input);
    switch (key) {
        case 'i': arguments->filename = arg;
            cout << "worked" << endl;
            break;
        case ARGP_KEY_ARG: return 0;
        default: return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

// 不想和命令行斗争了。。。

int main(int argc, char **argv)
{
    /*
    arguments arguments;
    arguments.filename = nullptr;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    for (int i = 0; i < argc; i++)
        cout << i << "th: " << argv[i] << endl;
    cout << arguments.filename << endl;
    assert(arguments.filename == 0);*/
    char* filename = argv[1];
    string input, line;
    ifstream fin(filename);
    while (getline(fin, line)) input += line;
    Board board = boardParser(input);
//    cout << input << endl;
    /*
    std::string strValue = "{\"key1\":\"value1\",\"array\":[{\"key2\":\"value2\"},{\"key2\":\"value3\"},{\"key2\":\"value4\"}]}";

    Json::Reader reader;
    Json::Value value;

    if (reader.parse(strValue, value))
    {
        std::string out = value["key1"].asString();
        std::cout << out << std::endl;
        const Json::Value arrayObj = value["array"];
        for (int i=0; i<arrayObj.size(); i++)
        {
            out = arrayObj[i]["key2"].asString();
            std::cout << out;
            if (i != arrayObj.size() - 1)
                std::cout << std::endl;
        }
    }*/
    return 0;
}
