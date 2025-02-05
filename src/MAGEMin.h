#ifndef __MAGEMIN_H_
#define __MAGEMIN_H_

#include "MAGEMin.h"

#ifdef _WIN32
#define mkdir(path,mode) _mkdir(path) 
#endif

#define n_em_db 291
#define nEl 11

#include "nlopt.h"
#include "gem_function.h"

/*---------------------------------------------------------------------------*/ 
int runMAGEMin(					int argc, 
								char ** argv			);

/* Function declaration from Initialize.h file */
int find_EM_id(					char* em_tag			);

/** Normalize array so that the sum = 1 */
double* norm_array(				double *array, 
								int size				);

/** declare function to get benchmark bulk rock composition **/
void get_bulk(					double *bulk_rock, 
								int test, 
								int n_El				);

/** Function to retrieve database structure **/
struct EM_db Access_EM_DB(		int id, 
								int EM_database			);

/** Function to retrieve the endmember names from the database **/
char** get_EM_DB_names(			int EM_database			);

/** Return the number of zero element in the bulk-rock, position of zeros and non zeros elements **/
struct bulk_info zeros_in_bulk(	double *bulk_rock, 
								double P, 
								double T				);

/*---------------------------------------------------------------------------*/ 
/* structure declaration */
/** store endmember database **/
struct EM_db {
	char   Name[20];			/** pure species name 														*/
    double Comp[12];       	 	/** pure species composition [0-10] + number of atom [11] 					*/
    double input_1[3];          /** first line of the thermodynamics datable 								*/
    double input_2[4];          /** second line of the thermodynamics datable 								*/
    double input_3[11];         /** third line of the thermodynamics datable 								*/
    double input_4[3];         	/** third line of the thermodynamics datable 								*/
};

