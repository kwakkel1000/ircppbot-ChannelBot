//
//
//  @ Project : channelbot
//  @ File Name : cauth.h
//  @ Date : 15-01-2013
//  @ Author : Gijs Kwakkel
//
//
// Copyright (c) 2013 Gijs Kwakkel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//


#ifndef SRC_INCLUDE_CAUTH_H
#define SRC_INCLUDE_CAUTH_H
#include <string>
#include <map>
#include <set>
#include <atomic>
#include <mutex>
#include <memory>
#include <cstddef>

class cauth
{
    public:
        cauth();
        cauth(const cauth&);
        ~cauth();

        void setAccess(size_t access);
        size_t getAccess();

        void setNoAutoOp(bool noAutoOp);
        bool getNoAutoOp();

        void setAutoInvite(bool autoInvite);
        bool getAutoInvite();

        void setInfo(std::string info);
        std::string getInfo();

    private:
        size_t m_Access;
        bool m_NoAutoOp;
        bool m_AutoInvite;
        std::string m_Info;
};

#endif // SRC_INCLUDE_CAUTH_H
