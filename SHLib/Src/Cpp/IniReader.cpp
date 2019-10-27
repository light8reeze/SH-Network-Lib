#include "IniReader.h"

namespace SHLib
{
	CIniReader::CIniReader() : m_strFilePath(), m_IsInitialized(FALSE)
	{
		ZeroMemory(m_strFilePath, PATH_SIZE);
	}


	CIniReader::~CIniReader()
	{
	}

	void CIniReader::ReadFile(const char* _strFilePath)
	{
		int nSize = strnlen_s(_strFilePath, PATH_SIZE);
		strcpy_s(m_strFilePath, PATH_SIZE, _strFilePath);
		m_IsInitialized = TRUE;
	}

	std::string CIniReader::ReadString(const char* _strSection, const char* _strKey) const
	{
		assert(m_IsInitialized == TRUE);

		std::string strValue;
		char strData[20];
		ReadString(_strSection, _strKey, strData, 20);
		strValue = strData;

		return strValue;
	}

	void CIniReader::ReadString(const char* _strSection, const char* _strKey, char* _strBuf, int _nSize) const
	{
		assert(m_IsInitialized == TRUE);
		GetPrivateProfileString(_strSection, _strKey, "", _strBuf, _nSize, m_strFilePath);
	}

	UINT CIniReader::ReadInt(const char* _strSection, const char* _strKey) const
	{
		assert(m_IsInitialized == TRUE);
		return GetPrivateProfileInt(_strSection, _strKey, INFINITE, m_strFilePath);
	}
}