/* Declare structures to hold reference gbase, composition and factor for solid solutions */
/* "bi","cpx","crd","ep","fl","g","hb","ilm","liq","mu","ol","opx","pl4T","spn" */
typedef struct SS_refs {
	double P;					/** used to pass to local minimizer, would allow to have pressure difference for liq/solid */
	double T;
	double R;

	/** end-member list */
	char   **EM_list;			/** solution phase list */

	/** flags */
	double   ss_n;				/** fraction of solution phase in estimated phase assemblage 				*/
	double   delta_ss_n;		/** fraction of solution phase in estimated phase assemblage 				*/
	int     *ss_flags;			/** integer table for solution phase list 									*/
	int 	 CstFactor;			/** flag to indicate if the solution model have a p-dependent apf			*/

	/** data needed for levelling and/or PGE **/
	int      n_pc;				/** maximum number of pseudocompounds to store 								*/
	int      tot_pc;			/** total number of pseudocompounds  										*/
	int      id_pc;				/** total number of pseudocompounds  										*/
	int     *n_swap;			/** number of time PC has been added to the assemblage 						*/
	int     *info;				/** store some infos for debugging 											*/
	double  *G_pc;				/** array to store the gibbs energy of the pseudocompounds 					*/
	double  *DF_pc;				/** array to store the final driving force of the pseudocompounds 			*/
	double **comp_pc;			/** compositional array of the pseudocompounds 								*/
	double **p_pc;				/** compositional array of the pseudocompounds 								*/
	double **mu_pc;				/** compositional array of the pseudocompounds 								*/
	double **xeos_pc;			/** x-eos array of the pseudocompounds 										*/
	double  *factor_pc;			/** normalization factor of each PC, mainly useful for liquid 				*/
	double  *ub_pc;				/** upper bounds for pc 													*/
	double  *lb_pc;				/** lower bounds for pc 													*/
	
	/** data needed for phase change and solvus processing **/	
	int	    *solvus_id;
	
	/** data needed for levelling and/or PGE **/
	int	     min_mode;			/** flag of the minimization mode 											*/
	int		 is_liq;			/** check if phase is "liq" 												*/
	int      symmetry;			/** solution phase symmetry  												*/
	int      n_em;				/** number od endmembers 													*/
	int 	 n_xeos;			/** number of compositional variables 										*/
	int      n_sf;				/** number of site fraction 												*/
	int      n_w;				/** number of margules entries												*/
	
	double **eye;				/** identity matrix															*/
	double  *W;					/** margules 																*/
	double  *v;					
	double   sum_v;
	int		 n_v;
	
	int 	 sf_ok;				/** site fractions are satisfied? 											*/
    double **Comp;    			/** 2d array of endmember composition 										*/
    double  *gbase;        		/** 1d array of gbase 														*/

    double **mu_array;        	/** 2d array of gbase, including values for numerical differentiation 		*/
    double  *gb_lvl;
    double   factor;			/** normalizing factor 														*/
    double **bounds;			/** x-eos bounds 															*/
    double **bounds_ref;		/** x-eos bounds 															*/
    double  *z_em; 				/** 1d array to deactivate endmembers when bulk-rock = 0; this part is needed to calculat xi in PGE method */
    int      n_guess;			/** number of initial guesses used to solve for solvi (or local minimum) 	*/
    double  *iguess;    		/** 2d array of initial guess 												*/
	double  *dguess;    		/** 2d array of default guess 												*/
	
    /** data needed for local minimization **/
    double   check_df;			/** driving force from PC, stored for mode 3								*/
	int 	 forced_stop;		/** 0-1, check if local minimization left to a forced stop 					*/
	int      xeos_sf_ok_saved;	/** 0-1, to check if the xeos are saved										*/
	int      status;			/** status of the local minimization (ideally 0) */
	int      nlopt_verb; 		/** verbose of NLopt, to show local iterations   							*/
	double  *tol_sf;			/** array of tolerance for the inequality constraints 						*/
	double  *lb;				/** array of tolerance for the inequality constraints 						*/
	double  *ub;				/** array of tolerance for the inequality constraints 						*/
	

	/** NLopt memory allocation */
	nlopt_opt opt;				/** send NLopt optimizer													*/
	double   fbc;				/** saved number of atoms of the bulk rock composition 						*/
	double   sum_apep;			/** saved number of atoms of the bulk rock composition 						*/
	double  *p;					/** Following declarations needed for local minimizer 						*/
	double  *ape;				/** Number of atoms per endmember 					 						*/
	double  *mat_phi;
	double  *mu_Gex;			/** excess energy 															*/
	double  *sf;				/** site fractions 															*/
	double  *dsf;				/** jacobian of site fraction												*/
	double  *mu;				/** chemical potentials 													*/
	double  *dfx;				/** gradient of objective function 											*/
	double **dp_dx;				/** partial derivative of endmember fraction as function of compositional variables */
	double   df;				/** save driving force: delta_G from G-hyperplane 							*/
	double   df_raw;			/** save driving force: delta_G from G-hyperplane 							*/
	double   LM_time;			/** local minimization time  												*/

	/* data needed for PGE iterations */
    double  *ss_comp;			/** 1d array of solid solution composition */
	double  *xi_em;				/** endmember fraction calculated for PGE method (depends on exp expression) */
	double   sum_xi;			/** store sum of xi 									*/
	double  *xeos; 				/** previous minimized x-eos	 						*/
	double  *xeos_sf_ok;		/** save xeos that satisfy the SF 						*/
	
	/* data output */
	double  *ElShearMod;		/** density of the endmembers 							*/
	double  *density;			/** density of the endmembers 							*/
	double   phase_density;		/** density of the phase 								*/
	double   volume;			/** volume of the phase 								*/
	double   mass;				/** mass of the phase 									*/

} SS_ref;

/* structure to store input data */
typedef struct IODATA {
	int 	 n_phase;			/** number of phase for which x-eos has to be loaded 	*/
	double 	 P;					/** prescribed pressure 								*/
	double 	 T;					/** prescribed temperature 								*/
	double  *bulk;				/** bulk rock composition if no test has been given 	*/
	double  *in_gam;			/** provided gamma from input file 						*/
	char   **phase_names;		/** solution phase names  								*/
	double **phase_xeos;		/** solution phases compositional variables	 			*/
	double **phase_emp;			/** solution phases endmember proportion	 			*/
	
} io_data;

/* structure to store output data (mostly for use with julia) */
typedef struct OUTDATA {
	int 	 n_phase;			/** number of phase for which x-eos has to be loaded 	*/
	double 	 P;					/** prescribed pressure 								*/
	double 	 T;					/** prescribed temperature 								*/
	double   G_system;			/** G of the system @ equilibrium 						*/
	double 	 BR_norm;			/** Bulk rock norm 										*/
	double 	 dG_norm;			/** Bulk rock norm 										*/
	int 	 number;			/** number of point										*/
	int 	 status;			/** status of calculation								*/

	double  *Gamma;				/** Gamma of stable solution 							*/
	
	int 	 n_SS;				/** # of stable solid solutions 						*/
	int 	 n_PP;				/** # of stable pure phases								*/
	char   **StableSolutions;	/** Names of the stable solutions 						*/
	double	*StableFractions;	/* Fractions of the stable solutions 					*/
	double 	*Phasedensity;		/* Density of each of the phases 						*/
	int 	 max_num_EM;		/* Max. number of endmembers (hardcoded)				*/
	int 	*n_em;				/* # of endmembers for each solid solution  			*/
	double **xEOS;				/* Compositional variables for each stable EM within a solid solution	*/
	double **p_EM;				/* Proportions of each of the endmembers   within a solid solution		*/
	
} out_data;

