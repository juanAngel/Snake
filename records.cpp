#include "records.h"

#define MAX_RECORD 5

recodList* loadRecord(){
    char strBuff[100],numBuff[11],*tmp;
    recodList* nodes = nullptr,*lastNode = nullptr,*newNode;
    FILE* f = fopen("record.conf","r");
    if(f){
        while (!feof(f)) {
            char* result = fgets(strBuff,sizeof(strBuff),f);
            if(result){
                //quitamos el caracter de nueva linea
                tmp =strstr(result,"\n");
                if(tmp)
                    *tmp = 0;
                printf("%s\n",result);

                result = fgets(numBuff,sizeof(numBuff),f);
                if(result){
                    //quitamos el caracter de nueva linea
                    tmp =strstr(result,"\n");
                    if(tmp)
                        *tmp = 0;
                    printf("%s\n",result);

                    //Si todo se lello bien, creamos una entrada en la lista
                    newNode = (recodList*)malloc(sizeof(*newNode));
                    memset(newNode,0,sizeof(*newNode));
                    //Seteamos los valores adecuados
                    newNode->Points = atoi(numBuff);
                    newNode->PlayerName = (char*)malloc((strlen(strBuff)+1)*sizeof(*strBuff));
                    strcpy(newNode->PlayerName,strBuff);

                    if(!nodes){
                        nodes = lastNode = newNode;
                    }else{
                        lastNode->next = newNode;
                        lastNode = newNode;
                    }
                }
            }

        }
        fclose(f);
    }
    return nodes;
}
void saveRecord(recodList* r){
    FILE* f = fopen("record.conf","w");
    //Testeo si se abrio el archivo
    if(f){
        //escribo mientras queden records
        while (r) {
            fprintf(f,"%s\n%u\n",r->PlayerName,r->Points);

            r = (recodList*)r->next;
        }
        fclose(f);
    }
}
void freeRecordList(recodList* r){
    recodList* tmp;
    while (r) {
        free(r->PlayerName);
        tmp = r;
        r = (recodList*)r->next;
        free(tmp);
    }
}


recodList *manageRecord(recodList *records, uint newPoint, const char *playerName, int *recordIndex){
    //Gestion de records
    recodList* tmpRecord = records,*myRecord = nullptr,*beforeRecord = nullptr;
    uint i = 0;
    for(;i<MAX_RECORD && tmpRecord && tmpRecord->Points > newPoint;i++){
        beforeRecord = tmpRecord;
        tmpRecord = (recodList*)tmpRecord->next;
    }
    if(i<MAX_RECORD){
        myRecord = (recodList*)malloc(sizeof(*myRecord));
        memset(myRecord,0,sizeof(*myRecord));

        myRecord->Points = newPoint;
        myRecord->PlayerName = (char*)malloc((strlen(playerName)+1)*sizeof(*playerName));
        strcpy(myRecord->PlayerName,playerName);

        if(i>0){
            *recordIndex = i;
            insertNode(beforeRecord,myRecord);

            tmpRecord = myRecord;
            for(;i<MAX_RECORD && tmpRecord;i++){
                beforeRecord = tmpRecord;
                tmpRecord = (recodList*)tmpRecord->next;
            }
            if(tmpRecord){
                freeRecordList((recodList*)tmpRecord);
                beforeRecord->next = nullptr;
            }
        }else{
            *recordIndex = 0;
            myRecord->next = records;
            records = myRecord;
        }
    }else
        *recordIndex = -1;
    return records;
}
