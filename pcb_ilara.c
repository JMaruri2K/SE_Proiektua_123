#include "pcb_ilara.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


//PCB* sortu_prozesua(){
    
    PCB* prozesu_berria = (PCB*)malloc(sizeof(PCB));

    int random_pid = (rand() % 9000) + 1000; 
    int exekuzioa = (rand() % 7) + 1; //1-7 segundura.
    int billeteak = (rand() % 10) + 1; //1-10
    prozesu_berria->pid = random_pid;
    prozesu_berria->exekuzio_denbora = exekuzioa;
    prozesu_berria->billete_kopurua = billeteak;

    return prozesu_berria;
//}


void hasieratu_Queue(Queue *q){
    q->hasiera = NULL;
    q->bukaera = NULL;
    q->count = 0;
}


void sartu_ilaran(Queue *q, PCB *prozesu_berria){
    Nodoa *ilara_berria = (Nodoa*)malloc(sizeof(Nodoa));
    ilara_berria->prozesua = prozesu_berria;
    ilara_berria->hurrengoa = NULL;

    if(q->bukaera == NULL){
        q->hasiera = ilara_berria;
        q->bukaera = ilara_berria;
    }else{
        q->bukaera->hurrengoa = ilara_berria;
        q->bukaera = ilara_berria;
    }

    q->count++;

    //printf("Prozesu berria ilaran sartu da: %d\n", prozesu_berria->pid); 
}


PCB* atera_ilaratik(Queue *q){

    if(q->hasiera == NULL){
        return NULL;
    }

    Nodoa *nodoa_ezabatu = q->hasiera;
    PCB *ezabatutako_prozesua = nodoa_ezabatu->prozesua;

    q->hasiera = q->hasiera->hurrengoa;

    if(q->hasiera == NULL){
        q->bukaera=NULL; 
    }

    q->count--;

    free(nodoa_ezabatu); 

    return ezabatutako_prozesua;
}

//Prozesu bakoitzak dado bat botatzen du, berdinketa badago prozesuen hartean berri egiten da batek irabazi harte.
PCB* dado_txapelketa(Queue *q){

    //Frogatu ea ilara hutsik dagoen.
    if ( q->hasiera == NULL){
        return NULL;
    }

    //Array bat sortzen dugu ez apurtzeko benetazko ilara.
    int n = q->count;
    Nodoa **kandidatuak = malloc(sizeof(Nodoa*) * n);
    Nodoa *aux = q->hasiera;

    //Prozesu guztiak sartzen ditugu array-aren barruan.
    for ( int i = 0; i < n; i++){
        kandidatuak[i] = aux;
        aux = aux->hurrengoa;
    }

    int irabazle_kop = n;

    //Prozesuren bat baino gehiago badago puntuazio altuenarekin berriz egin behar dugu.
    while (irabazle_kop > 1){

        int max_dadoa = 0;
        int emaitzak[irabazle_kop]; 

        //Bakoitzak bere dadoa botatzen du.
        for(int i = 0; i < irabazle_kop; i++){

            emaitzak[i] = (rand() % 6) + 1; //Dado bat botatzea 1-etik 6-ra.

            //Ikusten dugu ea zein izan den puntuaziorik altuena.
            if (emaitzak[i] > max_dadoa){
                max_dadoa = emaitzak[i];
            }
        }

        //Bakarrik puntuazio altuena atera dutenak segitzen dute berriz botatzera.
        int j = 0;
        for ( int i = 0; i<irabazle_kop; i++){
            if(emaitzak[i] == max_dadoa){
                kandidatuak[j] = kandidatuak[i];//Segitzen dutenak array-arn aurreko partera mugitzen ditugu.
                j++;
            }
        }

        irabazle_kop = j;

    }

    //Irabazle bat daukagu.
    Nodoa *irabazle_nodoa = kandidatuak[0];
    PCB *pcb_irabazlea = irabazle_nodoa->prozesua;
    printf("Irabazlea %d\n", pcb_irabazlea->pid);


    //Orain benetazko ilaran aurkitu behar dugu ateratzeko.
    Nodoa *oraingoa = q->hasiera;
    Nodoa *aurrekoa = NULL;

    //Ilara pasatzen dugu nodo irabazlea aurkitu harte.
    while ( oraingoa !=irabazle_nodoa){
        aurrekoa = oraingoa;
        oraingoa = oraingoa->hurrengoa;
    }

    //Punteroa aktualizatzen dugu, kendu nahi dugun nodoa kentzeko.
    if ( aurrekoa == NULL){
    
        q->hasiera = oraingoa->hurrengoa;

    }else{

        aurrekoa->hurrengoa = oraingoa->hurrengoa;

    }

    if(oraingoa == q->bukaera){

        q->bukaera = aurrekoa;
    }

    //Nodo bat atera dugulez count-ari 1 kendu behar zaio.
    q->count--;

    //Egin ditugun memoria erreserbak libre hutsi.
    free(kandidatuak);
    free(oraingoa);

    return pcb_irabazlea;

}

