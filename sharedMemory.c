#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

int main() {
    int shmid;
    key_t key;
    char *shmaddr;
    char buffer[SHM_SIZE];

    // Generate a unique key
    key = ftok(".", 'a');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Create the shared memory segment
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    printf("Enter a message to send: ");
    fgets(buffer, SHM_SIZE, stdin);

    // Write to the shared memory
    strcpy(shmaddr, buffer);

    printf("Message sent to shared memory: %s", shmaddr);

    // Detach the shared memory segment
    if (shmdt(shmaddr) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}









//PRODUCER 

#include<sys/stat.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
int main()
{
    const char *name="0S";
    int count,i;
    char s =' ';
    printf(" enter the number of strings");
    scanf("%d",&count);
    char words[count][10];
    for(i=0; i<count; i++)
    {
        printf("enter the words\n");
        scanf("%s",words[i]);
        strncat(words[i],&s,1);
    }
    int id;
    void *ptr;
    id=shm_open(name,O_CREAT|O_RDWR,0666);
    ftruncate(id,1024);
    ptr=mmap(0,1024,PROT_WRITE,MAP_SHARED,id,0);
    for(i=0; i<count; i++)
    {
        sprintf(ptr,"%s",words[i]);
        int length=strlen(words[i]);
        ptr+=(length);
    }
    return 0;
}





//CONSUMER

#include<sys/stat.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
int main()
{
    const char *name="0S";
    int id;
    char*ptr;
    id=shm_open(name,O_RDONLY,0666);
    ftruncate(id,1024);
    ptr=mmap(0,1024,PROT_READ,MAP_SHARED,id,0);
    printf("data read from the memory is \n %s",ptr);
    shm_unlink(name);
    return 0;
}
