/* #include <stdio.h>
#include <string.h>
#include <vector>
#include <string> */

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <pthread.h>

#include <vector>
#include <string>
#include <map>

#define SOCKET int

using namespace std;

// Enums
enum connType
{
    SERVER,
    CLIENT
};

enum errCodes
{
    CONN,
    MESG,
    MERR,
    GONE
};

// Structs
struct Person
{
    char name[15] = "";
    char messageToWho[15] = "";
    char message[100] = "";
};

struct Msg
{
    errCodes err;
    Person person;
    connType type;
};

struct UserDetail
{
    // char *name;
    char name[15] = "";
    SOCKET socket;
};

struct serverAdresses
{
    const char hostAddress[10] = "127.0.0.1";
    int portAddress = 8888;
};

// Functions
char *vectorToChar(vector<UserDetail> &users)
{
    string listHeader = "\n\n----------------------------------\n|----- Welcome to the Server -----\n|\n|# User List #\n| Global\n";

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

string getErrCode(errCodes errcode)
{
    switch (errcode)
    {
    case CONN:
    { /* code */
        return "CONN|";
    }
    case MESG:
    { /* code */
        return "MESG|";
    }
    case MERR:
    { /* code */
        return "MERR|";
    }
    case GONE:
    { /* code */
        return "GONE|";
    }

    default:
        break;
    }
    return "MERR";
}

string getConnType(connType conntype)
{
    switch (conntype)
    {
    case SERVER:
    { /* code */
        return "SERVER";
    }
    case CLIENT:
    { /* code */
        return "CLIENT";
    }

    default:
        break;
    }
    return "ERR";
}

string generateMsg(Msg msg)
{
    string message = getErrCode(msg.err);

    message += (string)msg.person.name + "|" + (string)msg.person.messageToWho + "|" + (string)msg.person.message + "|";

    message += getConnType(msg.type);

    printf("generated message: %s\n", message.c_str());
    return message;
}

errCodes encodeErrCode(string errcode)
{
    printf("you are in encode err function\n");
    if (errcode.find("CONN") != -1)
    { /* code */
        printf("we assign your parameter as CONN\n");
        return CONN;
    }
    if (errcode.find("MESG") != -1)
    { /* code */
        printf("we assign your parameter as MESG\n");
        return MESG;
    }
    if (errcode.find("MERR") != -1)
    { /* code */
        printf("we assign your parameter as MERR\n");
        return MERR;
    }
    if (errcode.find("GONE") != -1)
    { /* code */
        printf("we assign your parameter as GONE\n");
        return GONE;
    }
    return MERR;
}

connType encodeConnType(string conntype)
{
    if (conntype.find("SERVER") != -1)
    { /* code */
        return SERVER;
    }
    if (conntype.find("CLIENT") != -1)
    { /* code */
        return CLIENT;
    }
    return CLIENT;
}

void assignPartsToMsg(Msg *msg, string param, int paramCount)
{
    printf("Parameter: %s\n", param.c_str());
    switch (paramCount)
    {
    case 1:
    { /* code */
        msg->err = encodeErrCode(param);
        break;
    }
    case 2:
    { /* code */
        strcpy(msg->person.name, param.c_str());
        break;
    }
    case 3:
    { /* code */
        strcpy(msg->person.messageToWho, param.c_str());
        break;
    }
    case 4:
    { /* code */
        strcpy(msg->person.message, param.c_str());
        break;
    }
    case 5:
    { /* code */
        msg->type = encodeConnType(param);
        break;
    }

    default:
        break;
    }
}

void encodeMsg(Msg *msg, string message)
{
    printf("You are in encode msg function\n");
    size_t nextIndex = 0;
    int foundParam = 0;

    string partOfMessage;
    while (nextIndex < message.length())
    {
        int index = message.find("|", nextIndex);
        if (index == -1)
        {
            partOfMessage = message.substr(nextIndex, index - nextIndex);
            assignPartsToMsg(msg, partOfMessage, 5);
            break;
        }
        foundParam += 1;
        partOfMessage = message.substr(nextIndex, index - nextIndex);
        assignPartsToMsg(msg, partOfMessage, foundParam);

        nextIndex = index + 1;
    }
}