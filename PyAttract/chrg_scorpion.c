/* chrg_scorpion.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Common Block Declarations */

struct {
    doublereal zcharge[20000], zcoorx[20000], zcoory[20000], zcoorz[20000], 
	    zcgcox[2000], zcgcoy[2000], zcgcoz[2000], xpts[100000], ypts[
	    100000], zpts[100000], bx, by, bz;
    integer znatom;
} chgcoo_;

#define chgcoo_1 chgcoo_

/* Table of constant values */

static doublereal c_b4 = .001;
static integer c__9 = 9;
static integer c__1 = 1;
static integer c__3 = 3;
static integer c__5 = 5;
static logical c_true = TRUE_;
static integer c__2 = 2;

/* Subroutine */ int chrg_scorpion__(integer *natom, doublereal *charge, 
	doublereal *radius, doublereal *coorx, doublereal *coory, doublereal *
	coorz, integer *nbead, doublereal *cgchg, doublereal *cgrad, 
	doublereal *cgcox, doublereal *cgcoy, doublereal *cgcoz, doublereal *
	delgrid)
{
    /* Format strings */
    static char fmt_27[] = "(a8,f9.3,a8,3f9.3)";

    /* System generated locals */
    integer i__1, i__2, i__3, i__4;
    doublereal d__1, d__2;
    olist o__1;

    /* Builtin functions */
    double sqrt(doublereal);
    integer f_open(olist *), s_wsle(cilist *), do_lio(integer *, integer *, 
	    char *, ftnlen), e_wsle(void), s_wsfe(cilist *), do_fio(integer *,
	     char *, ftnlen), e_wsfe(void);

    /* Local variables */
    static integer na, nc, ii, jj, kk, ll;
    static doublereal wa[200110000];
    static integer mm, no;
    static doublereal qo, rr, rx, ry, rz, xx, yy, zz, dca, cob, dco, xca, yca,
	     zca, sib;
    static integer iwa[2000], lwa;
    static doublereal xco, yco, zco, upx, upy, upz, uqx, uqy, uqz;
    static integer info;
    static doublereal dpot[100000], xmin, ymin, zmin;
    static integer npts, npts1, iflag, ngrid;
    static doublereal optch[2000];
    extern /* Subroutine */ int lmdif1_(S_fp, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, integer *, integer *, 
	    doublereal *, integer *);
    static doublereal aatotc, cgtotc;
    extern /* Subroutine */ int diffpot_(integer *, integer *, doublereal *, 
	    doublereal *, integer *);
    static doublereal dpotmax, aatotpx, aatotpy, aatotpz, rminsol, zradius[
	    20000], cgtotpx, cgtotpy;
    static logical solvent;
    static doublereal cgtotpz;

    /* Fortran I/O blocks */
    static cilist io___58 = { 0, 1, 0, 0, 0 };
    static cilist io___59 = { 0, 1, 0, 0, 0 };
    static cilist io___60 = { 0, 1, 0, 0, 0 };
    static cilist io___61 = { 0, 1, 0, 0, 0 };
    static cilist io___62 = { 0, 1, 0, fmt_27, 0 };
    static cilist io___63 = { 0, 1, 0, fmt_27, 0 };


/* *********************************************************************** */
/*                                                                      * */
/*     Optimise les charges gros grain à partir d'un PDB sans H         * */
/*                                                                      * */
/*     Appelle les subroutines DIFFPOT, LMDIF1, LMDIF                   * */
/*                                                                      * */
/* ---- Last update 07/06/2012-------------------------------------------* */
/*                                                                      * */
/* *********************************************************************** */
/* ======================= DECLARATIONS ================================== */
/* -------------------- PARAMETERS --------------------------------------- */
/* ----------------------- INPUT VARIABLES ------------------------------- */
/* -------------------- LOCAL VARIABLES ---------------------------------- */
/* ==================== EXECUTABLE STATEMENTS ============================ */
/* ----------------------------------------------cas des résidus terminaux */
    /* Parameter adjustments */
    --cgcoz;
    --cgcoy;
    --cgcox;
    --cgrad;
    --cgchg;
    --coorz;
    --coory;
    --coorx;
    --radius;
    --charge;

    /* Function Body */
    charge[1] += 1.;
    for (ii = *natom; ii >= 1; --ii) {
	if (radius[ii] == 1.908) {
	    if (charge[ii] == .5973 || charge[ii] == .7341 || charge[ii] == 
		    .5366 || charge[ii] == .5896) {
		nc = ii;
		no = ii + 1;
	    }
	} else if (radius[ii] == 1.824) {
	    if (-charge[ii] == .1438 || -charge[ii] == .0732 || -charge[ii] ==
		     .2227 || -charge[ii] == .2548) {
		na = ii + 1;
		goto L2709;
	    }
	}
    }
L2709:
    qo = charge[nc] + charge[no] - 1.;
    charge[no] = qo;
    charge[nc] = -qo;
    xco = coorx[no] - coorx[nc];
    yco = coory[no] - coory[nc];
    zco = coorz[no] - coorz[nc];
    dco = sqrt(xco * xco + yco * yco + zco * zco);
    xco /= dco;
    yco /= dco;
    zco /= dco;
    xca = coorx[nc] - coorx[na];
    yca = coory[nc] - coory[na];
    zca = coorz[nc] - coorz[na];
    dca = sqrt(xca * xca + yca * yca + zca * zca);
    xca /= dca;
    yca /= dca;
    zca /= dca;
    upx = yca * zco - zca * yco;
    upy = xco * zca - zco * xca;
    upz = xca * yco - yca * xco;
    uqx = upy * zca - upz * yca;
    uqy = xca * upz - zca * upx;
    uqz = upx * yca - upy * xca;
    dca = sqrt(uqx * uqx + uqy * uqy + uqz * uqz);
    uqx /= dca;
    uqy /= dca;
    uqz /= dca;
    ++(*natom);
    charge[*natom] = charge[no];
    radius[*natom] = radius[no];
    dco = 1.25;
    cob = .454;
    sib = .891;
    coorx[no] = dco * (cob * xca + sib * uqx) + coorx[nc];
    coory[no] = dco * (cob * yca + sib * uqy) + coory[nc];
    coorz[no] = dco * (cob * zca + sib * uqz) + coorz[nc];
    coorx[*natom] = dco * (cob * xca - sib * uqx) + coorx[nc];
    coory[*natom] = dco * (cob * yca - sib * uqy) + coory[nc];
    coorz[*natom] = dco * (cob * zca - sib * uqz) + coorz[nc];
/* ------test */
/*      write(6,18) 'ATOM',na,'CA ','CTR',1, */
/*     & coorx(na),coory(na),coorz(na),charge(na),radius(na) */
/*      write(6,18) 'ATOM',nc,'C  ','CTR',1, */
/*     & coorx(nc),coory(nc),coorz(nc),charge(nc),radius(nc) */
/*      write(6,18) 'ATOM',no,'O  ','CTR',1, */
/*     & coorx(no),coory(no),coorz(no),charge(no),radius(no) */
/*      write(6,18) 'ATOM',natom,'O2 ','CTR',1, */
/*     & coorx(natom),coory(natom),coorz(natom),charge(natom), */
/*     & radius(natom) */
/* 18    format(a4,2x,i5,2x,a3,1x,a3,3x,i3,4x,3f8.3,2x,2f8.3) */
/* ------------------------------------------verification entree variables */
    chgcoo_1.znatom = *natom;
    i__1 = *natom;
    for (ii = 1; ii <= i__1; ++ii) {
	chgcoo_1.zcharge[ii - 1] = charge[ii];
	zradius[ii - 1] = radius[ii];
	chgcoo_1.zcoorx[ii - 1] = coorx[ii];
	chgcoo_1.zcoory[ii - 1] = coory[ii];
	chgcoo_1.zcoorz[ii - 1] = coorz[ii];
    }
    i__1 = *nbead;
    for (ii = 1; ii <= i__1; ++ii) {
	chgcoo_1.zcgcox[ii - 1] = cgcox[ii];
	chgcoo_1.zcgcoy[ii - 1] = cgcoy[ii];
	chgcoo_1.zcgcoz[ii - 1] = cgcoz[ii];
    }
/* -----------------------------------------centre de masse de la proteine */
    chgcoo_1.bx = 0.;
    chgcoo_1.by = 0.;
    chgcoo_1.bz = 0.;
    i__1 = *natom;
    for (ii = 1; ii <= i__1; ++ii) {
	chgcoo_1.bx += coorx[ii];
	chgcoo_1.by += coory[ii];
	chgcoo_1.bz += coorz[ii];
    }
    chgcoo_1.bx /= (real) (*natom);
    chgcoo_1.by /= (real) (*natom);
    chgcoo_1.bz /= (real) (*natom);
    xmin = chgcoo_1.bx - 75.;
    ymin = chgcoo_1.by - 75.;
    zmin = chgcoo_1.bz - 75.;
/* ------------------------------------------------interieur du solute ? */
    npts = 0;
    ngrid = (integer) (150. / *delgrid) + 1;
    rx = xmin - *delgrid;
    i__1 = ngrid;
    for (kk = 1; kk <= i__1; ++kk) {
	rx += *delgrid;
	ry = ymin - *delgrid;
	i__2 = ngrid;
	for (ll = 1; ll <= i__2; ++ll) {
	    ry += *delgrid;
	    rz = zmin - *delgrid;
	    i__3 = ngrid;
	    for (mm = 1; mm <= i__3; ++mm) {
		rz += *delgrid;
		solvent = TRUE_;
		rminsol = 150.;
		i__4 = *natom;
		for (ii = 1; ii <= i__4; ++ii) {
		    xx = rx - coorx[ii];
		    yy = ry - coory[ii];
		    zz = rz - coorz[ii];
		    rr = sqrt(xx * xx + yy * yy + zz * zz);
		    if (rr < rminsol) {
			rminsol = rr;
		    }
		    if (rr <= radius[ii]) {
			solvent = FALSE_;
			goto L2101;
		    }
		}
		i__4 = *nbead;
		for (jj = 1; jj <= i__4; ++jj) {
		    xx = rx - cgcox[jj];
		    yy = ry - cgcoy[jj];
		    zz = rz - cgcoz[jj];
		    rr = sqrt(xx * xx + yy * yy + zz * zz);
		    if (rr < rminsol) {
			rminsol = rr;
		    }
		    if (rr <= cgrad[jj]) {
			solvent = FALSE_;
			goto L2101;
		    }
		}
		if (rminsol > 10.) {
		    solvent = FALSE_;
		}
L2101:
		if (solvent) {
		    ++npts;
		    chgcoo_1.xpts[npts - 1] = rx;
		    chgcoo_1.ypts[npts - 1] = ry;
		    chgcoo_1.zpts[npts - 1] = rz;
		}
	    }
	}
    }
/* ---------------------------------------------------minimisation */
    npts1 = npts + 4;
    i__1 = *nbead;
    for (jj = 1; jj <= i__1; ++jj) {
	optch[jj - 1] = cgchg[jj];
    }
    lwa = npts1 * *nbead + *nbead * 5 + npts1;
    diffpot_(&npts1, nbead, optch, dpot, &iflag);
    lmdif1_((S_fp)diffpot_, &npts1, nbead, optch, dpot, &c_b4, &info, iwa, wa,
	     &lwa);
/* ------------------------------------difference de potentiel maximum */
    dpotmax = 0.;
    i__1 = npts1;
    for (kk = 1; kk <= i__1; ++kk) {
	if ((d__1 = dpot[kk - 1], abs(d__1)) > dpotmax) {
	    dpotmax = (d__2 = dpot[kk - 1], abs(d__2));
	}
    }
/* ------------------------------------retour des charges optimisees */
    i__1 = *nbead;
    for (jj = 1; jj <= i__1; ++jj) {
	cgchg[jj] = optch[jj - 1];
    }
/* -------------------------------------verifications et output */
    aatotc = 0.;
    aatotpx = 0.;
    aatotpy = 0.;
    aatotpz = 0.;
    i__1 = *natom;
    for (ii = 1; ii <= i__1; ++ii) {
	aatotc += charge[ii];
	aatotpx += charge[ii] * (coorx[ii] - chgcoo_1.bx);
	aatotpy += charge[ii] * (coory[ii] - chgcoo_1.by);
	aatotpz += charge[ii] * (coorz[ii] - chgcoo_1.bz);
    }
    cgtotc = 0.;
    cgtotpx = 0.;
    cgtotpy = 0.;
    cgtotpz = 0.;
    i__1 = *nbead;
    for (jj = 1; jj <= i__1; ++jj) {
	cgtotc += cgchg[jj];
	cgtotpx += cgchg[jj] * (cgcox[jj] - chgcoo_1.bx);
	cgtotpy += cgchg[jj] * (cgcoy[jj] - chgcoo_1.by);
	cgtotpz += cgchg[jj] * (cgcoz[jj] - chgcoo_1.bz);
    }
    o__1.oerr = 0;
    o__1.ounit = 1;
    o__1.ofnmlen = 16;
    o__1.ofnm = "opt_scorpion.out";
    o__1.orl = 0;
    o__1.osta = "unknown";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    f_open(&o__1);
    s_wsle(&io___58);
    do_lio(&c__9, &c__1, " WARNING : INFO = ", (ftnlen)18);
    do_lio(&c__3, &c__1, (char *)&info, (ftnlen)sizeof(integer));
    e_wsle();
    s_wsle(&io___59);
    do_lio(&c__9, &c__1, " OPTIMISATION DE ", (ftnlen)17);
    do_lio(&c__3, &c__1, (char *)&(*nbead), (ftnlen)sizeof(integer));
    do_lio(&c__9, &c__1, " CHARGES GROS GRAIN", (ftnlen)19);
    e_wsle();
    s_wsle(&io___60);
    do_lio(&c__9, &c__1, " SUR ", (ftnlen)5);
    do_lio(&c__3, &c__1, (char *)&npts, (ftnlen)sizeof(integer));
    do_lio(&c__9, &c__1, " POINTS DE GRILLE", (ftnlen)17);
    e_wsle();
    s_wsle(&io___61);
    do_lio(&c__9, &c__1, " DPOTMAX = ", (ftnlen)11);
    do_lio(&c__5, &c__1, (char *)&dpotmax, (ftnlen)sizeof(doublereal));
    e_wsle();
    s_wsfe(&io___62);
    do_fio(&c__1, "  QAA = ", (ftnlen)8);
    do_fio(&c__1, (char *)&aatotc, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, "  PAA = ", (ftnlen)8);
    do_fio(&c__1, (char *)&aatotpx, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&aatotpy, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&aatotpz, (ftnlen)sizeof(doublereal));
    e_wsfe();
    s_wsfe(&io___63);
    do_fio(&c__1, "  QCG = ", (ftnlen)8);
    do_fio(&c__1, (char *)&cgtotc, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, "  PCG = ", (ftnlen)8);
    do_fio(&c__1, (char *)&cgtotpx, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&cgtotpy, (ftnlen)sizeof(doublereal));
    do_fio(&c__1, (char *)&cgtotpz, (ftnlen)sizeof(doublereal));
    e_wsfe();
/* ================= END OF EXECUTABLE STATEMENTS ======================== */
    return 0;
} /* chrg_scorpion__ */

