/*
 *  MandelbrotRendererCL.hpp
 *	Hyper Compute - Copyright (c) 2012 Maxime Griot
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
 */

#pragma once

#include <mpir/gmp.h>

class mpfreal
{
	mpf_t mImpl;

public:

	mpfreal();
	mpfreal(const mpfreal& val);
	~mpfreal();

	const mpf_ptr operator*() const; 


	void operator=(const mpfreal& val)
	{
		mpf_set(this->mImpl, *val);
	}

	void operator=(double val)
	{
		mpf_set_d(this->mImpl, val);
	}

	void operator=(int val)
	{
		mpf_set_si(this->mImpl, val);
	}

	void operator=(const char* val)
	{
		mpf_set_str(this->mImpl, val, 10);
	}
};