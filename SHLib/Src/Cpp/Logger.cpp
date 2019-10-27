#include "Logger.h"

namespace SHLib
{
	CLogger CLogger::m_Instance;

	CLogger::CLogger()
	{
		fopen_s(&m_pFile, "test.txt", "w+");
		assert(m_pFile != nullptr);
	}

	CLogger::~CLogger()
	{
		fclose(m_pFile);
	}

	void CLogger::Print(const char* _pLog, ...)
	{
		const int MSG_SIZE = 512;
		char buf[MSG_SIZE] = { 0, };
		va_list ap;

		va_start(ap, _pLog);
		vsprintf_s(buf, MSG_SIZE, _pLog, ap);
		va_end(ap);

		fprintf(m_pFile, buf);
	}
}