
#include "FLAME.h"

FLA_Error FLA_Axpyt_n( FLA_Obj alpha, FLA_Obj A, FLA_Obj B, fla_axpyt_t* cntl )
{
	FLA_Error r_val = FLA_SUCCESS;

	if      ( FLA_Cntl_variant( cntl ) == FLA_SUBPROBLEM )
	{
		r_val = FLA_Axpyt_n_task( alpha, A, B, cntl );
	}
	else if ( FLA_Cntl_variant( cntl ) == FLA_BLOCKED_VARIANT1 )
	{
		r_val = FLA_Axpyt_n_blk_var1( alpha, A, B, cntl );
	}
#ifdef FLA_ENABLE_NON_CRITICAL_CODE
	else if ( FLA_Cntl_variant( cntl ) == FLA_BLOCKED_VARIANT2 )
	{
		r_val = FLA_Axpyt_n_blk_var2( alpha, A, B, cntl );
	}
#endif
	else if ( FLA_Cntl_variant( cntl ) == FLA_BLOCKED_VARIANT3 )
	{
		r_val = FLA_Axpyt_n_blk_var3( alpha, A, B, cntl );
	}
#ifdef FLA_ENABLE_NON_CRITICAL_CODE
	else if ( FLA_Cntl_variant( cntl ) == FLA_BLOCKED_VARIANT4 )
	{
		r_val = FLA_Axpyt_n_blk_var4( alpha, A, B, cntl );
	}
#endif
	else
	{
		r_val = FLA_Check_error_code( FLA_NOT_YET_IMPLEMENTED );
	}

	return r_val;
}
