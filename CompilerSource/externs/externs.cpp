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

#include <map>
#include <string>
#include <iostream>
#include "../general/darray.h"
using namespace std;
#include "externs.h"


bool externs::is_function()
{
  if (refstack.topmostsymbol() == '(')
    return true;
  if (refstack.last != NULL and refstack.last->prev != NULL and refstack.last->prev->ref.symbol == '(')
    return true;
  return false;
}
int externs::parameter_count()
{
  if (refstack.topmostsymbol() == '(')
    return refstack.topmostcount();
  if (refstack.last != NULL and refstack.last->prev != NULL and refstack.last->prev->ref.symbol == '(')
    return refstack.last->prev->ref.count;
  return 0;
}

externs::externs()
{
  type = NULL;
  parent = NULL;
}
externs::externs(string n,externs* p,unsigned int f)
{
  name = n;
  type = NULL;
  parent = p;
  flags = f;
}
externs::externs(string n,externs* t,externs* p,unsigned int f)
{
  name = n;
  type = NULL;
  parent = p;
  flags = f;
}

macro_type::operator string() { return val; }
macro_type::macro_type(): argc(-1), recurse_danger(0) {}
macro_type::macro_type(string x): argc(-1), val(x), recurse_danger(0) {}
macro_type& macro_type::operator= (string x) { val = x; return *this; }
void macro_type::assign_func(string n) { if (argc==-1) argc=0; recurse_danger = check_recurse_danger(n); }
void macro_type::addarg(string x) { args[argc++] = x; }

#include "../general/parse_basics.h"
bool macro_type::check_recurse_danger(string n)
{
  register bool dgr = 0;
  for (unsigned i = 0; i < val.length(); i++)
  {
    if (is_letter(val[i]))
    {
      const unsigned si = i;
      while (is_letterd(val[i])) i++;
      if (val.substr(si,i-si) == n)
      { dgr = 1; break; }
    }
  }
  return dgr;
}



//Map to sort, darray for polymorphic things
map<string, varray<externs> > extarray;
externs global_scope,*current_scope,using_scope,*immediate_scope=NULL;
map<string,macro_type> macros;



extiter scope_find_member(string name)
{
  for (externs *s = current_scope; s != NULL; s = s->parent)
  {
    extiter it = s->members.find(name);
    if (it != s->members.end() or s == &global_scope)
      return it;
  }
  return global_scope.members.end();
}


externs* ext_retriever_var = NULL;
bool find_extname(string name,unsigned int flags)
{
  if (immediate_scope != NULL)
  {
    extiter f = immediate_scope->members.find(name);
    if (f == immediate_scope->members.end()) return false;
    ext_retriever_var = f->second;
    immediate_scope = NULL;
    return true;
  }
  externs* inscope=current_scope;
  externs::tempiter tit = inscope->tempargs.find(name);
  if (tit != inscope->tempargs.end())
  {
    ext_retriever_var = tit->second;
    return 1;
  }
  extiter it = inscope->members.find(name);
  while (it == inscope->members.end()) //Until we find it
  {
    if (inscope==&global_scope) //If we're at global scope, give up
      goto check_using_namespaces;
    inscope=inscope->parent; //This must ALWAYS be nonzero when != global_scope
    
    tit = inscope->tempargs.find(name);
    if (tit != inscope->tempargs.end())
    {
      ext_retriever_var = tit->second;
      return 1;
    }
    
    it = inscope->members.find(name);
  }
  ext_retriever_var = it->second;
  return ((it->second->flags & flags) != 0 or flags == 0xFFFFFFFF);
  
  check_using_namespaces:
  it = using_scope.members.find(name);
  if (it != using_scope.members.end())
  {
    ext_retriever_var = it->second;
    return 1;
  }
  for (it = using_scope.members.begin(); it != using_scope.members.end(); it++)
    if (it->second->flags & EXTFLAG_NAMESPACE)
    {
      extiter sit = it->second->members.find(name);
      if (sit != it->second->members.end())
      {
        ext_retriever_var = sit->second;
        return 1;
      }
    }
  return 0;
}
