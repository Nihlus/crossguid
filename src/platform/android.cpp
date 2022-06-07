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

#ifdef GUID_ANDROID

#include "internal/guid-android.hpp"
#include <jni.h>
#include <cassert>

AndroidGuidInfo androidInfo;
AndroidGuidInfo AndroidGuidInfo::fromJniEnv(JNIEnv *env)
{
	AndroidGuidInfo info;
	info.env = env;
	auto localUuidClass = env->FindClass("java/util/UUID");
	info.uuidClass = (jclass)env->NewGlobalRef(localUuidClass);
	env->DeleteLocalRef(localUuidClass);
	info.newGuidMethod = env->GetStaticMethodID(
		info.uuidClass, "randomUUID", "()Ljava/util/UUID;");
	info.mostSignificantBitsMethod = env->GetMethodID(
		info.uuidClass, "getMostSignificantBits", "()J");
	info.leastSignificantBitsMethod = env->GetMethodID(
		info.uuidClass, "getLeastSignificantBits", "()J");
	info.initThreadId = std::this_thread::get_id();
	return info;
}

void initJni(JNIEnv *env)
{
	androidInfo = AndroidGuidInfo::fromJniEnv(env);
}

namespace xg
{

    Guid newGuid(JNIEnv *env)
    {
	    assert(env != androidInfo.env || std::this_thread::get_id() == androidInfo.initThreadId);

	    jobject javaUuid = env->CallStaticObjectMethod(
		    androidInfo.uuidClass, androidInfo.newGuidMethod);
	    jlong mostSignificant = env->CallLongMethod(javaUuid,
		    androidInfo.mostSignificantBitsMethod);
	    jlong leastSignificant = env->CallLongMethod(javaUuid,
		    androidInfo.leastSignificantBitsMethod);

	    std::array<unsigned char, 16> bytes =
	    {
		    (unsigned char)((mostSignificant >> 56) & 0xFF),
		    (unsigned char)((mostSignificant >> 48) & 0xFF),
		    (unsigned char)((mostSignificant >> 40) & 0xFF),
		    (unsigned char)((mostSignificant >> 32) & 0xFF),
		    (unsigned char)((mostSignificant >> 24) & 0xFF),
		    (unsigned char)((mostSignificant >> 16) & 0xFF),
		    (unsigned char)((mostSignificant >> 8) & 0xFF),
		    (unsigned char)((mostSignificant) & 0xFF),
		    (unsigned char)((leastSignificant >> 56) & 0xFF),
		    (unsigned char)((leastSignificant >> 48) & 0xFF),
		    (unsigned char)((leastSignificant >> 40) & 0xFF),
		    (unsigned char)((leastSignificant >> 32) & 0xFF),
		    (unsigned char)((leastSignificant >> 24) & 0xFF),
		    (unsigned char)((leastSignificant >> 16) & 0xFF),
		    (unsigned char)((leastSignificant >> 8) & 0xFF),
		    (unsigned char)((leastSignificant) & 0xFF)
	    };

	    env->DeleteLocalRef(javaUuid);

	    return Guid{std::move(bytes)};
    }

    Guid newGuid()
    {
	    return newGuid(androidInfo.env);
    }
}

#endif