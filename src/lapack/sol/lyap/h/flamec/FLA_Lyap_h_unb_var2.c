/*

    Copyright (C) 2014, The University of Texas at Austin

    This file is part of libflame and is available under the 3-Clause
    BSD license, which can be found in the LICENSE file at the top-level
    directory, or at http://opensource.org/licenses/BSD-3-Clause

*/

#include "FLAME.h"

FLA_Error FLA_Lyap_h_unb_var2( FLA_Obj isgn, FLA_Obj A, FLA_Obj C )
{
  FLA_Obj ATL,   ATR,      A00,  a01,     A02, 
          ABL,   ABR,      a10t, alpha11, a12t,
                           A20,  a21,     A22;

  FLA_Obj CTL,   CTR,      C00,  c01,     C02, 
          CBL,   CBR,      c10t, gamma11, c12t,
                           C20,  c21,     C22;

  FLA_Obj WTL,   WTR,      W00,  w01,     W02, 
          WBL,   WBR,      w10t, omega11, w12t,
                           W20,  w21,     W22;

  FLA_Obj W, omega;

  FLA_Scal( isgn, C );

  FLA_Obj_create_conf_to( FLA_NO_TRANSPOSE, A, &W );
  FLA_Obj_create( FLA_Obj_datatype( A ), 1, 1, 0, 0, &omega );

  FLA_Part_2x2( A,    &ATL, &ATR,
                      &ABL, &ABR,     0, 0, FLA_TL );

  FLA_Part_2x2( C,    &CTL, &CTR,
                      &CBL, &CBR,     0, 0, FLA_TL );

  FLA_Part_2x2( W,    &WTL, &WTR,
                      &WBL, &WBR,     0, 0, FLA_TL );

  while ( FLA_Obj_length( CBR ) > 0 ){

    FLA_Repart_2x2_to_3x3( ATL, /**/ ATR,       &A00,  /**/ &a01,     &A02,
                        /* ************* */   /* ************************** */
                                                &a10t, /**/ &alpha11, &a12t,
                           ABL, /**/ ABR,       &A20,  /**/ &a21,     &A22,
                           1, 1, FLA_BR );

    FLA_Repart_2x2_to_3x3( CTL, /**/ CTR,       &C00,  /**/ &c01,     &C02,
                        /* ************* */   /* ************************** */
                                                &c10t, /**/ &gamma11, &c12t,
                           CBL, /**/ CBR,       &C20,  /**/ &c21,     &C22,
                           1, 1, FLA_BR );

    FLA_Repart_2x2_to_3x3( WTL, /**/ WTR,       &W00,  /**/ &w01,     &W02,
                        /* ************* */   /* ************************** */
                                                &w10t, /**/ &omega11, &w12t,
                           WBL, /**/ WBR,       &W20,  /**/ &w21,     &W22,
                           1, 1, FLA_BR );

    /*------------------------------------------------------------*/

    // c01 = inv( triu(A00) + conj(alpha11) * I )' * c01;
    FLA_Copyrt( FLA_UPPER_TRIANGULAR, FLA_NO_TRANSPOSE, A00, W00 );
    FLA_Shift_diag( FLA_CONJUGATE, alpha11, W00 );
    FLA_Trsv( FLA_UPPER_TRIANGULAR, FLA_CONJ_TRANSPOSE, FLA_NONUNIT_DIAG, W00, c01 );

    // gamma11 = gamma11 - a01' * c01 - c01' * a01;
    FLA_Dot2cs( FLA_CONJUGATE, FLA_MINUS_ONE, a01, c01, FLA_ONE, gamma11 );

    // gamma11 = gamma11 / ( alpha11 + alpha11' );
    FLA_Copyt( FLA_CONJ_NO_TRANSPOSE, alpha11, omega );
    FLA_Mult_add( FLA_ONE, alpha11, omega );
    FLA_Inv_scal( omega, gamma11 );

    // C02 = C02 - c01 * a12t;
    FLA_Ger( FLA_MINUS_ONE, c01, a12t, C02 );

    // c12t = c12t - gamma11 * a12t;
    // c12t = c12t - c01' * A02;
    //      = c12t - A02^T conj(c01);
    FLA_Axpys( FLA_MINUS_ONE, gamma11, a12t, FLA_ONE, c12t );
    FLA_Gemvc( FLA_TRANSPOSE, FLA_CONJUGATE, FLA_MINUS_ONE, A02, c01, FLA_ONE, c12t );

    /*------------------------------------------------------------*/

    FLA_Cont_with_3x3_to_2x2( &ATL, /**/ &ATR,       A00,  a01,     /**/ A02,
                                                     a10t, alpha11, /**/ a12t,
                            /* ************** */  /* ************************ */
                              &ABL, /**/ &ABR,       A20,  a21,     /**/ A22,
                              FLA_TL );

    FLA_Cont_with_3x3_to_2x2( &CTL, /**/ &CTR,       C00,  c01,     /**/ C02,
                                                     c10t, gamma11, /**/ c12t,
                            /* ************** */  /* ************************ */
                              &CBL, /**/ &CBR,       C20,  c21,     /**/ C22,
                              FLA_TL );

    FLA_Cont_with_3x3_to_2x2( &WTL, /**/ &WTR,       W00,  w01,     /**/ W02,
                                                     w10t, omega11, /**/ w12t,
                            /* ************** */  /* ************************ */
                              &WBL, /**/ &WBR,       W20,  w21,     /**/ W22,
                              FLA_TL );
  }

  FLA_Obj_free( &W );
  FLA_Obj_free( &omega );

  return FLA_SUCCESS;
}

