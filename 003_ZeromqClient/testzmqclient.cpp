#include "stdio.h"
#include "stdlib.h"
#include "zmq.h"

int main(int argc,char**argv)
{
    void* pCtx = NULL;
    void* pSock = NULL;
    
    const char* pAddr = "tcp://192.168.78.51:7788";
    
    if((pCtx = zmq_ctx_new()) == NULL)
    {
      return 0;
    }

    if((pSock = zmq_socket(pCtx,ZMQ_DEALER)) == NULL)
    {
      zmq_ctx_destroy(pCtx);
      return 0;
    }

    int iSndTimeout = 5000; //millsecond
    
    if(zmq_setsockopt(pSock,ZMQ_RCVTIMEO,&iSndTimeout,sizeof(iSndTimeout)) < 0)
    {
      zmq_close(pSock);
      zmq_ctx_destroy(pCtx);
      return 0;
    }
	
	if(zmq_connect(pSock,pAddr) < 0)
	{
		zmq_close(pSock);
		zmq_ctx_destroy(pCtx);
		return 0;
	}

    while(1)
    {
      static int i = 0;
      char szMsg[1024] = {0};
      snprintf(szMsg,sizeof(szMsg),"hello world :%3d",i++);
     printf("Enter to send...\n");
      if(zmq_send(pSock,szMsg,sizeof(szMsg),0) < 0)
      {
        fprintf(stderr,"send message failed!\n");
        continue;
      }

      printf("send message : [%s] succeed\n",szMsg);
      getchar();
    }

    return 0;
}
