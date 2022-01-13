/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "RPC_xFile.h"

bool_t
xdr_Input (XDR *xdrs, Input *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->nomeDir, MAX_STRING))
		 return FALSE;
	 if (!xdr_char (xdrs, &objp->carattere))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->numOccorrenze))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_NomeFile (XDR *xdrs, NomeFile *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->nomeFile, MAX_STRING))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Output (XDR *xdrs, Output *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->listaNomeFile, N,
		sizeof (NomeFile), (xdrproc_t) xdr_NomeFile))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->indice))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->esito))
		 return FALSE;
	return TRUE;
}