/*
 * This program gets the state of the chatmix dial on arctis headsets and sends 
 * a notification if it is not in the middle.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    /*
    * The argument is the interval to check the chatmix dial in seconds. Default
    * is 5 seconds.
    */
    int interval;
    int shouldExit = 0;
    int chatmixDialState = 0;
    int lastChatmixDialState = 0;
    FILE *fp;
    char output[2048];
    char headsetcontrolPath[2048];
    
    if (argc != 2){
        interval = 5;
    } else {
        if (atoi(argv[1]) > 0){
            interval = atoi(argv[1]);
        } else {
            printf("Invalid interval. Usage:\n%s <interval seconds>\nDefault interval = 5\n", argv[0]);
            exit(1);
        }
    }
    // Find headsetcontrol binary
    fp = popen("which headsetcontrol", "r");
    if (fp == NULL){
        printf("Error: headsetcontrol not found.\n");
        exit(1);
    }
    fgets(headsetcontrolPath, 2048, fp);
    pclose(fp);

    // Remove newline character
    headsetcontrolPath[strlen(headsetcontrolPath)-1] = '\0';

    sprintf(headsetcontrolPath, "%s -m", headsetcontrolPath);

    printf("Checking the chatmix dial every %d seconds.\n", interval);

    while (!shouldExit){
        // Get the state of the chatmix dial using headsetcontrol
        fp = popen(headsetcontrolPath, "r");
        if (fp == NULL){
            printf("Error opening pipe!\n");
            exit(1);
        }

        while (fgets(output, 2048, fp) != NULL){
            if (strstr(output, "Chat-Mix:") != NULL){
                if (strstr(output, "64") != NULL){
                    chatmixDialState = 1;
                } else {
                    chatmixDialState = 0;
                }
            }
        }
        pclose(fp);

        /*
         * If the chatmix dial is not in the middle and it is not equivalent to 
         * the previous chatmix state, send a notification
         */
        if (chatmixDialState != 1 && chatmixDialState != lastChatmixDialState){
            sprintf(output, "chatmixd: Chat-Mix dial is not in the middle.\n");
            printf("%s", output);
            system("/usr/bin/notify-send -t 5000 \"The Chat-Mix dial is not balanced.\"");
        }
        lastChatmixDialState = chatmixDialState;
        usleep(interval * 1000000);
    }
}
