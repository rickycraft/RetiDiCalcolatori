/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "operazioni.h"

bool_t
xdr_Giudice (XDR *xdrs, Giudice *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->nome, 256))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->votiTotali))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Giudici (XDR *xdrs, Giudici *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->giudici_ordinati, NUMGIUDICI,
		sizeof (Giudice), (xdrproc_t) xdr_Giudice))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Candidato (XDR *xdrs, Candidato *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->nomeCandidato, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->nomeGiudice, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_char (xdrs, &objp->categoria))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->nomeFile, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	 if (!xdr_char (xdrs, &objp->fase))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->voto))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_CandidatoVoto (XDR *xdrs, CandidatoVoto *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->nome, 256))
		 return FALSE;
	 if (!xdr_char (xdrs, &objp->tipoOperazione))
		 return FALSE;
	return TRUE;
}