/* structure to store position of zeros and non-zeros positions in bulk_rock composition */
struct bulk_info {
	double   P;					/** store pressure 										*/
	double   T;					/** store temperature 									*/
	double   R;
	double  *bulk_rock;			/** bulk rock composition in weigth  					*/
	int      nzEl_val;			/** number of non zero entries in the bulk 				*/
	int      zEl_val;			/** number of zero entries in the bulk 					*/
    int     *nzEl_array;   		/** position of non zero entries in the bulk 			*/
    int     *zEl_array;    	 	/** position of zero entries in the bulk 				*/
    double  *apo;				/** atom per oxide 										*/
    double   fbc;				/** atom per oxide 										*/
    double  *masspo;			/** atom per oxide 										*/
};

/* structure to informations about the considered set of phases during  minimization 	*/
typedef struct csd_phase_sets {
	char   *name;				/** local copy of the phase name 						*/
	
	int     split;
	int     in_iter;
	int 	id;					/** id of solution phas 								*/
	int 	n_xeos;				/** number of compositional variables 					*/
	int 	n_em;	
	int 	n_sf;
	int		sf_ok;
	
	int    *ss_flags;		
	
	double 	ss_n;
	double  delta_ss_n;
	double 	df;
	double 	factor;
	double  min_time;
	double  sum_xi;
	double  sum_dxi;

	double *p_em;
	double *xi_em;
	double *lvlxeos;
	double *dguess;
	double *xeos;
	double **dpdx; 				/** This one is needed for the back2feasible system function */
	
	double *dfx;
	double *mu;
	double *mu0;	
	double *delta_mu;
	double *sf;
	double *ss_comp;
	double *gbase;				/** chemical potentials 									*/

	double mass;
	double volume;
	double phase_density;
	double phase_cp;
	double phase_expansivity;
	double phase_bulkModulus;
	double phase_shearModulus;

} csd_phase_set;

/* hold information of solution phases */
typedef struct stb_SS_phases {
	int      nOx;
	
	double   f;
	double   G;
	double   deltaG;
	double   V;
	double   alpha;
	double   cp;
	double   rho;
	double   bulkMod;
	double   shearMod;
	double   Vp;
	double   Vs;
	
	int      n_xeos;
	int      n_em;
	
	double  *Comp;
	double  *compVariables;
	
	char   **emNames;
	double  *emFrac;
	double  *emChemPot;
	double **emComp;
	
	//double  *siteFrac;
	
} stb_SS_phase;

/* hold information of pure phases */
typedef struct stb_PP_phases {
	int    	 nOx;
	
	double   f;
	double   G;
	double   deltaG;
	double   V;
	double   alpha;
	double   cp;
	double   rho;
	double   bulkMod;
	double   shearMod;
	double   Vp;
	double   Vs;	
	
	double  *Comp;
	
} stb_PP_phase;

/* structure to store informations of stable phase equilibria */
typedef struct stb_systems {
	
	char   *MAGEMin_ver;
	
	double  bulk_res_norm;
	int     n_iterations;
	int     status;
	
	int     nOx;
	char  **oxides;
	
	double  P;
	double  T;
	double *bulk;
	
	double *gamma;
	double  G;
	double  rho;
	
	double  bulkMod;
	double  shearMod;
	double  Vp;
	double  Vs;
	
	double *bulk_S; double frac_S; double rho_S;  	/* Solid system informations 												*/
	double *bulk_M; double frac_M; double rho_M; 	/* Melt system informations 												*/
	double *bulk_F; double frac_F; double rho_F; 	/* Fluid system informations 												*/
	
	int     n_ph;									/* number of predicted stable phases 										*/
	int     n_PP;									/* number of predicted stable pure phases 									*/
	int     n_SS;									/* number of predicted stable solution phases 								*/

	char  **ph;										/* phases names 															*/
	double *ph_frac; 								/* phase fractions															*/
	int    *ph_type; 								/* 0 -> Solution phases; 1 -> Pure phases									*/
	int    *ph_id;									/* position in the related stb_SS_phase or stb_PP_phase structure arrays	*/
	
	stb_SS_phase *SS;
	stb_PP_phase *PP;

} stb_system;

