#include "gameMusic.hpp"
#include "hardware.hpp"
#include "login.hpp"
#include "users.hpp"

char userFileName[100];

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

void generateFileName(USER* user) {
    strcpy(userFileName, "/sd/Users/");
    strcat(userFileName, user->username);
    strcat(userFileName, "_");
    char strUserNum[3];
    sprintf(strUserNum, "%d", user->userNum);
    strcat(userFileName, strUserNum);
    strcat(userFileName, "_");
    strcat(userFileName, user->password);
    strcat(userFileName, ".txt");
}

PLAYER_SKIN getSavedSkin(USER* user) {
    // File name format: uName_uNum_pWord.txt
    generateFileName(user);
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime, updateCount;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        fclose(userFile);
        return PLAYER_SKIN::ICE_BLUE;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d, %d\n", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime, &updateCount);
    fclose(userFile);
    return defaultSkin;
}

int getSavedHighScore(USER* user) {
    // File name format: uName_uNum_pWord.txt
    generateFileName(user);
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime, updateCount;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        fclose(userFile);
        return 0;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d, %d\n", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime, &updateCount);
    fclose(userFile);
    return highScore;
}

int getSavedEnemiesKilled(USER* user) {
    // File name format: uName_uNum_pWord.txt
    generateFileName(user);
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime, updateCount;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        fclose(userFile);
        return 0;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d, %d\n", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime, &updateCount);
    fclose(userFile);
    return enemiesKilled;
}

int getSavedPlayerDeaths(USER* user) {
    // File name format: uName_uNum_pWord.txt
    generateFileName(user);
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime, updateCount;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        fclose(userFile);
        return 0;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d, %d\n", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime, &updateCount);
    fclose(userFile);
    return playerDeaths;
}

int getSavedTotalPoints(USER* user) {
    // File name format: uName_uNum_pWord.txt
    generateFileName(user);
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime, updateCount;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        fclose(userFile);
        return 0;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d, %d\n", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime, &updateCount);
    fclose(userFile);
    return totalPoints;
}

int getSavedPlayTime(USER* user) {
    // File name format: uName_uNum_pWord.txt
    generateFileName(user);
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime, updateCount;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        fclose(userFile);
        return 0;
    }

    // Read user data
    fscanf(userFile, "%c, %d, %d, %d, %d, %d, %d\n", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime, &updateCount);
    fclose(userFile);
    return totalPlayTime;
}

int getUpdateCount(USER* user) {
    // File name format: uName_uNum_pWord.txt
    generateFileName(user);
    FILE* userFile = fopen(userFileName, "r");
    PLAYER_SKIN defaultSkin;
    int highScore, enemiesKilled, playerDeaths, totalPoints, totalPlayTime, updateCount;
    if (userFile == NULL) {
        printf("Error opening the user file.\n");
        fclose(userFile);
        return -1;
    }
    // Read user data
    if (fscanf(userFile, "%c, %d, %d, %d, %d, %d, %d\n", &defaultSkin, &highScore, &enemiesKilled, &playerDeaths, &totalPoints, &totalPlayTime, &updateCount) != 7) {
        printf("Error reading user data.\n");
        fclose(userFile);
        return -1;
    }
    fclose(userFile);
    return updateCount;
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
        int fileUserNum;
        strcpy(fileName, entry->d_name);
        sscanf(fileName, "%19[^_]_%d", fileUserName, &fileUserNum);
        // If user is deleted, file will be in the form USERNAME_USERNUM_PASSWORD_DELETED
        if (strstr(fileName, "DELETED") != NULL) {
            return 1;
        }
        // Check if the fileUserName matches the passed in username
        bool matchingName = strcmp(username, fileUserName) == 0;
        bool matchingNum = (userNum == fileUserNum);
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
        bool matchingFile = strcmp(searchFileName, fileName) == 0;
        if (matchingFile) {
            closedir(allUsersData);
            return 1;
        }
    }
    closedir(allUsersData);
    return 0;
}

void updateUser(USER* user) {
    // File name format: uName_uNum_pWord.txt
    generateFileName(user);
    int updateCount = getUpdateCount(user) + 1;
    // Check if failed to get updateCount
    if (updateCount == 0) {
        return;
    }
    FILE* writeUserFile = fopen(userFileName, "w");
    if (writeUserFile == NULL) {
        printf("Error, could not open user file.\n");
        fclose(writeUserFile);
        return;
    }
    // Overwrite old data
    fprintf(writeUserFile, "%c, %d, %d, %d, %d, %d, %d", user->defaultSkin, user->highScore, user->totalEnemiesKilled, user->totalPlayerDeaths, user->totalPoints, user->totalPlayTime, updateCount);
    fclose(writeUserFile);
}

void deleteUser(USER* user) {
    // Open user file using user credentials
    generateFileName(user);
    if (remove(userFileName)) printf("Unable to delete user.\n");
    else printf("User deleted.\n");
    /**FILE* userFile = fopen(userFileName, "r");
    char copyUserFileName[100];
    strcpy(copyUserFileName, userFileName);
    copyUserFileName[strlen(copyUserFileName) - 4] = '\0';
    if (rename(userFileName, strcat(copyUserFileName, "_DELETED.txt")) != 0) {
        printf("Unable to delete user.\n");
        
        fclose(userFile);
        return;
    }
    fclose(userFile);
    printf("User deleted.\n");*/
}