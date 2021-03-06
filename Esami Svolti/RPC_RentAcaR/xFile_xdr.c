/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "xFile.h"

bool_t
xdr_Licenza (XDR *xdrs, Licenza *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->targa, MAX_STRING))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->patente, 5))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Prenotazione (XDR *xdrs, Prenotazione *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->targa, MAX_STRING))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->patente, MAX_STRING))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->tipo, MAX_STRING))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->immagini, MAX_STRING))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Prenotazioni (XDR *xdrs, Prenotazioni *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->list, K,
		sizeof (Prenotazione), (xdrproc_t) xdr_Prenotazione))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->numero))
		 return FALSE;
	return TRUE;
}
