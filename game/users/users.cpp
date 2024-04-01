#include "login.hpp"
#include "users.hpp"

char userNames[15][20] = {
    "STARFIRE",
    "SHADOWBLADE",
    "STORMBREAKER",
    "NIGHTSTRIKER",
    "THUNDERHEART",
    "VOIDSLAYER",
    "SHADOWCASTER",
    "STARFURY",
    "STARDANCER",
    "SUNHAMMER",
    "NOVAFLARE",
    "DARKWATCHER",
    "STORMCHASER",
    "SKYWALKER",
    "ECLIPSER",
};

char (*getUsernameList())[20] {
    return userNames;
}

PLAYER_SKIN getSavedSkin(USER* user) {
    // File name format: uName_uNum_pWord.txt
    char userFileName[100] = "/sd/Users/";
    strcat(userFileName, user->username);
    strcat(userFileName, "_");
    char strUserNum[3];
    sprintf(strUserNum, "%d", user->userNum);
    strcat(userFileName, strUserNum);
    strcat(userFileName, "_");
    strcat(userFileName, user->password);
    strcat(userFileName, ".txt");
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        return PLAYER_SKIN::ICE_BLUE;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime);

    fclose(userFile);
    return defaultSkin;
}

int getSavedHighScore(USER* user) {
    // File name format: uName_uNum_pWord.txt
    char userFileName[100] = "/sd/Users/";
    strcat(userFileName, user->username);
    strcat(userFileName, "_");
    char strUserNum[3];
    sprintf(strUserNum, "%d", user->userNum);
    strcat(userFileName, strUserNum);
    strcat(userFileName, "_");
    strcat(userFileName, user->password);
    strcat(userFileName, ".txt");
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        return 0;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime);

    fclose(userFile);
    return highScore;
}

int getSavedEnemiesKilled(USER* user) {
    // File name format: uName_uNum_pWord.txt
    char userFileName[100] = "/sd/Users/";
    strcat(userFileName, user->username);
    strcat(userFileName, "_");
    char strUserNum[3];
    sprintf(strUserNum, "%d", user->userNum);
    strcat(userFileName, strUserNum);
    strcat(userFileName, "_");
    strcat(userFileName, user->password);
    strcat(userFileName, ".txt");
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        return 0;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime);

    fclose(userFile);
    return enemiesKilled;
}

int getSavedPlayerDeaths(USER* user) {
    // File name format: uName_uNum_pWord.txt
    char userFileName[100] = "/sd/Users/";
    strcat(userFileName, user->username);
    strcat(userFileName, "_");
    char strUserNum[3];
    sprintf(strUserNum, "%d", user->userNum);
    strcat(userFileName, strUserNum);
    strcat(userFileName, "_");
    strcat(userFileName, user->password);
    strcat(userFileName, ".txt");
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        return 0;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime);

    fclose(userFile);
    return playerDeaths;
}

int getSavedTotalPoints(USER* user) {
    // File name format: uName_uNum_pWord.txt
    char userFileName[100] = "/sd/Users/";
    strcat(userFileName, user->username);
    strcat(userFileName, "_");
    char strUserNum[3];
    sprintf(strUserNum, "%d", user->userNum);
    strcat(userFileName, strUserNum);
    strcat(userFileName, "_");
    strcat(userFileName, user->password);
    strcat(userFileName, ".txt");
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        return 0;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime);

    fclose(userFile);
    return totalPoints;
}

int getSavedPlayTime(USER* user) {
    // File name format: uName_uNum_pWord.txt
    char userFileName[100] = "/sd/Users/";
    strcat(userFileName, user->username);
    strcat(userFileName, "_");
    char strUserNum[3];
    sprintf(strUserNum, "%d", user->userNum);
    strcat(userFileName, strUserNum);
    strcat(userFileName, "_");
    strcat(userFileName, user->password);
    strcat(userFileName, ".txt");
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        return 0;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime);

    fclose(userFile);
    return totalPlayTime;
}