/* ======================================================================= */
/* ----- SUBROUTINE DIFFPOT ---------------------------------------------- */
/* ======================================================================= */
/* Subroutine */ int diffpot_(integer *npts1, integer *nbead, doublereal *
	optch, doublereal *dpot, integer *iflag)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer ii, jj, kk;
    static doublereal rr, rx, ry, rz, xx, yy, zz;
    static integer npts;
    static doublereal potaa, potcg, aatotc, cgtotc, plambda, qlambda, aatotpx,
	     aatotpy, aatotpz, cgtotpx, cgtotpy, cgtotpz;

/* ======================= DECLARATIONS ================================== */
/* ----------------------- INPUT VARIABLES ------------------------------- */
/* -------------------- LOCAL VARIABLES ---------------------------------- */
/* ==================== EXECUTABLE STATEMENTS ============================ */
/* -------------------------------------------------charge et dipole total */
    /* Parameter adjustments */
    --dpot;
    --optch;

    /* Function Body */
    qlambda = 1e5;
    plambda = 1e5;
    aatotc = 0.;
    aatotpx = 0.;
    aatotpy = 0.;
    aatotpz = 0.;
    i__1 = chgcoo_1.znatom;
    for (ii = 1; ii <= i__1; ++ii) {
	aatotc += chgcoo_1.zcharge[ii - 1];
	aatotpx += chgcoo_1.zcharge[ii - 1] * (chgcoo_1.zcoorx[ii - 1] - 
		chgcoo_1.bx);
	aatotpy += chgcoo_1.zcharge[ii - 1] * (chgcoo_1.zcoory[ii - 1] - 
		chgcoo_1.by);
	aatotpz += chgcoo_1.zcharge[ii - 1] * (chgcoo_1.zcoorz[ii - 1] - 
		chgcoo_1.bz);
    }
    cgtotc = 0.;
    cgtotpx = 0.;
    cgtotpy = 0.;
    cgtotpz = 0.;
    i__1 = *nbead;
    for (jj = 1; jj <= i__1; ++jj) {
	cgtotc += optch[jj];
	cgtotpx += optch[jj] * (chgcoo_1.zcgcox[jj - 1] - chgcoo_1.bx);
	cgtotpy += optch[jj] * (chgcoo_1.zcgcoy[jj - 1] - chgcoo_1.by);
	cgtotpz += optch[jj] * (chgcoo_1.zcgcoz[jj - 1] - chgcoo_1.bz);
    }
/* -------------------------------------------potentials on a grid */
    npts = *npts1 - 4;
    i__1 = npts;
    for (kk = 1; kk <= i__1; ++kk) {
	rx = chgcoo_1.xpts[kk - 1];
	ry = chgcoo_1.ypts[kk - 1];
	rz = chgcoo_1.zpts[kk - 1];
	potaa = 0.;
	i__2 = chgcoo_1.znatom;
	for (ii = 1; ii <= i__2; ++ii) {
	    xx = rx - chgcoo_1.zcoorx[ii - 1];
	    yy = ry - chgcoo_1.zcoory[ii - 1];
	    zz = rz - chgcoo_1.zcoorz[ii - 1];
	    rr = sqrt(xx * xx + yy * yy + zz * zz);
	    potaa += chgcoo_1.zcharge[ii - 1] / rr;
	}
	potcg = 0.;
	i__2 = *nbead;
	for (jj = 1; jj <= i__2; ++jj) {
	    xx = rx - chgcoo_1.zcgcox[jj - 1];
	    yy = ry - chgcoo_1.zcgcoy[jj - 1];
	    zz = rz - chgcoo_1.zcgcoz[jj - 1];
	    rr = sqrt(xx * xx + yy * yy + zz * zz);
	    potcg += optch[jj] / rr;
	}
	dpot[kk] = potcg - potaa;
    }
/* -----------------------conservation de la charge et du dipole total */
    dpot[*npts1 - 3] = plambda * (cgtotpx - aatotpx);
    dpot[*npts1 - 2] = plambda * (cgtotpy - aatotpy);
    dpot[*npts1 - 1] = plambda * (cgtotpz - aatotpz);
    dpot[*npts1] = qlambda * (cgtotc - aatotc);
/* ================= END OF EXECUTABLE STATEMENTS ======================== */
    return 0;
} /* diffpot_ */

