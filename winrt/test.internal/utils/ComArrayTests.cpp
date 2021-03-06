// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Licensed under the MIT License. See LICENSE.txt in the project root for license information.

#include "pch.h"

TEST_CLASS(ComArrayTests)
{
    class Tracker
    {
    public:
        static void* CoTaskMemAlloc(size_t bytes)
        {
            auto p = ::CoTaskMemAlloc(bytes);
            GetAllocations()->insert(p);
            return p;
        }
        
        static void CoTaskMemFree(void* p)
        {
            if (p)
            {
                Assert::AreEqual<size_t>(1U, GetAllocations()->count(p));
                GetAllocations()->erase(p);
            }
            ::CoTaskMemFree(p);
        }

        static void Reset()
        {
            GetAllocations()->clear();
        }

        static void Verify()
        {
            Assert::IsTrue(GetAllocations()->empty(), L"CoTaskMem leak detected");
        }

    private:
        static std::set<void*>* GetAllocations()
        {
            static std::set<void*> allocations;
            return &allocations;
        }
    };

    TEST_METHOD_INITIALIZE(Initialize)
    {
        Tracker::Reset();
    }

    TEST_METHOD_CLEANUP(Cleanup)
    {
        Tracker::Verify();
    }

    TEST_METHOD_EX(ComArray_ZeroSizedArray_HasNonNullData)
    {
        ComArray<float, Tracker> array(0);

        Assert::AreEqual(0U, array.GetSize());
        Assert::IsNotNull(array.GetData());
    }

    TEST_METHOD_EX(ComArray_SizedArray)
    {
        ComArray<float, Tracker> array(100);
        Assert::AreEqual(100U, array.GetSize());
        Assert::IsNotNull(array.GetData());
    }

    TEST_METHOD_EX(ComArray_ArraySubscriptOperator)
    {
        ComArray<float, Tracker> array(100);

        Assert::AreEqual(array.GetData(), &array[0]);
        Assert::AreEqual(array.GetData() + 1, &array[1]);
    }

    TEST_METHOD_EX(ComArray_MoveConstructor)
    {
        ComArray<float, Tracker> sourceArray(100);
        auto originalData = sourceArray.GetData();

        ComArray<float, Tracker> destArray(std::move(sourceArray));

        Assert::AreEqual(0U, sourceArray.GetSize());
        Assert::IsNull(sourceArray.GetData());

        Assert::AreEqual(100U, destArray.GetSize());
        Assert::AreEqual(originalData, destArray.GetData());
    }

    TEST_METHOD_EX(ComArray_MoveAssignment)
    {
        ComArray<float, Tracker> sourceArray(100);
        auto originalData = sourceArray.GetData();

        ComArray<float, Tracker> destArray;
        destArray = std::move(sourceArray);

        Assert::AreEqual(0U, sourceArray.GetSize());
        Assert::IsNull(sourceArray.GetData());

        Assert::AreEqual(100U, destArray.GetSize());
        Assert::AreEqual(originalData, destArray.GetData());
    }

    TEST_METHOD_EX(ComArray_Detach)
    {
        ComArray<float, Tracker> array(100);

        uint32_t size;
        float* data;
        array.Detach(&size, &data);

        Assert::AreEqual(100U, size);
        Tracker::CoTaskMemFree(data);
    }

    TEST_METHOD_EX(ComArray_GetAddressOfData_ReleasesAndTracksNewValue)
    {
        ComArray<float, Tracker> array(100);
        *array.GetAddressOfData() = static_cast<float*>(Tracker::CoTaskMemAlloc(1));

        // For this test to pass, the memory allocated by the constructor must
        // have been freed and the memory assigned in the above line must also
        // have been freed.
    }

    TEST_METHOD_EX(ComArray_ConstructFromIterators_Empty)
    {
        std::vector<float> src;
        ComArray<float, Tracker> array(src.begin(), src.end());
        Assert::AreEqual(0U, array.GetSize());
        Assert::IsNotNull(array.GetData());
    }

    TEST_METHOD_EX(CopyToComArray_WithData)
    {
        std::vector<float> src{ 1, 2, 3 };

        ComArray<float, Tracker> array(src.begin(), src.end());

        Assert::AreEqual(3U, array.GetSize());
        auto d = array.GetData();
        Assert::AreEqual(1.0f, d[0]);
        Assert::AreEqual(2.0f, d[1]);
        Assert::AreEqual(3.0f, d[2]);
    }

    TEST_METHOD_EX(TransformToComArray_Empty)
    {
        auto transformFn = [](int v) { Assert::Fail(L"Don't expect transformFn to get called"); return 0.0; };
        std::vector<int> src;

        auto array = TransformToComArray<double, decltype(src.begin()), decltype(transformFn), Tracker>(src.begin(), src.end(), transformFn);
        
        Assert::AreEqual(0U, array.GetSize());
        Assert::IsNotNull(array.GetData());
    }

    TEST_METHOD_EX(TransformToComArray_WithData)
    {
        auto transformFn = [](int v) { return static_cast<double>(v+1.0); };
        std::vector<int> src{ 1, 2, 3 };

        auto array = TransformToComArray<double, decltype(src.begin()), decltype(transformFn), Tracker>(src.begin(), src.end(), transformFn);
        
        Assert::AreEqual(3U, array.GetSize());
        auto d = array.GetData();
        Assert::AreEqual(2.0, d[0]);
        Assert::AreEqual(3.0, d[1]);
        Assert::AreEqual(4.0, d[2]);
    }
};
