/*
 *   Copyright (C) 2009 Dominik Neumeister <domar@gmx.at>
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
#ifndef SIMON_TOKEN_H_F9C205BEB5644D829F4AF5515883F7FD
#define SIMON_TOKEN_H_F9C205BEB5644D829F4AF5515883F7FD

class Token
{

  public:
    Token(char arOp, short type);
    Token(double num);
    char getArOperator() const;
    double getNumber() const;
    short getType() const;

  private:
    char arOperator;
    double number;
    //@ type: -1=(,); 0=number; 1=+,-; 2=*,/; 3=%;
    short type;
};
#endif