/* structure to store global variables */
typedef struct global_variables {
	
	/* GLOBAL PARAMETERS */
	char    *version;			/** MAGEMin version */
	int      verbose;			/** verbose variable: 0, none; 1, all */
	char    *outpath;			/** output path */
	int      Mode;				/** calcultion mode, 0 = full minimization, 1 = extract solution phases informations, 2 = local minimization */
	double **numDiff;
	int      n_Diff;
	int      status;			/** status of the minimization */
	
	/* GENERAL PARAMETERS */
	double   relax_PGE;
	double   relax_PGE_val;
	double   PC_df_add;
	double   PC_min_dist;
	double	 PC_check_val;
	int	     check_PC;
	int 	 check_PC_ite;
	double   act_varFac_stab;
	int      len_pp;			/** initial number of active pure phases */
	int      len_ss;			/** initial number of active solution phases */
	int      len_ox;			/** number of components (number of oxides in the chemical system) */
	int 	 max_n_cp;			/** number of considered solution phases */
	int 	 len_cp;
	char   **ox;				/** component names (for outputing purpose only) */
	double  *gam_tot;     		/** chemical potential of components (gamma) */
	double  *del_gam_tot;     	/** chemical potential of components (gamma) */
	double  *delta_gam_tot;     /** chemical potential of components (gamma) */
				
	int      n_flags;			/** number of column in the flag array */
	char   **PP_list;			/** pure phase list */
	char   **SS_list;			/** solution phase list */
	
	double  *pp_n;				/** fraction of pure phase in estimated phase assemblage */
	double  *pp_xi;				/** penalty term -> distance from G-hyperplane */
	double  *delta_pp_n;		/** fraction of pure phase in estimated phase assemblage */
	double  *delta_pp_xi;		/** penalty term -> distance from G-hyperplane*/
	int    **pp_flags;			/** integer table for pure phase list */	

	int      numPoint; 			/** the number of the current point */
	int      global_ite;		/** global iteration increment */

	/* LEVELLING */
	double   LVL_time;			/** time taken for levelling (ms) */
	double   em2ss_shift;		/** small value to retrieve x-eos from pure endmember after levelling */
	
	/* PSEUDOCOMPOUNDS */
	double   bnd_filter_pc;     /** value of driving force the pseudocompound is considered to reduce the compositional space */
	int  	 n_pc;
	double 	 max_G_pc;
	int     *n_SS_PC;
	double  *SS_PC_stp;
	double   eps_sf_pc;	
	
	/* SOLVI */
	int     *verifyPC;			/** allow to check for solvi */
	int 	*n_solvi;			/** number of phase considered for solvi */
	int    **id_solvi;			/** cp id of the phases considered for solvi */
			
	/* LOCAL MINIMIZATION */
	double   ineq_res;			/** relative residual for local minimization (inequality constraints)*/
	double   obj_tol;			/** relative residual for local minimization */

	int     *newly_added;       /** index of the newly added solution phase */
	double   box_size_mode_1;	/** edge size of the hyperdensity used during local minimization */
	int   	 maxeval;			/** maximum number of objective function evaluations during local minimization */
	int   	 maxeval_mode_1;	/** maximum number of objective function evaluations during local minimization for mode 1 */
	double   bnd_val;			/** boundary value for x-eos when the fraction of an endmember = 0. */
	double 	*init_prop;			/** holds the initial proportions of the EM's, in case we do Mode=2 with only a single point */

	/* PARTITIONING GIBBS ENERGY */ 
	double 	*A_PGE;				/** RHS */
	double  *b_PGE;				/** LHS */
	double 	*dn_cp;
	double 	*dn_pp;
	int 	*cp_id;
	int 	*pp_id;	
	double   fc_norm_t1;
	int      outter_PGE_ite;    /** number of PGE outter iterations */
	int      inner_PGE_ite;     /** number of PGE outter iterations */
	double   inner_PGE_ite_time;
	
	int      n_phase;			/** number of estimated stable phases */	
	int 	 n_pp_phase;		/** number of active pure phases */
	int 	 n_cp_phase;		/** number of considered solution phases */

	double   max_n_phase;		/** maximum wt% change during PGE iteration     */
	double   max_g_phase;		/** maximum Gamma change during PGE iteration */
	double   br_liq_x;
	double   max_fac;			/** max updating factor */
	double 	 max_br;
	int      it_1;              /** first critical iteration                                                      */
	double   ur_1;              /** under relaxing factor on mass constraint if iteration is bigger than it_1     */
	int      it_2;              /** second critical iteration                                                     */
	double   ur_2;              /** under relaxing factor on mass constraint if iteration is bigger than it_2     */
	int      it_3;              /** third critical iteration                                                      */
	double   ur_3;              /** under relaxing factor on mass constraint if iteration is bigger than it_3     */
	int      it_f;              /** send a failed message when the number of iteration is greater than this value */
	int      it_slow;           /** critical iteration for slow convergence                                       */
	double   ur_slow;           /** under relaxing factor on mass constraint defining overly slow convergence     */
	double   ur_break;          /** under relaxing factor on mass constraint defining a breaking iteration        */
	int      div;               /** send status of divergence */

	/* DECLARE ARRAY FOR PGE CALCULATION */	
	double	*dGamma;			/** array to store gamma change */
	
	double  *PGE_mass_norm;		/** save the evolution of the norm */
	double  *PGE_total_norm;	/** save the evolution of the moving norm */
	double  *gamma_norm;		/** save the evolution of the gamma norm */
	double  *ite_time;
	double   G_system;      	/** Gibbs energy of the system */
	double   G_system_mu;		/** Gibbs energy of the system based on fraction and chemical potential of phases **/

	double   br_norm;			/** norm of bulk-rock mass constraint (residual) */
	double   br_max_tol;    	/** max residual on mass-constraint */
	double   alpha;				/** active under-relaxing factor of PGE, used to check if a phase can be reintroduced */
	
	/* PHASE UPDATE */ 
	double   merge_value;		/** norm distance between two instance of a solution phase under which the instances are merged into 1 */
	double   re_in_n;			/** fraction of phase when being reintroduce.  */
	double   remove_dG_val; 	/** delta_G value at which a phase can be removed */ 
	double   remove_sum_xi;		/** sum xi value at which a phase can be removed */
	int      ph_change;

	/* LEAST SQUARE OPTIMIZATION */
	double **A;					/** save stoechiometry matrix to pass to least square optimization */
	double  *b;					/** save bulk rock to pass to least square optimization */
	double  *mass_residual;		/** bulk rock residual */
	double   BR_norm;			/** norm of bulk rock residual  */
	
	/* DENSITY/CP MODULUS CALC */
	double   gb_P_eps;			/** small value to calculate V using finite difference: V = dG/dP */
	double   gb_T_eps;			/** small value to calculate V using finite difference: V = dG/dP */
	double   system_density;
	double   system_bulkModulus;
	double   system_shearModulus;
	double   system_Vp;
	double   system_Vs;
	
} global_variable;

