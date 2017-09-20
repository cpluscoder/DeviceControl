#ifndef _AUTO_HANDLE_H_
#define _AUTO_HANDLE_H_

#include <stdexcept>
#include "Win32Types.h"

class CAutoHandleException : public std::runtime_error
{
public:
	CAutoHandleException(const std::string& message) : std::runtime_error(message) {}
};

class CAutoHandle 
{
public:
	struct Nothrow {};

public:
	CAutoHandle(HANDLE handle);
	CAutoHandle(HANDLE handle, Nothrow);
	virtual ~CAutoHandle(void);

public:
	HANDLE GetHandle(void) const;
	operator HANDLE(void) const;

public:
	HANDLE Release(void);
	void Reset(void);
	void Reset(HANDLE newHandle);

private:
	CAutoHandle(const CAutoHandle&);
	CAutoHandle& operator=(const CAutoHandle&);

private:
	HANDLE handle_;
};

#endif //_AUTO_HANDLE_H_