/*********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\*********************************************************************************/

typedef implicit_stack<string> iss;
typedef implicit_stack<unsigned int> isui;

void handle_macro_pop(iss &c_file,isui &position,isui &cfile_length);
unsigned int handle_macros(const string n,iss &c_file,isui &position,isui &cfile_length);
bool extreg_deprecated_struct(bool idnamed,string &last_identifier,int &last_named,int & last_named_phase, externs *&last_type);
int handle_identifiers(const string n,string &last_identifier,unsigned int &pos,int &last_named,int &last_named_phase,externs* &last_type,int &fparam_named,bool at_scope_accessor);
