#ifndef JSCB_LKT_INC_H
#define JSCB_LKT_INC_H

typedef int (*FwdCallback) (int tunnel_id, byte* data, uint len);

int RegLktFwdCCallback(FwdCallback callback);   /* FWDS-->LKT(CallBack)-->FWDC */

int RegLktFwdSCallback(FwdCallback callback);   /* FWDC-->LKT(CallBack)-->FWDS */


#endif


