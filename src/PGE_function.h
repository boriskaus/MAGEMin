#ifndef __PGE_FUNCTION_H_
#define __PGE_FUNCTION_H_

#include "MAGEMin.h"

global_variable PGE(			struct bulk_info 	z_b,
								global_variable 	gv,
								
								PP_ref 				*PP_ref_db,
								SS_ref 				*SS_ref_db,
								csd_phase_set  		*cp	);

double norm_vector(double *array ,int n);

#endif
