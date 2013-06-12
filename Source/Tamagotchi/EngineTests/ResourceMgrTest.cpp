#include "stdafx.h"

#include "ResourceMgr/ResourceMgr.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ResourceMgrTest)
{
public:
	std::shared_ptr<ResourceMgr> resourceMgr;

	ResourceMgrTest()
	{
		resourceMgr = std::shared_ptr<ResourceMgr>(TG_NEW ResourceMgr(static_cast<unsigned int>(MB_TO_B(50)), NULL));
	}

	TEST_METHOD(InitTest)
	{
		Assert::IsTrue(resourceMgr->Init(), L"1");
	}
};