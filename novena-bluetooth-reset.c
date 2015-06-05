#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>

static void kick_bluetooth(void) {
        static int event_count = 0;

	fprintf(stderr, "%d: Resetting bluetooth\n", event_count++);
	system("/bin/hciconfig hci0 down");
	system("/bin/hciconfig hci0 up");
}

static int open_by_name(const char *name) {

        DIR *devinput = opendir("/dev/input");
        char devname[256];
        struct dirent *fil;
        int fd = -1;
        
        while ((fil = readdir(devinput)) != NULL) {
                char fullname[256];

                snprintf(fullname, sizeof(fullname)-1, "%s/%s",
                                "/dev/input",fil->d_name);
                fd = open(fullname, O_RDONLY);
                if (fd == -1)
                        continue;

                ioctl(fd, EVIOCGNAME(sizeof(devname)), devname);

                if (!strcmp(name, devname))
                        break;
                close(fd);
        }

        closedir(devinput);

        return fd;
}

int main(int argc, char **argv) {
        int fd = open_by_name("20b8000.kpp");
        struct input_event event;

        if (-1 == fd) {
                perror("Unable to open 20b8000.kpp");
                return 1;
        }

        while (read(fd, &event, sizeof(event)) == sizeof(event)) {
		if ((event.type == EV_KEY)
		 && (event.code == KEY_CONFIG)
		 && (event.value)) {
			kick_bluetooth();
		}
        }

        close(fd);

        return 0;
}
