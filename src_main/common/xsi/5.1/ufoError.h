
#ifndef	__ufoError
#define	__ufoError

#include "ufoTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef	enum 
{
	ufoNoError,	
	ufoDsoOpenError, 
	ufoDsoCloseError, 
	ufoNoProcessDefineFunc,	
	ufoParamTypeNotSupported, 
	ufoNoProcessRenderFunc
		
} ufoError;



UFOAPI
ufoError ufoGetError(void);

UFOAPI
void	 ufoSetError(ufoError error_no);

UFOAPI
void	 ufoResetError(void);


#ifdef __cplusplus
}
#endif

#endif
