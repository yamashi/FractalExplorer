
/*
 *  MandelbrotRendererCL.cpp
 *	Mandelbrot Fractal Explorer Project - Copyright (c) 2012 Lucas Soltic & Maxime Griot
 *
 *  This software is provided 'as-is', without any express or
 *  implied warranty. In no event will the authors be held
 *  liable for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute
 *  it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented;
 *  you must not claim that you wrote the original software.
 *  If you use this software in a product, an acknowledgment
 *  in the product documentation would be appreciated but
 *  is not required.
 *
 *  2. Altered source versions must be plainly marked as such,
 *  and must not be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any
 *  source distribution.
 *
 */

#include "Common.hpp"
#ifdef CL_BUILD

#include "MandelbrotRendererCL.hpp"
#include <iostream>
#include <SFML/System.hpp>
#include "FPReal.hpp"

GPU_ADD_STATIC_CODE(
	"#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n"
	,extension_atomic_inc_pragma);

GPU_ONLY(
	// Increment U
	uint4 inc128(uint4 u)
{
	// Compute all carries to add
	int4 h = (int4)(u == (uint4)(0xFFFFFFFF)); // Note that == sets ALL bits if true [6.3.d]
	uint4 c = (uint4)(h.y&h.z&h.w&1,h.z&h.w&1,h.w&1,1);
	return u+c;
}

// Return -U
uint4 neg128(uint4 u)
{
	return inc128(u ^ (uint4)(0xFFFFFFFF)); // 1 + not U
}

// Return representation of signed integer K
uint4 set128(int k)
{
	uint4 u = (uint4)((uint)abs(k),0,0,0);
	return (k<0)?neg128(u):u;
}

// Return U+V
uint4 add128(uint4 u,uint4 v)
{
	uint4 s = u+v;
	uint4 h = convert_uint4((int4)(s < u));
	uint4 c1 = h.yzwx & (uint4)(1,1,1,0); // Carry from U+V
	h = convert_uint4((int4)(s == (uint4)(0xFFFFFFFF)));
	uint4 c2 = (uint4)((c1.y|(c1.z&h.z))&h.y,c1.z&h.z,0,0); // Propagated carry
	return s+c1+c2;
}

// Return U<<1
uint4 shl128(uint4 u)
{
	uint4 h = (u>>(uint4)(31U)) & (uint4)(0,1,1,1); // Bits to move up
	return (u<<(uint4)(1U)) | h.yzwx;
}

// Return U>>1
uint4 shr128(uint4 u)
{
	uint4 h = (u<<(uint4)(31U)) & (uint4)(0x80000000U,0x80000000U,0x80000000U,0); // Bits to move down
	return (u>>(uint4)(1U)) | h.wxyz;
}

// Return U*K.
// U MUST be positive.
uint4 mul128u(uint4 u,uint k)
{
	uint4 s1 = u * (uint4)(k);
	uint4 s2 = (uint4)(mul_hi(u.y,k),mul_hi(u.z,k),mul_hi(u.w,k),0);
	return add128(s1,s2);
}

// Return U*K, handles signs (K != INT_MIN).
uint4 mul128(uint4 u,int k)
{
	// Sign bits
	uint su = u.x & 0x80000000U;
	uint sk = (uint)k & 0x80000000U;
	// Abs values
	uint4 uu = (su)?neg128(u):u;
	uint kk = (uint)((sk)?(-k):k);
	// Product
	uint4 p = mul128u(uu,kk);
	// Product sign
	return (su^sk)?neg128(p):p;
}

// Return U*V truncated to keep the position of the decimal point.
// U and V MUST be positive.
uint4 mulfpu(uint4 u,uint4 v)
{
	// Diagonal coefficients
	uint4 s = (uint4)(u.x*v.x,mul_hi(u.y,v.y),u.y*v.y,mul_hi(u.z,v.z));
	// Off-diagonal
	uint4 t1 = (uint4)(mul_hi(u.x,v.y),u.x*v.y,mul_hi(u.x,v.w),u.x*v.w);
	uint4 t2 = (uint4)(mul_hi(v.x,u.y),v.x*u.y,mul_hi(v.x,u.w),v.x*u.w);
	s = add128(s,add128(t1,t2));
	t1 = (uint4)(0,mul_hi(u.x,v.z),u.x*v.z,mul_hi(u.y,v.w));
	t2 = (uint4)(0,mul_hi(v.x,u.z),v.x*u.z,mul_hi(v.y,u.w));
	s = add128(s,add128(t1,t2));
	t1 = (uint4)(0,0,mul_hi(u.y,v.z),u.y*v.z);
	t2 = (uint4)(0,0,mul_hi(v.y,u.z),v.y*u.z);
	s = add128(s,add128(t1,t2));
	// Add 3 to compensate for the truncation
	return add128(s,(uint4)(0,0,0,3));
}

// Return U*U truncated to keep the position of the decimal point.
// U MUST be positive.
uint4 sqrfpu(uint4 u)
{
	// Diagonal coefficients
	uint4 s = (uint4)(u.x*u.x,mul_hi(u.y,u.y),u.y*u.y,mul_hi(u.z,u.z));
	// Off-diagonal
	uint4 t = (uint4)(mul_hi(u.x,u.y),u.x*u.y,mul_hi(u.x,u.w),u.x*u.w);
	s = add128(s,shl128(t));
	t = (uint4)(0,mul_hi(u.x,u.z),u.x*u.z,mul_hi(u.y,u.w));
	s = add128(s,shl128(t));
	t = (uint4)(0,0,mul_hi(u.y,u.z),u.y*u.z);
	s = add128(s,shl128(t));
	// Add 3 to compensate for the truncation
	return add128(s,(uint4)(0,0,0,3));
}

// Return U*V, handles signs
uint4 mulfp(uint4 u,uint4 v)
{
	// Sign bits
	uint su = u.x & 0x80000000U;
	uint sv = v.x & 0x80000000U;
	// Abs values
	uint4 uu = (su)?neg128(u):u;
	uint4 vv = (sv)?neg128(v):v;
	// Product
	uint4 p = mulfpu(uu,vv);
	// Product sign
	return (su^sv)?neg128(p):p;
}

// Return U*U, handles signs
uint4 sqrfp(uint4 u)
{
	// Sign bit
	uint su = u.x & 0x80000000U;
	// Abs value
	uint4 uu = (su)?neg128(u):u;
	// Product is positive
	return sqrfpu(uu);
}
);

