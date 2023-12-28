#include <stdio.h>
#include <map>

using namespace std;

enum errcodes{
    CONN,
    MESG,
    MERR,
    GONE
};

map<errcodes, char*> errorHandler(errcodes errcode){
    map<errcodes, char*> errmap;
    switch (errcode)
    {
    case CONN:
        errmap[errcode]="Connection error...\n";
        return errmap;
    case MESG:
        errmap[errcode]="Connection error...\n";
        return errmap;
    case MERR:
        errmap[errcode]="Connection error...\n";
        return errmap;
    case GONE:
        errmap[errcode]="Connection error...\n";
        return errmap;
    default:
        break;
    }
}
