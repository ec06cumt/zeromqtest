#include "stdio.h"
#include "stdlib.h"
#include "zmq.h"
#include "zmq_utils.h"

int main(int argc,char* *argv)
{

    void* pCtx = NULL;
    void* pSock = NULL;
    const char* pAddr = "tcp://*:7788";
    //create context,zmq socket 
    if((pCtx = zmq_ctx_new()) == NULL)
    {
       return 0;
    }
    
    if((pSock = zmq_socket(pCtx,ZMQ_DEALER)) == NULL)
    {
       zmq_ctx_destroy(pCtx);
       return 0;
    }

    int iRcvTimeout = 5000;    //millsecond

    //set timeout 5s
    if(zmq_setsockopt(pSock,ZMQ_RCVTIMEO,&iRcvTimeout,sizeof(iRcvTimeout)) < 0)
    {
      zmq_close(pSock);
      zmq_ctx_destroy(pCtx);
      return 0;
    }

    //bind ipaddress port:7788
    if(zmq_bind(pSock,pAddr) < 0)
    {
      zmq_close(pSock);
      zmq_ctx_destroy(pCtx);
      return 0;
    }
    printf("bind at:%s\n",pAddr);
    while(1)
    {
      char szMsg[1024] = {0};
      printf("waitting...\n");
      

      if(zmq_recv(pSock,szMsg,sizeof(szMsg),0) < 0)
      {
        continue;
      }

      printf("received message : %s\n",szMsg);
    }

    printf("argc=%d\n",argc);
    printf("test zmq project!\n");
    return 0;
}
