/*
 * listener.c -- joins a multicast group and echoes all data it receives from
 *              the group to its stdout...
 *
 * Antony Courtney,     25/11/94
 */


#include <stdio.h>
#include <stdlib.h>

#include <strings.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/utsname.h>


#define MSGBUFSIZE 256

main(int argc, char *argv[])
{
     struct sockaddr_in addr;
     int fd, nbytes,addrlen;
     struct ip_mreq mreq;
     char msgbuf[MSGBUFSIZE];

     if (argc < 3) {
       printf("Usage: a.out address port.\n");
       exit(1);
     }

     /* create what looks like an ordinary UDP socket */
     if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
          perror("socket");
          exit(1);
     }

     /* set up destination address */
     memset(&addr,0,sizeof(addr));
     addr.sin_family=AF_INET;
     addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
     addr.sin_port=htons((short)(atoi(argv[2])));

     /* bind to receive address */
     if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
          perror("bind");
          exit(1);
     }

     /* use setsockopt() to request that the kernel join a multicast group */
     mreq.imr_multiaddr.s_addr=inet_addr(argv[1]);
     mreq.imr_interface.s_addr=htonl(INADDR_ANY);
     if (setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
          perror("setsockopt");
          exit(1);
     }

     /* now just enter a read-print loop */
     while (1) {
          addrlen=sizeof(addr);
          if ((nbytes=recvfrom(fd,msgbuf,MSGBUFSIZE,0,
                               (struct sockaddr *) &addr,&addrlen)) < 0) {
               perror("recvfrom");
               exit(1);
         }
	printf("from %s %d bytes: %s\n", inet_ntoa(addr.sin_addr),
                  nbytes, msgbuf);
     }
}

