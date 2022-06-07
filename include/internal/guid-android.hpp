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


#ifndef CROSSGUID_GUID_ANDROID_H
#define CROSSGUID_GUID_ANDROID_H

#ifdef GUID_ANDROID
#include <thread>
#include <jni.h>

struct AndroidGuidInfo
{
	static AndroidGuidInfo fromJniEnv(JNIEnv *env);

	JNIEnv *env;
	jclass uuidClass;
	jmethodID newGuidMethod;
	jmethodID mostSignificantBitsMethod;
	jmethodID leastSignificantBitsMethod;
	std::thread::id initThreadId;
};

extern AndroidGuidInfo androidInfo;

void initJni(JNIEnv *env);

// overloading for multi-threaded calls
Guid newGuid(JNIEnv *env);
#endif

#endif //CROSSGUID_GUID_ANDROID_H