PCB* SJF(Queue *q){

    if(q->hasiera ==  NULL){

        return NULL;

    }

    Nodoa *oraingoa = q->hasiera;
    Nodoa *aurrekoa = NULL;


    Nodoa *motzena = q->hasiera;
    Nodoa *motzena_aurrekoa = NULL;


    //Denbora gutxiena behar duen nodoa aurkitzeko 
    while(oraingoa->hurrengoa != NULL){

        aurrekoa = oraingoa;
        oraingoa = oraingoa->hurrengoa;

        if(oraingoa->prozesua->exekuzio_denbora < motzena->prozesua->exekuzio_denbora){


            motzena = oraingoa;
            motzena_aurrekoa = aurrekoa;

        }

    }

    if(motzena_aurrekoa == NULL){

        q->hasiera = motzena->hurrengoa;

    }else{

        motzena_aurrekoa->hurrengoa = motzena->hurrengoa;

    }

    if(motzena == q->bukaera){

        q->bukaera = motzena_aurrekoa;

    }

    PCB* p = motzena->prozesua;
    free(motzena);
    q->count--;
    return p;


}

PCB* zozketa(Queue *q){

    if(q->hasiera ==  NULL || q->count == 0){

        return NULL;

    }

    //Billete guztiak kontuan hartu behar dira zozketa egin ahal izateko
    int billete_guztiak = 0;
    Nodoa *aux = q->hasiera;
    while(aux != NULL){

        billete_guztiak += aux->prozesua->billete_kopurua;
        aux = aux->hurrengoa;

    }

    int irabazlea = rand() % billete_guztiak;
    int uneko_batura = 0;

    Nodoa *oraingoa = q->hasiera;
    Nodoa *aurrekoa = NULL;

    while(oraingoa != NULL){

        uneko_batura += oraingoa->prozesua->billete_kopurua;
        if(uneko_batura > irabazlea){
            //Irabazlea aurkituta ilaratik atera.
            if(aurrekoa == NULL){

                q->hasiera = oraingoa->hurrengoa;

            }else{

                aurrekoa->hurrengoa = oraingoa->hurrengoa;

            }

            if(oraingoa == q->bukaera){

                q->bukaera = aurrekoa;

            }

            PCB* p = oraingoa->prozesua;
            free(oraingoa);
            q->count--;
            return p;

        }

        aurrekoa = oraingoa;
        oraingoa = oraingoa->hurrengoa;

    }

}


void ilara_garbitu(Queue *q){

    Nodoa *nodoa;
    PCB *prozesua;
    while(q->hasiera != NULL){

        nodoa = q->hasiera;
        prozesua = nodoa->prozesua;
        q->hasiera =  q->hasiera->hurrengoa;

        if(prozesua != NULL){
            free(prozesua);
        }
        free(nodoa);
    }

    q->hasiera = NULL;
    q->bukaera = NULL;
    q->count = 0;

    printf("Ilara garbitua izan da.\n");
} 
