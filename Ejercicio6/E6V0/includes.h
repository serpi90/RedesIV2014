#ifndef INCLUDES_H
#define	INCLUDES_H

// Definitions

#define HOLDER_CAPACITY 6
#define SAMPLER_AMOUNT 6
#define ANALYZER_AMOUNT 8

// IPC Definitions

#define PATH "/home/knoppix/Maestri/Ejercicio6/E6V0"
#define Q_SAMPLER_FROM_INTERFACE  1
#define Q_SAMPLER_TO_INTERFACE    2
#define Q_ANALYZER_FROM_INTERFACE 3
#define Q_ANALYZER_TO_INTERFACE   4
#define SHM_SAMPLE_HOLDER         5
#define SEM_MUTEX                 6
#define SEM_EMPTY                 7
#define SEM_FULL                  8

// Message Types

#define M_SAMPLER 1
#define M_ANALYZER (M_SAMPLER + 1 + SAMPLER_AMOUNT)

// Shared memory structures

enum analyisisStatus {
    NOT_ANALYZED, ANALYZED
};

enum workingStatus {
    WAITING, WORKING
};

struct sample {
    long id;
};

struct sampleHolder {
    struct sample samples[HOLDER_CAPACITY];
    unsigned int read;
    unsigned int write;
    unsigned int amount;
    enum analyisisStatus status[HOLDER_CAPACITY][ANALYZER_AMOUNT];
    enum workingStatus analyzerStatus[ANALYZER_AMOUNT];
};

// Messages

enum iMessageType {
    PLACE_SAMPLE, SAMPLE_PLACED,
    GET_SAMPLE, GOT_SAMPLE
};

union iMessageContent {
    long number;
    struct sample sample;
};

struct iMessage {
    long type;
    long sender;
    enum iMessageType message;
    union iMessageContent data;
};

#endif	/* INCLUDES_H */
