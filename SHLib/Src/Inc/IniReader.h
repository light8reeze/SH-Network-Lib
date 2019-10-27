#pragma once
#include <string>
#include "SHLib.h"

#define PATH_SIZE static_cast<size_t>(30)

namespace SHLib
{
	class SH_EXPORT CIniReader
	{
	private:
		char m_strFilePath[PATH_SIZE];
		BOOL m_IsInitialized;

	public:
		CIniReader();
		~CIniReader();

		void		ReadFile(const char* _strFilePath);
		std::string ReadString(const char* _strSection, const char* _strKey) const;
		void		ReadString(const char* _strSection, const char* _strKey, char* _strBuf, int _nSize) const;
		UINT		ReadInt(const char* _strSection, const char* _strKey) const;
	};
}