#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#define TEST_FILE		"/home/sam/test/hello.txt"

int main(void)
{
	int fd;

	if ((fd = open(TEST_FILE, O_CREAT | O_RDWR, S_IRUSER | S_IWUSER | S_IXUSER)) == -1) {
		printf("error, open file:%s error!\n", TEST_FILE);
		return -1;
	}

	printf("OK, fd=%d\n", fd);

	return 0;
}

