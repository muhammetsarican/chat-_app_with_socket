#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>

#define SOCKET int

using namespace std;

struct Person
{
    char name[15] = "";
    char messageToWho[15] = "";
    char message[100] = "";
};

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
    string listHeader = "\n\n----------------------------------\n|----- Welcome to the Server -----\n|\n|# User List #\n| Global\n ";

    size_t totalLength = listHeader.length();

    if (users.empty())
    {
        // # User List #
        char *convertedstr = new char[totalLength + 1];

        strncpy(convertedstr, listHeader.c_str(), totalLength);

        return convertedstr;
    }
    else
    {
        for (size_t i = 0; i <= users.size() - 1; ++i)
        {
            totalLength += strlen(users[i].name) + 1;
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

        return convertedstr;
    }
}