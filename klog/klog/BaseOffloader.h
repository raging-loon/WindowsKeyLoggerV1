#ifndef BASE_OFFLOADER_H_
#define BASE_OFFLOADER_H_
#include "types.h"

#include <vector>
class BaseOffloader
{
public:

    // virtual bool testConnection() = 0;
    virtual void sendData(const KeyBuffer& buffer) = 0;


};

#endif // BASE_OFFLOADER_H_