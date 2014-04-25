/* 
 * File:   includes.h
 * Author: lubuntu
 *
 * Created on April 6, 2014, 1:39 PM
 */

#ifndef INCLUDES_H
#define	INCLUDES_H

// Definitions

#define ROBOT_AMOUNT 4
#define PLAT_SIZE 25
#define ACT_PER_REQUEST 10

#define MAX_PILES 20
#define MAX_DISPOSITIVES 10

// IPC Definitions

#define PATH "/home/knoppix/Maestri/Ejercicio1/V0"
#define Q_PLAT_TO_INTERFACE 1
#define Q_PLAT_FROM_INTERFACE 2
#define SEM_MUTEX 3
#define SEM_SEMA 4
#define SEM_SEMB 5
#define SEM_SEME 6
#define SHM_PLAT 7
// Message Types

#define M_PLATA 1
#define M_PLATB ROBOT_AMOUNT + 1 + M_PLATA
#define M_ROBOTA ROBOT_AMOUNT + 1 + M_PLATB
#define M_ROBOTB ROBOT_AMOUNT + 1 + M_ROBOTA

// Shared memory structures

enum dispStatus {
    ACTIVE, INACTIVE
};

enum slotStatus {
    EMPTY, RESERVED, OCCUPIED
};

enum robotStatus {
    OFF, IDLE, WORKING
};

enum robotWaitingStatus {
    WAITING, NOT_WAITING
};

struct dispositive {
    long id;
    unsigned short tipo;
    dispStatus status;
};

struct slot {
    struct dispositive dispositive;
    slotStatus state;
};

struct plate {
    struct slot slots[PLAT_SIZE];
    robotStatus statusA[ROBOT_AMOUNT];
    robotStatus statusB[ROBOT_AMOUNT];
    robotWaitingStatus statusE[ROBOT_AMOUNT];
    unsigned short activeRobotsA;
    unsigned short activeRobotsB;
    unsigned short storedAmount;
};

// Messages

enum iPlatMessage {
    RESERVE, RESERVE_OK,
    PUT, PUT_OK,
    TAKE, TAKE_OK, TAKE_NONE,
    QUERY_ROBOTS_OR_DISPOSITIVES, QUERY_ROBOTS_OR_DISPOSITIVES_YES, QUERY_ROBOTS_OR_DISPOSITIVES_NO,
    SHUTDOWN, SHUTDOWN_OK, SHUTDOWN_LAST
};

struct putMessage {
    int location;
    long idDispositive;
};

union iMessageContent {
    int location;
    char rtype;
    struct putMessage putMessage;
};

struct iMessage {
    long type;
    long senderType;
    enum iPlatMessage function;
    int robot;
    union iMessageContent content;
};


#include <cstdio>
#include <sstream>
#include <string>

#endif	/* INCLUDES_H */
