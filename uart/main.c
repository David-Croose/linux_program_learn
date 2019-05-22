#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define SIM7600_TRANSPARENT_MODE    "AT+CIPMODE=1\r\n"
#define SIM7600_OPEN_NETWORK        "AT+NETOPEN\r\n"
#define SIM7600_CONNECT_TO_SERVER   "AT+CIPOPEN=0,\"TCP\",\"183.15.176.237\",6677\r\n"

/**
 * seting up the uart device
 * @param fd: the file description of the uart device
 * @param baudrate: how many bits transfered per seconds. could be the following values:
 *                  0 50 75 110 134 150 200 300 600 1200 1800 2400 4800 9600 19200
 *                  38400 57600 115200 128000 230400 256000 460800 500000 576000 921600
 *                  1000000 1152000 1500000 2000000 2500000 3000000
 * @param bits: the data bits. could be 7 or 8
 * @param check: the check bit. could be 'O', 'E', or 'N'
 * @param stopbit: the stop bit. could be 1 or 2
 * @return: 0 --- ok
 *          other --- error
 */
int uart_set(int fd, int baudrate, int bits, char check, int stopbit)
{
    struct termios newtio, oldtio;
    speed_t speed;

    if(tcgetattr(fd, &oldtio) != 0)
    {
        perror("tcgetattr error");
        return -1;
    }
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch(bits)
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;

    default:
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch(check)
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;

    case 'E':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;

    default:
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch(baudrate)
    {
    case 0:       speed = B0;       break;
    case 50:      speed = B50;      break;
    case 75:      speed = B75;      break;
    case 110:     speed = B110;     break;
    case 134:     speed = B134;     break;
    case 150:     speed = B150;     break;
    case 200:     speed = B200;     break;
    case 300:     speed = B300;     break;
    case 600:     speed = B600;     break;
    case 1200:    speed = B1200;    break;
    case 1800:    speed = B1800;    break;
    case 2400:    speed = B2400;    break;
    case 4800:    speed = B4800;    break;
    case 9600:    speed = B9600;    break;
    case 19200:   speed = B19200;   break;
    case 38400:   speed = B38400;   break;
    case 57600:   speed = B57600;   break;
    case 115200:  speed = B115200;  break;
    case 128000:  speed = B128000;  break;
    case 230400:  speed = B230400;  break;
    case 256000:  speed = B256000;  break;
    case 460800:  speed = B460800;  break;
    case 500000:  speed = B500000;  break;
    case 576000:  speed = B576000;  break;
    case 921600:  speed = B921600;  break;
    case 1000000: speed = B1000000; break;
    case 1152000: speed = B1152000; break;
    case 1500000: speed = B1500000; break;
    case 2000000: speed = B2000000; break;
    case 2500000: speed = B2500000; break;
    case 3000000: speed = B3000000; break;
    default:      speed = B9600;
    }
    cfsetispeed(&newtio, speed);
    cfsetospeed(&newtio, speed);

    switch(stopbit)
    {
    case 2:
        newtio.c_cflag |= CSTOPB;
        break;

    default:
    case 1:
        newtio.c_cflag &= ~CSTOPB;
        break;
    }

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd, TCIFLUSH);
    if((tcsetattr(fd, TCSANOW, &newtio)) != 0)
    {
        perror("set error");
        return -1;
    }

    return 0;
}

int main(int argc, char** argv)
{
    int fd;
    char uart[100] = {0};

    if(argc != 2)
    {
        printf("usage: ./a.out /dev/ttyS*\n");
        return -1;
    }
    strcpy(uart, argv[1]);
    printf("you enter device: %s\n", uart);

    if((fd = open(uart, O_RDWR | O_NOCTTY)) < 0)
    {
        printf("error. the device(%s) open failed!\n");
        return -2;
    }

    if(uart_set(fd, 115200, 8, 'N', 1) != 0)
    {
        printf("error. @uart_set failed!");
        return -3;
    }

    write(fd, SIM7600_TRANSPARENT_MODE, strlen(SIM7600_TRANSPARENT_MODE));
    sleep(1);

    write(fd, SIM7600_OPEN_NETWORK, strlen(SIM7600_OPEN_NETWORK));
    sleep(1);

    write(fd, SIM7600_CONNECT_TO_SERVER, strlen(SIM7600_CONNECT_TO_SERVER));

    close(fd);
    return 0;
}
