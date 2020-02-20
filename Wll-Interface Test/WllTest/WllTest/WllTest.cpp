// WllTest.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "wll_interface.h"

//assemble image (slow! possibly due to inefficient copy)
auto createimg(const wll::tensor<double, 3>& imgraw, const wll::tensor<int,2>& depth, int nx, int ny)
{
	auto dims = imgraw.dimensions();
	auto dimsd = depth.dimensions();

	wll::tensor<double, 3> img({ dims[0] * ny, dims[1] * nx, dims[2] }, wll::memory_type::manual);

	auto dimsf = img.dimensions();

	auto offsetx = (dimsf[1] - dimsd[1]) / 2;
	auto offsety = (dimsf[0] - dimsd[0]) / 2;
	
	//initialization
	double kt = 0.;
	for (uint64_t i = 0; i < nx; i++)
		for (uint64_t x = 0; x < dims[1] && dims[1] * i + x < offsetx + dimsd[1] + dims[1] / 2; x++)
			for (uint64_t j = 0; j < ny; j++)
				for (uint64_t y = 0; y < dims[0]; y++)
					for (uint64_t k = 0; k < dims[2]; k++)
						img(dims[0] * j + y, dims[1] * i + x, k) = imgraw(y, x, k);

	//shifting part
	for (uint64_t x = 0; x < dimsd[1]; x++)
		for (uint64_t y = 0; y < dimsd[0]; y++)
			for (uint64_t k = 0; k < dims[2]; k++)
				img(offsety + y, offsetx + x + dims[1] / 2 - depth(y, x), k) = img(offsety + y, offsetx + x - dims[1] / 2 + depth(y, x), k);

	//deal with the rest
	for (uint64_t x = offsetx + dimsd[1] + dims[1] / 2; x < dimsf[1]; x++)
		for (uint64_t y = 0; y < dimsf[0]; y++)
			for (uint64_t k = 0; k < dims[2]; k++)
				img(y, x, k) = img(y, x - dims[1], k);

	return img;
}

//shift pixel
void sft(wll::tensor<double,3>& img, const wll::tensor<int,2>& shift, int maxshift)
{
	const uint64_t xmin = (img.dimension(1) - shift.dimension(1)) / 2;
	const uint64_t ymin = (img.dimension(0) - shift.dimension(0)) / 2;

	for (uint64_t y = ymin; y < ymin + shift.dimension(0); y++)
		for (uint64_t k = 0; k < img.dimension(2); k++)
		{
			for (uint64_t x = xmin; x < xmin + shift.dimension(1); x++)
				img(y, x + shift(y - ymin, x - xmin), k) = img(y, x - shift(y - ymin, x - xmin), k);

			for (uint64_t x = xmin + shift.dimension(1); x < img.dimension(1) - maxshift; x++)
				img(y, x + maxshift, k) = img(y, x - maxshift, k);
		}

	return;
}

DEFINE_WLL_FUNCTION(sft)
DEFINE_WLL_FUNCTION(createimg)