/* ======================================================================= */
/* --- subroutines lmdif1 lmdif dpmapr enorm fdjac2 lmpar qrfac qrsolv --- */
/* --- nécessaires au minimiseur de la fonction de moindre carres -------- */
/* ======================================================================= */
/* Subroutine */ int lmdif1_(S_fp fcn, integer *m, integer *n, doublereal *x, 
	doublereal *fvec, doublereal *tol, integer *info, integer *iwa, 
	doublereal *wa, integer *lwa)
{
    /* Initialized data */

    static doublereal factor = 100.;
    static doublereal zero = 0.;

    static integer mp5n, mode, nfev;
    static doublereal ftol, gtol, xtol;
    extern /* Subroutine */ int lmdif_(S_fp, integer *, integer *, doublereal 
	    *, doublereal *, doublereal *, doublereal *, doublereal *, 
	    integer *, doublereal *, doublereal *, integer *, doublereal *, 
	    integer *, integer *, integer *, doublereal *, integer *, integer 
	    *, doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);
    static doublereal epsfcn;
    static integer maxfev, nprint;

/*     ********** */

/*     subroutine lmdif1 */

/*     the purpose of lmdif1 is to minimize the sum of the squares of */
/*     m nonlinear functions in n variables by a modification of the */
/*     levenberg-marquardt algorithm. this is done by using the more */
/*     general least-squares solver lmdif. the user must provide a */
/*     subroutine which calculates the functions. the jacobian is */
/*     then calculated by a forward-difference approximation. */

/*     the subroutine statement is */

/*       subroutine lmdif1(fcn,m,n,x,fvec,tol,info,iwa,wa,lwa) */

/*     where */

/*       fcn is the name of the user-supplied subroutine which */
/*         calculates the functions. fcn must be declared */
/*         in an external statement in the user calling */
/*         program, and should be written as follows. */

/*         subroutine fcn(m,n,x,fvec,iflag) */
/*         integer m,n,iflag */
/*         double precision x(n),fvec(m) */
/*         ---------- */
/*         calculate the functions at x and */
/*         return this vector in fvec. */
/*         ---------- */
/*         return */
/*         end */

/*         the value of iflag should not be changed by fcn unless */
/*         the user wants to terminate execution of lmdif1. */
/*         in this case set iflag to a negative integer. */

/*       m is a positive integer input variable set to the number */
/*         of functions. */

/*       n is a positive integer input variable set to the number */
/*         of variables. n must not exceed m. */

/*       x is an array of length n. on input x must contain */
/*         an initial estimate of the solution vector. on output x */
/*         contains the final estimate of the solution vector. */

/*       fvec is an output array of length m which contains */
/*         the functions evaluated at the output x. */

/*       tol is a nonnegative input variable. termination occurs */
/*         when the algorithm estimates either that the relative */
/*         error in the sum of squares is at most tol or that */
/*         the relative error between x and the solution is at */
/*         most tol. */

/*       info is an integer output variable. if the user has */
/*         terminated execution, info is set to the (negative) */
/*         value of iflag. see description of fcn. otherwise, */
/*         info is set as follows. */

/*         info = 0  improper input parameters. */

/*         info = 1  algorithm estimates that the relative error */
/*                   in the sum of squares is at most tol. */

/*         info = 2  algorithm estimates that the relative error */
/*                   between x and the solution is at most tol. */

/*         info = 3  conditions for info = 1 and info = 2 both hold. */

/*         info = 4  fvec is orthogonal to the columns of the */
/*                   jacobian to machine precision. */

/*         info = 5  number of calls to fcn has reached or */
/*                   exceeded 200*(n+1). */

/*         info = 6  tol is too small. no further reduction in */
/*                   the sum of squares is possible. */

/*         info = 7  tol is too small. no further improvement in */
/*                   the approximate solution x is possible. */

/*       iwa is an integer work array of length n. */

/*       wa is a work array of length lwa. */

/*       lwa is a positive integer input variable not less than */
/*         m*n+5*n+m. */

/*     subprograms called */

/*       user-supplied ...... fcn */

/*       minpack-supplied ... lmdif */

/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

/*     ********** */
    /* Parameter adjustments */
    --fvec;
    --iwa;
    --x;
    --wa;

    /* Function Body */
    *info = 0;

/*     check the input parameters for errors. */

    if (*n <= 0 || *m < *n || *tol < zero || *lwa < *m * *n + *n * 5 + *m) {
	goto L10;
    }

/*     call lmdif. */

    maxfev = (*n + 1) * 200;
    ftol = *tol;
    xtol = *tol;
    gtol = zero;
    epsfcn = zero;
    mode = 1;
    nprint = 0;
    mp5n = *m + *n * 5;
    lmdif_((S_fp)fcn, m, n, &x[1], &fvec[1], &ftol, &xtol, &gtol, &maxfev, &
	    epsfcn, &wa[1], &mode, &factor, &nprint, info, &nfev, &wa[mp5n + 
	    1], m, &iwa[1], &wa[*n + 1], &wa[(*n << 1) + 1], &wa[*n * 3 + 1], 
	    &wa[(*n << 2) + 1], &wa[*n * 5 + 1]);
    if (*info == 8) {
	*info = 4;
    }
L10:

/*     last card of subroutine lmdif1. */

    return 0;
} /* lmdif1_ */

/* ======================================================================= */
/* Subroutine */ int lmdif_(S_fp fcn, integer *m, integer *n, doublereal *x, 
	doublereal *fvec, doublereal *ftol, doublereal *xtol, doublereal *
	gtol, integer *maxfev, doublereal *epsfcn, doublereal *diag, integer *
	mode, doublereal *factor, integer *nprint, integer *info, integer *
	nfev, doublereal *fjac, integer *ldfjac, integer *ipvt, doublereal *
	qtf, doublereal *wa1, doublereal *wa2, doublereal *wa3, doublereal *
	wa4)
{
    /* Initialized data */

    static doublereal p5 = .5;
    static doublereal p25 = .25;
    static doublereal p75 = .75;
    static doublereal p0001 = 1e-4;
    static doublereal zero = 0.;
    static doublereal one = 1.;
    static doublereal p1 = .1;

    /* System generated locals */
    integer fjac_dim1, fjac_offset, i__1, i__2;
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, l;
    static doublereal par, sum;
    static integer iter;
    static doublereal temp, temp1, temp2;
    static integer iflag;
    static doublereal delta;
    extern /* Subroutine */ int qrfac_(integer *, integer *, doublereal *, 
	    integer *, logical *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *), lmpar_(integer *, doublereal *, 
	    integer *, integer *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *);
    static doublereal ratio;
    extern doublereal enorm_(integer *, doublereal *);
    static doublereal fnorm, gnorm;
    extern /* Subroutine */ int fdjac2_(S_fp, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, integer *, integer *, 
	    doublereal *, doublereal *);
    static doublereal pnorm, xnorm, fnorm1, actred, dirder, epsmch, prered;
    extern doublereal dpmpar_(integer *);

/*     ********** */

/*     subroutine lmdif */

/*     the purpose of lmdif is to minimize the sum of the squares of */
/*     m nonlinear functions in n variables by a modification of */
/*     the levenberg-marquardt algorithm. the user must provide a */
/*     subroutine which calculates the functions. the jacobian is */
/*     then calculated by a forward-difference approximation. */

/*     the subroutine statement is */

/*       subroutine lmdif(fcn,m,n,x,fvec,ftol,xtol,gtol,maxfev,epsfcn, */
/*                        diag,mode,factor,nprint,info,nfev,fjac, */
/*                        ldfjac,ipvt,qtf,wa1,wa2,wa3,wa4) */

/*     where */

/*       fcn is the name of the user-supplied subroutine which */
/*         calculates the functions. fcn must be declared */
/*         in an external statement in the user calling */
/*         program, and should be written as follows. */

/*         subroutine fcn(m,n,x,fvec,iflag) */
/*         integer m,n,iflag */
/*         double precision x(n),fvec(m) */
/*         ---------- */
/*         calculate the functions at x and */
/*         return this vector in fvec. */
/*         ---------- */
/*         return */
/*         end */

/*         the value of iflag should not be changed by fcn unless */
/*         the user wants to terminate execution of lmdif. */
/*         in this case set iflag to a negative integer. */

/*       m is a positive integer input variable set to the number */
/*         of functions. */

/*       n is a positive integer input variable set to the number */
/*         of variables. n must not exceed m. */

/*       x is an array of length n. on input x must contain */
/*         an initial estimate of the solution vector. on output x */
/*         contains the final estimate of the solution vector. */

/*       fvec is an output array of length m which contains */
/*         the functions evaluated at the output x. */

/*       ftol is a nonnegative input variable. termination */
/*         occurs when both the actual and predicted relative */
/*         reductions in the sum of squares are at most ftol. */
/*         therefore, ftol measures the relative error desired */
/*         in the sum of squares. */

/*       xtol is a nonnegative input variable. termination */
/*         occurs when the relative error between two consecutive */
/*         iterates is at most xtol. therefore, xtol measures the */
/*         relative error desired in the approximate solution. */

/*       gtol is a nonnegative input variable. termination */
/*         occurs when the cosine of the angle between fvec and */
/*         any column of the jacobian is at most gtol in absolute */
/*         value. therefore, gtol measures the orthogonality */
/*         desired between the function vector and the columns */
/*         of the jacobian. */

/*       maxfev is a positive integer input variable. termination */
/*         occurs when the number of calls to fcn is at least */
/*         maxfev by the end of an iteration. */

/*       epsfcn is an input variable used in determining a suitable */
/*         step length for the forward-difference approximation. this */
/*         approximation assumes that the relative errors in the */
/*         functions are of the order of epsfcn. if epsfcn is less */
/*         than the machine precision, it is assumed that the relative */
/*         errors in the functions are of the order of the machine */
/*         precision. */

/*       diag is an array of length n. if mode = 1 (see */
/*         below), diag is internally set. if mode = 2, diag */
/*         must contain positive entries that serve as */
/*         multiplicative scale factors for the variables. */

/*       mode is an integer input variable. if mode = 1, the */
/*         variables will be scaled internally. if mode = 2, */
/*         the scaling is specified by the input diag. other */
/*         values of mode are equivalent to mode = 1. */

/*       factor is a positive input variable used in determining the */
/*         initial step bound. this bound is set to the product of */
/*         factor and the euclidean norm of diag*x if nonzero, or else */
/*         to factor itself. in most cases factor should lie in the */
/*         interval (.1,100.). 100. is a generally recommended value. */

/*       nprint is an integer input variable that enables controlled */
/*         printing of iterates if it is positive. in this case, */
/*         fcn is called with iflag = 0 at the beginning of the first */
/*         iteration and every nprint iterations thereafter and */
/*         immediately prior to return, with x and fvec available */
/*         for printing. if nprint is not positive, no special calls */
/*         of fcn with iflag = 0 are made. */

/*       info is an integer output variable. if the user has */
/*         terminated execution, info is set to the (negative) */
/*         value of iflag. see description of fcn. otherwise, */
/*         info is set as follows. */

/*         info = 0  improper input parameters. */

/*         info = 1  both actual and predicted relative reductions */
/*                   in the sum of squares are at most ftol. */

/*         info = 2  relative error between two consecutive iterates */
/*                   is at most xtol. */

/*         info = 3  conditions for info = 1 and info = 2 both hold. */

/*         info = 4  the cosine of the angle between fvec and any */
/*                   column of the jacobian is at most gtol in */
/*                   absolute value. */

/*         info = 5  number of calls to fcn has reached or */
/*                   exceeded maxfev. */

/*         info = 6  ftol is too small. no further reduction in */
/*                   the sum of squares is possible. */

/*         info = 7  xtol is too small. no further improvement in */
/*                   the approximate solution x is possible. */

/*         info = 8  gtol is too small. fvec is orthogonal to the */
/*                   columns of the jacobian to machine precision. */

/*       nfev is an integer output variable set to the number of */
/*         calls to fcn. */

/*       fjac is an output m by n array. the upper n by n submatrix */
/*         of fjac contains an upper triangular matrix r with */
/*         diagonal elements of nonincreasing magnitude such that */

/*                t     t           t */
/*               p *(jac *jac)*p = r *r, */

/*         where p is a permutation matrix and jac is the final */
/*         calculated jacobian. column j of p is column ipvt(j) */
/*         (see below) of the identity matrix. the lower trapezoidal */
/*         part of fjac contains information generated during */
/*         the computation of r. */

/*       ldfjac is a positive integer input variable not less than m */
/*         which specifies the leading dimension of the array fjac. */

/*       ipvt is an integer output array of length n. ipvt */
/*         defines a permutation matrix p such that jac*p = q*r, */
/*         where jac is the final calculated jacobian, q is */
/*         orthogonal (not stored), and r is upper triangular */
/*         with diagonal elements of nonincreasing magnitude. */
/*         column j of p is column ipvt(j) of the identity matrix. */

/*       qtf is an output array of length n which contains */
/*         the first n elements of the vector (q transpose)*fvec. */

/*       wa1, wa2, and wa3 are work arrays of length n. */

/*       wa4 is a work array of length m. */

/*     subprograms called */

/*       user-supplied ...... fcn */

/*       minpack-supplied ... dpmpar,enorm,fdjac2,lmpar,qrfac */

/*       fortran-supplied ... dabs,dmax1,dmin1,dsqrt,mod */

/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

/*     ********** */
    /* Parameter adjustments */
    --wa4;
    --fvec;
    --wa3;
    --wa2;
    --wa1;
    --qtf;
    --ipvt;
    --diag;
    --x;
    fjac_dim1 = *ldfjac;
    fjac_offset = 1 + fjac_dim1;
    fjac -= fjac_offset;

    /* Function Body */

/*     epsmch is the machine precision. */

    epsmch = dpmpar_(&c__1);

    *info = 0;
    iflag = 0;
    *nfev = 0;

/*     check the input parameters for errors. */

    if (*n <= 0 || *m < *n || *ldfjac < *m || *ftol < zero || *xtol < zero || 
	    *gtol < zero || *maxfev <= 0 || *factor <= zero) {
	goto L300;
    }
    if (*mode != 2) {
	goto L20;
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	if (diag[j] <= zero) {
	    goto L300;
	}
/* L10: */
    }
L20:

/*     evaluate the function at the starting point */
/*     and calculate its norm. */

    iflag = 1;
    (*fcn)(m, n, &x[1], &fvec[1], &iflag);
    *nfev = 1;
    if (iflag < 0) {
	goto L300;
    }
    fnorm = enorm_(m, &fvec[1]);

/*     initialize levenberg-marquardt parameter and iteration counter. */

    par = zero;
    iter = 1;

/*     beginning of the outer loop. */

L30:

/*        calculate the jacobian matrix. */

    iflag = 2;
    fdjac2_((S_fp)fcn, m, n, &x[1], &fvec[1], &fjac[fjac_offset], ldfjac, &
	    iflag, epsfcn, &wa4[1]);
    *nfev += *n;
    if (iflag < 0) {
	goto L300;
    }

/*        if requested, call fcn to enable printing of iterates. */

    if (*nprint <= 0) {
	goto L40;
    }
    iflag = 0;
    if ((iter - 1) % *nprint == 0) {
	(*fcn)(m, n, &x[1], &fvec[1], &iflag);
    }
    if (iflag < 0) {
	goto L300;
    }
L40:

/*        compute the qr factorization of the jacobian. */

    qrfac_(m, n, &fjac[fjac_offset], ldfjac, &c_true, &ipvt[1], n, &wa1[1], &
	    wa2[1], &wa3[1]);

/*        on the first iteration and if mode is 1, scale according */
/*        to the norms of the columns of the initial jacobian. */

    if (iter != 1) {
	goto L80;
    }
    if (*mode == 2) {
	goto L60;
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	diag[j] = wa2[j];
	if (wa2[j] == zero) {
	    diag[j] = one;
	}
/* L50: */
    }
L60:

/*        on the first iteration, calculate the norm of the scaled x */
/*        and initialize the step bound delta. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa3[j] = diag[j] * x[j];
/* L70: */
    }
    xnorm = enorm_(n, &wa3[1]);
    delta = *factor * xnorm;
    if (delta == zero) {
	delta = *factor;
    }
