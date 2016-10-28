build: g++ -lrt -g metaopen.cpp -o metaopen

Command-line syntax:
   ./metaopen <file num> <directory list file>   
说明：   
file num: 每个目录下建立的文件个数
directory list file： 目标文件中存放的是测试的目录列表。每个目录会启动一个fork子进程，达到并发测试的效果。


example: ./metaopen 20000 mt.txt    在/172.16.149.63_fs/mt/{1|2|3|4}目录下并发同时建立20000文件。
mt.txt的内容：
/172.16.149.63_fs/mt/1/
/172.16.149.63_fs/mt/2/
/172.16.149.63_fs/mt/3/
/172.16.149.63_fs/mt/4/


循环批量脚本建议
命令：while true; do ./bc.sh;done
bc.sh内容：
mv /172.16.149.63_fs/mt /172.16.149.63_fs/mt_$(date +%s)
for i in {1..9};do mkdir -p /172.16.149.63_fs/mt/$i;done
./a.out 20000 mt.txt

