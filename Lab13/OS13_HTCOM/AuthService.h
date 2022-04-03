#pragma once
#include "pch.h"
#include "Values.h"

#define USERS_GROUP_NOT_EXIST_ERROR "Specified users group of storage not exist in OS."
#define	USER_NOT_HT_GROUP_ERROR "Access denied. User doesn't belongs to HT group."
#define	USER_NOT_ADMIN_GROUP_ERROR "Access denied. User doesn't belongs to Administators group."
#define	LOGIN_FAILED_ERROR "Access denied. Login failed"

struct AuthService
{
public: // Constructors

private: // Fields
	const wchar_t* adminGroupName = L"Администраторы";

public: // Getters & Setters

private: // Private methods

public: // Public methods
	void checkHtGroupOnExist(const char usersGroupName[USERS_GROUP_NAME_SIZE]);
	void checkUserOnHtGroups(const char usersGroupName[USERS_GROUP_NAME_SIZE], const wchar_t user[USER_NAME_SIZE]);
	void checkUserOnHtGroups(const char usersGroupName[USERS_GROUP_NAME_SIZE], const char user[USER_NAME_SIZE]);
	void checkUserOnAdmin(const wchar_t user[USER_NAME_SIZE]);
	void checkUserOnAdmin(const char user[USER_NAME_SIZE]);
	void loginAsUser(const char user[USER_NAME_SIZE], const char password[USER_NAME_SIZE]);
};
