#include "client\include\asyncgameclientconnector.h"

AsyncGameClientConnector::AsyncGameClientConnector()
{

}

AsyncGameClientConnector::~AsyncGameClientConnector()
{
    thread.quit();
    thread.wait();
}

