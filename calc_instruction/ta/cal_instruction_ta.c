/*
 * Copyright (c) 2017, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <cal_instruction_ta.h>
enum
{
	OP_BR = 0, /* branch */
	OP_ADD,	   /* add  */
	OP_LD,	   /* load */
	OP_ST,	   /* store */
	OP_JSR,	   /* jump register */
	OP_AND,	   /* bitwise and */
	OP_LDR,	   /* load register */
	OP_STR,	   /* store register */
	OP_RTI,	   /* unused */
	OP_NOT,	   /* bitwise not */
	OP_LDI,	   /* load indirect */
	OP_STI,	   /* store indirect */
	OP_JMP,	   /* jump */
	OP_RES,	   /* reserved (unused) */
	OP_LEA,	   /* load effective address */
	OP_TRAP	   /* execute trap */
};

TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}
uint32_t parse_instruction(uint32_t instr)
{
	return instr >> 12;
}
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
									TEE_Param __maybe_unused params[4],
									void __maybe_unused **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
											   TEE_PARAM_TYPE_NONE,
											   TEE_PARAM_TYPE_NONE,
											   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	(void)&params;
	(void)&sess_ctx;

	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	(void)&sess_ctx;
}


static TEE_Result cal_instruction(uint32_t param_types,
								  TEE_Param params[4])
{
	uint32_t exp_param_types =
		TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE,
						TEE_PARAM_TYPE_NONE);

	DMSG("has been called");
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	uint32_t instr = params[0].value.a;
	uint16_t r0 = (instr >> 9) & 0x7;
	uint16_t r1 = (instr >> 6) & 0x7;
	uint16_t imm_flag = (instr >> 5) & 0x1;
	switch (parse_instruction(instr))
	{

	case OP_ADD:
	{
		/*
		if (imm_flag)
		{
		}
		else
		{
			
			
		}
		*/
		uint16_t r2 = instr & 0x7;
		r0 = r1 + r2;
		IMSG("OP_ADD r0 = %d\n", r0);
		break;
	}
	case OP_AND:
	{
		/*
		if (imm_flag)
		{
		}
		else
		{
			uint16_t r2 = instr & 0x7;
			r0 = r1 & r2;
		}
		*/
		uint16_t r2 = instr & 0x7;
		r0 = r1 & r2;
		IMSG("OP_AND r0 = %d\n", r0);
		break;
	}
	default:
		break;
	}
	params[0].value.a = r0;
	return TEE_SUCCESS;
}
TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
									  uint32_t cmd_id,
									  uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx;

	switch (cmd_id)
	{
	case TA_TEST:
		//return random_number_generate(param_types, params);
		return cal_instruction(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
