build: g++ -lrt -g metaopen.cpp -o metaopen

Command-line syntax:
   ./metaopen <file num> <directory list file>   
˵����   
file num: ÿ��Ŀ¼�½������ļ�����
directory list file�� Ŀ���ļ��д�ŵ��ǲ��Ե�Ŀ¼�б�ÿ��Ŀ¼������һ��fork�ӽ��̣��ﵽ�������Ե�Ч����


example: ./metaopen 20000 mt.txt    ��/172.16.149.63_fs/mt/{1|2|3|4}Ŀ¼�²���ͬʱ����20000�ļ���
mt.txt�����ݣ�
/172.16.149.63_fs/mt/1/
/172.16.149.63_fs/mt/2/
/172.16.149.63_fs/mt/3/
/172.16.149.63_fs/mt/4/


ѭ�������ű�����
���while true; do ./bc.sh;done
bc.sh���ݣ�
mv /172.16.149.63_fs/mt /172.16.149.63_fs/mt_$(date +%s)
for i in {1..9};do mkdir -p /172.16.149.63_fs/mt/$i;done
./a.out 20000 mt.txt

