// xos
// Copyright (C) 2020 Milan Gallo <gallo.milan.jr@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//
// Created by xgallom on 5/22/20.
//

#include <xos/cpuid.h>
#include "entry.h"
#include "isAvailable.h"

namespace cpuid {
    bool initialize()
    {
	    if (!isAvailable())
		    return false;

	    requestVendorString(RequestType::GetVendorString);
	    requestOther(RequestType::GetFeatures);
	    requestOther(RequestType::GetTLB);
	    requestOther(RequestType::GetSerial);

	    requestVendorString(RequestType::IntelExtended);
	    requestOther(RequestType::IntelFeatures);
	    requestOther(RequestType::IntelBrandString);
	    requestOther(RequestType::IntelBrandStringMore);
	    requestOther(RequestType::IntelBrandStringEnd);

	    return true;
    }
}
