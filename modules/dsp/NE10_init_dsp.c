/*
 *  Copyright 2012-14 ARM Limited
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of ARM Limited nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>

#include "NE10_dsp.h"

ne10_result_t ne10_init_dsp (ne10_int32_t is_NEON_available)
{
#if !defined(INTEL_MAC_PLATFORM)
    if (NE10_OK == is_NEON_available)
    {
        ne10_fft_c2c_1d_float32 = ne10_fft_c2c_1d_float32_neon;
        ne10_fft_r2c_1d_float32 = ne10_fft_r2c_1d_float32_neon;
        ne10_fft_c2r_1d_float32 = ne10_fft_c2r_1d_float32_neon;

        //ne10_fft_c2c_1d_int32 = ne10_fft_c2c_1d_int32_neon;
        //ne10_fft_r2c_1d_int32 = ne10_fft_r2c_1d_int32_neon;
        //ne10_fft_c2r_1d_int32 = ne10_fft_c2r_1d_int32_neon;

        //ne10_fft_c2c_1d_int16 = ne10_fft_c2c_1d_int16_neon;
        //ne10_fft_c2r_1d_int16 = ne10_fft_c2r_1d_int16_neon;
        //ne10_fft_r2c_1d_int16 = ne10_fft_r2c_1d_int16_neon;

        ne10_fir_float = ne10_fir_float_neon;
        ne10_fir_decimate_float = ne10_fir_decimate_float_neon;
        ne10_fir_interpolate_float = ne10_fir_interpolate_float_neon;
        ne10_fir_lattice_float = ne10_fir_lattice_float_neon;
        ne10_fir_sparse_float = ne10_fir_sparse_float_neon;

        ne10_iir_lattice_float = ne10_iir_lattice_float_neon;
    }
    else
#endif
    {
        ne10_fft_c2c_1d_float32 = ne10_fft_c2c_1d_float32_c;
        ne10_fft_r2c_1d_float32 = ne10_fft_r2c_1d_float32_c;
        ne10_fft_c2r_1d_float32 = ne10_fft_c2r_1d_float32_c;

        //ne10_fft_c2c_1d_int32 = ne10_fft_c2c_1d_int32_c;
        //ne10_fft_r2c_1d_int32 = ne10_fft_r2c_1d_int32_c;
        //ne10_fft_c2r_1d_int32 = ne10_fft_c2r_1d_int32_c;

        //ne10_fft_c2c_1d_int16 = ne10_fft_c2c_1d_int16_c;
        //ne10_fft_r2c_1d_int16 = ne10_fft_r2c_1d_int16_c;
        //ne10_fft_c2r_1d_int16 = ne10_fft_c2r_1d_int16_c;

        ne10_fir_float = ne10_fir_float_c;
        ne10_fir_decimate_float = ne10_fir_decimate_float_c;
        ne10_fir_interpolate_float = ne10_fir_interpolate_float_c;
        ne10_fir_lattice_float = ne10_fir_lattice_float_c;
        ne10_fir_sparse_float = ne10_fir_sparse_float_c;

        ne10_iir_lattice_float = ne10_iir_lattice_float_c;
    }
    return NE10_OK;
}

// These are actual definitions of our function pointers that are declared in inc/NE10_dsp.h
void (*ne10_fft_c2c_1d_float32) (ne10_fft_cpx_float32_t *fout,
                                 ne10_fft_cpx_float32_t *fin,
                                 ne10_fft_cfg_float32_t cfg,
                                 ne10_int32_t inverse_fft);

void (*ne10_fft_r2c_1d_float32) (ne10_fft_cpx_float32_t *fout,
                                 ne10_float32_t *fin,
                                 ne10_fft_r2c_cfg_float32_t cfg);

void (*ne10_fft_c2r_1d_float32) (ne10_float32_t *fout,
                                 ne10_fft_cpx_float32_t *fin,
                                 ne10_fft_r2c_cfg_float32_t cfg) ;

void (*ne10_fft_c2c_1d_int32) (ne10_fft_cpx_int32_t *fout,
                               ne10_fft_cpx_int32_t *fin,
                               ne10_fft_cfg_int32_t cfg,
                               ne10_int32_t inverse_fft,
                               ne10_int32_t scaled_flag);


void (*ne10_fft_r2c_1d_int32) (ne10_fft_cpx_int32_t *fout,
                               ne10_int32_t *fin,
                               ne10_fft_r2c_cfg_int32_t cfg,
                               ne10_int32_t scaled_flag);

void (*ne10_fft_c2r_1d_int32) (ne10_int32_t *fout,
                               ne10_fft_cpx_int32_t *fin,
                               ne10_fft_r2c_cfg_int32_t cfg,
                               ne10_int32_t scaled_flag);

void (*ne10_fft_c2c_1d_int16) (ne10_fft_cpx_int16_t *fout,
                               ne10_fft_cpx_int16_t *fin,
                               ne10_fft_cfg_int16_t cfg,
                               ne10_int32_t inverse_fft,
                               ne10_int32_t scaled_flag);

void (*ne10_fft_r2c_1d_int16) (ne10_fft_cpx_int16_t *fout,
                               ne10_int16_t *fin,
                               ne10_fft_r2c_cfg_int16_t cfg,
                               ne10_int32_t scaled_flag);

void (*ne10_fft_c2r_1d_int16) (ne10_int16_t *fout,
                               ne10_fft_cpx_int16_t *fin,
                               ne10_fft_r2c_cfg_int16_t cfg,
                               ne10_int32_t scaled_flag);

void (*ne10_fir_float) (const ne10_fir_instance_f32_t * S,
                        ne10_float32_t * pSrc,
                        ne10_float32_t * pDst,
                        ne10_uint32_t blockSize);

void (*ne10_fir_decimate_float) (
    const ne10_fir_decimate_instance_f32_t * S,
    ne10_float32_t * pSrc,
    ne10_float32_t * pDst,
    ne10_uint32_t blockSize);

void (*ne10_fir_interpolate_float) (
    const ne10_fir_interpolate_instance_f32_t * S,
    ne10_float32_t * pSrc,
    ne10_float32_t * pDst,
    ne10_uint32_t blockSize);

void (*ne10_fir_lattice_float) (
    const ne10_fir_lattice_instance_f32_t * S,
    ne10_float32_t * pSrc,
    ne10_float32_t * pDst,
    ne10_uint32_t blockSize);

void (*ne10_fir_sparse_float) (
    ne10_fir_sparse_instance_f32_t * S,
    ne10_float32_t * pSrc,
    ne10_float32_t * pDst,
    ne10_float32_t * pScratchIn,
    ne10_uint32_t blockSize);

void (*ne10_iir_lattice_float) (const ne10_iir_lattice_instance_f32_t * S,
                                ne10_float32_t * pSrc,
                                ne10_float32_t * pDst,
                                ne10_uint32_t blockSize);