L80:

/*        form (q transpose)*fvec and store the first n components in */
/*        qtf. */

    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	wa4[i__] = fvec[i__];
/* L90: */
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	if (fjac[j + j * fjac_dim1] == zero) {
	    goto L120;
	}
	sum = zero;
	i__2 = *m;
	for (i__ = j; i__ <= i__2; ++i__) {
	    sum += fjac[i__ + j * fjac_dim1] * wa4[i__];
/* L100: */
	}
	temp = -sum / fjac[j + j * fjac_dim1];
	i__2 = *m;
	for (i__ = j; i__ <= i__2; ++i__) {
	    wa4[i__] += fjac[i__ + j * fjac_dim1] * temp;
/* L110: */
	}
L120:
	fjac[j + j * fjac_dim1] = wa1[j];
	qtf[j] = wa4[j];
/* L130: */
    }

/*        compute the norm of the scaled gradient. */

    gnorm = zero;
    if (fnorm == zero) {
	goto L170;
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	if (wa2[l] == zero) {
	    goto L150;
	}
	sum = zero;
	i__2 = j;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    sum += fjac[i__ + j * fjac_dim1] * (qtf[i__] / fnorm);
/* L140: */
	}
/* Computing MAX */
	d__2 = gnorm, d__3 = (d__1 = sum / wa2[l], abs(d__1));
	gnorm = max(d__2,d__3);
L150:
/* L160: */
	;
    }
L170:

/*        test for convergence of the gradient norm. */

    if (gnorm <= *gtol) {
	*info = 4;
    }
    if (*info != 0) {
	goto L300;
    }

/*        rescale if necessary. */

    if (*mode == 2) {
	goto L190;
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
	d__1 = diag[j], d__2 = wa2[j];
	diag[j] = max(d__1,d__2);
/* L180: */
    }
L190:

/*        beginning of the inner loop. */

L200:

/*           determine the levenberg-marquardt parameter. */

    lmpar_(n, &fjac[fjac_offset], ldfjac, &ipvt[1], &diag[1], &qtf[1], &delta,
	     &par, &wa1[1], &wa2[1], &wa3[1], &wa4[1]);

/*           store the direction p and x + p. calculate the norm of p. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa1[j] = -wa1[j];
	wa2[j] = x[j] + wa1[j];
	wa3[j] = diag[j] * wa1[j];
/* L210: */
    }
    pnorm = enorm_(n, &wa3[1]);

/*           on the first iteration, adjust the initial step bound. */

    if (iter == 1) {
	delta = min(delta,pnorm);
    }

/*           evaluate the function at x + p and calculate its norm. */

    iflag = 1;
    (*fcn)(m, n, &wa2[1], &wa4[1], &iflag);
    ++(*nfev);
    if (iflag < 0) {
	goto L300;
    }
    fnorm1 = enorm_(m, &wa4[1]);

/*           compute the scaled actual reduction. */

    actred = -one;
    if (p1 * fnorm1 < fnorm) {
/* Computing 2nd power */
	d__1 = fnorm1 / fnorm;
	actred = one - d__1 * d__1;
    }

/*           compute the scaled predicted reduction and */
/*           the scaled directional derivative. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa3[j] = zero;
	l = ipvt[j];
	temp = wa1[l];
	i__2 = j;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    wa3[i__] += fjac[i__ + j * fjac_dim1] * temp;
/* L220: */
	}
/* L230: */
    }
    temp1 = enorm_(n, &wa3[1]) / fnorm;
    temp2 = sqrt(par) * pnorm / fnorm;
/* Computing 2nd power */
    d__1 = temp1;
/* Computing 2nd power */
    d__2 = temp2;
    prered = d__1 * d__1 + d__2 * d__2 / p5;
/* Computing 2nd power */
    d__1 = temp1;
/* Computing 2nd power */
    d__2 = temp2;
    dirder = -(d__1 * d__1 + d__2 * d__2);

/*           compute the ratio of the actual to the predicted */
/*           reduction. */

    ratio = zero;
    if (prered != zero) {
	ratio = actred / prered;
    }

/*           update the step bound. */

    if (ratio > p25) {
	goto L240;
    }
    if (actred >= zero) {
	temp = p5;
    }
    if (actred < zero) {
	temp = p5 * dirder / (dirder + p5 * actred);
    }
    if (p1 * fnorm1 >= fnorm || temp < p1) {
	temp = p1;
    }
/* Computing MIN */
    d__1 = delta, d__2 = pnorm / p1;
    delta = temp * min(d__1,d__2);
    par /= temp;
    goto L260;
L240:
    if (par != zero && ratio < p75) {
	goto L250;
    }
    delta = pnorm / p5;
    par = p5 * par;
L250:
L260:

/*           test for successful iteration. */

    if (ratio < p0001) {
	goto L290;
    }

