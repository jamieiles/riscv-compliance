/*
 * Copyright (c) 2019 Jamie Iles
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef _COMPLIANCE_IO_H
#define _COMPLIANCE_IO_H

#define STRINGIFY(x) #x
#define TOSTRING(x)  STRINGIFY(x)

#define RVTEST_CUSTOM1 0x0005200B

#define RVTEST_IO_INIT
/*
 * Use a custom instruction to signal to the simulator that we have an IO
 * request and put mapping from PC to string in a well defined section name.
 * The simulator can then find the string in memory and print it to stderr.
 */
#define RVTEST_IO_WRITE_STR(_SP, _STR)					\
800000:									\
	.word RVTEST_CUSTOM1;						\
.pushsection ".rodata.str1.1", "a";					\
800001:									\
	.asciz _STR;							\
.popsection;								\
.pushsection ".rvtest_io_write";					\
	.word 800000b;							\
	.word 800001b;							\
.popsection

#define RVTEST_IO_CHECK()

/*
 * Similar to RVTEST_IO_WRITE_STR, but build a table mapping instructions to
 * register names and expected values.  This means the simulator can perform
 * the assertion itself rather than relying on the model to correctly perform
 * the test.
 */
#define RVTEST_IO_ASSERT_GPR_EQ(_SP, _R, _I)				\
900000:									\
	.word RVTEST_CUSTOM1;						\
.pushsection ".rodata.str1.1", "a";					\
900001:									\
	.asciz TOSTRING(_R);						\
900002:									\
	.ascii __FILE__, ":", TOSTRING(__LINE__), "\0";			\
.popsection;								\
.pushsection ".rvtest_gpr_assert";					\
	.word 900000b;							\
	.word 900001b;							\
	.word _I;							\
	.word 900002b;							\
.popsection
#define RVTEST_IO_ASSERT_SFPR_EQ(_F, _R, _I)
#define RVTEST_IO_ASSERT_DFPR_EQ(_D, _R, _I)

#endif // _COMPLIANCE_IO_H
