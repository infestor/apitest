/*****************************************************************************
***
***    apicalls.c
*** 
***    C-Anbindung an API.DLL (EDIABAS fuer Windows) bzw. API32.DLL (EDIABAS
***    fuer WIN32) und Abbildung der "normalen" API-Schnittstelle.
***
***    Fuer den Link-Vorgang sind alle benoetigten DLL-Schnittstellen-
***    funktionen in der entsprechenden DEF-Datei zu importieren, z.B. API.DLL:
***         IMPORTS    api.__apiInit
***                    api.__apiEnd
***                    api.__apiJob
***
***
***    Allgemeines zur DLL-Schnittstelle von EDIABAS fuer Windows:
***    -----------------------------------------------------------
***
***    1. Typkonventionen:
***    Die DLL-Funktionen entsprechen dem Windows-Standard und zeigen deshalb
***    abweichende Typkonventionen zur normalen API-Schnittstelle:
***      -  Alle DLL-Funktionen sind vom Typ FAR PASCAL
***      -  Alle uebergebenen Adressen sind vom Typ FAR 
***    
***    2. Namenskonventionen:
***    Um die normale API-Schnittstelle trotzdem abbilden zu koennen, beginnen
***    zur Unterscheidung der Funktionsnamen alle DLL-Funktionen mit doppeltem
***    Underscore (z.B. __apiInit).
***    
***    3. Applikations-Locking ueber Handle:
***    Die API-Schnittstellenfunktionen __apiInit/__apiInitExt vergeben als 
***    Ergebnis einen Handle vom Typ "unsigned int", welcher dann jeder anderen
***    API-Schnittstellenfunktion als 1. Parameter uebergeben werden muss.
***    Mit diesem Mechanismus des Applikations-Lockings kann nun von
***    verschiedenen Prozessen auf EDIABAS zugegriffen werden, ohne API
***    zwischenzeitlich zurueckzusetzen.
***    Voraussetzung hierfuer ist jedoch, dass die beteiligten Prozesse den
***    Handle kennen.
***
***    4. Geaenderter Aufruf zur Ermittlung des Fehlertextes:
***    Da die Rueckgabe von C-Stringadressen zu Problemen bei anderen Windows-
***    Programmiersystemen fuehren koennen, ist die DLL-Schnittstellenfunktion
***    __apiErrorText anders wie bisher aufzurufen. Der DLL-Schnittstellen-
***    funktion ist eine Pufferadresse und die max. Anzahl zu kopierender
***    Zeichen (mit abschliessendem '\0') zu uebergeben. Der Fehlertext wird
***    dann in den entsprechenden Puffer kopiert. Liegt kein Fehler vor, so
***    wird ein Leer-String im Puffer abgelegt.
***
***    5. Keine Unterstuetzung von Callback/Error-Handler
***    Callback/Error-Handler koennen ueber den API.DLL bzw. den EDIABAS-Server
***    nicht bedient werden, die entsprechenden API-Funktionen fehlen deshalb.
***
***
***    Die nachfolgende C-Anbindung verdeckt alle genannten DLL-Besonderheiten
***    und stellt die "normale" API-Schnittstelle mit Callback/Error-Handler
***    zur Verfuegung!
***
******************************************************************************/


/***** Include-Dateien *******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "api.h"
#include "apidll.h"


/***** Statische Variable ****************************************************/

/* Handle fuer Applikations-Locking */
static unsigned int handle;

/* fuer Callback-Mechanismus */
static APIBOOL (*callbackHandler)(void)=NULL;
int jobState=APIREADY;

/* fuer Error-Handler-Mechanismus */
static void (*errorHandler)(void)=NULL;
APIBOOL errorHandlerCalled=APIFALSE;


/***** Hilfsfunktion *********************************************************/

static APIBOOL watchApi(APIBOOL functionOK)
{
    if (!functionOK)
        if (__apiErrorCode(handle)!=EDIABAS_ERR_NONE)
            if (!errorHandlerCalled && errorHandler!=NULL) {
                (*errorHandler)();
                errorHandlerCalled=APITRUE;
            }
    return functionOK;
}

                        
/***** API-Funktionsabbildungen **********************************************/

APIBOOL apiInit(void)
{
    errorHandler = NULL;
	callbackHandler = NULL;
    errorHandlerCalled=APIFALSE;
    jobState=APIREADY;
    return watchApi(__apiInit(&handle));
}

APIBOOL apiInitExt(const char *device,const char *devConnection,const char *devApplication,const char *reserved)
{
    errorHandler = NULL;
	callbackHandler = NULL;
    errorHandlerCalled=APIFALSE;
    jobState=APIREADY;
    return watchApi(__apiInitExt(&handle,device,devConnection,devApplication,reserved));
}

void apiEnd(void)
{
    if (apiState()==APIBUSY) {
        apiBreak();
        watchApi(APIFALSE);
    }
    __apiEnd(handle);
}

APIBOOL apiSwitchDevice(const char *connection,const char *application)
{
    return watchApi(__apiSwitchDevice(handle,connection,application));
}

