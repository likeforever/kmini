
/*
  Copyright (C) 2014 SourceXR

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include <sys/epoll.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char **argv) {

    if (argc < 3) {
        std::cerr << "Use " << argv[0] << " host port\n";
        return 0;
    }
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset (&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction (SIGPIPE, &sa, 0);

    const int size = 10;
    int epollfd = epoll_create (size);
    if (epollfd == -1) {
        std::cerr << strerror (errno) << "\n";
        return 1;
    }

    // SocketSetup
    int fd = socket (AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (fd == -1) {
        std::cerr << strerror (errno) << "\n";
        return 1;
    }
    // SocketSetup

    // Connect
    // issue connect (may be established immediately)
    const char *ip = argv[1];
    const unsigned short port = atoi (argv[2]);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons (port);
    inet_aton (ip, &addr.sin_addr);

    bool connected = true;
    int i = connect (fd, reinterpret_cast<sockaddr *> (&addr),
                     sizeof (sockaddr_in));
    if ((i == -1)
        && (errno == EINPROGRESS)) {
        // connection not yet established, use reactor
        connected = false;
    }
    // Connect

    epoll_event event;

    // InProgress
    // add fd to reactor
    event.events = EPOLLIN;
    if (!connected) {
        event.events |= EPOLLOUT; // connection established is a write event
    }
    event.data.fd = fd; // user data
    int r = epoll_ctl (epollfd, EPOLL_CTL_ADD, fd, &event);
    if (r == -1) {
        std::cerr << strerror (errno) << "\n";
        close (epollfd);
        close (fd);
        return 1;
    }
    // InProgress

    // watch possibly other fds
    // ...

    // Wait
    while (true) {

        epoll_event events[size];
        const int infinity = -1;
        // and wait for events
        int r = epoll_wait (epollfd, events, size, infinity);
        if (r == -1) {
            close (epollfd);
            close (fd);
            return 1;
        }

        // demultiplex events
        int i = 0;
        while (i < r) {
            if (events[i].data.fd == fd) {
                if (events[i].events & EPOLLOUT) {
                    if (!connected) {
                        // connection established
                        // check pending error if ever
                        int err = 0;
                        socklen_t len = sizeof (int);
                        int i = getsockopt (fd, SOL_SOCKET, SO_ERROR, &err,
                                            &len);
                        if (i != -1) {
                            if (err == 0) {
                                std::cout << "Connection established\n";
                                // disable write event: to reenable when
                                // data are to be sent on the socket
                                epoll_event event;
                                event.events = EPOLLIN;
                                event.data.fd = fd; // user data
                                int r = epoll_ctl (epollfd, EPOLL_CTL_MOD,
                                                   fd, &event);
                                if (r == -1) {
                                    std::cerr << strerror (errno) << "\n";
                                    close (epollfd);
                                    close (fd);
                                    return 1;
                                }
                            }
                            else {
                                // connection failed
                                std::cerr << strerror (err) << "\n";
                                epoll_event event;
                                event.events = 0;
                                event.data.fd = fd; // user data
                                epoll_ctl (epollfd, EPOLL_CTL_DEL,
                                            fd, &event);
                                close (fd);
                            }
                            // Wait
                        }
                        else {
                            std::cerr << strerror (errno) << "\n";
                            epoll_event event;
                            event.events = 0;
                            event.data.fd = fd; // user data
                            epoll_ctl (epollfd, EPOLL_CTL_DEL, fd, &event);
                            close (fd);
                        }
                        connected = true;
                    }
                    else {
                        // usual write event on the socket
                    }
                }
                if (events[i].events & EPOLLERR) {
                    epoll_event event;
                    event.events = 0;
                    event.data.fd = fd; // user data
                    epoll_ctl (epollfd, EPOLL_CTL_DEL, fd, &event);
                    int err = 0;
                    socklen_t len = sizeof (int);
                    int i = getsockopt (fd, SOL_SOCKET, SO_ERROR, &err,
                                        &len);
                    if (i != -1) {
                        // connection failed
                        std::cerr << "Connection failed: "
                                  << strerror (err) << "\n";
                        close (fd);
                    }
                }
                // other events on the socket
                // ...
            }
            ++i;
        }
    }
    close (epollfd);
    close (fd);
    return 0;
}