/*           successful iteration. update x, fvec, and their norms. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	x[j] = wa2[j];
	wa2[j] = diag[j] * x[j];
/* L270: */
    }
    i__1 = *m;
    for (i__ = 1; i__ <= i__1; ++i__) {
	fvec[i__] = wa4[i__];
/* L280: */
    }
    xnorm = enorm_(n, &wa2[1]);
    fnorm = fnorm1;
    ++iter;
L290:

/*           tests for convergence. */

    if (abs(actred) <= *ftol && prered <= *ftol && p5 * ratio <= one) {
	*info = 1;
    }
    if (delta <= *xtol * xnorm) {
	*info = 2;
    }
    if (abs(actred) <= *ftol && prered <= *ftol && p5 * ratio <= one && *info 
	    == 2) {
	*info = 3;
    }
    if (*info != 0) {
	goto L300;
    }

/*           tests for termination and stringent tolerances. */

    if (*nfev >= *maxfev) {
	*info = 5;
    }
    if (abs(actred) <= epsmch && prered <= epsmch && p5 * ratio <= one) {
	*info = 6;
    }
    if (delta <= epsmch * xnorm) {
	*info = 7;
    }
    if (gnorm <= epsmch) {
	*info = 8;
    }
    if (*info != 0) {
	goto L300;
    }

/*           end of the inner loop. repeat if iteration unsuccessful. */

    if (ratio < p0001) {
	goto L200;
    }

/*        end of the outer loop. */

    goto L30;
L300:

/*     termination, either normal or user imposed. */

    if (iflag < 0) {
	*info = iflag;
    }
    iflag = 0;
    if (*nprint > 0) {
	(*fcn)(m, n, &x[1], &fvec[1], &iflag);
    }

/*     last card of subroutine lmdif. */

    return 0;
} /* lmdif_ */

/* ======================================================================= */
doublereal dpmpar_(integer *i__)
{
    /* Initialized data */

    static struct {
	doublereal e_1[3];
	doublereal fill_2[1];
	} equiv_2 = { 2.22044604926e-16, 2.22507385852e-308, 
		1.79769313485e308 };


    /* System generated locals */
    doublereal ret_val;

    /* Local variables */
#define dmach ((doublereal *)&equiv_2)
#define minmag ((integer *)&equiv_2 + 2)
#define maxmag ((integer *)&equiv_2 + 4)
#define mcheps ((integer *)&equiv_2)

/*     ********** */

/*     Function dpmpar */

/*     This function provides double precision machine parameters */
/*     when the appropriate set of data statements is activated (by */
/*     removing the c from column 1) and all other data statements are */
/*     rendered inactive. Most of the parameter values were obtained */
/*     from the corresponding Bell Laboratories Port Library function. */

/*     The function statement is */

/*       double precision function dpmpar(i) */

/*     where */

/*       i is an integer input variable set to 1, 2, or 3 which */
/*         selects the desired machine parameter. If the machine has */
/*         t base b digits and its smallest and largest exponents are */
/*         emin and emax, respectively, then these parameters are */

/*         dpmpar(1) = b**(1 - t), the machine precision, */

/*         dpmpar(2) = b**(emin - 1), the smallest magnitude, */

/*         dpmpar(3) = b**emax*(1 - b**(-t)), the largest magnitude. */

/*     Argonne National Laboratory. MINPACK Project. November 1996. */
/*     Burton S. Garbow, Kenneth E. Hillstrom, Jorge J. More' */

/*     ********** */

/*     Machine constants for the IBM 360/370 series, */
/*     the Amdahl 470/V6, the ICL 2900, the Itel AS/6, */
/*     the Xerox Sigma 5/7/9 and the Sel systems 85/86. */

/*     data mcheps(1),mcheps(2) / z34100000, z00000000 / */
/*     data minmag(1),minmag(2) / z00100000, z00000000 / */
/*     data maxmag(1),maxmag(2) / z7fffffff, zffffffff / */

/*     Machine constants for the Honeywell 600/6000 series. */

/*     data mcheps(1),mcheps(2) / o606400000000, o000000000000 / */
/*     data minmag(1),minmag(2) / o402400000000, o000000000000 / */
/*     data maxmag(1),maxmag(2) / o376777777777, o777777777777 / */

/*     Machine constants for the CDC 6000/7000 series. */

/*     data mcheps(1) / 15614000000000000000b / */
/*     data mcheps(2) / 15010000000000000000b / */

/*     data minmag(1) / 00604000000000000000b / */
/*     data minmag(2) / 00000000000000000000b / */

/*     data maxmag(1) / 37767777777777777777b / */
/*     data maxmag(2) / 37167777777777777777b / */

/*     Machine constants for the PDP-10 (KA processor). */

/*     data mcheps(1),mcheps(2) / "114400000000, "000000000000 / */
/*     data minmag(1),minmag(2) / "033400000000, "000000000000 / */
/*     data maxmag(1),maxmag(2) / "377777777777, "344777777777 / */

/*     Machine constants for the PDP-10 (KI processor). */

/*     data mcheps(1),mcheps(2) / "104400000000, "000000000000 / */
/*     data minmag(1),minmag(2) / "000400000000, "000000000000 / */
/*     data maxmag(1),maxmag(2) / "377777777777, "377777777777 / */

/*     Machine constants for the PDP-11. */

/*     data mcheps(1),mcheps(2) /   9472,      0 / */
/*     data mcheps(3),mcheps(4) /      0,      0 / */

/*     data minmag(1),minmag(2) /    128,      0 / */
/*     data minmag(3),minmag(4) /      0,      0 / */

/*     data maxmag(1),maxmag(2) /  32767,     -1 / */
/*     data maxmag(3),maxmag(4) /     -1,     -1 / */

/*     Machine constants for the Burroughs 6700/7700 systems. */

/*     data mcheps(1) / o1451000000000000 / */
/*     data mcheps(2) / o0000000000000000 / */

/*     data minmag(1) / o1771000000000000 / */
/*     data minmag(2) / o7770000000000000 / */

/*     data maxmag(1) / o0777777777777777 / */
/*     data maxmag(2) / o7777777777777777 / */

/*     Machine constants for the Burroughs 5700 system. */

/*     data mcheps(1) / o1451000000000000 / */
/*     data mcheps(2) / o0000000000000000 / */

/*     data minmag(1) / o1771000000000000 / */
/*     data minmag(2) / o0000000000000000 / */

/*     data maxmag(1) / o0777777777777777 / */
/*     data maxmag(2) / o0007777777777777 / */

/*     Machine constants for the Burroughs 1700 system. */

/*     data mcheps(1) / zcc6800000 / */
/*     data mcheps(2) / z000000000 / */

/*     data minmag(1) / zc00800000 / */
/*     data minmag(2) / z000000000 / */

/*     data maxmag(1) / zdffffffff / */
/*     data maxmag(2) / zfffffffff / */

/*     Machine constants for the Univac 1100 series. */

/*     data mcheps(1),mcheps(2) / o170640000000, o000000000000 / */
/*     data minmag(1),minmag(2) / o000040000000, o000000000000 / */
/*     data maxmag(1),maxmag(2) / o377777777777, o777777777777 / */

/*     Machine constants for the Data General Eclipse S/200. */

/*     Note - it may be appropriate to include the following card - */
/*     static dmach(3) */

/*     data minmag/20k,3*0/,maxmag/77777k,3*177777k/ */
/*     data mcheps/32020k,3*0/ */

/*     Machine constants for the Harris 220. */

/*     data mcheps(1),mcheps(2) / '20000000, '00000334 / */
/*     data minmag(1),minmag(2) / '20000000, '00000201 / */
/*     data maxmag(1),maxmag(2) / '37777777, '37777577 / */

/*     Machine constants for the Cray-1. */

/*     data mcheps(1) / 0376424000000000000000b / */
/*     data mcheps(2) / 0000000000000000000000b / */

/*     data minmag(1) / 0200034000000000000000b / */
/*     data minmag(2) / 0000000000000000000000b / */

/*     data maxmag(1) / 0577777777777777777777b / */
/*     data maxmag(2) / 0000007777777777777776b / */

/*     Machine constants for the Prime 400. */

/*     data mcheps(1),mcheps(2) / :10000000000, :00000000123 / */
/*     data minmag(1),minmag(2) / :10000000000, :00000100000 / */
/*     data maxmag(1),maxmag(2) / :17777777777, :37777677776 / */

/*     Machine constants for the VAX-11. */

/*     data mcheps(1),mcheps(2) /   9472,  0 / */
/*     data minmag(1),minmag(2) /    128,  0 / */
/*     data maxmag(1),maxmag(2) / -32769, -1 / */

/*     Machine constants for IEEE machines. */


    ret_val = dmach[(0 + (0 + (*i__ - 1 << 3))) / 8];

/*     Last card of function dpmpar. */

    return ret_val;
} /* dpmpar_ */

#undef mcheps
#undef maxmag
#undef minmag
#undef dmach


