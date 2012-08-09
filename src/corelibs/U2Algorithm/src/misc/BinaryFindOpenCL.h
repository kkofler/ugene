/**
 * UGENE - Integrated Bioinformatics Tools.
 * Copyright (C) 2008-2012 UniPro <ugene@unipro.ru>
 * http://ugene.unipro.ru
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef _U2_BINARYFINDOPENCL_H_
#define _U2_BINARYFINDOPENCL_H_

#ifdef OPENCL_SUPPORT

#include <U2Core/AppContext.h>

#include <U2Algorithm/OpenCLGpuRegistry.h>
#include <U2Algorithm/OpenCLHelper.h>

namespace U2 {

typedef cl_long NumberType;

class U2ALGORITHM_EXPORT BinaryFindOpenCL {
public:
    BinaryFindOpenCL(const NumberType* _haystack, const int _haystackSize, const NumberType* _needles, const int _needlesSize, const int *_windowSizes);
    ~BinaryFindOpenCL();
    NumberType* launch();
	bool hasError() {return isError;}
private:
    bool hasOPENCLError(cl_int err, QString errorMessage);
    void initMiniHaystack(const NumberType* haystack, int lowerBound, int upperBound,
                             NumberType* miniHaystackOffsets, NumberType* miniHaystack, const int miniHaystackSize);
	void logProfilingInfo(const OpenCLHelper &openCLHelper, const cl_event &event, const QString &msgPrefix);

	bool isError;

    const NumberType* haystack;
    const int haystackSize;
    const NumberType* needles;
    const size_t needlesSize;
    const int *windowSizes;

    cl_event clEvent1;
    cl_event clEvent2;
    cl_kernel clKernel;
    cl_program clProgram;
    cl_command_queue clCommandQueue;
    cl_context clContext;

    cl_mem buf_sortedHaystackArray;
    cl_mem buf_needlesArray;
	cl_mem buf_windowSizesArray;
    cl_mem buf_outPutArray;
    cl_mem buf_miniHaystackOffsets;
    cl_mem buf_miniHaystack;
};

}//namespace

#endif /*OPENCL_SUPPORT*/

#endif // _U2_BINARYFINDOPENCL_H_