GPU_FILLKERNEL_2D(unsigned int,
	mandelbrot,(double sz, double zoom, double xoff,double yoff, int resolution),

	double fw = w * sz;
double fh = h * sz;

double2 c=(double2)(
	(fw*xoff - w/2 + i)/zoom - 2.1,
	(fh*yoff - h/2 + j)/zoom - 1.2
	);
double2 z=c;

int count;
for (count=0;count<resolution;count++)
{
	if ((z.x*z.x+z.y*z.y)>4.0f) 
		break;
	z=(double2)(
		z.x*z.x-z.y*z.y + c.x,
		2.0f*z.x*z.y + c.y
		);
}

result=count;
)

typedef unsigned int uint;

GPU_FILLKERNEL_2D(unsigned int,
	mandelbrot_fp128,(__global<uint*> coords, int xsign, int ysign, int resolution),

	uint4 sz = vload4(0, coords);
	uint4 zoom = vload4(1, coords);
	uint4 zoominv = vload4(2, coords);
	uint4 xoff = vload4(3, coords);
	uint4 yoff = vload4(4, coords);

	if(xsign < 0) xoff = neg128(xoff);
	if(ysign < 0) yoff = neg128(yoff);

	uint4 fw = mul128(sz, w);
	uint4 fh = mul128(sz, h);

	uint4 cx = mulfp(fw ,xoff);
	cx = add128(cx, set128(-w/2));
	cx = add128(cx, set128(i));
	cx = mulfp(cx, zoominv);
	cx = add128(cx, set128(-2.1));

	uint4 cy = mulfp(fh ,yoff);
	cy = add128(cy, set128(-h/2));
	cy = add128(cy, set128(j));
	cy = mulfp(cy, zoominv);
	cy = add128(cy, set128(-1.2));

	uint4 zx = cx;
	uint4 zy = cy;

int count;
for (count=0;count<resolution;count++)
{
	uint4 x2 = sqrfp(zx);
	uint4 y2 = sqrfp(zy); 
	uint4 aux = add128(x2,y2); // x^2+y^2
	if (aux.x>4.0f) 
		break;
	uint4 twoxy = shl128(mulfp(zx,zy));
	zx = add128(cx, add128(x2, neg128(y2)));
	zy = add128(cy, twoxy);
}

result=count;
)

MandelbrotRendererCL::MandelbrotRendererCL(unsigned char *pixelBuffer, unsigned width, unsigned heigth):
m_pixelBuffer(pixelBuffer),
m_pixelBufferWidth(width),
m_pixelBufferHeigth(heigth),
m_img(width, heigth)
{
}


void MandelbrotRendererCL::operator()(bool fp128, double zoom, int resolution,const Vector2lf& normalizedPosition)
{
	if(fp128)
	{
		gpu_vector<unsigned int> coords(20);
		unsigned int xcoords[20];

		FPReal<8> fpZoom(zoom);
		FPReal<8> fpZoom2(zoom * m_pixelBufferHeigth / (2.4));
		FPReal<8> fpZoom2Inv(1.0 / (zoom * m_pixelBufferHeigth / (2.4)));
		FPReal<8> posX((double)normalizedPosition.x);
		FPReal<8> posY((double)normalizedPosition.y);

		int posYsign = posY.sgn();
		int posXsign = posX.sgn();

		fpZoom.getWords(4, xcoords);
		fpZoom2.getWords(4, xcoords + 4);
		fpZoom2Inv.getWords(4, xcoords + 8);
		posX.getWords(4, xcoords + 12);
		posY.getWords(4, xcoords + 16);

		coords.write(xcoords, 0, 20);

		m_img = mandelbrot_fp128(coords, posXsign, posYsign, resolution);
	}
	else
		m_img = mandelbrot(zoom, zoom * m_pixelBufferHeigth / (2.4),normalizedPosition.x,normalizedPosition.y, resolution);
	

	unsigned int* ca = new unsigned int[m_pixelBufferWidth*m_pixelBufferHeigth];
	m_img.read(ca);
	for(unsigned x = 0; x < m_pixelBufferWidth;++x)
		for(unsigned y = 0; y < m_pixelBufferHeigth;++y)
		{
			if (ca[y * m_pixelBufferWidth + x] == resolution)
			{
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 0] = 0;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 1] = 0;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 2] = 0;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 3] = 255;
			}
			else
			{
				int val = ca[y * m_pixelBufferWidth + x] * 255 / resolution;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 0] = val;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 1] = 0;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 2] = 0;
				m_pixelBuffer[(y * m_pixelBufferWidth + x) * 4 + 3] = 255;
			}
		}

	delete[] ca;
}

#endif