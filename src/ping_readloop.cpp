#include	"ping0.h"

extern pthread_mutex_t mtr;
extern pthread_mutex_t mt_req_send;
extern int send_stopped;
extern int req_stopped;


void * readloop(void  *ina) {
//void readloop(void) {
	int				size;
	char			recvbuf[BUFSIZE];
	char			controlbuf[BUFSIZE];
	struct msghdr	msg;
	struct iovec	iov;
	ssize_t			n;
	struct timeval	tval;


    struct proto    proto_v4 = { proc_v4, NULL, NULL, NULL, NULL, 0, IPPROTO_ICMP };
    pr = &proto_v4;

	//int sockrd = Socket(pr->sasend->sa_family, SOCK_RAW, pr->icmpproto);
	int sockrd = Socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	setuid(getuid());		/* don't need special permissions any more */
	if (pr->finit)
		(*pr->finit)();

	size = 60 * 1024;		/* OK if setsockopt fails */
	setsockopt(sockrd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	//sig_alrm(SIGALRM);		/* send first packet */

	iov.iov_base = recvbuf;
	iov.iov_len = sizeof(recvbuf);
	//msg.msg_name = pr->sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = controlbuf;
	for ( ; ; ) {
		//msg.msg_namelen = pr->salen;
		msg.msg_controllen = sizeof(controlbuf);
		n = recvmsg(sockrd, &msg, 0);
		if (n < 0) {
			if (errno == EINTR)
				continue;
			else
				err_sys("recvmsg error");
		}

		Gettimeofday(&tval, NULL);
        pthread_mutex_lock(&mtr);
		(*pr->fproc)(recvbuf, n, &msg, &tval);
        pthread_mutex_unlock(&mtr);
        
        pthread_mutex_lock(&mt_req_send);
        if(req_stopped == 1) {
            send_stopped = 1;
        }
        pthread_mutex_unlock(&mt_req_send);
    /*
      */
	}
    return NULL;
}
