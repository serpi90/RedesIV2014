/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "idManager.h"
#include "../includes.h"
#define FILENAME "ids.dat"
#define HOST_KIND_AMOUNT (MAX_CONSUMMERS + 1)

enum kind {
    PRODUCER,
    CONSUMMER
};

struct hostInfo {
    char address[MAX_ADDRESS_LENGTH];
    long mtype;
};

struct hostArray {
    unsigned amount;
    unsigned lastUsed;
    struct hostInfo * hosts;
};

struct fileData {
    long nextMtype;
    struct hostArray producers, disks, processors, motherboards;
};

void addHost(struct hostArray *array, struct hostInfo host) {
    if (array->hosts) {
        array->hosts = realloc(array->hosts, (array->amount + 1) * sizeof (struct hostInfo));
    } else {
        array->hosts = malloc(sizeof (struct hostInfo));
    }
    array->hosts[array->amount++] = host;
}

struct fileData readFile(const char * filename) {
    struct fileData data;
    unsigned i;
    struct hostArray temp[HOST_KIND_AMOUNT];
    FILE * file = fopen(filename, "rb");
    if (file) {
        if (fread(&(data.nextMtype), sizeof (data.nextMtype), 1, file)) {
            for (i = 0; i < HOST_KIND_AMOUNT; i++) {
                fread(&(temp[i].amount), sizeof (temp[i].amount), 1, file);
                fread(&(temp[i].lastUsed), sizeof (temp[i].lastUsed), 1, file);
            }
            for (i = 0; i < HOST_KIND_AMOUNT; i++) {
                if (temp[i].amount) {
                    temp[i].hosts = malloc(sizeof (struct hostInfo) * temp[i].amount);
                    fread(temp[i].hosts, sizeof (struct hostInfo), temp[i].amount, file);
                } else {
                    temp[i].hosts = NULL;
                }
            }
            data.producers = temp[0];
            data.disks = temp[1];
            data.processors = temp[2];
            data.motherboards = temp[3];
        } else {
            bzero(&data, sizeof (data));
            data.nextMtype = M_PROD + 1;
        }
        fclose(file);
    } else {
        bzero(&data, sizeof (data));
        data.nextMtype = M_PROD + 1;
    }
    printf("N : %2ld PR: %2u\n", data.nextMtype, data.producers.amount);
    printf("D : %2u P : %2u M : %2u\n", data.disks.amount, data.processors.amount, data.motherboards.amount);
    printf("LD: %2u LP: %2u LM: %2u\n", data.disks.lastUsed, data.processors.lastUsed, data.motherboards.lastUsed);

    for (i = 0; i < data.producers.amount; i++) {
        printf("PR: %2ld %12s\n", data.producers.hosts[i].mtype, data.producers.hosts[i].address);
    }
    for (i = 0; i < data.disks.amount; i++) {
        printf("D : %2ld %12s\n", data.disks.hosts[i].mtype, data.disks.hosts[i].address);
    }
    for (i = 0; i < data.processors.amount; i++) {
        printf("P : %2ld %12s\n", data.processors.hosts[i].mtype, data.processors.hosts[i].address);
    }
    for (i = 0; i < data.motherboards.amount; i++) {

        printf("M : %2ld %12s\n", data.motherboards.hosts[i].mtype, data.motherboards.hosts[i].address);
    }
    return data;
}

bool_t writeFile(const char * filename, struct fileData data) {
    FILE * file = fopen(filename, "wb");
    struct hostArray temp[HOST_KIND_AMOUNT];
    unsigned i;
    temp[0] = data.producers;
    temp[1] = data.disks;
    temp[2] = data.processors;
    temp[3] = data.motherboards;
    if (file) {
        fwrite(&(data.nextMtype), sizeof (data.nextMtype), 1, file);
        for (i = 0; i < HOST_KIND_AMOUNT; i++) {
            fwrite(&(temp[i].amount), sizeof (temp[i].amount), 1, file);
            fwrite(&(temp[i].lastUsed), sizeof (temp[i].lastUsed), 1, file);
        }
        for (i = 0; i < HOST_KIND_AMOUNT; i++) {
            if (temp[i].amount) {

                fwrite(temp[i].hosts, sizeof (struct hostInfo), temp[i].amount, file);
            }
        }
        fclose(file);
    } else {
        return FALSE;
    }
    return TRUE;
}

void clearData(struct fileData* dataPtr) {
    struct hostArray temp[HOST_KIND_AMOUNT];
    unsigned i;
    temp[0] = dataPtr->producers;
    temp[1] = dataPtr->disks;
    temp[2] = dataPtr->processors;
    temp[3] = dataPtr->motherboards;

    dataPtr->nextMtype = M_CONS + 1;
    for (i = 0; i < HOST_KIND_AMOUNT; i++) {
        if (temp[i].hosts) {
            free(temp[i].hosts);
        }
        temp[i].hosts = (struct hostInfo*) NULL;
        temp[i].amount = 0;
        temp[i].lastUsed = 0;
    }
    dataPtr->producers = temp[0];
    dataPtr->disks = temp[1];
    dataPtr->processors = temp[2];
    dataPtr->motherboards = temp[3];
}