/* ======================================================================= */
doublereal enorm_(integer *n, doublereal *x)
{
    /* Initialized data */

    static doublereal one = 1.;
    static doublereal zero = 0.;
    static doublereal rdwarf = 3.834e-20;
    static doublereal rgiant = 1.304e19;

    /* System generated locals */
    integer i__1;
    doublereal ret_val, d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__;
    static doublereal s1, s2, s3, xabs, x1max, x3max, agiant, floatn;

/*     ********** */

/*     function enorm */

/*     given an n-vector x, this function calculates the */
/*     euclidean norm of x. */

/*     the euclidean norm is computed by accumulating the sum of */
/*     squares in three different sums. the sums of squares for the */
/*     small and large components are scaled so that no overflows */
/*     occur. non-destructive underflows are permitted. underflows */
/*     and overflows do not occur in the computation of the unscaled */
/*     sum of squares for the intermediate components. */
/*     the definitions of small, intermediate and large components */
/*     depend on two constants, rdwarf and rgiant. the main */
/*     restrictions on these constants are that rdwarf**2 not */
/*     underflow and rgiant**2 not overflow. the constants */
/*     given here are suitable for every known computer. */

/*     the function statement is */

/*       double precision function enorm(n,x) */

/*     where */

/*       n is a positive integer input variable. */

/*       x is an input array of length n. */

/*     subprograms called */

/*       fortran-supplied ... dabs,dsqrt */

/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

/*     ********** */
    /* Parameter adjustments */
    --x;

    /* Function Body */

    s1 = zero;
    s2 = zero;
    s3 = zero;
    x1max = zero;
    x3max = zero;
    floatn = (doublereal) (*n);
    agiant = rgiant / floatn;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	xabs = (d__1 = x[i__], abs(d__1));
	if (xabs > rdwarf && xabs < agiant) {
	    goto L70;
	}
	if (xabs <= rdwarf) {
	    goto L30;
	}

/*              sum for large components. */

	if (xabs <= x1max) {
	    goto L10;
	}
/* Computing 2nd power */
	d__1 = x1max / xabs;
	s1 = one + s1 * (d__1 * d__1);
	x1max = xabs;
	goto L20;
L10:
/* Computing 2nd power */
	d__1 = xabs / x1max;
	s1 += d__1 * d__1;
L20:
	goto L60;
L30:

/*              sum for small components. */

	if (xabs <= x3max) {
	    goto L40;
	}
/* Computing 2nd power */
	d__1 = x3max / xabs;
	s3 = one + s3 * (d__1 * d__1);
	x3max = xabs;
	goto L50;
L40:
	if (xabs != zero) {
/* Computing 2nd power */
	    d__1 = xabs / x3max;
	    s3 += d__1 * d__1;
	}
L50:
L60:
	goto L80;
L70:

/*           sum for intermediate components. */

/* Computing 2nd power */
	d__1 = xabs;
	s2 += d__1 * d__1;
L80:
/* L90: */
	;
    }

/*     calculation of norm. */

    if (s1 == zero) {
	goto L100;
    }
    ret_val = x1max * sqrt(s1 + s2 / x1max / x1max);
    goto L130;
L100:
    if (s2 == zero) {
	goto L110;
    }
    if (s2 >= x3max) {
	ret_val = sqrt(s2 * (one + x3max / s2 * (x3max * s3)));
    }
    if (s2 < x3max) {
	ret_val = sqrt(x3max * (s2 / x3max + x3max * s3));
    }
    goto L120;
L110:
    ret_val = x3max * sqrt(s3);
L120:
L130:

/*     last card of function enorm. */

    return ret_val;
} /* enorm_ */

/* ======================================================================= */
/* Subroutine */ int fdjac2_(S_fp fcn, integer *m, integer *n, doublereal *x, 
	doublereal *fvec, doublereal *fjac, integer *ldfjac, integer *iflag, 
	doublereal *epsfcn, doublereal *wa)
{
    /* Initialized data */

    static doublereal zero = 0.;

    /* System generated locals */
    integer fjac_dim1, fjac_offset, i__1, i__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal h__;
    static integer i__, j;
    static doublereal eps, temp, epsmch;
    extern doublereal dpmpar_(integer *);

/*     ********** */

/*     subroutine fdjac2 */

/*     this subroutine computes a forward-difference approximation */
/*     to the m by n jacobian matrix associated with a specified */
/*     problem of m functions in n variables. */

/*     the subroutine statement is */

/*       subroutine fdjac2(fcn,m,n,x,fvec,fjac,ldfjac,iflag,epsfcn,wa) */

/*     where */

/*       fcn is the name of the user-supplied subroutine which */
/*         calculates the functions. fcn must be declared */
/*         in an external statement in the user calling */
/*         program, and should be written as follows. */

/*         subroutine fcn(m,n,x,fvec,iflag) */
/*         integer m,n,iflag */
/*         double precision x(n),fvec(m) */
/*         ---------- */
/*         calculate the functions at x and */
/*         return this vector in fvec. */
/*         ---------- */
/*         return */
/*         end */

/*         the value of iflag should not be changed by fcn unless */
/*         the user wants to terminate execution of fdjac2. */
/*         in this case set iflag to a negative integer. */

/*       m is a positive integer input variable set to the number */
/*         of functions. */

/*       n is a positive integer input variable set to the number */
/*         of variables. n must not exceed m. */

/*       x is an input array of length n. */

/*       fvec is an input array of length m which must contain the */
/*         functions evaluated at x. */

/*       fjac is an output m by n array which contains the */
/*         approximation to the jacobian matrix evaluated at x. */

/*       ldfjac is a positive integer input variable not less than m */
/*         which specifies the leading dimension of the array fjac. */

/*       iflag is an integer variable which can be used to terminate */
/*         the execution of fdjac2. see description of fcn. */

/*       epsfcn is an input variable used in determining a suitable */
/*         step length for the forward-difference approximation. this */
/*         approximation assumes that the relative errors in the */
/*         functions are of the order of epsfcn. if epsfcn is less */
/*         than the machine precision, it is assumed that the relative */
/*         errors in the functions are of the order of the machine */
/*         precision. */

/*       wa is a work array of length m. */

/*     subprograms called */

/*       user-supplied ...... fcn */

/*       minpack-supplied ... dpmpar */

/*       fortran-supplied ... dabs,dmax1,dsqrt */

/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

/*     ********** */
    /* Parameter adjustments */
    --wa;
    --fvec;
    --x;
    fjac_dim1 = *ldfjac;
    fjac_offset = 1 + fjac_dim1;
    fjac -= fjac_offset;

    /* Function Body */

/*     epsmch is the machine precision. */

    epsmch = dpmpar_(&c__1);

    eps = sqrt((max(*epsfcn,epsmch)));
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	temp = x[j];
	h__ = eps * abs(temp);
	if (h__ == zero) {
	    h__ = eps;
	}
	x[j] = temp + h__;
	(*fcn)(m, n, &x[1], &wa[1], iflag);
	if (*iflag < 0) {
	    goto L30;
	}
	x[j] = temp;
	i__2 = *m;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    fjac[i__ + j * fjac_dim1] = (wa[i__] - fvec[i__]) / h__;
/* L10: */
	}
/* L20: */
    }
L30:

/*     last card of subroutine fdjac2. */

    return 0;
} /* fdjac2_ */

/* ======================================================================= */
/* Subroutine */ int lmpar_(integer *n, doublereal *r__, integer *ldr, 
	integer *ipvt, doublereal *diag, doublereal *qtb, doublereal *delta, 
	doublereal *par, doublereal *x, doublereal *sdiag, doublereal *wa1, 
	doublereal *wa2)
{
    /* Initialized data */

    static doublereal p1 = .1;
    static doublereal p001 = .001;
    static doublereal zero = 0.;

    /* System generated locals */
    integer r_dim1, r_offset, i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k, l;
    static doublereal fp;
    static integer jm1, jp1;
    static doublereal sum, parc, parl;
    static integer iter;
    static doublereal temp, paru, dwarf;
    static integer nsing;
    extern doublereal enorm_(integer *, doublereal *);
    static doublereal gnorm;
    extern doublereal dpmpar_(integer *);
    static doublereal dxnorm;
    extern /* Subroutine */ int qrsolv_(integer *, doublereal *, integer *, 
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     doublereal *);

/*     ********** */

/*     subroutine lmpar */

/*     given an m by n matrix a, an n by n nonsingular diagonal */
/*     matrix d, an m-vector b, and a positive number delta, */
/*     the problem is to determine a value for the parameter */
/*     par such that if x solves the system */

/*           a*x = b ,     sqrt(par)*d*x = 0 , */

/*     in the least squares sense, and dxnorm is the euclidean */
/*     norm of d*x, then either par is zero and */

/*           (dxnorm-delta) .le. 0.1*delta , */

/*     or par is positive and */

/*           abs(dxnorm-delta) .le. 0.1*delta . */

/*     this subroutine completes the solution of the problem */
/*     if it is provided with the necessary information from the */
/*     qr factorization, with column pivoting, of a. that is, if */
/*     a*p = q*r, where p is a permutation matrix, q has orthogonal */
/*     columns, and r is an upper triangular matrix with diagonal */
/*     elements of nonincreasing magnitude, then lmpar expects */
/*     the full upper triangle of r, the permutation matrix p, */
/*     and the first n components of (q transpose)*b. on output */
/*     lmpar also provides an upper triangular matrix s such that */

/*            t   t                   t */
/*           p *(a *a + par*d*d)*p = s *s . */

/*     s is employed within lmpar and may be of separate interest. */

/*     only a few iterations are generally needed for convergence */
/*     of the algorithm. if, however, the limit of 10 iterations */
/*     is reached, then the output par will contain the best */
/*     value obtained so far. */

/*     the subroutine statement is */

/*       subroutine lmpar(n,r,ldr,ipvt,diag,qtb,delta,par,x,sdiag, */
/*                        wa1,wa2) */

/*     where */

/*       n is a positive integer input variable set to the order of r. */

/*       r is an n by n array. on input the full upper triangle */
/*         must contain the full upper triangle of the matrix r. */
/*         on output the full upper triangle is unaltered, and the */
/*         strict lower triangle contains the strict upper triangle */
/*         (transposed) of the upper triangular matrix s. */

/*       ldr is a positive integer input variable not less than n */
/*         which specifies the leading dimension of the array r. */

/*       ipvt is an integer input array of length n which defines the */
/*         permutation matrix p such that a*p = q*r. column j of p */
/*         is column ipvt(j) of the identity matrix. */

/*       diag is an input array of length n which must contain the */
/*         diagonal elements of the matrix d. */

/*       qtb is an input array of length n which must contain the first */
/*         n elements of the vector (q transpose)*b. */

/*       delta is a positive input variable which specifies an upper */
/*         bound on the euclidean norm of d*x. */

/*       par is a nonnegative variable. on input par contains an */
/*         initial estimate of the levenberg-marquardt parameter. */
/*         on output par contains the final estimate. */

/*       x is an output array of length n which contains the least */
/*         squares solution of the system a*x = b, sqrt(par)*d*x = 0, */
/*         for the output par. */

/*       sdiag is an output array of length n which contains the */
/*         diagonal elements of the upper triangular matrix s. */

/*       wa1 and wa2 are work arrays of length n. */

/*     subprograms called */

/*       minpack-supplied ... dpmpar,enorm,qrsolv */

/*       fortran-supplied ... dabs,dmax1,dmin1,dsqrt */

/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

/*     ********** */
    /* Parameter adjustments */
    --wa2;
    --wa1;
    --sdiag;
    --x;
    --qtb;
    --diag;
    --ipvt;
    r_dim1 = *ldr;
    r_offset = 1 + r_dim1;
    r__ -= r_offset;

    /* Function Body */

/*     dwarf is the smallest positive magnitude. */

    dwarf = dpmpar_(&c__2);

/*     compute and store in x the gauss-newton direction. if the */
/*     jacobian is rank-deficient, obtain a least squares solution. */

    nsing = *n;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa1[j] = qtb[j];
	if (r__[j + j * r_dim1] == zero && nsing == *n) {
	    nsing = j - 1;
	}
	if (nsing < *n) {
	    wa1[j] = zero;
	}
/* L10: */
    }
    if (nsing < 1) {
	goto L50;
    }
    i__1 = nsing;
    for (k = 1; k <= i__1; ++k) {
	j = nsing - k + 1;
	wa1[j] /= r__[j + j * r_dim1];
	temp = wa1[j];
	jm1 = j - 1;
	if (jm1 < 1) {
	    goto L30;
	}
	i__2 = jm1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    wa1[i__] -= r__[i__ + j * r_dim1] * temp;
/* L20: */
	}
