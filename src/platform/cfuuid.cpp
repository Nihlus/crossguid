/*
The MIT License (MIT)

Copyright (c) 2014 Graeme Hill (http://graemehill.ca)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifdef GUID_CFUUID

#include <CoreFoundation/CFUUID.h>

namespace xg
{
    Guid newGuid()
    {
	    auto newId = CFUUIDCreate(NULL);
	    auto bytes = CFUUIDGetUUIDBytes(newId);
	    CFRelease(newId);

	    std::array<unsigned char, 16> byteArray =
	    {{
		    bytes.byte0,
		    bytes.byte1,
		    bytes.byte2,
		    bytes.byte3,
		    bytes.byte4,
		    bytes.byte5,
		    bytes.byte6,
		    bytes.byte7,
		    bytes.byte8,
		    bytes.byte9,
		    bytes.byte10,
		    bytes.byte11,
		    bytes.byte12,
		    bytes.byte13,
		    bytes.byte14,
		    bytes.byte15
	    }};

	    return Guid{std::move(byteArray)};
    }
}

#endif