registerResult *
register_consummer_1_svc(consummerType *type, struct svc_req *rqstp) {
    static registerResult result;
    char * ctype;
    struct fileData data;
    struct hostInfo host;

    data = readFile(FILENAME);

    host.mtype = data.nextMtype;
    strncpy(host.address, inet_ntoa(rqstp->rq_xprt->xp_raddr.sin_addr), sizeof (host.address));
    switch (*type) {
        case DISKS:
            addHost(&(data.disks), host);
            ctype = "DISKS";
            break;
        case PROCCESSORS:
            addHost(&(data.processors), host);
            ctype = "PROCESSORS";
            break;
        case MOTHERBOARDS:
            addHost(&(data.motherboards), host);
            ctype = "MOTHERBOARDS";
            break;
        default:
            result.cod_ret = -1;
            result.registerResult_u.error = UNKNOWN_CONSUMMER_TYPE;
            return &result;
    }
    result.registerResult_u.mtype = host.mtype;
    data.nextMtype++;
    if (!writeFile(FILENAME, data)) {
        result.cod_ret = -1;
        result.registerResult_u.error = FILE_ACCESS_FAILURE;
        clearData(&data);
        return &result;
    }
    result.cod_ret = 0;
    clearData(&data);

    printf("REGISTER CONSUMMER %s as id %3ld from %s\n", ctype, host.mtype, host.address);

    result.registerResult_u.mtype = host.mtype;

    return &result;
}

queryResult *
query_consummers_1_svc(void *nothing, struct svc_req *rqstp) {
    static queryResult result;
    struct fileData data = readFile(FILENAME);
    struct hostArray arrs[MAX_CONSUMMERS];
    unsigned idx[MAX_CONSUMMERS], i;
    idx[0] = (unsigned) DISKS;
    arrs[0] = data.disks;
    idx[1] = (unsigned) PROCCESSORS;
    arrs[1] = data.processors;
    idx[2] = (unsigned) MOTHERBOARDS;
    arrs[2] = data.motherboards;
    for (i = 0; i < MAX_CONSUMMERS; i++) {
        if (arrs[i].amount) {
            result.queryResult_u.mtype[idx[i]] = arrs[i].hosts[arrs[i].lastUsed].mtype;
            arrs[i].lastUsed = (arrs[i].lastUsed + 1) % arrs[i].amount;
        } else {
            result.queryResult_u.mtype[idx[i]] = 0;
        }
    }
    data.disks = arrs[0];
    data.processors = arrs[1];
    data.motherboards = arrs[2];
    if (!writeFile(FILENAME, data)) {
        result.cod_ret = -1;
        result.queryResult_u.error = FILE_ACCESS_FAILURE;
        clearData(&data);
        return &result;
    }
    clearData(&data);
    result.cod_ret = 0;
    printf("QUERY: %ld %ld %ld\n", result.queryResult_u.mtype[0], result.queryResult_u.mtype[1], result.queryResult_u.mtype[2]);

    return &result;
}

getResult *
get_1_svc(long *mtype, struct svc_req *rqstp) {
    static getResult result;
    static struct hostInfo host;
    unsigned i, j;
    struct fileData data = readFile(FILENAME);
    struct hostArray temp[HOST_KIND_AMOUNT];
    temp[0] = data.producers;
    temp[1] = data.disks;
    temp[2] = data.processors;
    temp[3] = data.motherboards;
    host.mtype = 0;
    for (i = 0; i < HOST_KIND_AMOUNT && host.mtype == 0; i++) {
        for (j = 0; j < temp[i].amount && host.mtype == 0; j++) {
            if (temp[i].hosts[j].mtype == *mtype) {
                host = temp[i].hosts[j];
            }
        }
    }
    clearData(&data);
    if (host.mtype) {
        result.cod_ret = 0;
        strncpy(result.getResult_u.address, host.address, MAX_ADDRESS_LENGTH);
        printf("GET %3ld is %s\n", host.mtype, host.address);
    } else {

        result.cod_ret = -1;
        result.getResult_u.error = INVIALID_ID;
    }
    return &result;
}

registerResult *
register_producer_1_svc(void *nothing, struct svc_req *rqstp) {
    static registerResult result;
    struct fileData data;
    struct hostInfo host;
    data = readFile(FILENAME);
    host.mtype = data.nextMtype;
    strncpy(host.address, inet_ntoa(rqstp->rq_xprt->xp_raddr.sin_addr), sizeof (host.address));
    addHost(&(data.producers), host);

    result.registerResult_u.mtype = host.mtype;
    data.nextMtype++;
    if (!writeFile(FILENAME, data)) {
        result.cod_ret = -1;
        result.registerResult_u.error = FILE_ACCESS_FAILURE;
        clearData(&data);
        return &result;
    }
    result.cod_ret = 0;
    clearData(&data);

    printf("REGISTER PRODUCER as id %3ld from %s\n", host.mtype, host.address);

    result.registerResult_u.mtype = host.mtype;

    return &result;
}
