#pragma once

#include "REX/TEnumSet.h"

namespace REX::W32
{
	using RTTITypeDescriptor = std::type_info;

	struct RTTIClassHierarchyDescriptor;

	struct RTTIBaseClassDescriptor2
	{
		enum class Attribute : std::uint32_t
		{
			None = 0,
			NotVisible = 1 << 0,
			Ambiguous = 1 << 1,
			Private = 1 << 2,
			PrivateOrProtectedBase = 1 << 3,
			Virtual = 1 << 4,
			NonPolymorphic = 1 << 5,
			HasHierarchyDescriptor = 1 << 6
		};

		struct PMD
		{
			// members
			std::int32_t mDisp;  // 0x00
			std::int32_t pDisp;  // 0x04
			std::int32_t vDisp;  // 0x08
		};
		static_assert(sizeof(PMD) == 0xC);

		// members
		RTTITypeDescriptor*                 typeDescriptor;     // 0x00
		std::uint32_t                       numContainedBases;  // 0x08
		PMD                                 where;              // 0x0C
		TEnumSet<Attribute>                 attributes;         // 0x18
		const RTTIClassHierarchyDescriptor* classDescriptor;    // 0x1C
	};
	static_assert(sizeof(RTTIBaseClassDescriptor) == 0x24);

	struct RTTIBaseClassArray
	{
		const RTTIBaseClassDescriptor2* arrayOfBaseClassDescriptors[];
	};
	static_assert(sizeof(RTTIBaseClassArray) == 0x8);

	struct RTTIClassHierarchyDescriptor
	{
		enum class Attribute : std::uint32_t
		{
			NoInheritance = 0,
			MultipleInheritance = 1 << 0,
			VirtualInheritance = 1 << 1,
			AmbiguousInheritance = 1 << 2
		};

		// members
		std::uint32_t                   signature;       // 0x00
		TEnumSet<Attribute>             attributes;      // 0x04
		std::uint32_t                   numBaseClasses;  // 0x08
		const RTTIBaseClassDescriptor2* baseClassArray;  // 0x0C
	};
	static_assert(sizeof(RTTIClassHierarchyDescriptor) == 0x14);

	struct RTTICompleteObjectLocator
	{
		enum class Signature : std::uint32_t
		{
			x86 = 0,
			x64 = 1
		};

		// members
		Signature                           signature;        // 0x00
		std::uint32_t                       offset;           // 0x04
		std::uint32_t                       ctorDispOffset;   // 0x08
		RTTITypeDescriptor*                 typeDescriptor;   // 0x0C
		const RTTIClassHierarchyDescriptor* classDescriptor;  // 0x14
		const RTTICompleteObjectLocator*    self;             // 0x1C
	};
	static_assert(sizeof(RTTICompleteObjectLocator) == 0x24);
}
