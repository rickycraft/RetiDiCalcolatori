/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RPC_XFILE_H_RPCGEN
#define _RPC_XFILE_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#define TARGA_LENGTH 7
#define PATENTE_LENGTH 5
#define DATA_LENGTH 11
#define MAX_STRING 256
#define MAX_PRENOTAZIONI 10

struct Prenotazione {
	char targa[TARGA_LENGTH];
	char patente[PATENTE_LENGTH];
	char tipo[DATA_LENGTH];
	char dir[MAX_STRING];
};
typedef struct Prenotazione Prenotazione;

struct Input {
	char targa[TARGA_LENGTH];
	char patente[PATENTE_LENGTH];
};
typedef struct Input Input;

struct Output {
	Prenotazione prenotazioni[MAX_PRENOTAZIONI];
};
typedef struct Output Output;

struct Tipo {
	char tipo[DATA_LENGTH];
};
typedef struct Tipo Tipo;

#define RPCESAME 0x20000020
#define RPCESAMEVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define visualizza_prenotazioni 1
extern  Output * visualizza_prenotazioni_1(Tipo *, CLIENT *);
extern  Output * visualizza_prenotazioni_1_svc(Tipo *, struct svc_req *);
#define aggiorna_licenza 2
extern  int * aggiorna_licenza_1(Input *, CLIENT *);
extern  int * aggiorna_licenza_1_svc(Input *, struct svc_req *);
extern int rpcesame_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define visualizza_prenotazioni 1
extern  Output * visualizza_prenotazioni_1();
extern  Output * visualizza_prenotazioni_1_svc();
#define aggiorna_licenza 2
extern  int * aggiorna_licenza_1();
extern  int * aggiorna_licenza_1_svc();
extern int rpcesame_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_Prenotazione (XDR *, Prenotazione*);
extern  bool_t xdr_Input (XDR *, Input*);
extern  bool_t xdr_Output (XDR *, Output*);
extern  bool_t xdr_Tipo (XDR *, Tipo*);

#else /* K&R C */
extern bool_t xdr_Prenotazione ();
extern bool_t xdr_Input ();
extern bool_t xdr_Output ();
extern bool_t xdr_Tipo ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_RPC_XFILE_H_RPCGEN */