// 0 if user exists, 1 if not
int userExists(char username[NAME_LENGTH], int userNum) {
    DIR *allUsersData;
    struct dirent *entry;
    allUsersData = opendir("/sd/Users");
    if (allUsersData == NULL) {
        printf("Unable to open Users directory\n");
        return -1;
    }

    while ((entry = readdir(allUsersData)) != NULL) {
        char fileName[100];
        char fileUserName[20];
        char fileUserNum[3];
        char strUserNum[3];
        sprintf(strUserNum, "%d", userNum);
        char *token;
        strcpy(fileName, entry->d_name);
        token = strtok(fileName, "_");
        strcpy(fileUserName, token);
        token = strtok(NULL, "_");
        strcpy(fileUserNum, token);
        // Check if the fileUserName matches the passed in username
        bool matchingName = true;
        int i = 0;
        while (username[i] && fileUserName[i]) {
            if (username[i] != fileUserName[i]) {
                matchingName = false;
                break;
            }
            i++;
        }
        bool matchingNum = true;
        i = 0;
        while (strUserNum[i] && fileUserNum[i]) {
            if (strUserNum[i] != fileUserNum[i]) {
                matchingNum = false;
                break;
            }
            i++;
        }
        if (matchingName && matchingNum) {
            closedir(allUsersData);
            return 0;
        }
    }
    closedir(allUsersData);
    return 1;
}

// 1 if correct password, 0 if not
int checkPassword(USER* user) {
    DIR *allUsersData;
    struct dirent *entry;
    allUsersData = opendir("/sd/Users");
    if (allUsersData == NULL) {
        printf("Unable to open Users directory\n");
        return -1;
    }
    // File name to look for
    char searchFileName[100] = "";
    strcat(searchFileName, user->username);
    strcat(searchFileName, "_");
    char strUserNum[3];
    sprintf(strUserNum, "%d", user->userNum);
    strcat(searchFileName, strUserNum);
    strcat(searchFileName, "_");
    strcat(searchFileName, user->password);
    strcat(searchFileName, ".txt");
    while ((entry = readdir(allUsersData)) != NULL) {
        char fileName[100];
        int i = 0;
        strcpy(fileName, entry->d_name);
        // Check if the fileUserName matches the passed in username
        bool matchingFile = true;
        i = 0;
        while (searchFileName[i] && fileName[i]) {
            if (searchFileName[i] != fileName[i]) {
                matchingFile = false;
                break;
            }
            i++;
        }
        if (matchingFile) return 1;
    }
    return 0;
}

void updateUser(USER* user) {
    // Open user file using user credentials
    char newUserFileName[100] = "/sd/Users/";
    strcat(newUserFileName, user->username);
    strcat(newUserFileName, "_");
    char strUserNum[3];
    sprintf(strUserNum, "%d", user->userNum);
    strcat(newUserFileName, strUserNum);
    strcat(newUserFileName, "_");
    strcat(newUserFileName, user->password);
    strcat(newUserFileName, ".txt");
    FILE* userFile = fopen(newUserFileName, "w");
    if (userFile == NULL) {
        printf("Error, could not open user file.\n");
        return;
    }
    // Overwrite old data
    fprintf(userFile, "%c, %d, %d, %d, %d, %d\n", user->defaultSkin, user->highScore, user->totalEnemiesKilled, user->totalPlayerDeaths, user->totalPoints, user->totalPlayTime);
    fclose(userFile);
}

/** Delete a user
 */
void deleteUser(USER* user) {
    // Open user file using user credentials
    char userFileName[100] = "/sd/Users/";
    strcat(userFileName, user->username);
    strcat(userFileName, "_");
    char strUserNum[3];
    sprintf(strUserNum, "%d", user->userNum);
    strcat(userFileName, strUserNum);
    strcat(userFileName, "_");
    strcat(userFileName, user->password);
    strcat(userFileName, ".txt");
    if (remove(userFileName)) printf("Unable to delete user.\n");
    else printf("User deleted.\n");
}