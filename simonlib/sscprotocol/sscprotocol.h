/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SSCPROTOCOL_H
#define SSCPROTOCOL_H

/*
 * "length" always refers to a 64bit signed integer (qint64) which contains the length of the rest of the packet
 * (excluding the 12 bytes of "header" data (qint32 type and the qint64 length itself))
 *
 * length is ONLY ommitted if it would be 0 (i.e. no data after the type) or if the length of the packet is not
 * variable
 */
namespace SSC
{
	enum Request {
		Ok=1000,
		Login=1001, 
		VersionIncompatible=1002,
		LoginSuccessful=1005,
		
		GetInstitutes=6001,
		Institutes=6002,
		GetInstitute=6003,
		Institute=6004,
		InstituteRetrievalFailed=6005,

		GetLanguages=6011,
		Languages=6012,
		GetLanguage=6013,
		Language=6014,
		LanguageRetrievalFailed=6015,

		GetUsers=6021,
		Users=6022,
		GetUser=6023,
		GetInstituteUser=6024,
		UserRetrievalFailed=6025,
		User=6026,
		GetUserInstitutionAssociation=6027,
		UserInstitutionAssociation=6028,
		RemoveUserInstitutionAssociation=6029,
		AddUserInstitutionAssociation=6030,
		AddUser=6031,
		AddUserFailed=6032,
		ModifyUser=6033,
		RemoveUser=6034,
		RemoveUserFailed=6035,

		Sample=6041
	};
}

#endif
