/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "idManager.h"

bool_t
xdr_information (XDR *xdrs, information *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->nombre, MAX_NAME_SIZE,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_request (XDR *xdrs, request *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_long (xdrs, &objp->id))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->nombre, MAX_NAME_SIZE,
		sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_infoResult (XDR *xdrs, infoResult *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->cod_ret))
		 return FALSE;
	switch (objp->cod_ret) {
	case 0:
		 if (!xdr_information (xdrs, &objp->infoResult_u.data))
			 return FALSE;
		break;
	case -1:
		 if (!xdr_vector (xdrs, (char *)objp->infoResult_u.message, MAX_MESSAGE_SIZE,
			sizeof (char), (xdrproc_t) xdr_char))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}

bool_t
xdr_idResult (XDR *xdrs, idResult *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->cod_ret))
		 return FALSE;
	switch (objp->cod_ret) {
	case 0:
		 if (!xdr_long (xdrs, &objp->idResult_u.id))
			 return FALSE;
		break;
	case -1:
		 if (!xdr_vector (xdrs, (char *)objp->idResult_u.message, MAX_MESSAGE_SIZE,
			sizeof (char), (xdrproc_t) xdr_char))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}