L30:
/* L40: */
	;
    }
L50:
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	x[l] = wa1[j];
/* L60: */
    }

/*     initialize the iteration counter. */
/*     evaluate the function at the origin, and test */
/*     for acceptance of the gauss-newton direction. */

    iter = 0;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa2[j] = diag[j] * x[j];
/* L70: */
    }
    dxnorm = enorm_(n, &wa2[1]);
    fp = dxnorm - *delta;
    if (fp <= p1 * *delta) {
	goto L220;
    }

/*     if the jacobian is not rank deficient, the newton */
/*     step provides a lower bound, parl, for the zero of */
/*     the function. otherwise set this bound to zero. */

    parl = zero;
    if (nsing < *n) {
	goto L120;
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	wa1[j] = diag[l] * (wa2[l] / dxnorm);
/* L80: */
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	sum = zero;
	jm1 = j - 1;
	if (jm1 < 1) {
	    goto L100;
	}
	i__2 = jm1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    sum += r__[i__ + j * r_dim1] * wa1[i__];
/* L90: */
	}
L100:
	wa1[j] = (wa1[j] - sum) / r__[j + j * r_dim1];
/* L110: */
    }
    temp = enorm_(n, &wa1[1]);
    parl = fp / *delta / temp / temp;
L120:

/*     calculate an upper bound, paru, for the zero of the function. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	sum = zero;
	i__2 = j;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    sum += r__[i__ + j * r_dim1] * qtb[i__];
/* L130: */
	}
	l = ipvt[j];
	wa1[j] = sum / diag[l];
/* L140: */
    }
    gnorm = enorm_(n, &wa1[1]);
    paru = gnorm / *delta;
    if (paru == zero) {
	paru = dwarf / min(*delta,p1);
    }

/*     if the input par lies outside of the interval (parl,paru), */
/*     set par to the closer endpoint. */

    *par = max(*par,parl);
    *par = min(*par,paru);
    if (*par == zero) {
	*par = gnorm / dxnorm;
    }

/*     beginning of an iteration. */

L150:
    ++iter;

/*        evaluate the function at the current value of par. */

    if (*par == zero) {
/* Computing MAX */
	d__1 = dwarf, d__2 = p001 * paru;
	*par = max(d__1,d__2);
    }
    temp = sqrt(*par);
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa1[j] = temp * diag[j];
/* L160: */
    }
    qrsolv_(n, &r__[r_offset], ldr, &ipvt[1], &wa1[1], &qtb[1], &x[1], &sdiag[
	    1], &wa2[1]);
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa2[j] = diag[j] * x[j];
/* L170: */
    }
    dxnorm = enorm_(n, &wa2[1]);
    temp = fp;
    fp = dxnorm - *delta;

/*        if the function is small enough, accept the current value */
/*        of par. also test for the exceptional cases where parl */
/*        is zero or the number of iterations has reached 10. */

    if (abs(fp) <= p1 * *delta || parl == zero && fp <= temp && temp < zero ||
	     iter == 10) {
	goto L220;
    }

/*        compute the newton correction. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	wa1[j] = diag[l] * (wa2[l] / dxnorm);
/* L180: */
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa1[j] /= sdiag[j];
	temp = wa1[j];
	jp1 = j + 1;
	if (*n < jp1) {
	    goto L200;
	}
	i__2 = *n;
	for (i__ = jp1; i__ <= i__2; ++i__) {
	    wa1[i__] -= r__[i__ + j * r_dim1] * temp;
/* L190: */
	}
L200:
/* L210: */
	;
    }
    temp = enorm_(n, &wa1[1]);
    parc = fp / *delta / temp / temp;

/*        depending on the sign of the function, update parl or paru. */

    if (fp > zero) {
	parl = max(parl,*par);
    }
    if (fp < zero) {
	paru = min(paru,*par);
    }

/*        compute an improved estimate for par. */

/* Computing MAX */
    d__1 = parl, d__2 = *par + parc;
    *par = max(d__1,d__2);

/*        end of an iteration. */

    goto L150;
L220:

/*     termination. */

    if (iter == 0) {
	*par = zero;
    }

/*     last card of subroutine lmpar. */

    return 0;
} /* lmpar_ */

/* ======================================================================= */
/* Subroutine */ int qrfac_(integer *m, integer *n, doublereal *a, integer *
	lda, logical *pivot, integer *ipvt, integer *lipvt, doublereal *rdiag,
	 doublereal *acnorm, doublereal *wa)
{
    /* Initialized data */

    static doublereal one = 1.;
    static doublereal p05 = .05;
    static doublereal zero = 0.;

    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k, jp1;
    static doublereal sum;
    static integer kmax;
    static doublereal temp;
    static integer minmn;
    extern doublereal enorm_(integer *, doublereal *);
    static doublereal epsmch;
    extern doublereal dpmpar_(integer *);
    static doublereal ajnorm;

/*     ********** */

/*     subroutine qrfac */

/*     this subroutine uses householder transformations with column */
/*     pivoting (optional) to compute a qr factorization of the */
/*     m by n matrix a. that is, qrfac determines an orthogonal */
/*     matrix q, a permutation matrix p, and an upper trapezoidal */
/*     matrix r with diagonal elements of nonincreasing magnitude, */
/*     such that a*p = q*r. the householder transformation for */
/*     column k, k = 1,2,...,min(m,n), is of the form */

/*                           t */
/*           i - (1/u(k))*u*u */

/*     where u has zeros in the first k-1 positions. the form of */
/*     this transformation and the method of pivoting first */
/*     appeared in the corresponding linpack subroutine. */

/*     the subroutine statement is */

/*       subroutine qrfac(m,n,a,lda,pivot,ipvt,lipvt,rdiag,acnorm,wa) */

/*     where */

/*       m is a positive integer input variable set to the number */
/*         of rows of a. */

/*       n is a positive integer input variable set to the number */
/*         of columns of a. */

/*       a is an m by n array. on input a contains the matrix for */
/*         which the qr factorization is to be computed. on output */
/*         the strict upper trapezoidal part of a contains the strict */
/*         upper trapezoidal part of r, and the lower trapezoidal */
/*         part of a contains a factored form of q (the non-trivial */
/*         elements of the u vectors described above). */

/*       lda is a positive integer input variable not less than m */
/*         which specifies the leading dimension of the array a. */

/*       pivot is a logical input variable. if pivot is set true, */
/*         then column pivoting is enforced. if pivot is set false, */
/*         then no column pivoting is done. */

/*       ipvt is an integer output array of length lipvt. ipvt */
/*         defines the permutation matrix p such that a*p = q*r. */
/*         column j of p is column ipvt(j) of the identity matrix. */
/*         if pivot is false, ipvt is not referenced. */

/*       lipvt is a positive integer input variable. if pivot is false, */
/*         then lipvt may be as small as 1. if pivot is true, then */
/*         lipvt must be at least n. */

/*       rdiag is an output array of length n which contains the */
/*         diagonal elements of r. */

/*       acnorm is an output array of length n which contains the */
/*         norms of the corresponding columns of the input matrix a. */
/*         if this information is not needed, then acnorm can coincide */
/*         with rdiag. */

/*       wa is a work array of length n. if pivot is false, then wa */
/*         can coincide with rdiag. */

/*     subprograms called */

/*       minpack-supplied ... dpmpar,enorm */

/*       fortran-supplied ... dmax1,dsqrt,min0 */

/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

/*     ********** */
    /* Parameter adjustments */
    --wa;
    --acnorm;
    --rdiag;
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --ipvt;

    /* Function Body */

/*     epsmch is the machine precision. */

    epsmch = dpmpar_(&c__1);

/*     compute the initial column norms and initialize several arrays. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	acnorm[j] = enorm_(m, &a[j * a_dim1 + 1]);
	rdiag[j] = acnorm[j];
	wa[j] = rdiag[j];
	if (*pivot) {
	    ipvt[j] = j;
	}
/* L10: */
    }

/*     reduce a to r with householder transformations. */

    minmn = min(*m,*n);
    i__1 = minmn;
    for (j = 1; j <= i__1; ++j) {
	if (! (*pivot)) {
	    goto L40;
	}

/*        bring the column of largest norm into the pivot position. */

	kmax = j;
	i__2 = *n;
	for (k = j; k <= i__2; ++k) {
	    if (rdiag[k] > rdiag[kmax]) {
		kmax = k;
	    }
/* L20: */
	}
	if (kmax == j) {
	    goto L40;
	}
	i__2 = *m;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    temp = a[i__ + j * a_dim1];
	    a[i__ + j * a_dim1] = a[i__ + kmax * a_dim1];
	    a[i__ + kmax * a_dim1] = temp;
/* L30: */
	}
	rdiag[kmax] = rdiag[j];
	wa[kmax] = wa[j];
	k = ipvt[j];
	ipvt[j] = ipvt[kmax];
	ipvt[kmax] = k;
