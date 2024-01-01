#include <stdio.h>
#include <string.h>

using namespace std;

struct person
{
    char name[15] = "";
    char messageToWho[15] = "";
    char message[100] = "";
};

person describeYourself()
{
    person user;

    printf("Enter your name: ");
    fgets(user.name, 15, stdin);

    return user;
}

person subOfString(person user, char comingMessage[100], int endIndex)
{
    char messageToWho[15] = "";
    char message[100] = "";

    for (int i = 0; i < strlen(comingMessage); i++)
    {
        if (i < endIndex)
        {
            messageToWho[i] = comingMessage[i];
        }
        if (i >= endIndex + 2)
        {
            message[i - (endIndex + 2)] = comingMessage[i];
        }
    }

    strcpy(user.messageToWho, messageToWho);
    strcpy(user.message, message);

    return user;
}

person messageFormat(person user, char message[100])
{
    for (int i = 0; i < strlen(message); i++)
    {
        if (message[i] == '-')
        {
            if (message[i + 1] == '>')
            {
                user = subOfString(user, message, i);
            }
        }
    }

    return user;
}

person getMessage(person user)
{
    if (strlen(user.name) == 0)
    {
        user = describeYourself();
    }
    else
    {
        printf("You introduced yourself successfully.\n");
    }

    char message[100] = "";

    printf("Please enter your message (Format: friend_name->message): ");
    fgets(message, 100, stdin);

    user = messageFormat(user, message);

    return user;
}