global_variable global_variable_init(void);

/** Stores databases **/
typedef struct Database {	PP_ref     		*PP_ref_db;			/** Pure phases 											*/
							SS_ref     		*SS_ref_db;			/** Solid solution phases phases 							*/
							csd_phase_set   *cp;				/** considered solution phases (solvus setup) 				*/
							stb_system      *sp;				/** structure holding the informations of the stable phases */
							char 	  		**EM_names;			/** Names of endmembers 									*/
} Databases;

Databases InitializeDatabases(				global_variable 	 gv, 
											int 				 EM_database			);

void FreeDatabases(							global_variable 	 gv, 
											Databases			 DB					);

global_variable ComputeEquilibrium_Point(	int 				 EM_database,
											io_data 			 input_data,
											int 				 Mode,
											struct bulk_info 	 z_b,
											global_variable 	 gv,
											PP_ref  			*PP_ref_db,
											SS_ref  			*SS_ref_db,
											csd_phase_set  		*cp					);
											
global_variable ComputePostProcessing(		int 				 EM_database,
											struct bulk_info 	 z_b,
											global_variable 	 gv,
											PP_ref  			*PP_ref_db,
											SS_ref  			*SS_ref_db,
											csd_phase_set  		*cp					);											

global_variable ReadCommandLineOptions(	global_variable   gv,
										int 			  argc, 
										char 			**argv, 
										int 			 *Mode_out, 
										int 			 *Verb_out, 
										int 			 *test_out, 
										int 			 *n_points_out, 
										double 			 *P, 
										double 			 *T, 
										double 			  Bulk[11], 
										double 			  Gam[11], 
										double 			  InitEM_Prop[15], 
										char 			  File[50], 
										char 			  Phase[50], 
										int 			 *maxeval_out, 
										int     		 *get_version_out,
										int 			 *get_help					);

/* function that prints output */
void PrintOutput(	global_variable 	gv, 
					int 				rank, 
					int 				l, 
					Databases 			DB, 
					double 				time_taken, 
					struct bulk_info 	z_b)				;

/* function converting the solver status code to human-readable text and printing it to screen */
void PrintStatus(	int status );

#endif
