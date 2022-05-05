.. MAGEMin documentation

Problem definition
==================

Finding the most stable phase assemblage is a challenging constrained optimization problem including both equality and inequality constraints. One has to minimize the Gibbs energy (1) of the system while satisfying the Gibbs-Duhem (2) and mass equality constraints (3) while satisfying the mixing-in-sites inequality constraints (4).

1. Total Gibbs energy
**********************

The Gibbs energy of a multi-component multiphase system is given by the weighted summation of the chemical potentials of all end-members and pure phases:

.. math::
   G_{sys} = \sum_{\lambda=1}^{\Lambda} n_{\lambda} \sum_{i=1}^{N_{\lambda}} \mu_{i}(\lambda) p_{i}(\lambda) + \sum_{\omega=1}^{\Omega} n_{\omega} \mu_{\omega} 

where :math:`n_{\lambda}` is the molar fraction of the solution phase, :math:`p_{\lambda}` is the molar fraction of the endmembers and :math:`n_{\omega}` is the molar fraction of the pure phase.

The chemical potential of a phase is either a constant for a condensed (pure) phase:

.. math:: 
   \mu_{i} = G_{i}^{0}

or a function for a phase within a solution:

.. math:: 
   \mu_{i} = G_{i}^{0} + RTlog(a_{i}) + G_{i}^{ex}

where :math:`a_{i}` is the thermochemical activity related to the mole fraction and the activity coefficient by

.. math::
   a_{i} = x_{i} \gamma_{i}


For the case of ideal mixing between the end-members, the activity coefficient is unity. The mixing of a species dissolved in a condensed phase, however rarely behaves ideally
and is typically a function of both temperature and composition (mixing-on-sites formulation).

2. Gibbs-Duhem constraint
*************************

The Gibbs-Duhem constraint is defined as

.. math:: 
   \sum_{j=1}^{C} \Gamma_{j} a_{ij} - \mu_{i} = 0
   
where :math:`\Gamma_{j}` is the chemical potential of pure component (oxide) :math:`j` and :math:`a_{ij}` is the molar compositional of component :math:`j` in end-member/pure phase :math:`i`

3. Mass constraint
******************

The mass equality constraint is defined as

.. math::
   \sum_{\lambda=1}^{\Lambda} n_{\lambda} \sum_{i=1}^{N_{\lambda}} a_{ij}(\lambda) p_{i}(\lambda) + \sum_{\omega=1}^{\Omega} n_{\omega} a_{\omega j} - b_j = 0

where :math:`a_{ij}` is the molar composition of component :math:`j` in end-member :math:`i`, and :math:`a_{\omega j}` is the molar composition of component j in a pure phase

Minimization approach 
=====================

The Gibbs minimization approach employed in MAGEMin combines discretization of the equations of state in composition space with linear programming and extends the mass constrained Gibbs-hyperplane rotation method to account for the mixing-on-sites that takes place in silicate mineral solid solutions. For an exhaustive description of the methodology see Riel et (submitted). 

.. image:: /figs/GFE_intro.png
	:width: 640
	:align: center

|

Algorithm demonstration
=======================

A simplified example of the Gibbs energy minimization approach used in MAGEMin is provided at:

https://github.com/ComputationalThermodynamics/SandBox/blob/main/MAGEMin_PGE_and_LP.m

This MATLAB application includes two pure phases, sillimanite and quartz, and activity--composition (a-x) relations for feldspar (pl4T, Holland et al., 2021) in a reduced Na2O-CaO-K2O-Al2O3-SiO2 (NCKAS) chemical system. 

**References**

*Holland, T. J. B., Green, E. C. R., & Powell, R.(2021). A thermodynamic model for feldspars in KAlSi3O8-NaAlSi3O8-CaAl2Si2O8 for mineral equilibrium calculations*
