#pragma once

#define DECLARE_CLASS_ID() \
public: \
	static const void* GetClassIdStatic(); \
	const void* GetClassId() const override; \
	bool IsA(const void* targetId) const override;

#define DEFINE_CLASS_ID(ClassName, BaseClassName) \
	const void* ClassName::GetClassIdStatic() { \
		static const char id = 0; \
		return &id; \
	} \
	const void* ClassName::GetClassId() const { \
		return GetClassIdStatic(); \
	} \
	bool ClassName::IsA(const void* targetId) const { \
		if (GetClassIdStatic() == targetId) return true; \
		return BaseClassName::IsA(targetId); \
	}
