#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>

#define SOCKET int

using namespace std;

struct userDetail
{
    char *name;
    SOCKET socket;
};

struct serverAdresses
{
    const char hostAddress[10] = "127.0.0.1";
    int portAddress = 8888;
};

char *vectorToChar(vector<userDetail> &users)
{
    if (users.empty())
    {
        return nullptr;
    }
    // Convert vector to string
    string listHeader = "----- Welcome to the Server -----\n\n# User List #\nGlobal\n";

    size_t totalLength = listHeader.length();

    for (size_t i = 0; i <= users.size() - 1; ++i)
    {
        totalLength += strlen(users[i].name)+1;
    }
    // # User List #
    char *convertedstr = new char[totalLength + 1];

    strncpy(convertedstr, listHeader.c_str(), totalLength);

    size_t currentPos = listHeader.length();
    for (size_t i = 0; i < users.size(); ++i)
    {
        strncpy(&convertedstr[currentPos], users[i].name, strlen(users[i].name));
        currentPos += strlen(users[i].name);
    }
    printf("User List: %s\n", convertedstr);
    return convertedstr;
}