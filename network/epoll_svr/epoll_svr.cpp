#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#define MAXEVENTS 64

using namespace std;

void setSocketAttr(int s){
        int optval;
        socklen_t optlen = sizeof(optval);

        /* Set the option active */
        optval = 1;
        optlen = sizeof(optval);
        if (setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
                cout << "set SO_KEEPALIVE err:" << errno << endl;
                close(s);
                exit(EXIT_FAILURE);
        }
        printf("SO_KEEPALIVE set on socket\n");


        int val = 0; 
        if (getsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &val, &optlen) < 0 ){
                exit(0);
        }
        cout << "getsockopt SO_KEEPALIVE :" << val << endl;

        optval = 10;
        if (setsockopt(s, SOL_TCP, TCP_KEEPIDLE, &optval, optlen) < 0) {
                cout << "set TCP_KEEPIDLE err:" << errno << endl;
                close(s);
                exit(EXIT_FAILURE);
        }
        printf("TCP_KEEPIDLE set on socket\n"); 

        optval = 5;
        if (setsockopt(s, SOL_TCP, TCP_KEEPINTVL, &optval, optlen) < 0) {
                cout << "set TCP_KEEPINTVL err:" << errno << endl;
                close(s);
                exit(EXIT_FAILURE);
        }
        printf("TCP_KEEPINTVL set on socket\n"); 

        optval = 2;
        if (setsockopt(s, SOL_TCP, TCP_KEEPCNT, &optval, optlen) < 0) {
                cout << "set TCP_KEEPCNT err:" << errno << endl;
                close(s);
                exit(EXIT_FAILURE);
        }
        printf("TCP_KEEPCNT set on socket\n"); 
}

static int
make_socket_non_blocking(int sfd) {
        int flags, s;


        flags = fcntl(sfd, F_GETFL, 0);
        if (flags == -1) {
                perror("fcntl");
                return -1;
        }

        flags |= O_NONBLOCK;
        s = fcntl(sfd, F_SETFL, flags);
        if (s == -1) {
                perror("fcntl");
                return -1;
        }

        return 0;
}

static int
create_and_bind(int ip, char *port) {
        int s, sfd;
        struct sockaddr_in svr_addr;
        memset(&svr_addr, 0, sizeof(sockaddr_in));
        svr_addr.sin_family = 0;
        svr_addr.sin_addr.s_addr = ip; 
        svr_addr.sin_port = htons(atoi(port));

        sfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sfd == -1) {
                cout << "create socket failed  err:" << errno << endl;
                exit(0);
        }

        s = bind(sfd, (sockaddr*)&svr_addr, sizeof(svr_addr));
        if (0 != s) {
                cout << "bind socket failed err:" << errno << endl;
                exit(0);
        }
        return sfd; 
}


int
main(int argc, char *argv[]) {
        int sfd, s;
        int efd;
        struct epoll_event event;
        struct epoll_event *events;

        if (argc != 3) {
                fprintf(stderr, "Usage: %s [ip] [port]\n", argv[0]);
                exit(EXIT_FAILURE);
        }

        sfd = create_and_bind(inet_addr(argv[1]), argv[2]);
        if (sfd == -1) abort();

        s = make_socket_non_blocking(sfd);
        if (s == -1) abort();

        s = listen(sfd, SOMAXCONN);
        if (s == -1) {
                perror("listen");
                abort();
        }

        efd = epoll_create1(0);
        if (efd == -1) {
                perror("epoll_create");
                abort();
        }

        event.data.fd = sfd;
        event.events = EPOLLIN | EPOLLET;
        s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
        if (s == -1) {
                perror("epoll_ctl");
                abort();
        }

        /* Buffer where events are returned */
        events = (epoll_event*)calloc(MAXEVENTS, sizeof event);

        /* The event loop */
        while (1) {
                int n, i;

                n = epoll_wait(efd, events, MAXEVENTS, -1);
                for (i = 0; i < n; i++) {
                        cout << "epoll_wait " << i << " fd:" << events[i].data.fd << ", event:" << events[i].events << endl;
                        if ((events[i].events & EPOLLERR) ||
                            (events[i].events & EPOLLHUP) ||
                            (!(events[i].events & EPOLLIN ))) {
                                /* An error has occured on this fd, or the socket is not
                                   ready for reading (why were we notified then?) */
                                fprintf(stderr, "epoll error\n");
                                close(events[i].data.fd);
                                continue;
                        } else if (sfd == events[i].data.fd) {
                                /* We have a notification on the listening socket, which
                                   means one or more incoming connections. */
                                while (1) {
                                        struct sockaddr in_addr;
                                        socklen_t in_len;
                                        int infd;
                                        char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

                                        in_len = sizeof in_addr;
                                        infd = accept(sfd, &in_addr, &in_len);
                                        if (infd == -1) {
                                                if ((errno == EAGAIN) ||
                                                    (errno == EWOULDBLOCK)) {
                                                        /* We have processed all incoming
                                                           connections. */
                                                        break;
                                                } else {
                                                        perror("accept");
                                                        break;
                                                }
                                        }

                                        s = getnameinfo(&in_addr, in_len,
                                                        hbuf, sizeof hbuf,
                                                        sbuf, sizeof sbuf,
                                                        NI_NUMERICHOST | NI_NUMERICSERV);
                                        if (s == 0) {
                                                printf("Accepted connection on descriptor %d "
                                                       "(host=%s, port=%s)\n", infd, hbuf, sbuf);
                                        }

                                        /* Make the incoming socket non-blocking and add it to the
                                           list of fds to monitor. */
                                        s = make_socket_non_blocking(infd);
                                        if (s == -1) abort();

                                        setSocketAttr(infd);

                                        event.data.fd = infd;
                                        event.events = EPOLLIN | EPOLLET;
                                        s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
                                        if (s == -1) {
                                                perror("epoll_ctl");
                                                abort();
                                        }
                                }
                                continue;
                        } else {
                                /* We have data on the fd waiting to be read. Read and
                                   display it. We must read whatever data is available
                                   completely, as we are running in edge-triggered mode
                                   and won't get a notification again for the same
                                   data. */
                                int done = 0;

                                while (1) {
                                        ssize_t count;
                                        char buf[512];

                                        count = read(events[i].data.fd, buf, sizeof buf);
                                        if (count == -1) {
                                                /* If errno == EAGAIN, that means we have read all
                                                   data. So go back to the main loop. */
                                                if (errno != EAGAIN) {
                                                        cout << "read err:" << errno << endl;
                                                        done = 1;
                                                }
                                                break;
                                        } else if (count == 0) {
                                                /* End of file. The remote has closed the
                                                   connection. */
                                                cout << "read endof file:" << events[i].data.fd << " err:" << errno << endl;
                                                done = 1;
                                                break;
                                        }

                                        /* Write the buffer to standard output */
                                        s = write(1, buf, count);
                                        if (s == -1) {
                                                perror("write");
                                                abort();
                                        }

                                        const string replySize = "svr reply!\n";
                                        s = write(events[i].data.fd, replySize.c_str(), replySize.size());
                                        if (s == -1) {
                                                perror("reply err!");
                                                abort();
                                        }
                                }

                                if (done) {
                                        printf("Closed connection on descriptor %d\n",
                                               events[i].data.fd);

                                        /* Closing the descriptor will make epoll remove it
                                           from the set of descriptors which are monitored. */
                                        close(events[i].data.fd);
                                }
                        }
                }
        }

        free(events);

        close(sfd);

        return EXIT_SUCCESS;
}
