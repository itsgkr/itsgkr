#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdlib.h>

#define FILE_NAME_LENGTH 256
#define snprintf_nowarn(...) (snprintf(__VA_ARGS__) < 0 ? abort() : (void)0)

int main(int argc, char** argv){
	DIR *proc_dir_list;
	struct dirent *directory_entry;
	int ps_count, fd_self, ps_desc;
	char flag, *active_process;
	char proces_name[FILE_NAME_LENGTH], current_process[FILE_NAME_LENGTH], path[FILE_NAME_LENGTH], time_s[FILE_NAME_LENGTH];

	proc_dir_list = opendir("/proc");
	fd_self = open("/proc/self/fd/0", O_RDONLY);
	snprintf_nowarn(current_process, (FILE_NAME_LENGTH-1), "%s", ttyname(fd_self));
	printf("%5s %s\n", "PID", "NAME");
	while ((directory_entry = readdir(proc_dir_list)) != NULL) {
		flag = 1;
		for (ps_count = 0; directory_entry->d_name[ps_count]; ps_count++)
		if (!isdigit(directory_entry->d_name[ps_count])) { 
			flag = 0;
			break;
		}

		if (flag) {
			snprintf_nowarn(path, (FILE_NAME_LENGTH-1),"/proc/%s/fd/0", directory_entry->d_name);
			ps_desc = open(path, O_RDONLY);
			active_process = ttyname(ps_desc);

			if (active_process && strcmp(active_process, current_process) == 0) {
				FILE* ps_stat;
				int pid;
				snprintf_nowarn(path, (FILE_NAME_LENGTH-1),"/proc/%s/stat", directory_entry->d_name);
				ps_stat = fopen(path, "r");
				fscanf(ps_stat, "%d%s", &pid, proces_name);
				proces_name[strlen(proces_name) - 1] = '\0';
	 
				printf("%5s %s\n", directory_entry->d_name, proces_name+1);
				fclose(ps_stat);
			}
			close(ps_desc);
		}
    }
	close(fd_self);
	return 0;
}