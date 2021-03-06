/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "RPC_xFile.h"

bool_t
xdr_Evento (XDR *xdrs, Evento *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->id, MAX_STRING))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->tipo, MAX_STRING))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->data, DATA_LENGTH))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->luogo, MAX_STRING))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->bigliettiDisponibili))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->prezzo))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Input (XDR *xdrs, Input *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->id, MAX_STRING))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->numBigliettiDaAcquistare))
		 return FALSE;
	return TRUE;
}