L40:

/*        compute the householder transformation to reduce the */
/*        j-th column of a to a multiple of the j-th unit vector. */

	i__2 = *m - j + 1;
	ajnorm = enorm_(&i__2, &a[j + j * a_dim1]);
	if (ajnorm == zero) {
	    goto L100;
	}
	if (a[j + j * a_dim1] < zero) {
	    ajnorm = -ajnorm;
	}
	i__2 = *m;
	for (i__ = j; i__ <= i__2; ++i__) {
	    a[i__ + j * a_dim1] /= ajnorm;
/* L50: */
	}
	a[j + j * a_dim1] += one;

/*        apply the transformation to the remaining columns */
/*        and update the norms. */

	jp1 = j + 1;
	if (*n < jp1) {
	    goto L100;
	}
	i__2 = *n;
	for (k = jp1; k <= i__2; ++k) {
	    sum = zero;
	    i__3 = *m;
	    for (i__ = j; i__ <= i__3; ++i__) {
		sum += a[i__ + j * a_dim1] * a[i__ + k * a_dim1];
/* L60: */
	    }
	    temp = sum / a[j + j * a_dim1];
	    i__3 = *m;
	    for (i__ = j; i__ <= i__3; ++i__) {
		a[i__ + k * a_dim1] -= temp * a[i__ + j * a_dim1];
/* L70: */
	    }
	    if (! (*pivot) || rdiag[k] == zero) {
		goto L80;
	    }
	    temp = a[j + k * a_dim1] / rdiag[k];
/* Computing MAX */
/* Computing 2nd power */
	    d__3 = temp;
	    d__1 = zero, d__2 = one - d__3 * d__3;
	    rdiag[k] *= sqrt((max(d__1,d__2)));
/* Computing 2nd power */
	    d__1 = rdiag[k] / wa[k];
	    if (p05 * (d__1 * d__1) > epsmch) {
		goto L80;
	    }
	    i__3 = *m - j;
	    rdiag[k] = enorm_(&i__3, &a[jp1 + k * a_dim1]);
	    wa[k] = rdiag[k];
L80:
/* L90: */
	    ;
	}
L100:
	rdiag[j] = -ajnorm;
/* L110: */
    }

/*     last card of subroutine qrfac. */

    return 0;
} /* qrfac_ */

/* ======================================================================= */
/* Subroutine */ int qrsolv_(integer *n, doublereal *r__, integer *ldr, 
	integer *ipvt, doublereal *diag, doublereal *qtb, doublereal *x, 
	doublereal *sdiag, doublereal *wa)
{
    /* Initialized data */

    static doublereal p5 = .5;
    static doublereal p25 = .25;
    static doublereal zero = 0.;

    /* System generated locals */
    integer r_dim1, r_offset, i__1, i__2, i__3;
    doublereal d__1, d__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j, k, l, jp1, kp1;
    static doublereal tan__, cos__, sin__, sum, temp, cotan;
    static integer nsing;
    static doublereal qtbpj;

/*     ********** */

/*     subroutine qrsolv */

/*     given an m by n matrix a, an n by n diagonal matrix d, */
/*     and an m-vector b, the problem is to determine an x which */
/*     solves the system */

/*           a*x = b ,     d*x = 0 , */

/*     in the least squares sense. */

/*     this subroutine completes the solution of the problem */
/*     if it is provided with the necessary information from the */
/*     qr factorization, with column pivoting, of a. that is, if */
/*     a*p = q*r, where p is a permutation matrix, q has orthogonal */
/*     columns, and r is an upper triangular matrix with diagonal */
/*     elements of nonincreasing magnitude, then qrsolv expects */
/*     the full upper triangle of r, the permutation matrix p, */
/*     and the first n components of (q transpose)*b. the system */
/*     a*x = b, d*x = 0, is then equivalent to */

/*                  t       t */
/*           r*z = q *b ,  p *d*p*z = 0 , */

/*     where x = p*z. if this system does not have full rank, */
/*     then a least squares solution is obtained. on output qrsolv */
/*     also provides an upper triangular matrix s such that */

/*            t   t               t */
/*           p *(a *a + d*d)*p = s *s . */

/*     s is computed within qrsolv and may be of separate interest. */

/*     the subroutine statement is */

/*       subroutine qrsolv(n,r,ldr,ipvt,diag,qtb,x,sdiag,wa) */

/*     where */

/*       n is a positive integer input variable set to the order of r. */

/*       r is an n by n array. on input the full upper triangle */
/*         must contain the full upper triangle of the matrix r. */
/*         on output the full upper triangle is unaltered, and the */
/*         strict lower triangle contains the strict upper triangle */
/*         (transposed) of the upper triangular matrix s. */

/*       ldr is a positive integer input variable not less than n */
/*         which specifies the leading dimension of the array r. */

/*       ipvt is an integer input array of length n which defines the */
/*         permutation matrix p such that a*p = q*r. column j of p */
/*         is column ipvt(j) of the identity matrix. */

/*       diag is an input array of length n which must contain the */
/*         diagonal elements of the matrix d. */

/*       qtb is an input array of length n which must contain the first */
/*         n elements of the vector (q transpose)*b. */

/*       x is an output array of length n which contains the least */
/*         squares solution of the system a*x = b, d*x = 0. */

/*       sdiag is an output array of length n which contains the */
/*         diagonal elements of the upper triangular matrix s. */

/*       wa is a work array of length n. */

/*     subprograms called */

/*       fortran-supplied ... dabs,dsqrt */

/*     argonne national laboratory. minpack project. march 1980. */
/*     burton s. garbow, kenneth e. hillstrom, jorge j. more */

/*     ********** */
    /* Parameter adjustments */
    --wa;
    --sdiag;
    --x;
    --qtb;
    --diag;
    --ipvt;
    r_dim1 = *ldr;
    r_offset = 1 + r_dim1;
    r__ -= r_offset;

    /* Function Body */

/*     copy r and (q transpose)*b to preserve input and initialize s. */
/*     in particular, save the diagonal elements of r in x. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	i__2 = *n;
	for (i__ = j; i__ <= i__2; ++i__) {
	    r__[i__ + j * r_dim1] = r__[j + i__ * r_dim1];
/* L10: */
	}
	x[j] = r__[j + j * r_dim1];
	wa[j] = qtb[j];
/* L20: */
    }

/*     eliminate the diagonal matrix d using a givens rotation. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {

/*        prepare the row of d to be eliminated, locating the */
/*        diagonal element using p from the qr factorization. */

	l = ipvt[j];
	if (diag[l] == zero) {
	    goto L90;
	}
	i__2 = *n;
	for (k = j; k <= i__2; ++k) {
	    sdiag[k] = zero;
/* L30: */
	}
	sdiag[j] = diag[l];

/*        the transformations to eliminate the row of d */
/*        modify only a single element of (q transpose)*b */
/*        beyond the first n, which is initially zero. */

	qtbpj = zero;
	i__2 = *n;
	for (k = j; k <= i__2; ++k) {

/*           determine a givens rotation which eliminates the */
/*           appropriate element in the current row of d. */

	    if (sdiag[k] == zero) {
		goto L70;
	    }
	    if ((d__1 = r__[k + k * r_dim1], abs(d__1)) >= (d__2 = sdiag[k], 
		    abs(d__2))) {
		goto L40;
	    }
	    cotan = r__[k + k * r_dim1] / sdiag[k];
/* Computing 2nd power */
	    d__1 = cotan;
	    sin__ = p5 / sqrt(p25 + p25 * (d__1 * d__1));
	    cos__ = sin__ * cotan;
	    goto L50;
L40:
	    tan__ = sdiag[k] / r__[k + k * r_dim1];
/* Computing 2nd power */
	    d__1 = tan__;
	    cos__ = p5 / sqrt(p25 + p25 * (d__1 * d__1));
	    sin__ = cos__ * tan__;
L50:

/*           compute the modified diagonal element of r and */
/*           the modified element of ((q transpose)*b,0). */

	    r__[k + k * r_dim1] = cos__ * r__[k + k * r_dim1] + sin__ * sdiag[
		    k];
	    temp = cos__ * wa[k] + sin__ * qtbpj;
	    qtbpj = -sin__ * wa[k] + cos__ * qtbpj;
	    wa[k] = temp;

/*           accumulate the tranformation in the row of s. */

	    kp1 = k + 1;
	    if (*n < kp1) {
		goto L70;
	    }
	    i__3 = *n;
	    for (i__ = kp1; i__ <= i__3; ++i__) {
		temp = cos__ * r__[i__ + k * r_dim1] + sin__ * sdiag[i__];
		sdiag[i__] = -sin__ * r__[i__ + k * r_dim1] + cos__ * sdiag[
			i__];
		r__[i__ + k * r_dim1] = temp;
/* L60: */
	    }
L70:
/* L80: */
	    ;
	}
L90:

/*        store the diagonal element of s and restore */
/*        the corresponding diagonal element of r. */

	sdiag[j] = r__[j + j * r_dim1];
	r__[j + j * r_dim1] = x[j];
/* L100: */
    }

/*     solve the triangular system for z. if the system is */
/*     singular, then obtain a least squares solution. */

    nsing = *n;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	if (sdiag[j] == zero && nsing == *n) {
	    nsing = j - 1;
	}
	if (nsing < *n) {
	    wa[j] = zero;
	}
/* L110: */
    }
    if (nsing < 1) {
	goto L150;
    }
    i__1 = nsing;
    for (k = 1; k <= i__1; ++k) {
	j = nsing - k + 1;
	sum = zero;
	jp1 = j + 1;
	if (nsing < jp1) {
	    goto L130;
	}
	i__2 = nsing;
	for (i__ = jp1; i__ <= i__2; ++i__) {
	    sum += r__[i__ + j * r_dim1] * wa[i__];
/* L120: */
	}
L130:
	wa[j] = (wa[j] - sum) / sdiag[j];
/* L140: */
    }
L150:

/*     permute the components of z back to components of x. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	l = ipvt[j];
	x[l] = wa[j];
/* L160: */
    }

/*     last card of subroutine qrsolv. */

    return 0;
} /* qrsolv_ */
