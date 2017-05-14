#include "daemonizer.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <fstream>
#include <sstream>

#include "utils.h"
#include "../classes/Keyboard.h"

namespace daemonizer {
	
	void start(char *arg0, std::string scenarioFile) {
		pid_t pid, sid;
		
		pid = fork();
		if (pid < 0) exit(EXIT_FAILURE);
		if (pid > 0) exit(EXIT_SUCCESS);
		sid = setsid();
		if (sid < 0) exit(EXIT_FAILURE);
		signal(SIGCHLD,SIG_IGN); /* ignore child */
		signal(SIGHUP,SIG_IGN); /* ignore child */
		
		
		pid = fork();
		if (pid < 0) exit(EXIT_FAILURE);
		if (pid > 0) exit(EXIT_SUCCESS);
		umask(0);
		if (chdir("/") < 0) exit(EXIT_FAILURE);
		
		std::cout<<getpid()<<std::endl;
		
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		
		signal(SIGCHLD,SIG_IGN); /* ignore child */
		signal(SIGHUP,SIG_IGN); /* ignore child */
		signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
		signal(SIGTTOU,SIG_IGN);
		signal(SIGTTIN,SIG_IGN);		
		//signal(SIGINT, stop);
		
		LedKeyboard kbd;
		kbd.open();
		while(true){
			process(kbd, scenarioFile);
			usleep(1);
		}
	}
	/*
	void stop() { stop(2); }
	void stop(int signum) {
		// Set exit keyboard profile
		exit(signum);
	}
	*/
	void process(LedKeyboard &kbd, std::string scenarioFile) {
		static bool fileChanged = false;
		static std::string parsedFileString = "";
		static time_t scenaryFileLastUpdate = 0;
		
		if (! fileChanged) {
			struct stat file_stat;
			if (stat(scenarioFile.c_str(), &file_stat) == 0) {
				if(file_stat.st_mtime > scenaryFileLastUpdate) {
					fileChanged = true;
					scenaryFileLastUpdate = file_stat.st_mtime;
				}
			}
		}
		
		if (fileChanged) {
			fileChanged = false;
			std::ifstream file;
			file.open(scenarioFile);
			if (file.is_open()) {
				std::string line;
				parsedFileString = "";
				while (!file.eof()) {
					getline(file, line);
					if (line.substr(0, 2) == "p ") {
						std::ifstream profileFile;
						profileFile.open(line.substr(2));
						if (file.is_open()) {
							std::string profileLine;
							while (!profileFile.eof()) {
								getline(profileFile, profileLine);
								if (profileLine != "" && profileLine.substr(0,1) != "#") parsedFileString = parsedFileString + profileLine + "\n";
							}
							profileFile.close();
						}
					} else {
						if (line != "" && line.substr(0,1) != "#") parsedFileString = parsedFileString + line + "\n";
					}
				}
				file.close();
			}
		}
		
		std::istringstream parsedFile(parsedFileString);
		utils::parseProfile(kbd, parsedFile);
		
	}
}