void apiJob(const char *ecu,const char *job,const char *para,const char *result)
{
    errorHandlerCalled=APIFALSE;
    __apiJob(handle,ecu,job,para,result);
    jobState=APIBUSY;
    if (callbackHandler!=NULL)
        apiState();
    watchApi(APIFALSE);
}

void apiJobData(const char *ecu,const char *job,
                const unsigned char *parabuf,int paralen,const char *result)
{
    errorHandlerCalled=APIFALSE;
    __apiJobData(handle,ecu,job,parabuf,paralen,result);
    jobState=APIBUSY;
    if (callbackHandler!=NULL)
        apiState();
    watchApi(APIFALSE);
}

void apiJobExt(const char *ecu,const char *job,
               const unsigned char *stdparabuf,int stdparalen,
               const unsigned char *parabuf,int paralen,
               const char *result,long reserved)
{
    errorHandlerCalled=APIFALSE;
    __apiJobExt(handle,ecu,job,stdparabuf,stdparalen,parabuf,paralen,result,reserved);
    jobState=APIBUSY;
    if (callbackHandler!=NULL)
        apiState();
    watchApi(APIFALSE);
}

int apiJobInfo(char *infoText)
{
    int pos=__apiJobInfo(handle,infoText);
    watchApi(APIFALSE);
    return pos;
}

APIBOOL apiResultChar(APICHAR *buf,const char *result,APIWORD set)
{
    return watchApi(__apiResultChar(handle,buf,result,set));
}

APIBOOL apiResultByte(APIBYTE *buf,const char *result,APIWORD set)
{
    return watchApi(__apiResultByte(handle,buf,result,set));
}

APIBOOL apiResultInt(APIINTEGER *buf,const char *result,APIWORD set)
{
    return watchApi(__apiResultInt(handle,buf,result,set));
}

APIBOOL apiResultWord(APIWORD *buf,const char *result,APIWORD set)
{
    return watchApi(__apiResultWord(handle,buf,result,set));
}

APIBOOL apiResultLong(APILONG *buf,const char *result,APIWORD set)
{
    return watchApi(__apiResultLong(handle,buf,result,set));
}

APIBOOL apiResultDWord(APIDWORD *buf,const char *result,APIWORD set)
{
    return watchApi(__apiResultDWord(handle,buf,result,set));
}

APIBOOL apiResultReal(APIREAL *buf,const char *result,APIWORD set)
{
    return watchApi(__apiResultReal(handle,buf,result,set));
}

APIBOOL apiResultText(APITEXT *buf,const char *result,APIWORD set,const char *format)
{
    return watchApi(__apiResultText(handle,buf,result,set,format));
}

APIBOOL apiResultBinary(APIBINARY *buf,APIWORD *buflen,const char *result,APIWORD set)
{
    return watchApi(__apiResultBinary(handle,buf,buflen,result,set));
}

APIBOOL apiResultFormat(APIRESULTFORMAT *buf,const char *result,APIWORD set)
{
    return watchApi(__apiResultFormat(handle,buf,result,set));
}

APIBOOL apiResultNumber(APIWORD *buf,APIWORD set)
{
    return watchApi(__apiResultNumber(handle,buf,set));
}

APIBOOL apiResultName(char *buf,APIWORD index,APIWORD set)
{
    return watchApi(__apiResultName(handle,buf,index,set));
}

APIBOOL apiResultSets(APIWORD *sets)
{
    return watchApi(__apiResultSets(handle,sets));
}

APIBOOL apiResultVar(APITEXT *ecu)
{
    return watchApi(__apiResultVar(handle,ecu));
}

APIRESULTFIELD apiResultsNew(void)
{
    APIRESULTFIELD field = __apiResultsNew(handle);
    watchApi(APIFALSE);
    return field;
}

void apiResultsScope(APIRESULTFIELD field)
{
    __apiResultsScope(handle,field);
    watchApi(APIFALSE);
}

void apiResultsDelete(APIRESULTFIELD field)
{
    __apiResultsDelete(handle,field);
    watchApi(APIFALSE);
}

int apiState(void)
{
    if ((jobState=__apiState(handle))!=APIREADY) {
        if (callbackHandler!=NULL)
            if (jobState==APIBUSY)
                if ((*callbackHandler)())
                    apiBreak();
        watchApi(APIFALSE);
    }
    return jobState;
}

void apiBreak(void)
{
    __apiBreak(handle);
    jobState=APIBREAK;
    watchApi(APIFALSE);
}

void apiCallBack(APIBOOL (*action)(void))
{
    callbackHandler=action;
}

void apiErrorHandler(void (*action)(void))
{
    errorHandler=action;
}

int apiErrorCode(void)
{
    return __apiErrorCode(handle);
}

const char *apiErrorText()
{
    static char textbuffer[256];
    
    __apiErrorText(handle,textbuffer,256);
    
    return ((*textbuffer)? textbuffer:NULL);
}

APIBOOL apiSetConfig(const char *configName,const char *configValue)
{
    return __apiSetConfig(handle,configName,configValue);
}

APIBOOL apiGetConfig(const char *configName,char *configValue)
{
    return __apiGetConfig(handle,configName,configValue);
}


/*****************************************************************************/
