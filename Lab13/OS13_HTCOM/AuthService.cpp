#include "pch.h"
#include "AuthService.h"

void AuthService::checkHtGroupOnExist(const char usersGroupName[USERS_GROUP_NAME_SIZE])
{
	size_t outSize;

	size_t size = strlen(usersGroupName) + 1;
	wchar_t* usersGroupNameUtf = new wchar_t[size];
	mbstowcs_s(&outSize, usersGroupNameUtf, size, usersGroupName, size - 1);

	GROUP_INFO_0* groupsInfo;
	DWORD entriesRead = 0;
	DWORD entriesTotal = 0;

	DWORD status = NetLocalGroupEnum(NULL, 0, (LPBYTE*)&groupsInfo, MAX_PREFERRED_LENGTH, &entriesRead, &entriesTotal, NULL);
	if (status == NERR_Success)
	{
		for (DWORD i = 0; i < entriesRead; i++)
		{
			if (wcscmp(groupsInfo[i].grpi0_name, usersGroupNameUtf) == 0)
			{
				NetApiBufferFree(groupsInfo);
				return;
			}
		}
	}

	throw std::exception(USERS_GROUP_NOT_EXIST_ERROR);
}

void AuthService::checkUserOnHtGroups(const char usersGroupName[USERS_GROUP_NAME_SIZE], const wchar_t user[USER_NAME_SIZE])
{
	size_t outSize;
	size_t size = strlen(usersGroupName) + 1;
	wchar_t* usersGroupNameUtf = new wchar_t[size];
	mbstowcs_s(&outSize, usersGroupNameUtf, size, usersGroupName, size - 1);

	GROUP_USERS_INFO_0* userGroupsInfo;
	DWORD entriesRead = 0;
	DWORD entriesTotal = 0;
	NET_API_STATUS status = NetUserGetLocalGroups(NULL, user, 0, LG_INCLUDE_INDIRECT, (LPBYTE*)&userGroupsInfo, MAX_PREFERRED_LENGTH, &entriesRead, &entriesTotal);
	if (status == NERR_Success)
	{
		for (DWORD i = 0; i < entriesRead; i++)
		{
			if (wcscmp(userGroupsInfo[i].grui0_name, usersGroupNameUtf) == 0)
			{
				NetApiBufferFree(userGroupsInfo);
				return;
			}
		}

	}

	throw std::exception(USER_NOT_HT_GROUP_ERROR);
}

void AuthService::checkUserOnHtGroups(const char usersGroupName[USERS_GROUP_NAME_SIZE], const char user[USER_NAME_SIZE])
{
	size_t outSize;
	size_t size = strlen(user) + 1;
	wchar_t* userUtf = new wchar_t[size];
	mbstowcs_s(&outSize, userUtf, size, user, size - 1);
	
	checkUserOnHtGroups(usersGroupName, userUtf);
}

void AuthService::checkUserOnAdmin(const wchar_t user[USER_NAME_SIZE])
{
	GROUP_USERS_INFO_0* userGroupsInfo;
	DWORD entriesAmount = 0;
	DWORD totalEntries = 0;
	NET_API_STATUS status = NetUserGetLocalGroups(NULL, user, 0, LG_INCLUDE_INDIRECT, (LPBYTE*)&userGroupsInfo, MAX_PREFERRED_LENGTH, &entriesAmount, &totalEntries);
	if (status == NERR_Success)
	{
		for (DWORD i = 0; i < entriesAmount; i++)
		{
			if (wcscmp(userGroupsInfo[i].grui0_name, this->adminGroupName) == 0)
			{
				NetApiBufferFree(userGroupsInfo);
				return;
			}
		}

	}

	throw std::exception(USER_NOT_ADMIN_GROUP_ERROR);
}

void AuthService::checkUserOnAdmin(const char user[USER_NAME_SIZE])
{
	size_t outSize;

	size_t size = strlen(user) + 1;
	wchar_t* userUtf = new wchar_t[size];
	mbstowcs_s(&outSize, userUtf, size, user, size - 1);

	checkUserOnAdmin(userUtf);
}

void AuthService::loginAsUser(const char user[USER_NAME_SIZE], const char password[USER_NAME_SIZE])
{
	size_t outSize;

	size_t size = strlen(user) + 1;
	wchar_t* userUtf = new wchar_t[size];
	mbstowcs_s(&outSize, userUtf, size, user, size - 1);
	
	size = strlen(password) + 1;
	wchar_t* passwordUtf = new wchar_t[size];
	mbstowcs_s(&outSize, passwordUtf, size, password, size - 1);

	HANDLE hToken = 0;
	BOOL result = LogonUserW(userUtf, NULL, passwordUtf, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken);
	CloseHandle(hToken);

	if (result == FALSE)
	{
		throw std::exception(LOGIN_FAILED_ERROR);
	}
}
