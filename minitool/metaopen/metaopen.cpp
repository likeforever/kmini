#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <string>
#include <iostream>
#include <assert.h>

using namespace std;

long long GetMicroSecTime()
{
    struct timespec ts;
    int ret = clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec * 1000000 + ts.tv_nsec/1000);
}

long long GetMilliSecTime()
{
    return GetMicroSecTime()/1000;
}

string getTimeStr(){
	char szTime[128] = {0};

	time_t tmGmt = {0};
	struct tm* tmBeijing = NULL;
	time(&tmGmt);
	tmBeijing = localtime(&tmGmt);

        timeval curTime;
        gettimeofday(&curTime, NULL);
        int mill = curTime.tv_usec / 1000;

	sprintf(szTime,  "%04d-%02d-%02d %02d:%02d:%02d:%03d", (1900+tmBeijing->tm_year),(1+tmBeijing->tm_mon),tmBeijing->tm_mday,
		tmBeijing->tm_hour,tmBeijing->tm_min,tmBeijing->tm_sec, mill);

	string strCurTime = szTime;
	return strCurTime;

}

int main(int argc, char* argv[]){
    if (argc != 3) {
        printf("error para\n");
        return 0;
    }

    int filenum = atoi(argv[1]);
    string filelist = argv[2];

    ifstream fin(filelist.c_str());
    string line;
    vector<string> filenames;
    while (getline(fin, line)) {
        filenames.push_back(line);
    }

    for (int i = 0; i < filenames.size(); i++) {
        int pid = fork();
        assert(-1 != pid);
        string path  = filenames[i];
        if (0 == pid) {
	    sleep(4);
            long long avgOpen = 0, avgClose = 0;
            char filename[100] = {0};
            int j = 0;
            long long start = GetMicroSecTime();
            string startTime = getTimeStr();
            for (j = 0; j < filenum; j++) {
                sprintf(filename, "%s/kkkk_%d", path.c_str(), j);
                long long tm = GetMicroSecTime();

                int fd = ::open(filename, O_RDWR | O_CREAT, 00777);;

                avgOpen += (GetMicroSecTime() - tm);


                if(-1 == fd){
                    printf("open fail\n");
                }

                tm = GetMicroSecTime();
                close(fd);
                avgClose += (GetMicroSecTime() - tm);
            }
            long long end = GetMicroSecTime();
	    string endTime = getTimeStr();
            printf("fork[%d] avgopen[%lld], avgclose[%lld], start[%lld], starttime[%s], end[%lld], endtime[%s]\n", i, avgOpen / j, avgClose / j , start, startTime.c_str(), end, endTime.c_str());
            exit(0);
        }
    }

    int kid = -1;
    int status = 0;
    do {
        kid = waitpid(-1, &status, 0);
    } while (kid > 0);
    return 1;

}

