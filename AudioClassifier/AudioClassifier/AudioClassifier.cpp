#include "CommandLineFunctions.h"

#include <chrono>
#include <utility>
typedef std::chrono::high_resolution_clock::time_point TimeVar;

#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

int StartUp() {
	printf("\n");
	printf("   _____                       _    _____ _               _  __ _           \n");
	printf("  / ____|                     | |  / ____| |             (_)/ _(_)          \n");
	printf(" | (___   ___  _   _ _ __   __| | | |    | | __ _ ___ ___ _| |_ _  ___ _ __ \n");
	printf("  \\___ \\ / _ \\| | | | '_ \\ / _` | | |    | |/ _` / __/ __| |  _| |/ _ \\ '__|\n");
	printf("  ____) | (_) | |_| | | | | (_| | | |____| | (_| \\__ \\__ \\ | | | |  __/ |   \n");
	printf(" |_____/ \\___/ \\__,_|_| |_|\\__,_|  \\_____|_|\\__,_|___/___/_|_| |_|\\___|_|   \n");
	printf("                                                                            \n");

	printf("Copyright © 2021 Michael Kuperfish Steinberg\n\n\n");

	ChangeOutputPath();
	ChangeDataSetName();

	while (true) {
		if (ProccessCommands() == 1) {
			printf("# Command Failed!\n");
		}
	}

	return 0;
}

int main(int argc, char ** argv)
{
	return StartUp();
}